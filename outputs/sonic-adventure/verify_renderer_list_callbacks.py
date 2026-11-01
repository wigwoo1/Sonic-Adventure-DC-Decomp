"""All original callback branches, exact192-byte VRAM effects and skipped states."""
import dataclasses,hashlib,itertools,json
from pathlib import Path
import renderer_list_callbacks_fixtures as q
P=q.P;R=P.parents[1];ENTRY=0x8c651f1e


def main():
 target=P/'analysis/renderer_list_callbacks_verification.json';assert not target.exists()
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0));s.pc=c['entry']=0x8c6416ec;s.pr=0x8cfed000
 r,path,m,s,f=q.fast.compare(m,s,f,c);assert r['status']==0
 initial=(bytes(m),bytes(s),bytes(f),dict(c),r['graphics_out']);cases=[];covered=set();edges=set();checks=0
 def exact(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 for slot,mode,flags,state in itertools.product(range(4),(0,1),(0,1),(-1,0)):
  m=bytearray(initial[0]);s=q.v.effects.State.from_buffer_copy(initial[1]);f=q.v.effects.Fpu.from_buffer_copy(initial[2]);c=dict(initial[3]);c['graphics_in']=initial[4]
  c.update(entry=ENTRY,ticks=[0x12345678]);s.pc=ENTRY;s.pr=0x8cfed000;sp=s.r[15]
  arrays=list(initial[4].arrays);registers=list(arrays[0]);registers[(0x8138-0x8000)//4]=0x9000;arrays[0]=tuple(registers)
  c['graphics_in']=dataclasses.replace(initial[4],arrays=tuple(arrays))
  for a,value in [(0x8c8a30b0,slot),(0x8c8a3098+slot*4,state),(0x8c8a30c4,mode),(0x8c8a2ffc,0x1024),(0x8c8a2d5c,flags),(0x8c8a31d0,0x1000),(0x8c8a31d4,0x5000),(0x8c8a31f0,0x2000),(0x8c8a31f4,0x7000),(0x8c8a2f00,0xabcd9876),(0x8c8a2f08,0x123456),(0x8c8a2f60,0xffffff7),(0x8c8a323c+0x48,7),(0x8c8a323c+0x24,0xdeadface),(0x8c8a323c+0x28,0xdeadface)]:q.put(m,a,value)
  payload=bytes((i*7+slot*13+mode*17+flags*19)&255 for i in range(192));m[0x8a2df8:0x8a2df8+192]=payload
  r,path,a,s,f=q.fast.compare(m,s,f,c)
  exact(r['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'normal balanced original return')
  exact(r['full_state_match'] and r['ordered_boundaries_match'],'complete machine/device/memory equality')
  exact(q.get(a,0x8c8a2f60)==0,'original clears list busy flag')
  exact(q.get(a,0x8c8a323c+0x4c)==(7-(0xffffffff-0x12345678))&0xffffffff,'independent wrapping elapsed counter')
  wanted=bytearray(initial[4].vram);bank=slot&1;chosen=0x9000 if flags else (0x5000,0x7000)[bank]
  if state!=-1:wanted[chosen:chosen+192]=payload
  exact(r['graphics_out'].vram==wanted,'all8MiBVRAM including exact copied192bytes anduntouched bytes')
  exact(q.get(a,0x8c8a3098+slot*4)==(3 if state!=-1 and not mode else state&0xffffffff),'independent slot state transition')
  exact(q.get(a,0x8c8a2ffc)==(0x1024|0x20 if state!=-1 and not mode else 0x1024),'independent deferred flag')
  for b in (0,1):exact(q.get(a,0x8c8a323c+0x24+b*4)==(0x9000 if state!=-1 and bank==b else 0xdeadface),'only selected ring bank updated')
  exact(q.get(a,0x8c8a2f08)==(0xab000000|(((chosen-(0x1000,0x2000)[bank])*2)&0xfffff8) if state!=-1 else 0x123456),'independent parameter offset/mask')
  body=q.v.PER_BODY[ENTRY];covered.update(set(path)&set(body))
  for i,pc in enumerate(path):
   text=body.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  cases.append(dict(slot=slot,mode=mode,flags=flags,input_slot_state=state,steps=len(path),vram_sha256=hashlib.sha256(r['graphics_out'].vram).hexdigest()))
  print('Callback',slot,mode,flags,state,'matched',flush=True)
 helpers=[];helper_covered=set();helper_edges=set()
 for entry,value,callback in [(0x8c604546,0x87654321,cb) for cb in (0,0x8c6416ec,0x8c604570)]+[(0x8c604568,value,0) for value in (0,0xffffffff,0x12345678)]+[(0x8c604570,value,0) for value in (0,1,0xffffffff,0x80000000,0x8cfff000)]:
  m=bytearray(initial[0]);s=q.v.effects.State.from_buffer_copy(initial[1]);f=q.v.effects.Fpu.from_buffer_copy(initial[2]);c=dict(initial[3]);c['graphics_in']=initial[4]
  s.pc=c['entry']=entry;s.pr=0x8cfed000;s.r[4]=value;sp=s.r[15]
  for address,word in [(0x8c88f7d0,0xa5),(0x8c88f7d8,0x5a),(0x8c673478,callback),(0x8c88f7d4,value),(0x8c88f694,0xbeef)]:q.put(m,address,word)
  r,path,a,s,f=q.fast.compare(m,s,f,c)
  exact(r['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'registered callback balanced original return')
  exact(r['full_state_match'] and r['ordered_boundaries_match'],'registered callback complete state equality')
  if entry==0x8c604546:
   exact(q.get(a,0x8c88f7d0)==0 and q.get(a,0x8c88f7d8)==1,'canonical callback resets both flags')
   exact(q.get(a,0x8c88f694)==(value if callback==0x8c604570 else 0xbeef),'real optional callback effects')
   if callback:exact(callback in path,'optional original callback executed')
  elif entry==0x8c604568:exact(q.get(a,0x8c88f7d4)==0,'list callback original zero store')
  else:exact(q.get(a,0x8c88f694)==value,'DMA callback original argument store')
  body=q.v.PER_BODY[entry];helper_covered.update(set(path)&set(body))
  for i,pc in enumerate(path):
   text=body.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):helper_edges.add((pc,path[i+distance]))
  helpers.append(dict(entry=hex(entry),input=value,callback=hex(callback),steps=len(path)))
 manifest=json.loads((P/'analysis/renderer_list_callbacks_translation.json').read_text());routine=manifest['routines'][0]
 exact(covered=={int(x,16) for x in routine['reachable_addresses']},'all90 original instruction rows executed')
 exact({tuple(map(lambda x:int(x,16),edge)) for edge in routine['conditional_edges']}<=edges,'all8 original conditional edges executed')
 expected_helper_rows={int(a,16) for r in manifest['routines'][1:] for a in r['reachable_addresses']}
 expected_helper_edges={tuple(int(a,16) for a in edge) for r in manifest['routines'][1:] for edge in r['conditional_edges']}
 exact(helper_covered==expected_helper_rows,'all24 registered callback instruction rows executed')
 exact(expected_helper_edges<=helper_edges,'both canonical callback branches executed')
 files=[Path(__file__),P/'renderer_list_callbacks_reference.py',P/'write_watch_renderer_list_callbacks.py',P/'analysis/renderer_list_callbacks_translation.json',P/'analysis/renderer_list_callbacks_connection.json',R/'work/sonic-adventure/build/renderer_list_callbacks.dll']
 report=dict(status='original_list_completion_callback_full_branch_and_memory_domain_verified',original_native_comparisons=1+len(cases)+len(helpers),direct_callback_cases=len(cases),registered_callback_cases=len(helpers),checks=checks,instruction_rows_executed=len(covered|helper_covered),conditional_edges_executed=len(edges|helper_edges),cases=cases,registered_callback_cases_detail=helpers,registered_yet=False,new_original_routine_credit=0,scope='Standalone declared callback data coversall4slots,bothgates,bothbufferselectors,skipped andactive states. Exactcopy192bytes andall8MiBVRAM;wrapping rawdowncounter;state/mask/ringeffects independently checked. Allthreeactualregisteredcallbacks coverzerostore,argumentstore andcanonicalresetwithrealoptionalcallback execution. Full native/original CPU,FPU,hidden,platform,event,RAM/boundary comparisons remain mandatory. Integration qualification separately required. No retailpixels included.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Callback domain verified',len(cases),checks,flush=True)


if __name__=='__main__':main()
