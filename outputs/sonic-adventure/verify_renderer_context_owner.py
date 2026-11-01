"""Original renderer release ownership,real allocations,and independent list oracles."""
import collections,hashlib,itertools,json
from pathlib import Path
import renderer_context_owner_fixtures as q
P=q.P;R=P.parents[1]
META=0x8c67c2e0;POOL=0x8c8a83e4;POOL_SIZE=0x18000;PAIR1=0x8c8a33ec;PAIR2=0x8c8a33f0;BOOK=0x8c8a33f4;OWNER=0x8c88fdd8;EXTERNAL=0x8c6734ac;ARENA=0x8cd80000;ARENA_SIZE=0x10000
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/renderer_context_owner_verification.json';assert not target.exists()
 records=[];seen=set();edges=set();checks=oracles=0;ram=q.v.seed_ram()
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 def clone(base):
  m,s,f,c=base;return bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),q.v.Input(dict(c))
 def phase(base,entry,args,label):
  m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  result,path,m,s,f=q.fast.compare(m,s,f,c);require(result['status']==0,(label,result['status'],result['stop_pc']));require(result['full_state_match'] and result['ordered_boundaries_match'],'complete state andordered boundaries');require(s.r[15]==sp and s.pc==s.pr==0x8cfed000,'ordinary balanced return')
  c.update(graphics_in=result['graphics_out'],other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr']);seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),status=0,steps=len(path),return_value=hex(s.r[0]),boundaries=result['boundaries'],graphics_input=result['graphics_input_summary'],graphics_output=result['graphics_output_summary'],full_state_match=True,ordered_boundaries_match=True));return (m,s,f,c),result
 def pool_restored(m):
  exact(q.get(m,META)==q.get(m,META+4)==0,'independent no occupied bank0 records')
  node=q.get(m,META+8);exact(node!=0 and q.get(m,META+12)==node,'independent single coalesced free record')
  exact(q.get(m,node+4)==q.get(m,node+8)==0 and q.get(m,node+12)==0 and q.get(m,node+16)==q.get(m,META+16)==0x400000,'independent full4MiB interval andcapacity restored')
 def heap_intervals(m):
  root=q.get(m,0x8c88ef3c);node=root;visited=set();intervals=[]
  while node not in visited:
   require(node!=0 and len(visited)<16,'finite original system-heap ring');visited.add(node);units=q.get(m,node+4)
   if units:intervals.append((node,units*32))
   node=q.get(m,node)
  require(node==root,'original ring closes at cursor');return sorted(intervals)
 def allocate_pair(base,out1,out2,label):
  base,result=phase(base,0x8c6634a0,[64,out1,out2],label);exact(base[1].r[0]==0,'actual paired allocation succeeds');m=base[0]
  exact(q.get(m,out1)==0 and q.get(m,out2)==0x400000,'independent initial interleaved paired addresses')
  exact(q.get(m,META+16)==0x400000-128,'independent actual128-byte backing allocation');return base
 for mode,rm in itertools.product((0,4),(0,1)):
  initial=q.prepare(ram,dict(real_tables=True,mode=mode,rm=rm));initial[1].r[15]=0x8cfff000;q.put(initial[0],0x8c67c2c8,0)
  initial,_=phase(initial,0x8c663140,[],dict(mode=mode,rm=rm,phase='original VRAM memory-manager initialization'))
  initial,_=phase(initial,0x8c602128,[ARENA,ARENA_SIZE],dict(mode=mode,rm=rm,phase='original system-heap initialization ondeclared caller arena'))
  pristine_heap=heap_intervals(initial[0]);exact(pristine_heap==[(ARENA,ARENA_SIZE)],'independent complete system-heap interval')
  for address in (0,1,2,3,4,7,0x12345,0x3fffff,0x400000,0x7fffff,0x800000,0xbfffff,0xc00000,0xffffff):
   base=clone(initial);before=hashlib.sha256(base[0]).hexdigest();base,_=phase(base,0x8c665660,[address],dict(mode=mode,rm=rm,phase='bank-address inverse map',address=hex(address)))
   expected=(address&0x800000)|((address&0x3ffffc)<<1)|((address&0x400000)>>20)|(address&3)
   exact(base[1].r[0]==expected,'independent24-bit address-bit permutation');exact(hashlib.sha256(base[0]).hexdigest()==before,'map is read-only over complete RAM')
  for fallback in (False,True):
   base=allocate_pair(clone(initial),0x8cdb0000,0x8cdb0004,dict(mode=mode,rm=rm,phase='actual pair allocation for release',fallback=fallback))
   first=q.get(base[0],0x8cdb0000);second=q.get(base[0],0x8cdb0004)
   args=[0x3fffc0,first] if fallback else [first,second]
   base,result=phase(base,0x8c663920,args,dict(mode=mode,rm=rm,phase='actual direct pair release',fallback=fallback));exact(base[1].r[0]==0,'original release success');pool_restored(base[0])
   exact(sum(b['target']=='0x8c665660' for b in result['boundaries'])==(2 if fallback else 1),'independent fallback map-call count')
   before=(bytes(base[0][META&0xffffff:(META&0xffffff)+40]),bytes(base[0][POOL&0xffffff:(POOL&0xffffff)+POOL_SIZE]))
   base,_=phase(base,0x8c663920,[first,second],dict(mode=mode,rm=rm,phase='actual stale pair release'))
   exact(base[1].r[0]==7,'independent stale release not-found7');exact(before==(bytes(base[0][META&0xffffff:(META&0xffffff)+40]),bytes(base[0][POOL&0xffffff:(POOL&0xffffff)+POOL_SIZE])),'stale release preserves real allocation lists andpool')
  base=allocate_pair(clone(initial),PAIR1,PAIR2,dict(mode=mode,rm=rm,phase='original allocation directly produces stored pair globals'))
  pair=(q.get(base[0],PAIR1),q.get(base[0],PAIR2));book=bytes().join((0x71230000+i).to_bytes(4,'little') for i in range(5));base[0][BOOK&0xffffff:(BOOK&0xffffff)+20]=book
  base,_=phase(base,0x8c6567f2,[],dict(mode=mode,rm=rm,phase='stored pair release clears bookkeeping on success'));exact(base[1].r[0]==0 and base[0][BOOK&0xffffff:(BOOK&0xffffff)+20]==bytes(20),'independent successful20-byte bookkeeping reset');exact((q.get(base[0],PAIR1),q.get(base[0],PAIR2))==pair,'original pair handles deliberately remain unchanged');pool_restored(base[0])
  base[0][BOOK&0xffffff:(BOOK&0xffffff)+20]=book;base,_=phase(base,0x8c6567f2,[],dict(mode=mode,rm=rm,phase='stored stale pair returns error andpreserves bookkeeping'));exact(base[1].r[0]==7 and base[0][BOOK&0xffffff:(BOOK&0xffffff)+20]==book,'independent failed release preserves20-byte bookkeeping')
  for external in (0,1):
   base=allocate_pair(clone(initial),PAIR1,PAIR2,dict(mode=mode,rm=rm,phase='original pair allocation before owner wrapper',external=external))
   base,_=phase(base,0x8c6021d6,[96],dict(mode=mode,rm=rm,phase='actual owned system-heap allocation'));block=base[1].r[0];exact(ARENA<block<ARENA+ARENA_SIZE,'actual system-heap payload')
   busy=heap_intervals(base[0]);exact(busy!=pristine_heap,'actual system allocation consumes interval')
   # Declared caller ownership state,using an actual allocator result and its
   # uncached address alias. No callee return or private frame is substituted.
   q.put(base[0],OWNER,block|0x20000000);q.put(base[0],EXTERNAL,external);base[0][BOOK&0xffffff:(BOOK&0xffffff)+20]=book
   base,result=phase(base,0x8c6052e0,[],dict(mode=mode,rm=rm,phase='renderer-owned release wrapper',external=external));pool_restored(base[0])
   calls=[b for b in result['boundaries'] if b['target']=='0x8c602288'];exact(len(calls)==(0 if external else 1),'independent ownership flag controlsactual free-call count')
   if not external:exact(calls[0]['arguments'][0]==block,'independent original alias mask restores actual allocated pointer')
   exact(heap_intervals(base[0])==(busy if external else pristine_heap),'independent retained orfully coalesced system-heap interval')
   exact(base[0][BOOK&0xffffff:(BOOK&0xffffff)+20]==bytes(20),'owner wrapper performs actual success bookkeeping reset')
  print('RENDERER RELEASE',mode,rm,'PASS',len(records),flush=True)
 t=json.loads((P/'analysis/renderer_context_owner_translation.json').read_text());coverage=[]
 for b in t['routines']:
  wanted=set(b['reachable_addresses']);outcomes={tuple(x) for x in b['conditional_edges']};row=dict(address=b['address'],static_rows=len(wanted),executed_rows=len(wanted&set(map(hex,seen))),missing_rows=sorted(wanted-set(map(hex,seen))),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),unresolved_outcomes=sorted(outcomes-edges));require(not row['missing_rows'] and not row['unresolved_outcomes'],('complete new body coverage',row));coverage.append(row)
 files=[Path(__file__),P/'renderer_context_owner_fixtures.py',P/'renderer_context_owner_reference.py',P/'write_watch_renderer_context_owner.py',P/'write_watch_renderer_context_owner_state.py',P/'analysis/renderer_context_owner_translation.json',R/'work/sonic-adventure/build/renderer_context_owner.dll']
 report=dict(status='complete_renderer_surface_release_family_matches_original_and_independent_ownership_oracles',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,phases=records,coverage_by_routine=coverage,new_original_routine_credit=0,actual_graphics_allocator_producer='0x8c663140',actual_pair_allocator_producer='0x8c6634a0',actual_system_heap_initializer='0x8c602128',actual_system_heap_allocator='0x8c6021d6',no_argument_surface_release_entry='0x8c6567f2',incoming_root_argument_is_ignored=True,root88fcd4_is_not_written_by_this_family=True,callback_results_supplied=0,allocator_results_supplied=0,private_execution_frames_supplied=0,native_wait_guards_changed=False,overall_completion_percent=10,scope='Four full original bodies:owner wrapper,stored-pair release,fallback pair release and24-bit VRAM address inverse map. Actual original constructors/allocators supply handles,owned blocks andmetadata lists. Caller ownership flag/alias andbookkeeping bytes aredeclared component inputs. Both FPU banks/nearest-zero rounding. Exact entire state/boundaries andindependent coalesced4MiBbank/free system-heap intervals prove release. These functions perform cleanup,not renderer context creation;do not execute them as a startup producer. Scene startup/frame submission/pixels/fullgame remain unfinished.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('RELEASE FAMILY PROOF',len(records),checks,oracles,flush=True)
if __name__=='__main__':main()
