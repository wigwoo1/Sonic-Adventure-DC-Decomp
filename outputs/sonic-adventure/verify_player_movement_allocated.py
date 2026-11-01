"""Carry original heap/pool/task/constructor outputs into the full movement caller."""
import hashlib,itertools,json
from pathlib import Path
import player_movement_owner_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def phase(base,entry,args,label):
        m,s,f,cfg=base;before=type(s).from_buffer_copy(s);cfg['entry']=s.pc=entry;s.pr=0x8cfed000
        for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status']==0,(label,result['status'],result['stop_pc']))
        require(result['full_state_match'] and result['ordered_boundaries_match'],'complete software state and boundaries')
        require(s.pc==s.pr==0x8cfed000 and s.r[15]==before.r[15],'balanced original return')
        cfg.update(other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
        records.append(dict(phase=label,entry=hex(entry),status=result['status'],steps=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        return (m,s,f,cfg),result
    initial=q.prepare(ram,dict(major=9,minor=2,flags=1,velocity=(0,0,0)))
    initialized,_=phase(initial,0x8c098a10,(),'original_heap_initialization')
    initialized,_=phase(initialized,0x8c09844a,(),'original_task_pool_initialization')
    for mode,rm,index in itertools.product((0,4),(0,1),(0,1)):
        m,s,f,cfg=initialized
        base=bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),q.v.Input(dict(cfg))
        m,s,f,cfg=base;f.fpscr=0x40000|(mode<<19)|rm
        m[0x799920:0x799b20]=bytes(0x200)
        base,_=phase(base,0x8c09852e,[3,2,0,0],dict(kind='original_parent',mode=mode,rm=rm,index=index))
        m,s,f,cfg=base;task=s.r[0];actor=q.get(m,task+0x20);aux=q.get(m,task+0x24)
        q.put(m,actor+9,0,1);q.put(m,actor+8,index,1)
        base,_=phase(base,0x8c06c350,[index,task],dict(kind='original_constructor',mode=mode,rm=rm,index=index))
        m,s,f,cfg=base;state=q.get(m,aux);query=q.get(m,state+0x64)
        require(all(0x8c7ee8e0<=a and a+size<=0x8c87e8e0 for a,size in ((actor,0x40),(aux,0x40),(state,0x200),(query,0x84))),'actual original heap allocations')
        require(len({actor,aux,state,query})==4,'actual allocation base addresses differ')
        q.collision_inputs(m,dict(collision_flags=0),actor=actor)
        q.put(m,actor+4,1,2);q.vector(m,actor+0x20,(0,0,0));q.vector(m,aux+4,(1,0,0))
        base,result=phase(base,q.ENTRY,[actor,aux,state,0],dict(kind='whole_owner_on_actual_allocations',mode=mode,rm=rm,index=index))
        m,s,f,cfg=base;routes=[b for b in result['boundaries'] if b['target']=='0x8c0730d8'];tails=[b for b in result['boundaries'] if b['target']=='0x8c0777e0']
        require(bool(routes) and bool(tails),'real original caller and special tail both execute')
        require(all(b['arguments'][:3]==[actor,aux,state] for b in routes+tails),'actual allocated object/state pointers are forwarded without substitution')
        require(all(int(b['stack'],16)==0x8cf00000-0x4c for b in routes+tails),'original owner frame depth at routing and tail boundaries')
        require(q.get(m,aux)==state and q.get(m,state+0x64)==query,'constructor-owned state/query binding remains live')
        require(q.get(m,0x8c78c668+4*index)==state,'original player state table retains the allocated state')
        require(query+0x84<0x8cf00000-0x4c-0x238,'query allocation does not alias the declared private frame in these runs')
        print('ALLOCATED MOVEMENT',mode,rm,index,'PASS',flush=True)
    files=[Path(__file__),P/'player_movement_owner_fixtures.py',P/'player_movement_path_reference.py',P/'write_watch_player_movement_path.py',P/'analysis/player_movement_path_connection.json',q.v.effects.DLL]
    report=dict(status='actual_original_allocations_forward_through_whole_owner_and_routing',comparisons=len(records),checks=checks,movement_lifecycles=8,cases=records,
                new_original_routine_credit=0,supplied_allocator_results=0,supplied_execution_frames=0,retail_caller_stack_proven=False,caller_allocation_alias_contract_globally_proven=False,
                overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),scope='Original heap->4096-task pool->parent task->complete06c350->full072b44->real0730d8->full0777e0 on carried RAM/state. Collision descriptor,velocity,stage and stack are declared external inputs;host chooses phases. Confirms real allocation pointer forwarding and actual caller frame depths for eight cases,not normal boot/retail stack provenance or universal alias exclusion. Larger owner/routing coverage obligations remain.',sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_movement_allocated_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
    print('ALLOCATED MOVEMENT PASS',len(records),checks,'eight real allocation-to-owner-to-tail lifecycles',flush=True)
if __name__=='__main__':main()
