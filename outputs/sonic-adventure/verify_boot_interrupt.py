"""Original interrupt/completion/buffer code and explicit IRQ/VRAM bus state."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_boot_submit as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/boot_interrupt_translation.json';manifest=json.loads(MANIFEST.read_text())
IRQ,RESET,LAYOUT,SELECT,SCHEDULE,FLIP,REGS,QUAD,DIV,REM,TRANSFER,COPY,SUBMIT=(0x8c651bc0,0x8c650b60,0x8c665fa8,0x8c651570,0x8c6516a0,0x8c65194a,0x8c665e3c,0x8c665f42,0x8c10cb10,0x8c10cc80,0x8c65347c,0x8c65253e,0x8c641f00)
ACK,POLL,SCAN,VRAM=0xa05f6900,0xa05f6808,0xa05f810c,0x05000000
DMA=previous.DMA;NEW={int(r['address'],16) for r in manifest['routines']};ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
GUARDS={pc:(value,2) for pc,value in previous.GUARDS.items()};GUARD_ENTRY={}
for proof in (previous.manifest,manifest):
 for routine in proof['routines']:
  entry=int(routine['address'],16);own=routine.get('bounded_research_waits',[])
  if own:GUARD_ENTRY[entry]=[int(g['pc'],16) for g in own]
  for g in own:GUARDS[int(g['pc'],16)]=(g['continue_when_t'],g['max_continue_tests'])
def vram_address(a):return not a&3 and VRAM<=a&0x1fffffff<VRAM+(8<<20)
code=previous.code
code=code.replace("  if pc in GUARDS and bool(sr&1)==GUARDS[pc]:","  if pc in GUARD_ENTRY:\n   for guard in GUARD_ENTRY[pc]:guard_visits[guard]=0\n  if pc in GUARDS and bool(sr&1)==GUARDS[pc][0]:")
code=code.replace('if guard_visits[pc]>=2:stop','if guard_visits[pc]>=GUARDS[pc][1]:stop')
marker='tuple(queue_reference.dma.get(a,0) for a in DMA)';assert code.count(marker)==1
code=code.replace(marker,marker+',queue_reference.normal_status,hashlib.sha256(queue_reference.vram).digest(),queue_reference.poll_cursor')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if vram_address(a) and (a&0x1fffffff)+n>VRAM+(8<<20):stop(pc,r,sr,-3)")
code=code.replace(marker,marker+"\n   if (a&0x1fffffff==0x005f6900 or vram_address(a)) and c.get('null_bus'):stop(pc,r,sr,-2)\n   if c.get('reject_bus')==a:stop(pc,r,sr,c.get('bus_status',-3))\n   if a==POLL and text.split(' ',1)[1].split(',')[0].startswith('@') and queue_reference.poll_samples is not None and queue_reference.poll_cursor>=len(queue_reference.poll_samples):stop(pc,r,sr,-4)")
namespace=dict(previous.namespace,ROWS=ROWS,DMA=DMA,POLL=POLL,VRAM=VRAM,GUARDS=GUARDS,GUARD_ENTRY=GUARD_ENTRY,vram_address=vram_address,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
exec(compile(code,'interrupt_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_interrupt.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_interrupt_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
class InterruptMemory(previous.SubmitMemory):_fields_=[('submit_bus',effects.Bus),('normal_status',ctypes.c_uint32),('vram',ctypes.c_uint32*(2<<20))]
effects.LIB.sa_interrupt_bus_init.argtypes=[ctypes.POINTER(InterruptMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus)]
class InterruptReference(previous.SubmitReference):
 def __init__(self,ticks,capacity,c):
  super().__init__(ticks,capacity);self.normal_status=c.get('irq_status',8);self.vram=bytearray(8<<20);self.poll_samples=c.get('busy_samples');self.poll_cursor=0;self.poll_reads=[];self.registers[0x005f810c]=c.get('hardware_parity',0)<<10
 def __contains__(self,a):return a&0x1fffffff==0x005f6900 or vram_address(a) or super().__contains__(a)
 def __getitem__(self,a):
  if a&0x1fffffff==0x005f6900:self.events.append((13,a,self.normal_status,4));return self.normal_status
  if vram_address(a):
   value=load(self.vram,(a&0x1fffffff)-VRAM);self.events.append((15,a,value,4));return value
  if a==POLL and self.poll_samples is not None:
   self.dma[a]=self.poll_samples[self.poll_cursor]&0xffffffff;self.poll_cursor+=1;self.poll_reads.append((a,self.dma[a],4))
  return super().__getitem__(a)
 def __setitem__(self,a,value):
  value&=0xffffffff
  if a&0x1fffffff==0x005f6900:self.events.append((12,a,value,4));self.normal_status&=~value;return
  if vram_address(a):self.events.append((14,a,value,4));store(self.vram,(a&0x1fffffff)-VRAM,value);return
  super().__setitem__(a,value)
compare_code=previous.compare_code.replace('SubmitMemory()','InterruptMemory()').replace('SubmitReference(','InterruptReference(').replace('sa_submit_register_bus_init','sa_interrupt_bus_init')
compare_code=compare_code.replace("c.get('capacity',64))","c.get('capacity',64),c)",1)
compare_code=compare_code.replace("c.get('ticks',[0x12345678])","c.get('ticks',list(range(100,356)))")
compare_code=compare_code.replace('memory.packet_capacity=',"memory.normal_status=c.get('irq_status',8);memory.registers[0x10c//4]=c.get('hardware_parity',0)<<10;poll_reads=[];memory.packet_capacity=",1)
marker='tuple(memory.dma_registers)))';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,'tuple(memory.dma_registers),memory.normal_status,hashlib.sha256(memory.vram).digest(),len(poll_reads)))')
marker='  if a!=TIMER:return original_read(ctx,a,width,out)';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,"  if c.get('reject_bus')==a:return c.get('bus_status',-3)\n  if a==POLL and c.get('busy_samples') is not None:\n   samples=c['busy_samples']\n   if len(poll_reads)>=len(samples):return -4\n   value=samples[len(poll_reads)]&0xffffffff;memory.dma_registers[6]=value;poll_reads.append((a,value,width))\n"+marker)
marker="  if c.get('reject_dma')==a:return c.get('dma_status',-3)";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,"  if c.get('reject_bus')==a:return c.get('bus_status',-3)\n"+marker)
marker=' return dict(dma_writes=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert bytes(memory.vram)==bytes(ref.vram) and memory.normal_status==ref.normal_status and poll_reads==ref.poll_reads,(c,'complete VRAM/IRQ state and every busy sample')\n return dict(irq_status=memory.normal_status,vram_sha256=hashlib.sha256(memory.vram).hexdigest(),vram_writes=[list(e) for e in ref.events if e[0]==14],ack_writes=[list(e) for e in ref.events if e[0]==12],poll_reads=[list(e) for e in poll_reads],dma_writes=")
compare_code=compare_code.replace("boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15])))","boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8])))")
exec(compile(compare_code,'interrupt_comparison','exec'),dict(previous.previous.previous.__dict__,effects=effects,namespace=namespace,DMA=DMA,POLL=POLL,hashlib=hashlib,InterruptMemory=InterruptMemory,InterruptReference=InterruptReference,packet_snapshot=previous.previous.packet_snapshot),local:={})
compare=local['compare']

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);s.pc=c['entry'];m[0x8a2d5c:0x8a3400]=bytes(0x3400-0x2d5c)
 values={0x8c8a2d58:c.get('format',0),0x8c8a2d5c:c.get('flags',0),0x8c8a2d78:c.get('mode_flags',0),0x8c8a2d88:c.get('framebuffer_override',0),0x8c8a2dbc:1-c.get('order',0),0x8c8a2f14:c.get('blit_mask',0),0x8c8a2f18:c.get('stride_mode',0),0x8c8a2f1c:c.get('clip',0),0x8c8a2f20:c.get('clip_x',0),0x8c8a2f24:c.get('clip_y',0),0x8c8a2f28:c.get('boundary_mode',0),0x8c8a2f2c:c.get('boundary_selector',0),0x8c8a2f34:c.get('countdown',0),0x8c8a2f38:c.get('reload',0),0x8c8a2f5c:c.get('render_busy',0),0x8c8a2f60:c.get('transfer_busy',0),0x8c8a2f6c:0x20,0x8c8a2f70:0x40,0x8c8a2f78:c.get('timeout_a',0),0x8c8a2f7c:c.get('timeout_b',0),0x8c8a2ff4:c.get('dirty',0),0x8c8a2ffc:c.get('irq_flags',0),0x8c8a3080:0x100000,0x8c8a3084:0x200000,0x8c8a3090:c.get('renderer',1),0x8c8a30a8:c.get('slot',0),0x8c8a30bc:c.get('counter',0),0x8c8a30c0:c.get('waiting',1),0x8c8a30c4:c.get('stall',0),0x8c8a30c8:c.get('block',0),0x8c8a3170:c.get('exclude',4),0x8c8a3180:c.get('extra_header',0),0x8c8a3184:previous.HEADER,0x8c8a3188:32,0x8c8a31a8:c.get('width',2),0x8c8a31ac:c.get('height',2),0x8c8a31b0:c.get('quad_format',0),0x8c8a31f8:c.get('size_mode',0),0x8c8a3238:c.get('work_flag',0),0x8c8a323c:c.get('render_ticks',0),0x8c8a3240:c.get('command_ticks',0),0x8c8a3254:c.get('stall_ticks',0)}
 for a,v in values.items():store(m,a,v)
 store(m,0x8c67c008,c.get('parity',0));store(m,0x8c67c330,c.get('alias_last',0))
 store(m,0x8c8a3294,c.get('flip_parity',0))
 for i,value in enumerate(c.get('blocks',[1,1,1,1,1])):store(m,0x8c8a31fc+i*4,value)
 for i,value in enumerate(c.get('states',[-1]*4)):store(m,0x8c8a3098+i*4,value&0xffffffff)
 for bank in range(2):
  base=bank*0x100000
  for i,value in enumerate((0x10000,0x20000,0x40000,0x50000,0x60000,0x70000,0x80000,0x90000)):store(m,0x8c8a31bc+bank*32+i*4,base+value)
  store(m,0x8c8a31b8+bank*32,base+0x8000)
 store(m,0x8c8a31f8,c.get('size_mode',0))
 for lane in range(5):
  for slot in range(4):
   store(m,0x8c8a30cc+lane*16+slot*4,0x8c700100+lane*0x2000+slot*0x100)
   store(m,0x8c8a311c+lane*16+slot*4,c.get('lengths',[0]*5)[lane])
 for a in (0x8c8a3000,0x8c8a3004,0x8c8a3010,0x8c8a3014,0x8c8a3018,0x8c8a304c,0x8c8a3050):store(m,a,(a&0xffff)|0x22000000)
 store(m,0x8c8a2f08,0x13579bdf);store(m,0x8c8a2f04,0x2468acf1)
 store(m,0x8c8a2fbc,IRQ if c.get('callback','irq')=='irq' else previous.previous.NOP if c['callback']=='native' else previous.previous.UNKNOWN);store(m,0x8c8a2fc0,0);store(m,0x8c8a2fc4,previous.previous.NOP)
 for queue in range(2):
  store(m,previous.ENQUEUE_INDEX+queue*4,c.get('ring_slot',0));store(m,previous.ISSUED_INDEX+queue*4,6)
  for i in range(8):store(m,previous.record_address(queue,i)+24,0 if queue==0 else 8)
 store(m,previous.ACTIVE_INDEX,7);store(m,previous.ACTIVE_QUEUE,0);store(m,previous.record_address(0,7)+24,2)
 if c.get('busy_slot') is not None:store(m,previous.record_address(0,c['busy_slot'])+24,1)
 if c['entry'] in (LAYOUT,REGS):
  s.r[4]=c.get('width',2);s.r[5]=c.get('height',2);s.r[6]=c.get('surface',0);s.r[7]=1
  store(m,s.r[15],c.get('bank',0));store(m,s.r[15]+4,c.get('order',0))
 elif c['entry']==QUAD:s.r[4]=c.get('destination',0x30000);s.r[5]=c.get('start',0x10000)
 elif c['entry']==TRANSFER:s.r[4]=c.get('destination',0x2000);s.r[5]=previous.previous.SOURCE;s.r[6]=c.get('length',24)
 elif c['entry']==COPY:s.r[4]=c.get('destination',0xa5002000);s.r[5]=previous.previous.SOURCE;s.r[6]=c.get('length',24)
 elif c['entry'] in (DIV,REM):s.r[0]=c.get('divisor',3);s.r[1]=c.get('numerator',0x87654321)
 if c.get('bad_stack') is not None:s.r[15]=c['bad_stack']
 return m,s,f

def retained(initial,final):return initial.pr==final.pr and list(initial.r[8:16])==list(final.r[8:16])
def semantic(m,actual,initial,fp,final,f,record,c):
 if record['status']!=0:return 0
 entry=initial.pc
 if entry==RESET:
  wanted=bytearray(m);store(wanted,initial.r[15]-4,initial.r[14]);store(wanted,initial.r[15]-8,load(m,0x8c650c48))
  for a in (previous.ENQUEUE_INDEX,previous.ENQUEUE_INDEX+4,previous.ISSUED_INDEX,previous.ISSUED_INDEX+4,previous.SEQUENCE):store(wanted,a,0)
  store(wanted,previous.ACTIVE_INDEX,7);store(wanted,previous.ACTIVE_QUEUE,7)
  for queue in range(2):
   for i in range(8):store(wanted,previous.record_address(queue,i)+24,8*queue)
  assert actual==wanted and final.r[0]==0 and retained(initial,final) and bytes(fp)==bytes(f)
  assert record['dma_registers'][3]&1==0 and record['dma_registers'][6]==0
  assert record['register_writes']==[[7,0xa05f8008,1,4],[7,0xa05f8008,0,4],[7,0xa05f8008,1,4],[7,0xa05f8008,0,4]]
  return 4
 if entry in (DIV,REM):
  n,d=initial.r[1],initial.r[0];assert final.r[0]==((n//d if entry==DIV else n%d) if d else 0)
  if not d:assert load(actual,0x8c7ac108)==0x44e
  assert list(final.r[2:16])==list(initial.r[2:16]) and final.pr==initial.pr and bytes(fp)==bytes(f)
  return 3
 if entry==SELECT:
  wanted=bytearray(m);states=c.get('states',[-1]*4);candidate=next((i for i,x in enumerate(states) if x in (5,-3)),None);writes=[]
  for offset,index in enumerate((14,13,12,11,10),1):store(wanted,initial.r[15]-offset*4,initial.r[index])
  if candidate is not None:
   store(wanted,0x8c8a3098+4*candidate,7);flags=c.get('mode_flags',0)
   special=not flags&1 and flags&0x2c==0x20
   if special or c.get('framebuffer_override',0)!=1:
    left=load(m,0x8c8a3080+(0 if special else (candidate&1)*4))+load(m,0x8c8a2f6c)
    right=load(m,0x8c8a3080+(4 if special else (candidate&1)*4))+load(m,0x8c8a2f70)
    writes.extend([[7,0xa05f8050,left&0xffffffff,4],[7,0xa05f8054,right&0xffffffff,4]])
   if c.get('dirty',0)==1:
    writes.extend([[7,0xa05f80e8,load(m,0x8c8a3050),4],[7,0xa05f8044,load(m,0x8c8a3014),4]]);store(wanted,0x8c8a2ff4,0)
  assert actual==wanted and record['register_writes']==writes and retained(initial,final) and bytes(fp)==bytes(f)
  return 3
 if entry==REGS:
  bank=initial.r[6];base=0x8c8a31c4+bank*32;blocks=c.get('blocks',[1]*5)
  bits=(blocks[0]&3)|((blocks[1]&3)<<4)|((blocks[2]&3)<<8)|((blocks[3]&3)<<12)
  bits|=((c.get('size_mode',0)&1)<<20)|((blocks[4]&3)<<16) if c.get('format',0)==6 else (c.get('size_mode',0)&1)<<16
  values=[load(m,base),load(m,base+4),load(m,base+12),load(m,base+16),(((initial.r[5]-1)<<16)+initial.r[4]-1)&0xffffffff,bits,0x80000000]
  addresses=(0xa05f8124,0xa05f812c,0xa05f8128,0xa05f8130,0xa05f813c,0xa05f8140,0xa05f8144)
  assert record['register_writes']==[[7,a,v,4] for a,v in zip(addresses,values)] and retained(initial,final) and bytes(fp)==bytes(f)
  return 3
 if entry==QUAD and not fp.fpscr&0x180000:
  destination=(initial.r[4]|0xa5000000)&0xffffffff
  color=load(m,0x8c666004)
  values=[((((initial.r[4]-initial.r[5])&0xffffffff)>>2)+2)|0x80200000,0xf0000000,0x00800000,0x24900000,0,0,0,color,0x80ff0000,0,load(m,0x8c66600c),color,0x8000ff00,load(m,0x8c666014),0,color,0x800000ff]
  assert record['vram_writes']==[[14,destination+i*4,v&0xffffffff,4] for i,v in enumerate(values)] and actual==m and retained(initial,final) and final.r[0]==initial.r[4]
  return 3
 if entry in (COPY,TRANSFER):
  destination=initial.r[4] if entry==COPY else (initial.r[4]+0xa5000000)&0xffffffff;length=initial.r[6]
  expected=[[14,(destination+i)&0xffffffff,load(m,initial.r[5]+i),4] for i in range(0,length//4*4,4)]
  assert record['vram_writes']==expected and final.r[15]==initial.r[15] and final.pr==initial.pr and bytes(fp)==bytes(f)
  if entry==TRANSFER:assert final.r[0]==1
  return 3
 if entry==LAYOUT and c.get('order',0) in (0,1) and not c.get('flags',0)&32 and not c.get('boundary_mode',0):
  width,height=c.get('width',2),c.get('height',2);count=width*height;bank=c.get('bank',0);base=0x8c8a31bc+bank*32;destination=load(m,base)+c.get('surface',0)*count*24
  blocks=c.get('blocks',[1]*5);sizes=[load(m,0x8c67c320+i*4)*4 for i in blocks];bases=[load(m,base+8)]
  for size in sizes[:-1]:bases.append(bases[-1]+size*count)
  writes=[]
  for i in range(count):
   x,y=(i//height,i%height) if c.get('order',0)==0 else (i%width,i//width)
   clipped=bool(c.get('clip',0) and (x>c.get('clip_x',0) or y>c.get('clip_y',0)))
   values=[(0x80000000 if i==count-1 else 0)|(y&63)<<8|(x&63)<<2|(0x20000000 if c.get('stride_mode',0)==1 else 0)]
   values += [((base+size*(y*width+x))|(0x80000000 if not block or clipped else 0))&0xffffffff for base,size,block in zip(bases,sizes,blocks)]
   if c.get('alias_last',0):values[-1]=values[3]
   writes.extend([[14,0xa5000000+destination+i*24+j*4,v,4] for j,v in enumerate(values)])
  assert record['vram_writes']==writes,(c,'independent ordinary tile coordinates/list pointers/final marker')
  assert retained(initial,final)
  return 3+count
 if entry==SCHEDULE:
  states=c.get('states',[-1]*4);slot=next((i for i,x in enumerate(states) if x==1 and not c.get('transfer_busy',0)),None)
  calls=[b['arguments'] for b in record['boundaries'] if b['target']==hex(previous.ENQUEUE)]
  if slot is None:assert final.r[0]==1 and not calls and all(load(actual,0x8c8a3098+4*i)==(v&0xffffffff) for i,v in enumerate(states));return 2
  wanted=[];mask=0;lengths=c.get('lengths',[0]*5);busy=c.get('busy_slot');accepted=0;failed=False
  for lane,n in enumerate(lengths):
   if not n:continue
   mask|=1<<lane
   requests=[]
   if lane in (1,3) and c.get('extra_header')==1:requests.append([0x10000000,previous.HEADER,32,16])
   requests.append([0x10000000,load(m,0x8c8a30cc+lane*16+slot*4),(n+32)&0xffffffff,16])
   for request in requests:
    wanted.append(request)
    if busy is not None and (c.get('ring_slot',0)+accepted)&7==busy:failed=True;break
    accepted+=1
   if failed:break
  assert calls==wanted and final.r[0]==int(not failed) and load(actual,0x8c8a3098+slot*4)==2 and load(actual,0x8c8a30b4)==mask
  assert load(actual,previous.SEQUENCE)==(load(m,previous.SEQUENCE)+accepted)&0xffffffff and retained(initial,final)
  return 4+len(calls)
 if entry==IRQ:
  parity=c.get('hardware_parity',0) if c.get('flags',0)&2 else (c.get('parity',0)+1)&1
  assert load(actual,0x8c8a30bc)==(c.get('counter',0)+1)&0xffffffff and load(actual,0x8c8a2ffc)==c.get('irq_flags',0)|2
  assert load(actual,0x8c8a3294)==parity and record['ack_writes']==[[12,ACK,8,4]] and record['irq_status']==c.get('irq_status',8)&~8 and retained(initial,final)
  return 4
 if entry==SUBMIT:
  index=(c.get('slot',0)+1)&3
  assert load(actual,0x8c8a30a8)==index and retained(initial,final) and final.r[0]==0
  irqs=[b for b in record['boundaries'] if b['target']==hex(IRQ)]
  if not irqs:
   assert c.get('skip_submit')==1 or not c.get('waiting',1)
   return 2
  if c.get('block',0):assert load(actual,0x8c8a30c0)==1 and load(actual,0x8c8a30bc)==21 and any(b['target']==hex(previous.previous.TAIL) for b in record['boundaries'])
  else:assert load(actual,0x8c8a30c0)==0
  return 3
 return 0

def cases():
 yield dict(entry=IRQ,mode=0,states=[4,-1,-1,-1],render_busy=0)
 yield dict(entry=IRQ,mode=0,states=[1,-1,-1,-1],lengths=[32]*5,busy_slot=0)
 for blocks,stride,quad in itertools.product(([0]*5,[0,1,1,1,1],[0,1,0,1,1]),(0,1),(0,1)):
  yield dict(entry=LAYOUT,mode=0,width=1,height=1,flags=32,blocks=blocks,stride_mode=stride,quad_format=quad,transfer_busy=1)
 yield dict(entry=LAYOUT,mode=0,width=1,height=1,order=2,blocks=[0]*5)
 yield dict(entry=FLIP,mode=0,states=[3,-1,-1,-1],render_busy=1)
 yield dict(entry=FLIP,mode=0,states=[3,-1,-1,-1],mode_flags=32,flip_parity=1)
 for renderer,skip,flags,slot,mode in itertools.product((0,1,2),(0,1,2),(0,32),(0,3),(0,4)):
  yield dict(entry=SUBMIT,mode=mode,renderer=renderer,skip_submit=skip,flags=flags,slot=slot,lengths=[32]*5,waiting=0)
 yield dict(entry=SUBMIT,mode=0,renderer=1,busy_slot=0)
 for samples,mode in itertools.product((None,[1,0],[1,1],[1,1,0],[]),(0,4)):yield dict(entry=RESET,mode=mode,busy_samples=samples)
 for candidate,kind,flags,toggle in itertools.product((None,0,1,2,3),(5,-3),(0,32,33),(0,1)):
  states=[-1]*4
  if candidate is not None:states[candidate]=kind
  yield dict(entry=SELECT,mode=toggle*4,states=states,mode_flags=flags,dirty=toggle,framebuffer_override=toggle)
 for dimensions,fmt,bank in itertools.product(((0,0),(1,1),(2,3),(64,32)),(0,6),(0,1)):
  yield dict(entry=REGS,mode=bank*4,width=dimensions[0],height=dimensions[1],format=fmt,surface=bank,bank=bank,size_mode=int(fmt==6),blocks=[3,2,1,0,3])
 for mode,dest in itertools.product(range(8),(0x30000,0x7fffbc,0x7fffc0)):yield dict(entry=QUAD,mode=mode,destination=dest)
 for entry,mode,n in itertools.product((COPY,TRANSFER),(0,4),(4,24,64)):yield dict(entry=entry,mode=mode,length=n)
 for entry,n in itertools.product((DIV,REM),(0,1,0x7fffffff,0x80000000,0xffffffff)):
  for d in (0,1,3,0x1000,0xffffffff):yield dict(entry=entry,mode=(n&1)*4,numerator=n,divisor=d)
 for width,height,order,bank in itertools.product((0,1,2),(1,2),(0,1),(0,1)):
  yield dict(entry=LAYOUT,mode=bank*4,width=width,height=height,order=order,bank=bank,blocks=[0,1,2,3,1],clip=1,clip_x=0,clip_y=0,alias_last=bank,stride_mode=bank)
 for flags,quad,order,boundary,selector in itertools.product((0,32),(0,1),(0,1),(0,1),(0,1)):
  yield dict(entry=LAYOUT,mode=0,width=2,height=2,flags=flags,quad_format=quad,order=order,boundary_mode=boundary,boundary_selector=selector,blocks=[1,2,3,0,1],transfer_busy=1)
 for slot,header,busy,lengths in itertools.product(range(4),(0,1),(0,1),([0]*5,[32]*5,[0,32,0,32,0])):
  states=[-1]*4;states[slot]=1
  yield dict(entry=SCHEDULE,mode=(slot&1)*4,states=states,extra_header=header,transfer_busy=busy,lengths=lengths)
 for busy in range(7):yield dict(entry=SCHEDULE,mode=0,states=[1,-1,-1,-1],lengths=[32]*5,extra_header=1,busy_slot=busy)
 for slot,blit,flags,toggle in itertools.product(range(4),(0,3),(0,32,33),(0,1)):
  states=[-1]*4;states[slot]=3
  yield dict(entry=FLIP,mode=toggle*4,states=states,blit_mask=blit,flags=16*toggle,mode_flags=flags,framebuffer_override=toggle,width=2,height=1,quad_format=toggle)
 for countdown,mode_flags,parity,flags in itertools.product((0,1,2,0x80000000,0xffffffff),(0,4),(0,1),(0,2)):
  yield dict(entry=IRQ,mode=parity*4,countdown=countdown,mode_flags=mode_flags,parity=parity,flags=flags,hardware_parity=parity,work_flag=int(countdown==1),counter=0xffffffff if parity else 0)
 for state,slot,timeout,ticks in itertools.product((-3,-1,0,1,2,3,4,5),(0,3),(0,1),(0,4)):
  states=[-1]*4;states[slot]=state
  yield dict(entry=IRQ,mode=0,states=states,timeout_a=timeout,timeout_b=timeout,render_ticks=ticks,command_ticks=ticks,dirty=1,render_busy=int(state==4),blit_mask=3,width=1,height=1,flags=32 if timeout else 0,quad_format=ticks&1)
 for stall,ticks in itertools.product((0,1),(0,15,16,0xffffffff)):yield dict(entry=IRQ,mode=0,stall=stall,stall_ticks=ticks,block=int(not stall))
 for scenario,renderer,flags,slot,mode in itertools.product(('normal','second_exit','timeout'),(1,2),(0,32),(0,3),(0,4)):
  yield dict(entry=SUBMIT,mode=mode,renderer=renderer,flags=flags,slot=slot,lengths=[32]*5,command_ticks=8 if scenario=='normal' else 0,timeout_b=4 if scenario=='normal' else 0,timeout_a=0xffffffff if scenario=='second_exit' else 0,block=int(scenario=='timeout'))

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_interrupt_connection.json',PROJECT/'analysis/boot_submit_verification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for i,c in enumerate(cases()):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,actual,s,f=compare(m,s,f,c)
  record['independent_checks']=semantic(m,actual,initial,fp,s,f,record,c);records.append(record);coverage.update(path)
  for j,a in enumerate(path):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((a,path[j+distance]))
  if (i+1)%64==0:print(i+1,'interrupt/completion comparisons passed',flush=True)
 rows=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=PER_BODY[a];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 files=[Path(__file__),PROJECT/'verify_boot_submit.py',PROJECT/'verify_boot_device.py',PROJECT/'verify_boot_wait.py',PROJECT/'verify_draw_view_registers.py']+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=hashlib.sha256((PROJECT/manifest['source']).read_bytes()).hexdigest(),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,independent_checks=sum(r['independent_checks'] for r in records),vram_word_writes=sum(len(r['vram_writes']) for r in records),ack_writes=sum(len(r['ack_writes']) for r in records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original interrupt/completion/buffer family and existing arithmetic/copy adapters, with full reached/final machine/RAM/FPU/banks/MACH/MACL/GBR, eight-MiB VRAM, IRQ acknowledge state, register/queue/packet state and every timer/busy input matched. Native IRQ callbacks execute actual guest code; no game-service result is replaced. Registration remains separate.',limitations=['Direct software entry/callback tests do not establish hardware IRQ delivery, acceptance/vectoring, physical DMA/GPU execution, timing, visible pixels or natural startup.','Bounded 32-bit VRAM aperture and normal-event write-one-to-clear acknowledgement only. Other VRAM apertures and full device semantics are not implemented. Finite busy/timer samples are explicit peripheral inputs; exhaustion stops with -4.','Original waits are unbounded without the research define. The research callback waits permit 64 continued tests, while transfer retry/reset limits remain two; no successful game return is fabricated by a limit.','Runtime table/buffer and CPU state fixtures, inherited immutable code/literal, distinct stack/data and qualified numeric/MMU-off domains apply. All ports, gameplay and requested graphics/settings goals remain unfinished.'],cases=records)
 (PROJECT/'analysis/boot_interrupt_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','coverage_by_routine','independent_checks','vram_word_writes','ack_writes')},indent=2))

def smoke():
 ram=previous.previous.seed_ram()
 selected=[dict(entry=e,mode=0) for e in (RESET,SELECT,SCHEDULE,FLIP,REGS,QUAD,DIV,REM,TRANSFER,COPY,LAYOUT,IRQ)]+[dict(entry=SUBMIT,mode=0,command_ticks=8,timeout_b=4)]
 for c in selected:
  record,path,*_=compare(*prepare(ram,c),c);print(hex(c['entry']),record['status'],record['stop_pc'],len(path),'steps',flush=True)
  assert record['status']==0,c
 print('Interrupt smoke passed:',len(selected))
if __name__=='__main__':smoke() if '--smoke' in sys.argv else main()
