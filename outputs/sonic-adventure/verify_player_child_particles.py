"""Original particle creation/list drawing and actual texture-loading frontier."""
import hashlib,itertools,json,struct
from pathlib import Path
import player_child_particles_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_child_particles_verification.json';assert not target.exists()
 records=[];cases=[];frontiers=[];seen=set();edges=set();checks=oracles=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 def clone(base):
  m,s,f,c=base
  return bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),q.v.Input(dict(c))
 def phase(base,entry,args,label,count=None,expected_status=0):
  m,s,f,c=base;sp=s.r[15];c['entry']=s.pc=entry;s.pr=0x8cfed000
  for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
  if count is not None:q.put(m,sp,count)
  first_saved=9 if entry==0x8c604ba0 else 8;saved=list(s.r[first_saved:15])
  result,path,m,s,f=q.fast.compare(m,s,f,c)
  require(result['status']==expected_status,(label,result['status'],result['stop_pc']))
  require(result['full_state_match'] and result['ordered_boundaries_match'],'complete exact state and ordered boundaries')
  if not expected_status:
   require(s.r[15]==sp and s.pc==s.pr==0x8cfed000,'ordinary balanced original return')
   require(list(s.r[first_saved:15])==saved,'original callee-saved integer registers')
  c.update(other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
  seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),status=result['status'],stop_pc=result['stop_pc'],steps=len(path),boundaries=result['boundaries'],command_packets=result['command_packets'],full_state_match=True,ordered_boundaries_match=True))
  return (m,s,f,c),result
 def rng(m,n):
  seed_address=q.get(m,0x8c10d358);seed=q.get(m,seed_address);mult=q.get(m,0x8c10d35c);add=q.get(m,0x8c10d352,2)
  for _ in range(n):seed=(seed*mult+add)&0xffffffff
  return seed_address,seed
 def product(a,b,rm):
  value=q.number(a)*q.number(b);out=q.bits(value)
  if rm and abs(q.number(out))>abs(value):out-=1
  return out
 def chain(m,actor):
  head=q.get(m,actor+0x10);nodes=[]
  while head:
   require(head not in nodes and len(nodes)<32,'finite genuine particle list')
   nodes.append(head);head=q.get(m,head+0x30)
  return nodes
 def spawn(base,actor,height,flags,label,success):
  m,s,f,c=base;index=int(q.number(q.get(m,actor+0x34)));state=q.get(m,q.STATE_TABLE+4*index)
  q.put(m,state+0x1f4,flags);q.put(m,state+0x1ec,q.bits(height))
  oldhead=q.get(m,actor+0x10);address,expected=rng(m,7 if success else 0)
  base,result=phase(base,q.SPAWN,[actor],label);m,s,f,c=base
  exact(q.get(m,address)==expected,'independent seven-step LCG or unchanged rejected RNG seed')
  calls=[b for b in result['boundaries'] if b['target']=='0x8c10d334']
  exact(len(calls)==(7 if success else 0),'original random-call count')
  node=s.r[0]
  if success:
   exact(node!=0 and q.get(m,actor+0x10)==node,'actual spawned record becomes list head')
   exact(q.get(m,node+0x2c)==0 and q.get(m,node+0x30)==oldhead,'independent new record links')
   if oldhead:exact(q.get(m,oldhead+0x2c)==node,'independent old head backlink')
   exact(q.get(m,node)==q.bits(height),'original record copies declared scene height')
  else:exact(node==0 and q.get(m,actor+0x10)==oldhead,'original rejection/failure preserves list')
  return base,node
 # Prepare the declared identity matrix in the correct opposite FPU bank for
 # each initial FR/RM input. Changing FR after preparation would select the
 # fixture's patterned scratch bank as the matrix instead of the declared one.
 initials={};ram=q.v.seed_ram()
 for mode,rm in itertools.product((0,4),(0,1)):
  initial=q.prepare(ram,dict(kind=0,real_tables=True,mode=mode,rm=rm))
  require(initial[2].fpscr==0x40000|(mode<<19)|rm,'declared initial FPU mode')
  initial,_=phase(initial,0x8c098a10,[],dict(mode=mode,rm=rm,phase='original heap initialization'))
  initial,_=phase(initial,0x8c09844a,[],dict(mode=mode,rm=rm,phase='original task pool initialization'))
  initials[mode,rm]=initial
 for n,(index,mode,rm) in enumerate(itertools.product((0,1),(0,4),(0,1))):
  base=clone(initials[mode,rm]);m,s,f,c=base;m[0x799920:0x799b20]=bytes(0x200)
  q.put(m,q.FALLBACK_KIND,0,2);q.vector(m,q.SHAPES+8,(0,1,0));label=dict(index=index,mode=mode,rm=rm)
  base,_=phase(base,0x8c09852e,[3,2,0,0],dict(label,phase='original parent allocation'))
  m,s,f,c=base;parent=s.r[0];primary=q.get(m,parent+0x20);q.put(m,primary+8,index,1);q.put(m,primary+9,0,1)
  base,_=phase(base,0x8c06c350,[index,parent],dict(label,phase='original player construction'))
  base,_=phase(base,q.ATTACH,[parent,q.SHAPES,2,0],dict(label,phase='original collision attachment'),count=1)
  m=base[0];child=q.get(m,q.get(m,0x8c0986c8)+8);actor=q.get(m,child+0x20)
  base,_=phase(base,q.get(m,child+0x10),[child],dict(label,phase='actual stored initializer'))
  ready=clone(base)
  for height,flags,reason in ((100,0,'scene flag absent'),(-1000,2,'below lower height gate'),(1000,2,'beyond upper height gate')):
   rejected,_=spawn(clone(ready),actor,height,flags,dict(label,phase=reason),False)
  base,node1=spawn(base,actor,100,2,dict(label,phase='original first particle spawn'),True)
  base,node2=spawn(base,actor,100,2,dict(label,phase='original second particle spawn'),True)
  m=base[0];nodes=chain(m,actor);exact(nodes==[node2,node1],'independent two-particle order')
  expected=[]
  for node in nodes:
   word=q.get(m,node+0x1c,2);word=word if word<0x8000 else word|0xffff0000
   expected.append([q.get(m,node+o) for o in (4,8,12)]+[q.get(m,node+0x24),product(q.get(m,node+0x20),q.get(m,node+0x24),rm),word,q.get(m,0x8c0b4008),q.get(m,0x8c0b400c)])
  base,result=phase(base,q.get(m,child+0x14),[child],dict(label,phase='stored two-particle clipped draw'))
  calls=[b for b in result['boundaries'] if b['target']=='0x8c09e778']
  exact([b['particle_sprite_input'] for b in calls]==expected,'independent original packet positions,dimensions,word andstatic headers')
  exact(all(b['arguments'][1:3]==[0,0x31] for b in calls),'independent sprite index andflags')
  m=base[0];before=[bytes(m[a&0xffffff:(a&0xffffff)+0x34]) for a in nodes];q.put(m,q.RENDER_FLAG,1)
  base,result=phase(base,q.get(m,child+0x14),[child],dict(label,phase='stored suppressed nonempty draw'))
  exact(not any(b['target']=='0x8c09e778' for b in result['boundaries']),'positive render flag suppresses sprite calls')
  exact([bytes(base[0][a&0xffffff:(a&0xffffff)+0x34]) for a in nodes]==before,'draw suppression preserves complete particles')
  q.put(base[0],q.RENDER_FLAG,0)
  base,_=phase(base,q.get(base[0],child+0x10),[child],dict(label,phase='actual nonempty update'))
  base,_=phase(base,q.get(base[0],child+0x18),[child],dict(label,phase='actual cleanup'))
  exact(q.get(base[0],actor+0x10)==0,'cleanup consumes actual spawned/updated list')
  base,result=phase(base,q.DRAW,[child],dict(label,phase='empty draw'))
  exact(not any(b['target']=='0x8c09e778' for b in result['boundaries']),'empty list makes no sprite calls')
  # Actual exhaustion from the ready lifecycle, with the genuine allocator's
  # remaining free extent consumed rather than a fabricated allocation result.
  pressure=clone(ready);m=pressure[0];free=q.get(m,q.get(m,0x8c098b60));end=q.get(m,free+8)
  require(0x8c7ee8e0<=free<end<=0x8c87e8e0 and end-free>=0x50,'valid original remaining free extent')
  pressure,_=phase(pressure,0x8c098a80,[end-free-0x10],dict(label,phase='consume genuine remaining heap'))
  block=pressure[1].r[0];exact(block==free+0x10,'original exhaustion allocation address')
  pressure,_=spawn(pressure,actor,100,2,dict(label,phase='genuine spawn allocation failure'),False)
  pressure,_=phase(pressure,0x8c098b42,[block],dict(label,phase='release pressure allocation'))
  pressure,_=spawn(pressure,actor,100,2,dict(label,phase='spawn after genuine heap release'),True)
  pressure,_=phase(pressure,q.CLEANUP,[child],dict(label,phase='cleanup pressure witness'))
  # A separate copied lifecycle establishes the missing retail texture producer.
  # The declared shape position is supplied BEFORE original attachment copies it.
  # Execute that producer again; do not hand-fill a particle or texture descriptor.
  visible=clone(ready);m=visible[0];q.vector(m,q.SHAPES+8,(0,1,10))
  visible,_=phase(visible,q.ATTACH,[parent,q.SHAPES,2,0],dict(label,phase='visible scene original attachment'),count=1)
  visible,_=spawn(visible,actor,100,2,dict(label,phase='original visible particle spawn'),True)
  m=visible[0];screen=q.get(m,0x8c604ce0);q.put(m,screen+4,q.bits(640));q.put(m,screen+8,q.bits(480))
  visible,_=phase(visible,0x8c604ba0,[],dict(label,phase='original view builder from declared dimensions'))
  # With a valid viewport, actual sprite emission reaches the uninitialized
  # EFF_REGULAR texture entry. Both runs must stop at the exact original load.
  failed,result=phase(visible,q.DRAW,[child],dict(label,phase='visible texture-loading frontier'),expected_status=-3)
  exact(result['stop_pc']=='0x8c606a9e','matched original uninitialized texture descriptor load')
  frontiers.append(dict(input=label,status=-3,stop_pc=result['stop_pc'],scene_shape_position=[0,1,10],screen=[640,480],resource='SONICADV/EFF_REGULAR.PRS',texture_list='0x8c1c3068',next_original_loader='0x8c099750',partial_execution_discarded=True))
  cases.append(dict(input=label,parent=hex(parent),child=hex(child),child_actor=hex(actor),spawned_nodes=[hex(node1),hex(node2)]))
  print('PARTICLE LIFECYCLE',n+1,'PASS',flush=True)
 t=json.loads((P/'analysis/player_child_update_translation.json').read_text());coverage=[]
 for b in t['routines']:
  wanted=set(b['reachable_addresses']);outcomes={tuple(x) for x in b['conditional_edges']}
  row=dict(address=b['address'],static_rows=len(wanted),executed_rows=len(wanted&set(map(hex,seen))),missing_rows=sorted(wanted-set(map(hex,seen))),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),unresolved_outcomes=sorted(outcomes-edges));coverage.append(row)
  if int(b['address'],16) in (q.SPAWN,q.DRAW):require(not row['missing_rows'] and not row['unresolved_outcomes'],('complete scoped row/outcome coverage',row))
 files=[Path(__file__),P/'player_child_particles_fixtures.py',P/'write_watch_player_child_particles.py',P/'player_collision_attach_reference.py',P/'analysis/player_child_update_translation.json',P/'analysis/player_collision_attach_compile_checks.json',q.v.effects.DLL]
 report=dict(status='complete_original_spawn_and_draw_wrapper_match_in_declared_domains_with_visible_texture_frontier_retained',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,allocated_child_lifecycles=len(cases),cases=cases,phases=records,coverage_by_routine=coverage,visible_texture_frontiers=frontiers,new_original_routine_credit=0,supplied_allocator_results=0,supplied_game_callee_outputs=0,supplied_private_execution_frames=0,overall_completion_percent=10,
  limitations=['Spawn scene flag andheight are declared public state inputs;their retail producer remains unqualified.','Normal draw qualification covers clipped andsuppressed particles with independently captured actual caller packets. Visible scene drawing remains stopped atmissing texture descriptor606a9eafter the actual original view builder.','Visible fault-state execution is compared then discarded;it is not used as a new private-frame entry.','Rendering,retail texture loading,full scene/scheduler startup,physical GPU pixels andfullgame/ports remain unfinished.'],sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('PARTICLE COMPLETE',len(records),checks,oracles,flush=True)
if __name__=='__main__':main()
