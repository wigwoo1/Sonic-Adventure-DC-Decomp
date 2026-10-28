"""Full-state comparisons and semantic position/angle/callback/impulse oracles."""
import collections,hashlib,itertools,json,sys
from pathlib import Path
import movement_record_adjustment_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(m,a):return tuple(q.get(m,a+4*j) for j in range(3))
def vector(m,a):return tuple(q.number(x) for x in words(m,a))
def add(a,b):return tuple(x+y for x,y in zip(a,b))
def scaled(a,k):return tuple(k*x for x in a)
def configurations():
    for index,velocity,callback,mode,rm,view in itertools.product((0,1,255),((0,0,0),(3,4,12)),(0,q.IMPULSE),(0,4),(0,1),(0x8c000000,0xac000000)):
        yield dict(kind='record_callback_offsets',entry=q.OWNER,index=index,velocity=velocity,callback=callback,mode=mode,rm=rm,view=view)
    for entry,mode,rm,t in itertools.product((q.OWNER,q.IMPULSE),(0,4),(0,1),(0,1)):
        if entry==q.OWNER:yield dict(kind='null_record',entry=entry,enabled=False,mode=mode,rm=rm,t=t)
    for impulse,mode,rm,view in itertools.product(((1,2,3),(-2,-3,-4),(.25,.5,-.25),(0,0,0)),(0,4),(0,1),(0x8c000000,0x0c000000)):
        yield dict(kind='impulse_accumulation',entry=q.IMPULSE,impulse=impulse,mode=mode,rm=rm,view=view)
    for alias,mode,rm,velocity in itertools.product(('query_actor','displacement_actor','character_actor','query_displacement'),(0,4),(0,1),((0,0,0),(3,4,12))):
        yield dict(kind='live_input_alias',entry=q.OWNER,alias=alias,mode=mode,rm=rm,velocity=velocity,callback=q.IMPULSE,index=1)
    for index,mode,velocity in itertools.product((0,1),(0,4),((0,0,0),(3,4,12))):
        yield dict(kind='unknown_callback',entry=q.OWNER,index=index,mode=mode,velocity=velocity,callback=0x8cfef000)
    for entry,mode in itertools.product((q.OWNER,q.IMPULSE),(1,2,3,5,6,7)):
        yield dict(kind='reached_raw_numeric_mode',entry=entry,mode=mode,callback=q.IMPULSE)
    for entry,word,mode in itertools.product((q.OWNER,q.IMPULSE),(1,0x00800000,0x7f7fffff,0x7f800000,0x7fc00001,0x7f800001),(0,4)):
        yield dict(kind='raw_numeric_input',entry=entry,mode=mode,**{('velocity_words' if entry==q.OWNER else 'impulse_words'):(word,0,0)})
    for entry in (q.OWNER,q.IMPULSE):
        yield dict(kind='fpu_disabled',entry=entry,fd=True,callback=q.IMPULSE)
    for entry,bad in ((q.OWNER,'stack'),(q.OWNER,'actor'),(q.OWNER,'state'),(q.OWNER,'query'),(q.OWNER,'displacement'),(q.OWNER,'holder'),(q.OWNER,'record'),(q.OWNER,'character'),(q.IMPULSE,'actor'),(q.IMPULSE,'query'),(q.IMPULSE,'impulse')):
        yield dict(kind='mapped_guard',entry=entry,bad=bad,callback=q.IMPULSE)

def main():
    ram=q.v.seed_ram();records=[];checks=oracles=0;seen=set();edges=set();counts=collections.Counter()
    selected=list(configurations());case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
    if case_id is not None:selected=[selected[case_id]]
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for n,c in enumerate(selected):
        c=dict(c,case_id=n if case_id is None else case_id);m,s,f,cfg=q.prepare(ram,c)
        before=bytes(m);initial=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,1,-1,-2,-3),'complete/reached original numeric or mapped/callback stop')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'complete CPU/FPU/RAM/device/queue state and ordered boundaries')
        require(result['boundaries'][0]['target']==hex(c['entry']),'normal original entry')
        if c['kind']=='unknown_callback':
            require(result['status']==-2 and result['boundaries'][-1]['target']==hex(c['callback']),'unknown callback remains a reached unresolved target')
            require(s.pr==0x8c0750fc and s.r[15]==initial.r[15]-36,'caller frame and real callback return retained; no repaired frame')
        if result['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'original return and balanced stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
            if c['kind'] in ('record_callback_offsets','null_record','impulse_accumulation'):
                if c['entry']==q.IMPULSE:
                    impulse=vector(before,initial.r[6]+0x3c)
                    require(vector(m,q.A+0x20)==add(vector(before,q.A+0x20),scaled(impulse,3)),'independent actor position plus triple impulse')
                    old_query=vector(before,q.C+4);expected=(old_query[0]+2*impulse[0],old_query[1],old_query[2]+2*impulse[2])
                    require(vector(m,q.C+4)==expected,'independent query X/Z double impulse;Y unchanged')
                    require(s.sr==initial.sr,'leaf preserves complete integer status')
                    require(all(s.r[j]==initial.r[j] for j in range(2,16)),'leaf preserves input and non-scratch integer registers')
                elif not c.get('enabled',True):
                    require(m[q.A&0xffffff:(q.A&0xffffff)+0x200]==before[q.A&0xffffff:(q.A&0xffffff)+0x200] and m[q.C&0xffffff:(q.C&0xffffff)+0x200]==before[q.C&0xffffff:(q.C&0xffffff)+0x200],'null record leaves actor and query untouched')
                    require(len(result['boundaries'])==1,'null record calls no child')
                else:
                    index=c['index'];record=q.RECORD+(0x1c if index else 0)
                    origin=vector(before,q.A+0x20)
                    output=add(origin,vector(before,q.OUT)) if c['velocity']==(0,0,0) else vector(before,q.C+4)
                    require(vector(m,q.C+4)==add(output,vector(before,record+0x10)),'independent zero-speed fallback plus live record translation')
                    require(vector(m,q.A+0x20)==origin,'actor position restored after temporary callback position')
                    for j in range(3):require(q.get(m,q.A+0x14+4*j)==(q.get(before,q.A+0x14+4*j)+q.get(before,record+4+4*j))&0xffff,'independent modulo16bit angle offset')
                    callbacks=[x for x in result['boundaries'] if x['target']==hex(q.IMPULSE)]
                    require(bool(callbacks)==bool(c['callback']),'live callback selection')
                    if callbacks:
                        cb=callbacks[0];impulse=vector(before,record+0x3c)
                        expected_words=lambda xs:list(map(q.bits,xs))
                        require(cb['arguments'][:3]==[c['view']|(q.D&0xffffff),c['view']|(q.A&0xffffff),c['view']|(record&0xffffff)],'actual callback character/actor/selected-record arguments')
                        require(cb['record_callback_input']['actor_position']==expected_words(output) and cb['record_callback_input']['saved_position']==expected_words(origin),'read-only callback snapshot sees temporary position and saved original')
                        require(vector(m,q.D+0x20)==add(vector(before,q.D+0x20),scaled(impulse,3)),'actual original callback changes character position')
                        require(q.number(q.get(m,q.A+4))==q.number(q.get(before,q.A+4))+2*impulse[0] and q.number(q.get(m,q.A+0xc))==q.number(q.get(before,q.A+0xc))+2*impulse[2],'actual callback side effects outside restored position remain')
                oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('RECORD ADJUSTMENT DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted=set(map(lambda a:int(a,16),body['reachable_addresses']));outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if case_id is not None:print('SINGLE DIAGNOSTIC',records[0]['status'],coverage,flush=True);return
    files=[Path(__file__),P/'movement_record_adjustment_fixtures.py',P/'movement_record_adjustment_reference.py',P/'write_watch_movement_record_adjustment.py',q.v.MP,P/'analysis/movement_record_adjustment_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),supplied_game_callee_outputs=0,supplied_execution_frames=0,sha256={str(p):digest(p) for p in files},scope='Actual original07506e record callback/position/angle offsets and0751c0 impulse body. Authored initial callback records use the real original0751c0 as a legal argument-compatible test callback;no claim of a discovered retail callback assignment. Complete original/native state and ordered boundaries;unknown callback target remains unresolved. No supplied results/frames. Physicalhardware/fullgame remain unfinished.')
    (P/'analysis/movement_record_adjustment_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('RECORD ADJUSTMENT DIRECT PASS',len(records),checks,oracles,coverage,flush=True)
if __name__=='__main__':main()
