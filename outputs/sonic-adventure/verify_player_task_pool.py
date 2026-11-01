"""Original task-pool success and naturally exhausted heap, on unchanged native code."""
import hashlib,json
from pathlib import Path
import player_work_lifecycle_fixtures as q
P=q.P
ALLOCATE=0x8c098a80
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])
def main():
    records=[];seen=set();edges=set();checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def execute(base,entry,args,label):
        m,s,f,c=base;before=type(s).from_buffer_copy(s);fp=bytes(f)
        r,path,m,s,f=q.invoke(m,s,f,c,entry,args)
        require(r['status']==0,(label,r['status'],r['stop_pc']))
        require(r['full_state_match'] and r['ordered_boundaries_match'],(label,'full software state and boundaries'))
        require(s.pc==s.pr==q.RETURN and s.r[15]==before.r[15],(label,'return and stack'))
        require(list(s.r[8:15])==list(before.r[8:15]),(label,'callee saved registers'))
        require(bytes(f)==fp,(label,'all physical FPU banks and FPSCR untouched'))
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        records.append(dict(input=label,entry=hex(entry),status=r['status'],steps=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        return m,s,f,c
    initialized=execute(q.fresh(q.v.seed_ram()),q.HEAP_INIT,(),'original_fixed_heap_initialization')
    m,s,f,c=initialized;heap=q.get(m,0x8c098a68);end=heap+q.get(m,0x8c098a70)
    for mode in range(8):
        base=q.clone(initialized);m,s,f,c=base
        f.fpscr=0x40000|(mode<<19)|(mode%4)
        s.sr=(0,0x400000a1,0x600000a1,0x400080a1)[mode%4];c['sr']=s.sr
        base=execute(base,q.POOL_INIT,(),dict(phase='successful_pool_initialization',mode=mode))
        m,s,f,c=base;pool=q.get(m,q.get(m,0x8c0985ac));levels=q.get(m,0x8c0986c8)
        require(heap<=pool and pool+0x30000<=end,'pool allocation lies in the actual original heap')
        require(q.get(m,q.get(m,0x8c0985a8))==pool,'both original pool globals bind the allocation')
        expected=bytearray(0x30000)
        for j in range(4096):
            at=0x30*j
            expected[at:at+4]=(pool+at+0x30 if j<4095 else 0).to_bytes(4,'little')
            expected[at+4:at+8]=(pool+at-0x30 if j else 0).to_bytes(4,'little')
        require(sample(m,pool,0x30000)==expected,'all4096 complete records,forward/back links and zeroed fields')
        require(sample(m,levels,32)==bytes(32),'all eight scheduler heads cleared')
        base=execute(base,q.CREATE,[3,2,0,0],dict(phase='original_parent_creation',mode=mode))
        m,s,f,c=base;parent=s.r[0]
        require(q.get(m,levels+8)==parent and pool<=parent<pool+0x30000,'real allocated parent creates a nonempty scheduler level')
        root=q.get(m,0x8c098b60);free=q.get(m,root);terminal=q.get(m,free+8)
        request=terminal-free-16
        require(heap<=free<terminal==end and request>0 and request%16==0,'actual final free block supplies a whole-block allocation request')
        base=execute(base,ALLOCATE,[request],dict(phase='original_allocator_consumes_remaining_heap',mode=mode,request=request))
        m,s,f,c=base
        require(s.r[0]==free+16 and q.get(m,root)==end,'original allocation consumes the actual final free block')
        before_pool=sample(m,pool,0x30000);before_levels=sample(m,levels,32)
        base=execute(base,q.POOL_INIT,(),dict(phase='naturally_exhausted_pool_initialization',mode=mode))
        m,s,f,c=base
        require(s.r[0]==0,'original exhausted allocator returns null through the initializer')
        require(q.get(m,q.get(m,0x8c0985a8))==q.get(m,q.get(m,0x8c0985ac))==0,'original failure still clears both pool globals')
        require(sample(m,levels,32)==before_levels and q.get(m,levels+8)==parent,'original failure retains the existing nonempty scheduler heads')
        require(sample(m,pool,0x30000)==before_pool,'original failure retains the allocated task records')
        require(q.get(m,root)==end,'failed allocation leaves the exhausted free-list sentinel')
        print('PLAYER TASK POOL PROFILE',mode,'PASS',flush=True)
    t=json.loads((P/'analysis/player_work_allocation_translation.json').read_text());body=next(x for x in t['routines'] if x['address']==hex(q.POOL_INIT))
    wanted=set(body['reachable_addresses']);outcomes={tuple(x) for x in body['conditional_edges']}
    missing=wanted-{hex(x) for x in seen};missed=outcomes-edges
    require(missing=={'0x8c098496','0x8c0984ba','0x8c0984de'},'only the three fixed-phase rows remain unobserved')
    require(missed=={('0x8c09846e','0x8c09847e'),('0x8c098492','0x8c098496'),('0x8c0984b6','0x8c0984ba'),('0x8c0984da','0x8c0984de')},'only four fixed-constant loop outcomes remain unobserved')
    files=[Path(__file__),P/'player_work_lifecycle_fixtures.py',P/'player_work_allocation_reference.py',P/'write_watch_player_work_allocation.py',P/'analysis/player_work_allocation_translation.json',q.v.effects.DLL]
    report=dict(status='original_task_pool_success_and_natural_heap_exhaustion_match',comparisons=len(records),checks=checks,profiles=8,cases=records,
                coverage=dict(address=hex(q.POOL_INIT),static_rows=len(wanted),executed_rows=len(wanted)-len(missing),static_outcomes=len(outcomes),executed_outcomes=len(outcomes)-len(missed),missing_rows=sorted(missing),missing_outcomes=[list(x) for x in sorted(missed)]),
                actual_heap_start=hex(heap),actual_heap_end_exclusive=hex(end),natural_original_allocator_exhaustion_cases=8,supplied_allocator_results=0,supplied_execution_frames=0,
                new_original_routine_credit=0,overall_completion_percent=10,
                scope='Original heap initialization followed by eight carried-state profiles. Each original pool success creates all4096records,then original parent creation gives a nonempty scheduler level. The original allocator consumes its remaining free block before the initializer executes its actual null-allocation path. No free-head,callee result or frame is supplied. Host call order and caller stack are declared;this is not a normal complete boot or physical hardware proof. Counter-infeasibility qualification is separate.',
                sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_task_pool_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
    print('PLAYER TASK POOL PASS',len(records),checks,'111observed rows/12outcomes;4source-proof obligations',flush=True)
if __name__=='__main__':main()
