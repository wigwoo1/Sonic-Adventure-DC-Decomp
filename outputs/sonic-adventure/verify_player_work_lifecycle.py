"""Full original/native constructor lifecycles with independent field/heap checks."""
import collections
import hashlib
import itertools
import json
import sys
from pathlib import Path
import player_work_lifecycle_fixtures as q

P=q.P
FLAGS={0:((0x8d,2),(0x8e,1),(0x9f,4),(0xcd,8),(0xdb,16)),
       2:((0xcd,8),(0xdb,16)),3:((0x119,0x40),(0x11a,0x20)),
       5:((0x152,0x100),(0x15e,0x80)),
       6:((0x189,0x400),(0x18a,0x200),(0x1d0,0x1000),(0x1d1,0x800)),
       7:((0x1d0,0x1000),(0x1d1,0x800))}

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])

def configurations():
    for kind,mode,rm,enabled in itertools.product(range(8),(0,4),(0,1),(False,True)):
        yield dict(kind=kind,mode=mode,rm=rm,flags=[x for x,_ in FLAGS.get(kind,())] if enabled else [],
                   index=int(enabled),actor_mode=7 if enabled else 0)
    for kind,pairs in FLAGS.items():
        for flag,_ in pairs:
            for mode,rm in itertools.product((0,4),(0,1)):
                yield dict(kind=kind,mode=mode,rm=rm,flags=[flag],index=1,actor_mode=0)

def main():
    ram=q.v.seed_ram();records=[];seen=set();edges=set();checks=0
    manifests=[P/f'analysis/{name}_translation.json' for name in
               ('player_work_initialize','player_work_dependencies','player_work_allocation')]
    bodies=[r for file in manifests for r in json.loads(file.read_text())['routines']]
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def execute(base,entry,args,label):
        m,s,f,cfg=base;initial=type(s).from_buffer_copy(s)
        r,path,m,s,f=q.invoke(m,s,f,cfg,entry,args)
        require(r['status']==0,(label,'normal original-entry completion',r['status'],r['stop_pc']))
        require(r['full_state_match'] and r['ordered_boundaries_match'],(label,'complete software state/boundaries'))
        require(s.pc==s.pr==q.RETURN and s.r[15]==initial.r[15],(label,'balanced return'))
        require(list(s.r[8:15])==list(initial.r[8:15]),(label,'callee-saved integer registers'))
        seen.update(path)
        for i,a in enumerate(path):
            text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        records.append(dict(input=label,entry=hex(entry),status=r['status'],steps=len(path),
                            boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        return (m,s,f,cfg),r

    base,r=execute(q.fresh(ram),q.HEAP_INIT,(),'original_fixed_heap_initialization')
    m,s,f,cfg=base;heap=q.get(m,0x8c098a68);extent=q.get(m,0x8c098a70);end=heap+extent
    require(heap==0x8c7ee8e0 and extent==0x90000,'original unmodified heap bounds')
    require(q.get(m,q.get(m,0x8c098a6c))==heap,'original free-list root')
    require([q.get(m,heap+4*j) for j in range(4)]==[end,0,end,0],'first free block contract')
    require([q.get(m,end+4*j) for j in range(4)]==[0,heap,0,heap],'terminal sentinel contract')
    require(sample(m,heap+16,extent-16)==bytes([255])*(extent-16),'original heap byte fill')
    base,r=execute(base,q.POOL_INIT,(),'original_4096_task_pool_initialization')
    m,s,f,cfg=base;pool=q.get(m,q.get(m,0x8c0985ac));levels=q.get(m,0x8c0986c8)
    require(heap<=pool and pool+0x30000<=end,'actual pool allocated inside original heap')
    require(all(q.get(m,levels+4*j)==0 for j in range(8)),'original empty scheduler levels')
    for j in range(4096):
        task=pool+0x30*j
        require(q.get(m,task)==(task+0x30 if j<4095 else 0),'original complete free-task forward chain')
        require(q.get(m,task+4)==(task-0x30 if j else 0),'original complete free-task backward chain')
    initialized=q.clone(base)
    selected=list(configurations())
    if '--smoke' in sys.argv:selected=[selected[i] for i in (0,1,8,16,24,32,40,48,56)]
    for n,c in enumerate(selected):
        base=q.clone(initialized);m,s,f,cfg=base
        f.fpscr=0x40000|(c['mode']<<19)|c['rm'];cfg['mode']=c['mode']
        m[0x799920:0x799b20]=bytes(0x200)
        for flag in c['flags']:q.put(m,0x8c799920+flag,0x80 if n%2 else 1,1)
        base,_=execute(base,q.CREATE,[3,2,0,0],dict(phase='original_parent_creation',case=c))
        m,s,f,cfg=base;task=s.r[0];actor=q.get(m,task+0x20);aux=q.get(m,task+0x24)
        require(heap<=actor<end and heap<=aux<end,'original parent actor/aux allocation')
        q.put(m,actor+8,c['index'],1);q.put(m,actor+9,c['kind'],1);q.put(m,actor+2,c['actor_mode'],1)
        initial_fpu=bytes(f);old_fr15=int(f.banks[(f.fpscr>>21)&1][15])
        parameters=sample(m,q.get(m,0x8c06c4a0)+132*c['kind'],132)
        base,result=execute(base,q.INIT,[c['index'],task],dict(phase='complete_player_constructor',case=c))
        m,s,f,cfg=base;state=q.get(m,aux);query=q.get(m,state+0x64);character=q.get(m,actor+0x3c);record_pool=q.get(m,state+0x78)
        allocations=[(actor,0x40),(aux,0x40),(state,0x200),(query,0x84),(character,0x7c),(record_pool,15*0x20)]
        require(all(heap<=a and a+size<=end for a,size in allocations),'actual constructor allocations inside original heap')
        require(all(a+size<=b or b+length<=a for i,(a,size) in enumerate(allocations) for b,length in allocations[i+1:]),'independent live allocation disjointness')
        require(end<q.STACK-0x1000,'these original allocations are below declared caller stack')
        require(sample(m,query,0x84)==bytes(0x84),'actual zeroed query allocation')
        require(sample(m,character,0x7c)==bytes([3])+bytes(0x7b),'actual character auxiliary type3 and zeroed payload')
        require(sample(m,record_pool,15*0x20)==bytes(15*0x20),'actual fifteen-entry record-pool allocation')
        require(sample(m,state+0x9c,132)==parameters,'independent original parameter-table copy')
        require(q.get(m,state+0x120,2)==q.get(m,state+0x122,2)==q.get(m,state+0x12,2)==0,'original state reset fields')
        require(q.get(m,state+0x138)==state+0x38 and q.get(m,state+0x13c)==state,'original self pointers')
        expected=sum(mask for flag,mask in FLAGS.get(c['kind'],()) if flag in c['flags'])
        require(q.get(m,state+4,2)==expected,'independent character progression-bit mask')
        require(q.get(m,aux+0x3c)==0x3f800000,'original auxiliary unit scale')
        require(q.get(m,actor+2,1)==(c['actor_mode'] or 1),'original default actor mode')
        require(all(q.get(m,actor+off)==0x3f800000 for off in (0x2c,0x30,0x34)),'original actor unit scales')
        for literal,value in ((0x8c06c480,state),(0x8c06c484,aux),(0x8c06c488,actor),(0x8c06c48c,task)):
            require(q.get(m,q.get(m,literal)+4*c['index'])==value,'independent four player table bindings')
        child=q.get(m,levels+8);child_actor=q.get(m,child+0x20)
        require(child!=task and q.get(m,child+0x10)==0x8c0b3a2e,'actual spawned child and stored original callback')
        require(q.get(m,child_actor+0x34)==(0x3f800000 if c['index'] else 0),'original floating player index in child')
        require(int(f.banks[(f.fpscr>>21)&1][15])==old_fr15,'original saved floating register restored')
        targets=[b['target'] for b in result['boundaries']]
        require(all(hex(x) in targets for x in (q.AUXILIARY,q.RECORDS,q.CREATE,0x8c098b0c,0x8c098a80)),'actual allocation/helper/task dependencies execute')
        if n==0:
            before=bytes(m);base,_=execute(base,q.AUXILIARY,[actor],'existing_auxiliary_reused')
            require(q.get(base[0],actor+0x3c)==character and sample(base[0],character,0x7c)==sample(before,character,0x7c),'nonnull auxiliary retains pointer and payload')
        if (n+1)%16==0:print('PLAYER WORK LIFECYCLES',n+1,'/',len(selected),'checks',checks,flush=True)
    coverage=[]
    for body in bodies:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(x) for x in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=sorted(map(hex,wanted-seen)),missing_outcomes=[list(x) for x in sorted(outcomes-edges)]))
    if '--smoke' in sys.argv:
        print('PLAYER WORK SMOKE PASS',len(records),checks,[(x['address'],x['executed_rows'],x['executed_outcomes']) for x in coverage],flush=True);return
    files=[Path(__file__),P/'player_work_lifecycle_fixtures.py',P/'write_watch_player_work_allocation.py',P/'player_work_allocation_reference.py',*manifests,q.v.effects.DLL]
    report=dict(status='software_original_native_lifecycle_comparisons_passed',comparisons=len(records),independent_checks=checks,constructor_cases=len(selected),cases=records,coverage_by_routine=coverage,
                actual_heap_start=hex(heap),actual_heap_end_exclusive=hex(end),actual_pool_start=hex(pool),declared_caller_stack=hex(q.STACK),
                supplied_allocation_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,overall_completion_percent=10,
                scope='Original fixed heap initialization,4096-task pool construction,parent task creation and complete player constructors on unchanged carried state. Progression flags/character/index and caller stack are declared inputs. Full software RAM/CPU/FPU/device state and ordered boundaries plus independent allocations/tables/fields/flags checks. Host chooses calls between entries. Stored child callback is not executed. No physical hardware,normal complete boot,universal stack-alias contract or playable-game qualification.',
                sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_work_lifecycle_verification.json';assert not target.exists(),'Preserve earlier evidence;write a new revision'
    target.write_text(json.dumps(report,indent=2)+'\n')
    print('PLAYER WORK LIFECYCLE PASS',len(records),checks,[(x['address'],x['executed_rows'],x['executed_outcomes'],len(x['missing_rows']),len(x['missing_outcomes'])) for x in coverage],flush=True)

if __name__=='__main__':main()
