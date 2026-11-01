"""Original allocation, collision attachment and stored child callback lifecycle.

No allocator result, callee output or private execution frame is supplied.
Direct extent cases declare public descriptor inputs separately from lifecycles.
"""
import csv,hashlib,itertools,json,struct
from pathlib import Path
import player_collision_attach_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/player_collision_attach_verification.json';assert not target.exists()
 records=[];cases=[];seen=set();edges=set();checks=oracles=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 def clone(base):
  m,s,f,c=base
  return bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),q.v.Input(dict(c))
 def phase(base,entry,args,label,count=None):
  m,s,f,c=base;sp=s.r[15];c['entry']=s.pc=entry;s.pr=0x8cfed000
  for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
  if count is not None:q.put(m,sp,count) # Original public ABI stack argument.
  saved=list(s.r[8:15]);result,path,m,s,f=q.fast.compare(m,s,f,c)
  require(result['status']==0,(label,result['status'],result['stop_pc']))
  require(result['full_state_match'] and result['ordered_boundaries_match'],'exact complete state and ordered boundaries')
  require(s.r[15]==sp and s.pc==s.pr==0x8cfed000,'normal original return with balanced stack')
  require(list(s.r[8:15])==saved,'preserved callee-saved integer registers')
  c.update(other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
  seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),status=0,steps=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
  return m,s,f,c
 def shapes(m,count):
  for j in range(count):
   a=q.SHAPES+0x34*j;m[a&0xffffff:(a&0xffffff)+0x34]=bytes(0x34)
   q.put(m,a,3 if j%2 else 1);q.vector(m,a+8,(0,0,0))
   q.vector(m,a+0x14,(2,3,0) if j%2 else (3,4,0))
 def extent_bits(rm):
  # Both authored shapes have exact mathematical extent 5. Their original
  # multiplier is binary32 0x3f8147ae; its product with 5 is exact in binary64.
  value=struct.unpack('<f',struct.pack('<I',0x3f8147ae))[0]*5
  out=struct.unpack('<I',struct.pack('<f',value))[0]
  if rm and struct.unpack('<f',struct.pack('<I',out))[0]>value:out-=1
  return out
 initial=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True))
 initial=phase(initial,0x8c098a10,[],'original heap initialization');heap=clone(initial)
 initial=phase(initial,0x8c09844a,[],'original task pool initialization')
 for n,(kind,index) in enumerate(itertools.product(range(8),(0,1))):
  base=clone(initial);m,s,f,c=base;mode=4*(n%2);rm=(n//2)%2;f.fpscr=0x40000|(mode<<19)|rm
  m[0x799920:0x799b20]=bytes(0x200);q.put(m,q.FALLBACK_KIND,0,2);shapes(m,2)
  label=dict(collision_kind=kind,player_index=index,mode=mode,rm=rm)
  base=phase(base,0x8c09852e,[3,2,0,0],dict(label,phase='original parent allocation'))
  m,s,f,c=base;parent=s.r[0];actor=q.get(m,parent+0x20);q.put(m,actor+8,index,1);q.put(m,actor+9,0,1)
  base=phase(base,0x8c06c350,[index,parent],dict(label,phase='original player construction'))
  m,s,f,c=base;exact(q.get(m,actor+0x38)==0,'actual constructor leaves collision pointer unset')
  source=bytes(m[q.SHAPES&0xffffff:(q.SHAPES&0xffffff)+0x68])
  base=phase(base,q.ATTACH,[parent,q.SHAPES,kind,0xfeedface],dict(label,phase='original collision attachment'),count=0x102)
  m,s,f,c=base;collision=q.get(m,actor+0x38);provider=q.get(m,collision+0xc)
  exact(0x8c7ee8e0<=collision and collision+0x1c<=provider and provider+0x68<=0x8c87e8e0,'two actual disjoint heap allocations')
  exact(bytes(m[provider&0xffffff:(provider&0xffffff)+0x68])==source,'independent exact shape-copy bytes')
  exact(q.get(m,collision,2)==kind and q.get(m,collision+4,2)==2,'original kind and low-byte stack count')
  exact(q.get(m,collision+2,2)==(0x820 if kind in (2,3,7) else 0x20),'independent original flag rule')
  exact(q.get(m,collision+0x10)==parent,'original owner binding')
  exact(q.get(m,collision+8)==(q.bits(25) if kind in (0,1) else extent_bits(rm)),'independent default or computed extent bits')
  child=q.get(m,q.get(m,0x8c0986c8)+8);child_actor=q.get(m,child+0x20)
  base=phase(base,q.get(m,child+0x10),[child],dict(label,phase='actual stored child initializer'))
  base=phase(base,q.get(base[0],child+0x10),[child],dict(label,phase='actual stored child update'))
  m,s,f,c=base;exact(q.get(m,child_actor+1,1)==1,'actual update enters timer phase')
  exact(q.get(m,actor+0x38)==collision and q.get(m,collision+0xc)==provider,'original collision bindings stay live through geometry')
  # Exercise original list operations on real heap allocations, without drawing
  # uninitialized particle payloads from the raw record allocator.
  nodes=[]
  for j in range(3):
   oldhead=q.get(base[0],child_actor+0x10)
   base=phase(base,q.ALLOCATE,[child_actor],dict(label,phase='original record allocation',record=j))
   m,s,f,c=base;node=s.r[0];nodes.append(node)
   exact(node!=0 and q.get(m,child_actor+0x10)==node and q.get(m,node+0x2c)==0 and q.get(m,node+0x30)==oldhead,'independent head insertion links')
   if oldhead:exact(q.get(m,oldhead+0x2c)==node,'independent previous-head backlink')
  if n%2:
   base=phase(base,q.REMOVE,[child_actor,nodes[1]],dict(label,phase='remove actual middle node'))
   m=base[0];exact(q.get(m,nodes[2]+0x30)==nodes[0] and q.get(m,nodes[0]+0x2c)==nodes[2],'middle unlink reconnects both neighbors')
   base=phase(base,q.REMOVE,[child_actor,nodes[0]],dict(label,phase='remove actual tail node'))
   exact(q.get(base[0],nodes[2]+0x30)==0,'tail unlink terminates list')
  base=phase(base,q.get(base[0],child+0x18),[child],dict(label,phase='actual installed cleanup'))
  exact(q.get(base[0],child_actor+0x10)==0,'cleanup consumes real allocated record list')
  base=phase(base,q.CLEANUP,[child],dict(label,phase='empty cleanup'))
  cases.append(dict(input=label,parent=hex(parent),actor=hex(actor),child=hex(child),child_actor=hex(child_actor),collision=hex(collision),provider=hex(provider),record_allocations=list(map(hex,nodes))))
  print('COLLISION ATTACH LIFECYCLE',n+1,'PASS',flush=True)
 # Public extent API: include zero-count, negative/zero extents and both shape
 # formulas. These are explicit descriptor inputs, not retail producer claims.
 for n,(count,negative) in enumerate(itertools.product((0,1,2),(False,True))):
  base=q.prepare(q.v.seed_ram(),dict(kind=0));m,s,f,c=base;shapes(m,2)
  m[q.COLLISION&0xffffff:(q.COLLISION&0xffffff)+0x1c]=bytes(0x1c)
  q.put(m,q.COLLISION+4,count,2);q.put(m,q.COLLISION+0xc,q.SHAPES)
  if negative:
   for j in range(2):q.put(m,q.SHAPES+0x34*j,3);q.vector(m,q.SHAPES+0x34*j+0x14,(-2,-3,0))
  base=phase(base,q.EXTENT,[q.COLLISION],dict(direct_extent_count=count,negative=negative))
  exact(q.get(base[0],q.COLLISION+8)==(q.bits(25) if negative or count==0 else extent_bits(0)),'independent direct extent output')
 # Genuine second-allocation failure: consume most of an initialized heap,
 # leaving room for the header but not a 0x34-byte shape. No null is injected.
 base=clone(heap);base=phase(base,0x8c098a80,[0x8ffb0],'genuine heap pressure')
 m,s,f,c=base;exact(s.r[0]!=0,'real pressure allocation succeeds');q.put(m,q.TASK+0x20,q.CHILD_ACTOR);q.put(m,q.CHILD_ACTOR+0x38,0)
 base=phase(base,q.ATTACH,[q.TASK,q.SHAPES,2,0], 'genuine shape allocation failure',count=1)
 exact(q.get(base[0],q.CHILD_ACTOR+0x38)==0,'original second-allocation failure leaves actor unattached')
 # The existing byte-copy entry observer must distinguish the internal delayed
 # backedge from an actual call; prove the exact edge bytes independently.
 retail=P.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes()
 exact(raw[0x604506-0x10000:0x60450a-0x10000]==bytes.fromhex('fb8f0174'),'original internal BF/S backedge and non-PR delay instruction')
 coverage=[]
 manifests=[P/'analysis/player_collision_attach_translation.json',P/'analysis/player_child_update_translation.json']
 wanted_entries={q.ATTACH,q.EXTENT,q.COPY,q.ALLOCATE,q.REMOVE,q.CLEANUP,q.UPDATE}
 for mp in manifests:
  for body in json.loads(mp.read_text())['routines']:
   if int(body['address'],16) not in wanted_entries:continue
   wanted=set(body['reachable_addresses']);outcomes={tuple(x) for x in body['conditional_edges']}
   coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&set(map(hex,seen))),missing_rows=sorted(wanted-set(map(hex,seen))),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),unresolved_outcomes=sorted(outcomes-edges)))
 for address in (q.EXTENT,q.ALLOCATE,q.REMOVE,q.CLEANUP):
  row=next(x for x in coverage if x['address']==hex(address))
  if address!=q.ALLOCATE:require(not row['missing_rows'] and not row['unresolved_outcomes'],('complete scoped row/outcome coverage',row))
 files=[Path(__file__),P/'player_collision_attach_fixtures.py',P/'player_collision_attach_reference.py',P/'write_watch_player_collision_attach.py',*manifests,P/'analysis/player_collision_attach_connection.json',retail,q.v.effects.DLL]
 report=dict(status='original_collision_attachment_and_allocated_child_update_lifecycles_match_in_declared_domains',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,allocated_child_lifecycles=len(cases),cases=cases,phases=records,coverage_by_routine=coverage,new_original_routine_credit=0,canonical_duplicate_credit=0,supplied_allocator_results=0,supplied_game_callee_outputs=0,supplied_private_execution_frames=0,overall_completion_percent=10,
  limitations=['First header-allocation failure enters a retained original non-returning diagnostic loop and is not executed by this suite.','The complete effect update/spawn/draw family is not qualified; the ordinary no-particle update executes with declared player/shape inputs.','Host phase order is declared. Retail scheduler order, character setup, scene asset loading and physical hardware remain unqualified.','Direct extent fixtures are explicit public descriptor inputs and are separate from original allocation lifecycles.'],
  observer_internal_edge=dict(entry='0x8c604500',predecessor_delay_slot='0x8c604508',branch='0x8c604506',original_bytes='fb8f0174',reason='Local BF/S with non-PR delay instruction; existing internal-entry facility preserves actual call/tail-entry observations. CPU and exact comparison code unchanged.'),sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('COLLISION ATTACH COMPLETE',len(records),checks,oracles,flush=True)
if __name__=='__main__':main()
