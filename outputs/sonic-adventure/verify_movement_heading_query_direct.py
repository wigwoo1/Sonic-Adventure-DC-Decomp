"""Full-state verification and independent predicate/optional-output contracts."""
import collections,hashlib,itertools,json,sys
from pathlib import Path
import movement_heading_query_fixtures as q
P=q.P
OPTIONS=((False,False),(False,True),(True,False),(True,True))
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def signed16(x):return ((x&0xffff)^0x8000)-0x8000
def configurations():
    for override,outputs,index in itertools.product((0,1,255),OPTIONS,(0,255)):
        yield dict(kind='flag_override',flags=0x4000,override=override,heading_output=outputs[0],value_output=outputs[1],index=index)
    for word,flags,state,outputs,index in itertools.product((0x3f800000,0xc0000000),(0,0x2000),(0,1,65535),OPTIONS,(0,255)):
        yield dict(kind='table_query',word=word,flags=flags,character_word=state,heading_output=outputs[0],value_output=outputs[1],index=index)
    for word,actor_mode,flags,stage,outputs in itertools.product((0,0x80000000),(0,1),(0,0x100),((9,2),(9,3),(9,4),(0x8009,2),(9,0x8003)),OPTIONS):
        yield dict(kind='stage_fallback',word=word,actor_mode=actor_mode,flags=flags,major=stage[0],minor=stage[1],heading_output=outputs[0],value_output=outputs[1],index=1)
    for alias,mode,rm in itertools.product(('same_output','heading_table_value','heading_actor_index','value_actor_heading'),(0,4),(0,1)):
        yield dict(kind='live_output_alias',alias=alias,mode=mode,rm=rm,index=1,flags=0x2000)
    for word in (1,0x80000001,0x00800000,0x7f7fffff,0x7f800000,0xff800000,0x7fc00001,0x7f800001):
        for mode in (0,4):yield dict(kind='raw_float_word',word=word,mode=mode)
    for mode,flags in itertools.product((1,2,3,5,6,7),(0x4000,0x2000)):
        yield dict(kind='raw_precision_mode',mode=mode,flags=flags)
    for view,flags in itertools.product((0x8c000000,0xac000000,0x0c000000),(0x4000,0x2000,0x100)):
        yield dict(kind='ram_view',view=view,flags=flags,word=0 if flags==0x100 else 0x3f800000,minor=3)
    for flags in (0x4000,0x2000):yield dict(kind='fpu_disabled',flags=flags,fd=True)
    for bad in ('stack','actor','heading','value','character','override_object'):
        yield dict(kind='mapped_guard',bad=bad,flags=0x4000 if bad=='override_object' else 0,character_word=0)
    for bad in ('heading','value'):
        yield dict(kind='override_ignores_output_pointer',bad=bad,flags=0x4000)

def main():
    ram=q.v.seed_ram();records=[];checks=oracles=0;seen=set();edges=set();counts=collections.Counter()
    selected=[dict(c,case_id=n,mode=c.get('mode',4*(n%2)),rm=c.get('rm',(n//2)%2)) for n,c in enumerate(configurations())]
    diagnostic='--smoke' in sys.argv or '--case-id' in sys.argv
    if '--case-id' in sys.argv:selected=[selected[int(sys.argv[sys.argv.index('--case-id')+1])]]
    if '--smoke' in sys.argv:
        first={}
        for c in selected:first.setdefault(c['kind'],c)
        selected=list(first.values())
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for c in selected:
        m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s);initial_f=bytes(f)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,1,-1,-3),'completed or reached original numeric/mapped stop')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'full CPU/FPU/RAM/device/queue state and ordered boundaries')
        require(result['boundaries'][0]['target']==hex(q.ENTRY),'ordinary original entry')
        if result['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'normal return and balanced stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
            scalar=c['mode'] in (0,4) and c['kind'] not in ('raw_float_word','mapped_guard','fpu_disabled','override_ignores_output_pointer')
            override=bool(c.get('flags',0x2000)&0x4000)
            if override:require(bytes(f)==initial_f,'flag override executes no FPU instruction and preserves complete FPU state')
            if scalar:
                flags=c.get('flags',0x2000);zero=c.get('word',0x3f800000)&0x7fffffff==0
                stage=((signed16(c.get('major',9))<<8)|signed16(c.get('minor',2)))&0xffff
                fallback=zero and not override
                stage_gate=fallback and not c.get('actor_mode',0) and bool(flags&0x100)
                success=bool(c.get('override',1)) if override else bool(not c.get('actor_mode',0) and flags&0x100 and stage in (0x902,0x903)) if zero else bool(flags&0x2000 or c.get('character_word',0)==0)
                require(s.r[0]==int(success),'independent heading-query predicate')
                calls=[b for b in result['boundaries'] if b['target']=='0x8c04f7e0']
                require(len(calls)==(1 if stage==0x902 else 2) if stage_gate else len(calls)==0,'actual stage query count and short circuit')
                expected={}
                def read(a,n=4):return int.from_bytes(bytes(expected.get((a+j)&0xffffff,before[(a+j)&0xffffff]) for j in range(n)),'little')
                def write(a,value):
                    for j,b in enumerate((value&0xffffffff).to_bytes(4,'little')):expected[(a+j)&0xffffff]=b
                if success and not override:
                    if initial.r[5]:write(initial.r[5],read(q.A+0x18) if zero else read(q.TABLE+8*c.get('index',0)))
                    if initial.r[6]:write(initial.r[6],0 if zero else read(q.TABLE+8*read(q.A+8,1)+4))
                for a in (q.A,q.B,q.C):
                    off=a&0xffffff
                    require(m[off:off+0x200]==bytes(expected.get(off+j,before[off+j]) for j in range(0x200)),'independent optional outputs and live alias footprint')
                for a in expected:require(m[a]==expected[a],'independent raw output bytes including table/index aliases')
                if c.get('alias')=='heading_actor_index':require(q.get(m,q.C)==0x40400000,'value copy reloads index changed by original heading store')
                oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],return_value=s.r[0],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('HEADING QUERY DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in edge) for edge in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if diagnostic:print('HEADING DIAGNOSTIC PASS',len(records),checks,coverage,flush=True);return
    files=[Path(__file__),P/'movement_heading_query_fixtures.py',P/'movement_heading_query_reference.py',P/'write_watch_movement_heading_query.py',q.v.MP,P/'analysis/movement_heading_query_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original06cd22 conditional heading/value query. Authored actor/table/optional-pointer/stage-global inputs;actual original04f7e0 executes. Full-state comparison and independent predicates/output footprints/live index/table aliases;raw precision/numeric/FD/mapped stops remain documented software behavior. Physicalhardware/fullgame unfinished.')
    (P/'analysis/movement_heading_query_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('HEADING QUERY DIRECT PASS',len(records),checks,oracles,coverage,flush=True)
if __name__=='__main__':main()
