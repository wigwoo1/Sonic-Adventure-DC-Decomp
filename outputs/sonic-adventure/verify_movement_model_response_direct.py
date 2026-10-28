"""Whole original movement/model group with actual recursion, rendering and collision."""
import collections,hashlib,itertools,json,sys
from pathlib import Path
import movement_model_response_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def configurations():
    for entry,t,mask,mode in itertools.product((q.WALK,q.WRAP),range(8),range(8),(0,4)):
        yield dict(kind='model',entry=entry,type=t,flags=mask<<3,mode=mode)
    for entry,t,mode in itertools.product((q.WALK,q.WRAP),range(8),(0,4)):
        yield dict(kind='tree',entry=entry,type=t,tree=True,mode=mode,angles=(0x1000,0x2000,0x3000))
    for entry,t,mode in itertools.product((q.WALK,q.WRAP),range(8),(0,4)):
        yield dict(kind='culled_mesh',entry=entry,type=t,mesh_gate=0,mode=mode)
    for global_mode,character,actor_flags,external_flags,mode,point in itertools.product((0,1),(0,5,7),(0,0x2000),(0,0x4000),(0,4),((.5,.25,.5),(4,.25,4))):
        yield dict(kind='movement',entry=q.OWNER,global_mode=global_mode,character=character,actor_flags=actor_flags,external_flags=external_flags,mode=mode,point=point)
    for speed,height,low,cap,state_flags,mode in itertools.product((0,1,3),(-1,0,1),(0,.5),(1,2),(0,0x4000),(0,4)):
        yield dict(kind='state',entry=q.OWNER,global_mode=1,speed=speed,height=height,low=low,cap=cap,state_flags=state_flags,mode=mode)
    for character,velocity,orientation,delta,mode in itertools.product((0,7),((0,2,0),(3,0,0)),((0,1,0),(0,-1,0)),((0,0,0),(0,2,0)),(0,4)):
        yield dict(kind='vectors',entry=q.OWNER,character=character,velocity=velocity,orientation=orientation,delta=delta,mode=mode)
    for global_mode,character,mode in itertools.product((0,1),(0,5,7),(0,4)):
        yield dict(kind='rendered',entry=q.OWNER,global_mode=global_mode,character=character,mode=mode,debug=0)
    for entry,mode in itertools.product((q.WALK,q.WRAP,q.OWNER),range(8)):
        yield dict(kind='raw_mode',entry=entry,mode=mode)
    for entry,view,mode in itertools.product((q.WALK,q.WRAP,q.OWNER),(0x0c000000,0x8c000000,0xac000000),(0,4)):
        yield dict(kind='view',entry=entry,view=view,mode=mode)
    for rm,entry,mode in itertools.product((0,1),(q.WALK,q.WRAP,q.OWNER),(0,4)):
        yield dict(kind='rounding',entry=entry,rm=rm,mode=mode)
    for c in (dict(entry=q.WALK,bad='node'),dict(entry=q.WRAP,bad='node'),dict(entry=q.OWNER,bad='actor'),dict(entry=q.OWNER,bad='query'),dict(entry=q.OWNER,bad='external')):
        yield dict(kind='guard',**c)
    for entry in (q.WALK,q.WRAP,q.OWNER):yield dict(kind='guard',entry=entry,bad='stack')

def main():
    ram=q.v.seed_ram();records=[];checks=oracles=0;seen=set();edges=set();counts=collections.Counter()
    selected=list(configurations());case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
    if case_id is not None:selected=[selected[case_id]]
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for n,c in enumerate(selected):
        c=dict(c,case_id=n if case_id is None else case_id);m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,1,-1,-3),'complete/existing numeric stop/mapped guard;no substituted game callee')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'full CPU/FPU/RAM/device/queue state and ordered entries')
        require(result['boundaries'][0]['target']==hex(c['entry']),'normal original function entry')
        if result['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'normal return and stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'original callee-saved integer registers')
            require(q.get(m,0x8c88f69c)==q.get(before,0x8c88f69c) and q.get(m,0x8c88f5f8)==q.get(before,0x8c88f5f8),'independent matrix stack balance')
            if c['entry'] in (q.WALK,q.WRAP):
                for i in range(len(q.graph(c))):
                    a=(q.NODES+i*0x100)&0xffffff;require(m[a:a+0x34]==before[a:a+0x34],'independent immutable model node records')
                def order(index):
                    out=[]
                    while index is not None:
                        flags,child,sibling=q.graph(c)[index]
                        if not flags&8:out.append(hex((c.get('view',0x8c000000))|((q.MESHES+index*0x100)&0xffffff)))
                        if not flags&16:out.extend(order(child))
                        index=sibling
                    return out
                actual=[hex(b['arguments'][0]) for b in result['boundaries'] if b['target']=='0x8c037098']
                require(actual==order(0),'independent mesh order from draw/child/sibling flags and original recursive execution');oracles+=1
            else:
                require(s.r[0] in (0,1),'original boolean movement result')
                for a in (q.ACTOR,q.A,q.B,q.EXTRA):
                    off=a&0xffffff;require(m[off:off+0x240]==before[off:off+0x240],'independent input actor/state/velocity/external records preserved')
                if s.r[0]==0:
                    off=q.QUERY&0xffffff;require(m[off:off+0x84]==before[off:off+0x84],'independent complete132byte query restoration on false result');oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],result=s.r[0],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('MODEL RESPONSE DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if case_id is not None:print('SINGLE DIAGNOSTIC',records[0]['status'],coverage,flush=True);return
    files=[Path(__file__),P/'movement_model_response_fixtures.py',P/'movement_model_response_reference.py',P/'write_watch_movement_model_response.py',q.v.MP,P/'analysis/movement_model_response_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,
      new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},
      scope='Whole original0742d4 movement response and036bc0/036f12recursive model dependency group. Actual matrix/rotation/callback/collision/angle/math calls;initialactor/state/query/world/model inputs only,no suppliedresults/frames. Fullsoftwarestate/orderedboundary comparisons;independent modelmeshorder/nodeimmutability/matrixbalance/queryrestore/inputrecord/returnoracles. SpecialFPU/physicaldelivery/wholegameunqualified;coverage gaps explicitly retained.')
    (P/'analysis/movement_model_response_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MODEL RESPONSE DIRECT PASS',len(records),checks,coverage,flush=True)

if __name__=='__main__':main()
