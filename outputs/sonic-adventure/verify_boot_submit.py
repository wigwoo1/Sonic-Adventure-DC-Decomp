"""Original submission ring/draining with explicit register storage and wait stops."""
import collections,csv,ctypes,itertools,json,sys,types
from pathlib import Path
import verify_boot_device as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MANIFEST=PROJECT/'analysis/boot_submit_translation.json';manifest=json.loads(MANIFEST.read_text())
SUBMIT,DRAIN,ENQUEUE,READ=0x8c641f00,0x8c641e00,0x8c650bc8,0x8c653476
TABLE,ENQUEUE_INDEX,SEQUENCE,ISSUED_INDEX,ACTIVE_INDEX,ACTIVE_QUEUE=0x8c8a2b38,0x8c8a2d38,0x8c8a2d40,0x8c8a2d44,0x8c8a2d4c,0x8c8a2d50
OUT,HEADER=0x8ce20000,0x8ce30000
DMA=(0xffa00020,0xffa00024,0xffa00028,0xffa0002c,0xa05f6800,0xa05f6804,0xa05f6808)
ROWS=dict(previous.ROWS);OWN={};PER_BODY={};NEW={int(r['address'],16) for r in manifest['routines']}
GUARDS={int(g['pc'],16):g['continue_when_t'] for r in manifest['routines'] for g in r.get('bounded_research_waits',[])}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
code=previous.code
code=code.replace('stall_visits=0;entry_pr=','stall_visits=0;guard_visits={};entry_pr=',1)
marker='  if pc in tracked:events.append(event(pc,r,sr,pr))';assert code.count(marker)==1
code=code.replace(marker,marker+"\n  if pc in GUARDS and bool(sr&1)==GUARDS[pc]:\n   guard_visits[pc]=guard_visits.get(pc,0)+1\n   if guard_visits[pc]>=2:stop(pc,r,sr,-4)")
marker='tuple(queue_reference.packets)';assert code.count(marker)==1
code=code.replace(marker,marker+',tuple(queue_reference.dma.get(a,0) for a in DMA)')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if a in DMA and c.get('null_bus'):stop(pc,r,sr,-2)\n   if a in DMA and c.get('reject_dma')==a:stop(pc,r,sr,c.get('dma_status',-3))")
namespace=dict(previous.namespace,ROWS=ROWS,DMA=DMA,GUARDS=GUARDS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
exec(compile(code,'submission_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_submit.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_submit_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
class SubmitMemory(ctypes.Structure):
 _fields_=[('frame',previous.FrameMemory),('frame_bus',effects.Bus),('dma_registers',ctypes.c_uint32*7)]
 @property
 def vertex(self):return self.frame.vertex
 @property
 def registers(self):return self.frame.registers
 @property
 def packets(self):return self.frame.packets
 @property
 def packet_count(self):return self.frame.packet_count
 @property
 def packet_capacity(self):return self.frame.packet_capacity
 @packet_capacity.setter
 def packet_capacity(self,value):self.frame.packet_capacity=value
effects.LIB.sa_submit_register_bus_init.argtypes=[ctypes.POINTER(SubmitMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus)]
class SubmitReference(previous.FrameReference):
 def __init__(self,ticks,capacity):super().__init__(ticks,capacity);self.dma={}
 def __contains__(self,a):return a in DMA or super().__contains__(a)
 def __getitem__(self,a):
  if a not in DMA:return super().__getitem__(a)
  value=self.dma.get(a,0);self.events.append((11,a,value,4));return value
 def __setitem__(self,a,value):
  if a not in DMA:return super().__setitem__(a,value)
  value&=0xffffffff;self.dma[a]=value;self.events.append((10,a,value,4))
compare_code=previous.compare_code.replace('FrameMemory()','SubmitMemory()').replace('FrameReference(','SubmitReference(').replace('sa_frame_command_bus_init','sa_submit_register_bus_init')
compare_code=compare_code.replace('original_read=bus.read','original_read=type(bus.read)(ctypes.cast(bus.read,ctypes.c_void_p).value)')
compare_code=compare_code.replace('packet_snapshot(memory)))','packet_snapshot(memory),tuple(memory.dma_registers)))')
marker=' bus.read=read';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"\n original_write=type(bus.write)(ctypes.cast(bus.write,ctypes.c_void_p).value)\n @dict(effects.Bus._fields_)['write']\n def write(ctx,a,value,width):\n  if c.get('reject_dma')==a:return c.get('dma_status',-3)\n  return original_write(ctx,a,value,width)\n bus.write=write")
compare_code=compare_code.replace(' return dict(command_packets='," assert list(memory.dma_registers)==[ref.dma.get(a,0) for a in DMA]\n return dict(dma_writes=[list(e) for e in ref.events if e[0]==10],dma_registers=list(memory.dma_registers),command_packets=")
exec(compile(compare_code,'submission_comparison','exec'),dict(previous.previous.__dict__,effects=effects,namespace=namespace,DMA=DMA,SubmitMemory=SubmitMemory,SubmitReference=SubmitReference,packet_snapshot=previous.packet_snapshot),local:={})
compare=local['compare']

def record_address(queue,index):return TABLE+queue*256+index*32
def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);s.pc=c['entry'];s.sr=c.get('sr',s.sr)
 for queue in range(2):
  store(m,ENQUEUE_INDEX+4*queue,c.get('slot',0));store(m,ISSUED_INDEX+4*queue,c.get('issued',6))
  for index in range(8):
   a=record_address(queue,index)
   for i in range(8):store(m,a+i*4,0xabba0000+queue*0x1000+index*0x100+i)
   store(m,a+24,0)
 store(m,SEQUENCE,c.get('sequence',0xfffffffe));store(m,ACTIVE_QUEUE,c.get('active_queue',0));store(m,ACTIVE_INDEX,c.get('active_index',7))
 store(m,record_address(c.get('active_queue',0),c.get('active_index',7))+24,c.get('active_flags',2))
 if c.get('busy') is not None:
  queue=(c.get('transfer_flags',16)>>3)&1;store(m,record_address(queue,c.get('slot',0))+24,c['busy'])
 store(m,OUT,0xbadc0de);store(m,s.r[15],c.get('output',OUT))
 for i in range(8):store(m,HEADER+4*i,0x83000000+i*0x010203)
 store(m,0x8c8a3184,HEADER);store(m,0x8c8a3188,32);store(m,0x8c8a3180,c.get('extra_header',0))
 for lane in range(5):
  store(m,0x8c8a30cc+16*lane+4*c.get('slot',0),0x8c700100+lane*0x2000)
  store(m,0x8c8a311c+16*lane+4*c.get('slot',0),c.get('lengths',[32]*5)[lane])
 store(m,0x8c6736f8,c.get('skip_submit',0));store(m,0x8c8a30c0,c.get('waiting',0))
 store(m,0x8c8a2fbc,previous.NOP if c.get('wait_callback','native')=='native' else previous.UNKNOWN);store(m,0x8c8a2fc0,0)
 # The register commit path uses the original paired, 32-byte-stride tables.
 for literal in (0x8c642208,0x8c64220c,0x8c642210,0x8c642214,0x8c642218):
  base=load(m,literal)
  for i in range(2):store(m,base+i*32,0x00500000+(literal&255)*0x100+i*0x400)
 if c['entry']==ENQUEUE:
  s.r[4]=c.get('destination',0x10000000);s.r[5]=c.get('source',0x8c700100);s.r[6]=c.get('length',32);s.r[7]=c.get('transfer_flags',16)
 elif c['entry']==DRAIN:s.r[4]=c.get('exclude',4)
 elif c['entry']==READ:s.r[4]=c.get('offset',0x144)
 if c.get('bad_stack') is not None:s.r[15]=c['bad_stack']
 return m,s,f

def enqueue_checks(m,actual,initial,fp,final,f,record,c):
 if record['status']!=0:return 0
 wanted=bytearray(m);sp=initial.r[15]
 for i,reg in enumerate(range(14,8,-1)):store(wanted,sp-4*(i+1),initial.r[reg])
 count=initial.r[6];flags=initial.r[7];queue=(flags>>3)&1;index=load(m,ENQUEUE_INDEX+queue*4);a=record_address(queue,index);kick=False;writes=[]
 if not count:result=259
 else:
  store(wanted,sp-32,index);store(wanted,sp-28,queue*4)
  if load(m,a+24)&3:result=256
  else:
   result=0;source=(initial.r[5]+31)&0xffffffe0;count=(count+31)&0xffffffe0
   for offset,value in ((0,load(m,SEQUENCE)),(4,source),(12,initial.r[4]),(20,count),(24,flags|1)):store(wanted,a+offset,value)
   store(wanted,load(m,sp),a);store(wanted,ENQUEUE_INDEX+queue*4,(index+1)&7);store(wanted,SEQUENCE,(load(m,SEQUENCE)+1)&0xffffffff)
   active=record_address(load(m,ACTIVE_QUEUE),load(m,ACTIVE_INDEX))
   kick=not(load(wanted,active+24)&3) and (flags|1)&3==1
   if kick:
    store(wanted,a+24,((flags|1)&0xfffffffc)|2)
    control=(0x241 if flags&4 else 0x2c1)|(0x1000 if flags&16 else 0)
    values=(source,((initial.r[4]+0xf0000000)*2)&0xffffffff,count>>5,control,initial.r[4],count,1)
    writes=[(10,address,value,4) for address,value in zip(DMA,values)]
    store(wanted,ACTIVE_INDEX,index);store(wanted,ACTIVE_QUEUE,queue);store(wanted,ISSUED_INDEX+4*queue,(load(m,ISSUED_INDEX+4*queue)+1)&7)
 assert actual==wanted,(c,'independent complete RAM record/stack footprint')
 assert final.r[0]==result and list(map(tuple,record['dma_writes']))==writes
 assert record['dma_registers']==([e[2] for e in writes] if kick else [0]*7)
 assert final.r[15]==sp and final.pr==initial.pr and list(final.r[8:15])==list(initial.r[8:15]) and bytes(f)==bytes(fp)
 assert final.sr&~1==initial.sr&~1,(c,'interrupt mask/control fields restored')
 return 5

def drain_checks(m,actual,initial,fp,final,f,record,c):
 if record['status']!=0:return 0
 slot=c.get('slot',0);exclude=initial.r[4];lengths=c.get('lengths',[32]*5)
 headers=sum(int(c.get('extra_header',0)==1 and lane in (1,3)) for lane in range(5) if lengths[lane] and lane!=exclude)
 count=sum(bool(lengths[lane]) and lane!=exclude for lane in range(5))+headers
 assert load(actual,ENQUEUE_INDEX)==(load(m,ENQUEUE_INDEX)+count)&7 and load(actual,SEQUENCE)==(load(m,SEQUENCE)+count)&0xffffffff
 index=load(m,ENQUEUE_INDEX);expected=[]
 for lane in range(5):
  if not lengths[lane] or lane==exclude:continue
  if c.get('extra_header')==1 and lane in (1,3):expected.append((HEADER,32))
  expected.append((load(m,0x8c8a30cc+lane*16+4*slot),(lengths[lane]+32+31)&0xffffffe0))
 for i,(source,length) in enumerate(expected):
  a=record_address(0,(index+i)&7);assert load(actual,a+4)==(source+31)&0xffffffe0 and load(actual,a+12)==0x10000000 and load(actual,a+20)==length
 assert final.pr==initial.pr and final.r[15]==initial.r[15] and list(final.r[8:15])==list(initial.r[8:15]) and bytes(f)==bytes(fp)
 if headers:assert load(actual,HEADER)&0x0f000000==(max(lane for lane in (1,3) if lengths[lane] and lane!=exclude)<<24)
 return 3+len(expected)

def submit_checks(m,actual,initial,fp,final,f,record,c):
 if record['status']!=0:return 0
 index=(c.get('slot',0)+1)&3
 commit=c.get('renderer',0)==1 or c.get('skip_submit',0)==1
 expected_state=2 if commit and c.get('skip_submit',0) else 0
 assert load(actual,0x8c8a30a8)==index and load(actual,0x8c8a3098+index*4)==expected_state,(c,'independent alternating slot and post-commit state')
 assert len(record['register_writes'])==(8 if commit else 0)
 if commit:
  assert record['register_writes'][:2]==[[7,0xa05f8008,1,4],[7,0xa05f8008,0,4]]
  assert record['register_writes'][-1]==[7,0xa05f8144,0x80000000,4]
  assert load(actual,0x8c8a30b8)==0 and load(actual,0x8c8a30b4)==c.get('mask',31)
 assert final.r[0]==0 and final.pr==initial.pr and final.r[15]==initial.r[15] and list(final.r[8:15])==list(initial.r[8:15]) and bytes(f)==bytes(fp)
 return 3+int(commit)*3

def bus_checks():
 ram=bytearray(16<<20);view=effects.RAM_TYPE.from_buffer(ram);memory=SubmitMemory();bus=effects.Bus();effects.LIB.sa_submit_register_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));checks=0
 for i,a in enumerate(DMA):
  value=0x89abcdef^i;assert bus.write(bus.context,a,value,4)==0
  out=ctypes.c_uint32();assert bus.read(bus.context,a,4,ctypes.byref(out))==0 and out.value==value and memory.dma_registers[i]==value
  assert bus.write(bus.context,a,0,2)==-3 and bus.write(bus.context,a+1,0,4)==-3;checks+=4
 assert not any(ram) and not memory.packet_count;checks+=1
 out=ctypes.c_uint32(0xa5a5)
 for a in (0xffa0001c,0xffa00030,0xa05f680c,0xa05f67fc):assert bus.read(bus.context,a,4,ctypes.byref(out))==-3 and out.value==0xa5a5;checks+=1
 assert bus.write(bus.context,0xa05f8144,0x12345678,4)==0 and memory.registers[0x144//4]==0x12345678;checks+=1
 memory.vertex.trace_count=4096;dropped=memory.vertex.trace_dropped
 assert bus.write(bus.context,DMA[0],0x12345678,4)==0 and memory.dma_registers[0]==0x12345678 and memory.vertex.trace_dropped==dropped+1;checks+=1
 effects.LIB.sa_submit_register_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));assert not any(memory.dma_registers) and not memory.vertex.trace_count;checks+=1
 return dict(independent_checks=checks,registers=7,physical_dma_or_interrupt_completion=False)

def cases():
 for flags,source,count,slot,active in itertools.product((0,4,8,16,20,2),(0x8c700100,0x8c700101),(0,1,31,32,65),(0,7),(0,2)):
  yield dict(entry=ENQUEUE,mode=0,transfer_flags=flags,source=source,length=count,slot=slot,active_flags=active,active_index=6)
 for busy,flags,mode in itertools.product((1,2,3),(0,8,16),range(8)):yield dict(entry=ENQUEUE,mode=mode,busy=busy,transfer_flags=flags)
 for imask,t,mode in itertools.product((0,1,7,15),(0,1),range(8)):yield dict(entry=ENQUEUE,mode=mode,sr=0x60000302|(imask<<4)|t,active_flags=0)
 for source,count in ((0xfffffff9,1),(0x8c70011f,0xffffffff)):yield dict(entry=ENQUEUE,mode=0,source=source,length=count,active_flags=0)
 for a,status in itertools.product(DMA,(-2,-3)):yield dict(entry=ENQUEUE,mode=0,active_flags=0,reject_dma=a,dma_status=status)
 yield dict(entry=ENQUEUE,mode=0,active_flags=0,null_bus=True)
 for output in (0,0x8d000000):yield dict(entry=ENQUEUE,mode=0,output=output)
 yield dict(entry=ENQUEUE,mode=0,sr=0x200003f3)
 for entry in (ENQUEUE,DRAIN,SUBMIT):
  for stack in (0,0x8d000004):yield dict(entry=entry,mode=0,bad_stack=stack)
 for header,exclude,lengths,mode in itertools.product((0,1),(0,1,3,4,5),([0]*5,[32]*5,[0,1,0,65,32]),range(2)):
  yield dict(entry=DRAIN,mode=mode*4,extra_header=header,exclude=exclude,lengths=lengths,slot=mode*3)
 yield dict(entry=DRAIN,mode=0,lengths=[32]*5,busy=1)
 yield dict(entry=DRAIN,mode=0,lengths=[0,32,0,0,0],extra_header=1,busy=1)
 for offset,mode in itertools.product((0,8,0x144,0x1ffc,0x2000),range(8)):yield dict(entry=READ,mode=mode,offset=offset)
 yield dict(entry=READ,mode=0,null_bus=True)
 for renderer,skip,flags,slot,mode in itertools.product((0,1,2),(0,1,2),(0,32),(0,3),range(2)):
  yield dict(entry=SUBMIT,mode=mode*4,renderer=renderer,skip_submit=skip,flags=flags,selected=4,slot=slot,lengths=[32]*5)
 yield dict(entry=SUBMIT,mode=0,renderer=1,busy=1)
 for renderer in (0,1):yield dict(entry=SUBMIT,mode=0,renderer=renderer,waiting=1,wait_callback='unresolved')

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_submit_connection.json',PROJECT/'analysis/boot_device_verification.json']
 for p in proofs:
  for file,h in json.loads(p.read_text())['sha256'].items():assert previous.previous.boot.digest(file)==h,file
 ram=previous.seed_ram();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=ENQUEUE,mode=0,active_flags=0),dict(entry=ENQUEUE,mode=0,length=0),dict(entry=ENQUEUE,mode=0,busy=1),dict(entry=DRAIN,mode=0,extra_header=1),dict(entry=READ,mode=0),dict(entry=SUBMIT,mode=0,renderer=1),dict(entry=SUBMIT,mode=0,renderer=0)]
 records=[];coverage=set();edges=set()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f)
  record,path,actual,final,out=compare(m,s,f,c)
  record['independent_checks']=(enqueue_checks if c['entry']==ENQUEUE else drain_checks if c['entry']==DRAIN else submit_checks if c['entry']==SUBMIT else lambda *args:0)(m,actual,initial,fp,final,out,record,c)
  records.append(record);coverage.update(path)
  for j,a in enumerate(path):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((a,path[j+distance]))
  if (i+1)%128==0:print(i+1,'submission comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Submission smoke passed:',len(records));return
 rows=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};body=PER_BODY[a]
  rows.append(dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 for row in rows:
  if int(row['address'],16) in (DRAIN,ENQUEUE,READ):assert not row['missing_rows'] and not row['missing_edges'],row
 parent=next(r for r in rows if int(r['address'],16)==SUBMIT);assert parent['covered_rows']==236 and parent['conditional_edges_covered']==18
 assert len(records)==580 and collections.Counter(r['status'] for r in records)=={0:510,-2:11,-3:23,-4:36}
 sequences=[]
 for mask,mode in itertools.product((0,21,31),(0,4)):
  c=dict(entry=previous.FRAME,mode=mode,renderer=1,flags=0,slot=0,selected=4,mask=mask)
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f)
  producer,path,actual,s,f=compare(m,s,f,c);assert producer['status']==0 and producer['queue_events']==0 and not producer['clock_reads']
  producer['independent_checks']=previous.frame_checks(m,actual,initial,fp,s,f,producer,c)
  c=dict(c,entry=SUBMIT);m=actual;s.pc=SUBMIT
  consumer,path,actual,s,f=compare(m,s,f,c);assert consumer['status']==-4 and consumer['stop_pc']=='0x8c64207e'
  slot=load(m,0x8c8a30a8);selected=load(m,0x8c8a3170)
  count=1+sum(bool(load(m,0x8c8a311c+lane*16+slot*4)) for lane in range(5) if lane!=selected)
  assert load(actual,SEQUENCE)==(load(m,SEQUENCE)+count)&0xffffffff and load(actual,ENQUEUE_INDEX)==(load(m,ENQUEUE_INDEX)+count)&7
  assert load(actual,0x8c8a30c0)==1 and load(actual,0x8c8a30bc)==0
  assert sum(x['target']==hex(previous.NOP) for x in consumer['boundaries'])==2
  consumer['independent_checks']=4;sequences.append([producer,consumer])
 backend=bus_checks();files=[Path(__file__),PROJECT/'verify_boot_device.py',PROJECT/'verify_boot_wait.py',PROJECT/'verify_draw_view_registers.py']+proofs
 for p in proofs:files.extend(Path(file) for file in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=previous.previous.boot.digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,independent_checks=sum(r['independent_checks'] for r in records),backend=backend,dma_register_writes=sum(len(r['dma_writes']) for r in records),frame_producer_sequences=len(sequences),sequence_stage_comparisons=12,sequence_independent_checks=sum(r['independent_checks'] for seq in sequences for r in seq),submission_recovery_credit=0,external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=previous.previous.boot.digest(effects.DLL),sha256={str(p):previous.previous.boot.digest(p) for p in files},scope='Original transfer-ring record building, lane draining, register reading and submission paths; full reached/final machine/RAM/FPU/banks/MACH/MACL/GBR, timer, software graphics/transfer registers, ordered native boundaries, store queues and captured packets. Independent complete ring-builder RAM/stack footprint, wrap/alignment/record/start-register/interrupt-mask checks and drain semantics. Six actual frame-finalizer -> submission sequences reach the unresolved completion wait.',limitations=['The seven DMA command words are bounded host storage only; no asynchronous hardware transfer, interrupt delivery, completion or pixels are fabricated.','Research retry/wait bounds stop with -4 before the second continued branch test; builds without SA_BOUND_RETAIL_STALLS retain original unbounded loops. Typed host failures are not complete hardware exceptions.','Submission remains partial at its real callback/interrupt-dependent wait paths, with zero parent recovery credit. Initial runtime tables, native no-op/unknown callbacks and finite timer samples are explicit fixtures. Complete direct submission returns exercise explicit skip-submit state; starting a real transfer still reaches a pending completion wait.','The six producer/consumer sequences carry RAM and CPU/FPU state. The producer makes no bus/clock effects, so the reinitialized backend has identical empty state; the finalizer does not alter the extra integer/MACH/MACL/GBR state. This is not a general persistent interrupt/device scheduler.','Inherited privileged bank-1/FD=0, immutable code/literals, valid distinct stack/data and MMU-off domains apply. Gameplay, natural boot/scheduling and all requested ports/settings remain incomplete.'],cases=records,frame_producer_sequences_evidence=sequences)
 (PROJECT/'analysis/boot_submit_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','coverage_by_routine','independent_checks','backend','dma_register_writes')},indent=2))
if __name__=='__main__':main()
