"""Execute the constructor's actual stored initializer on its real allocated child."""
import hashlib,itertools,json
from pathlib import Path
import player_child_callback_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_child_callback_allocated_verification.json';assert not target.exists()
    records=[];cases=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def clone(base):
        m,s,f,cfg=base
        return bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),q.v.Input(dict(cfg))
    def phase(base,entry,args,label):
        m,s,f,cfg=base;initial_sp=s.r[15];cfg['entry']=s.pc=entry;s.pr=0x8cfed000
        for j,x in enumerate(args):s.r[4+j]=x&0xffffffff
        r,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status']==0,(label,r['status'],r['stop_pc']))
        require(r['full_state_match'] and r['ordered_boundaries_match'],'whole state and ordered boundaries')
        require(s.pc==s.pr==0x8cfed000 and s.r[15]==initial_sp,'balanced original return')
        cfg.update(other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
        records.append(dict(phase=label,entry=hex(entry),status=0,steps=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        return (m,s,f,cfg),r
    initial=q.prepare(q.v.seed_ram(),dict(entry=q.ENTRY,present=False))
    initial,_=phase(initial,0x8c098a10,(),'original heap initialization')
    initial,_=phase(initial,0x8c09844a,(),'original task pool initialization')
    table=[2,7,0,3,5]
    for n,(kind,index) in enumerate(itertools.product(range(8),(0,1))):
        mode=4*(n%2);rm=(n//2)%2
        base=clone(initial);m,s,f,cfg=base;f.fpscr=0x40000|(mode<<19)|rm
        m[0x799920:0x799b20]=bytes(0x200)
        q.put(m,q.FALLBACK_KIND,kind,2)
        label=dict(kind=kind,index=index,mode=mode,rm=rm)
        base,_=phase(base,0x8c09852e,[3,2,0,0],dict(label,phase='original parent allocation'))
        m,s,f,cfg=base;parent=s.r[0];actor=q.get(m,parent+0x20)
        q.put(m,actor+8,index,1);q.put(m,actor+9,kind,1)
        base,_=phase(base,0x8c06c350,[index,parent],dict(label,phase='original player constructor'))
        m,s,f,cfg=base;levels=q.get(m,0x8c0986c8);child=q.get(m,levels+8);child_actor=q.get(m,child+0x20);callback=q.get(m,child+0x10)
        require(child!=parent and callback==q.ENTRY,'actual spawned child stores original initializer pointer')
        require(0x8c7ee8e0<=child_actor and child_actor+0x40<=0x8c87e8e0,'actual child actor allocation in initialized heap')
        require(q.get(m,child_actor+0x34)==q.bits(index),'actual constructor stores floating player index')
        primary=q.get(m,q.PLAYER_TABLE)
        require(primary==(actor if index==0 else 0),'actual player0 table binding or absent primary slot')
        base,r=phase(base,callback,[child],dict(label,phase='actual stored child initializer'))
        m,s,f,cfg=base
        selected=q.CHARACTER_TABLE+0xd8*table.index(kind) if kind in table else 0
        require([q.get(m,child+o) for o in (0x10,0x14,0x18)]==[0x8c0b3b1a,0x8c0b3f2c,0x8c0b3afc],'original callback transition on actual child')
        require(q.get(m,child_actor+8)==selected,'independent table selection on actual child actor')
        require(q.get(m,child_actor+0x34)==q.bits(index),'original floating player index remains live')
        require(q.get(m,child_actor+1,1)==q.get(m,child_actor+3,1)==q.get(m,child_actor+6,2)==q.get(m,child_actor+0x10)==0,'actual original reset fields')
        queries=[b for b in r['boundaries'] if b['target']=='0x8c07ce40'];fallback=[b for b in r['boundaries'] if b['target']=='0x8c04f698']
        require(len(queries)==(table.index(kind)+1 if kind in table else 5) and all(b['arguments'][0]==0 for b in queries),'actual player0 query and original search count')
        require(len(fallback)==(len(queries) if index==1 else 0),'actual absent-player0 fallback path')
        require(not any(b['target']=='0x8c072b44' for b in r['boundaries']),'initializer does not invoke the movement owner')
        cases.append(dict(input=label,parent=hex(parent),child=hex(child),child_actor=hex(child_actor),actual_initial_callback=hex(callback),installed_callbacks=[hex(q.get(m,child+o)) for o in(0x10,0x14,0x18)],selected_table=hex(selected),character_source='primary actor byte' if index==0 else 'declared global fallback'))
        print('ALLOCATED CHILD',kind,index,'PASS',flush=True)
    files=[Path(__file__),P/'player_child_callback_fixtures.py',P/'player_child_callback_dependencies_reference.py',P/'write_watch_player_child_callback_dependencies.py',P/'analysis/player_child_callback_translation.json',P/'analysis/player_child_callback_dependencies_connection.json',q.v.effects.DLL]
    report=dict(status='actual_constructor_child_initializer_and_callback_transition_match',original_native_comparisons=len(records),checks=checks,allocated_child_lifecycles=len(cases),cases=cases,phases=records,supplied_allocator_results=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Actual original heap/pool/parent/constructor state is carried into the actual stored0b3a2ecallback on its allocated child. Eight character IDs and two player indices use four retained bank/rounding combinations. Primary actor table binding is original constructor output;absent player0 uses a declared global character input and executes the real getter. Exact callback transitions,table pointer,reset fields,index retention,query counts and fallback membership checked. Host phase order and character inputs are declared;subsequent assigned callbacks,retail scheduler order and movement startup invariants are not qualified.',sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('ALLOCATED CHILD COMPLETE',len(records),checks,len(cases),'actual child lifecycles',flush=True)
if __name__=='__main__':main()
