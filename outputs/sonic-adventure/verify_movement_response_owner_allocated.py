"""Real allocation->constructor->normal initialization->owner response lifecycles."""
import hashlib,itertools,json
from pathlib import Path
import player_collision_response_fixtures as q
import write_watch_movement_response_owner as fast
P=q.P;OWNER=0x8c072b44
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/movement_response_owner_allocated_verification.json';assert not target.exists()
    ram=q.v.seed_ram();records=[];cases=[];seen=set();edges=set();checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def clone(base):
        m,s,f,cfg=base
        return bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),q.v.Input(dict(cfg))
    def phase(base,entry,args,label,expected_status=0):
        m,s,f,cfg=base;before=type(s).from_buffer_copy(s);cfg['entry']=s.pc=entry;s.pr=0x8cfed000
        for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
        result,path,m,s,f=fast.compare(m,s,f,cfg)
        require(result['status']==expected_status,(label,result['status'],result['stop_pc']))
        require(result['full_state_match'] and result['ordered_boundaries_match'],'whole state and ordered boundaries')
        if expected_status==0:require(s.pc==s.pr==0x8cfed000 and s.r[15]==before.r[15],'balanced original return')
        else:require(result['stop_pc']=='0x8c639b68','cold constructor normal reaches retained numerical frontier')
        cfg.update(other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        records.append(dict(phase=label,entry=hex(entry),status=result['status'],stop_pc=result['stop_pc'],steps=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        return (m,s,f,cfg),result
    def scene(base,actor,aux,flags):
        m,s,f,cfg=base
        q.collision_inputs(m,dict(collision_flags=0x88,shape_type=flags,shape_position=(1,0,0)),actor=actor)
        q.put(m,actor+4,1,2);q.vector(m,aux+4,(2,0,0))
        return base
    initialized=q.prepare(ram,dict(entry=OWNER,major=9,minor=2,flags=1,velocity=(0,0,0)))
    initialized,_=phase(initialized,0x8c098a10,(),'original heap initialization')
    initialized,_=phase(initialized,0x8c09844a,(),'original task pool initialization')
    for mode,rm,index in itertools.product((0,4),(0,1),(0,1)):
        base=clone(initialized);m,s,f,cfg=base;f.fpscr=0x40000|(mode<<19)|rm
        m[0x799920:0x799b20]=bytes(0x200)
        label=dict(mode=mode,rm=rm,index=index)
        base,_=phase(base,0x8c09852e,[3,2,0,0],dict(label,kind='original parent allocation'))
        m,s,f,cfg=base;task=s.r[0];actor=q.get(m,task+0x20);aux=q.get(m,task+0x24)
        q.put(m,actor+9,0,1);q.put(m,actor+8,index,1)
        base,_=phase(base,0x8c06c350,[index,task],dict(label,kind='original player constructor'))
        m,s,f,cfg=base;state=q.get(m,aux);query=q.get(m,state+0x64)
        live=[(actor,0x40),(aux,0x40),(state,0x200),(query,0x84)]
        require(all(0x8c7ee8e0<=a and a+n<=0x8c87e8e0 for a,n in live),'actual live allocations inside initialized heap')
        require(all(a+n<=b or b+k<=a for i,(a,n) in enumerate(live) for b,k in live[i+1:]),'actual live intervals are disjoint in these cases')
        require(all(a+n<0x8cf00000-0x4c-0x88-0x238 for a,n in live),'actual allocations do not overlap declared nested private frames')
        require([q.get(m,state+0x50+4*j) for j in range(3)]==[0,0,0],'original constructor leaves old normal at zero')
        cold=scene(clone(base),actor,aux,0x801)
        cold,r=phase(cold,OWNER,[actor,aux,state,0],dict(label,kind='cold allocated owner response'),-1)
        cold_calls=[b for b in r['boundaries'] if b['target']=='0x8c072946']
        require(len(cold_calls)==1 and cold_calls[0]['owner_response_input']['old_normal']==[0,0,0],'cold actual caller reaches response with constructor normal')
        warm=clone(base);m,s,f,cfg=warm
        q.collision_inputs(m,dict(collision_flags=0),actor=actor)
        q.put(m,actor+4,2,2);q.vector(m,actor+0x20,(0,0,0));q.vector(m,aux+4,(0,0,0))
        warm,r=phase(warm,OWNER,[actor,aux,state,0],dict(label,kind='original movement initializes old normal'))
        m,s,f,cfg=warm
        require([q.get(m,state+0x50+4*j) for j in range(3)]==[q.bits(0),q.bits(1),q.bits(0)],'actual original movement writes up normal;no state output supplied')
        require(not any(b['target']=='0x8c072946' for b in r['boundaries']),'initializing update does not execute collision response')
        yes=scene(clone(warm),actor,aux,0x801);no=scene(clone(warm),actor,aux,1)
        changed=[i for i,(a,b) in enumerate(zip(yes[0],no[0])) if a!=b]
        require(changed==[(q.SHAPES&0xffffff)+1+0x34*j for j in range(4)],'paired scene inputs differ only by four shape0x800 bits')
        for base,enabled in ((yes,True),(no,False)):
            base,r=phase(base,OWNER,[actor,aux,state,0],dict(label,kind='initialized allocated owner',shape_flag_800=enabled))
            m,s,f,cfg=base;calls=[b for b in r['boundaries'] if b['target']=='0x8c072946']
            require(len(calls)==int(enabled),'shape flag controls actual owner response call in this declared state')
            if enabled:
                call=calls[0];actual=call['owner_response_input']
                require(call['pr']=='0x8c072f5c' and int(call['stack'],16)==0x8cf00000-0x4c,'actual072f58 call and owner frame')
                require(call['arguments'][:3]==[actor,aux,state],'real allocated arguments forwarded unchanged')
                require(actual['collision_flags']==0x88 and actual['actor_flags']==1 and actual['selected_shape_flags']==0x801,'actual response flag inputs')
                require(actual['info']==q.INFO and actual['provider']==q.PROVIDER and actual['shapes']==q.SHAPES and actual['index']==0 and actual['count']==4,'declared valid collision structures reach actual caller')
                require(actual['old_normal']==[q.bits(0),q.bits(1),q.bits(0)],'normal produced by the prior original update remains live')
                require(actual['velocity']==[q.bits(2),q.bits(0),q.bits(0)],'declared incoming motion reaches response')
            require(q.get(m,aux)==state and q.get(m,state+0x64)==query,'actual constructor-owned state/query binding persists')
            require(q.get(m,0x8c78c668+4*index)==state,'original player state table retains actual allocation')
        cases.append(dict(mode=mode,rm=rm,index=index,task=hex(task),actor=hex(actor),aux=hex(aux),state=hex(state),query=hex(query),cold_status=-1,initialized_response_status=0,control_status=0))
        print('ALLOCATED RESPONSE',mode,rm,index,'PASS',flush=True)
    coverage=[]
    for prefix,selected in (('player_movement_owner',{'0x8c072b44'}),('player_movement_dependencies',{'0x8c072946'}),('player_movement_route',{'0x8c0730d8'})):
        t=json.loads((P/f'analysis/{prefix}_translation.json').read_text())
        for b in t['routines']:
            if b['address'] not in selected:continue
            wanted=set(b['reachable_addresses']);outcomes={tuple(x) for x in b['conditional_edges']}
            missing=wanted-{hex(a) for a in seen};missed=outcomes-edges
            coverage.append(dict(address=b['address'],static_rows=len(wanted),executed_rows=len(wanted)-len(missing),static_outcomes=len(outcomes),executed_outcomes=len(outcomes)-len(missed),missing_rows=sorted(missing),missing_outcomes=[list(x) for x in sorted(missed)]))
    require(0x8c072f58 in seen and 0x8c072e84 not in seen,'feasible call observed;contradictory-gate call not observed')
    files=[Path(__file__),P/'player_collision_response_fixtures.py',P/'write_watch_movement_response_owner.py',P/'write_watch_collision_response_contracts.py',P/'analysis/player_movement_owner_translation.json',P/'analysis/player_movement_dependencies_translation.json',P/'analysis/player_movement_route_translation.json',q.v.effects.DLL]
    report=dict(status='actual_allocated_owner_response_normal_initialization_and_shape_flag_contract_matches',original_native_comparisons=len(records),checks=checks,lifecycles=8,cases=cases,phases=records,coverage_by_routine=coverage,
        actual_owner_response_calls=16,normal_initialized_owner_response_returns=8,matched_cold_numeric_frontiers=8,shape_flag_control_returns=8,supplied_allocator_results=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,old_normal_directly_seeded=False,
        new_original_routine_credit=0,owner_registered=False,response_helper_registered=False,retail_collision_validity_invariant_proven=False,retail_stack_proven=False,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Eight genuine heap/task/constructor/movement lifecycles on carried state across two player indices and four retained mode/rounding combinations. Cold constructor normal stays zero and reaches matching639b68 stop. An actual flags2/no-motion movement update produces up normal;subsequent initialized shape0x800 cases call072946at072f58and return normally. Paired control input differs only by four shape flag bits. Allocated intervals and declared nested stack are disjoint in these cases. Collision scene data and host phase order are declared;this does not establish every retail collision producer or universal stack/alias/initialization invariant. Larger owner/routing coverage remains partial.',
        sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('ALLOCATED OWNER RESPONSE',len(records),checks,'eight lifecycles;coverage',[(x['address'],x['executed_rows'],x['executed_outcomes']) for x in coverage],flush=True)
if __name__=='__main__':main()
