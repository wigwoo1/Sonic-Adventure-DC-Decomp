"""TA DMA endpoint contract: exact packets,register effects,capacity and faults."""
import ctypes,hashlib,itertools,json,types
from pathlib import Path
import renderer_frame_ta_dma_reference as model
import renderer_frame_ta_reference as v
import verify_boot_submit as submit
P=Path(__file__).resolve().parent;R=P.parents[1]


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_frame_ta_dma_verification.json';assert not target.exists()
 lib=ctypes.CDLL(str(R/'work/sonic-adventure/build/renderer_frame_ta.dll'));Bus=v.effects.Bus;State=model.state_type(Bus);model.declare(lib,Bus,State)
 lib.sa_submit_register_bus_init.argtypes=[ctypes.POINTER(submit.SubmitMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Bus)]
 Write=dict(Bus._fields_)['write'];Read=dict(Bus._fields_)['read'];Cache=dict(Bus._fields_)['cache'];records=[];checks=oracles=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 cases=[dict(length=n,source=a,dest=d,capacity=64) for n,a,d in itertools.product((0,32,64,160,192,2048),(0x8cd00000,0xacd00000),(0x10000000,0x10000100))]
 cases += [dict(length=32,source=0x8cffffe0,dest=0x107fffe0,capacity=64),dict(length=64,source=0x8cffffe0,dest=0x10000000,capacity=64),dict(length=64,source=0x8cd00000,dest=0x107fffe0,capacity=64),dict(length=33,source=0x8cd00000,dest=0x10000000,capacity=64),dict(length=64,source=0x8bd00000,dest=0x10000000,capacity=64),dict(length=64,source=0x8cd00000,dest=0x10800000,capacity=64)]
 cases += [dict(length=64,source=0x8cd00000,dest=0x10000000,capacity=64,dmor=0x8200),dict(length=64,source=0x8cd00000,dest=0x10000000,capacity=64,active=True)]
 cases += [dict(length=192,source=0x8cd00000,dest=0x10000000,capacity=n) for n in (0,1,2,5)]
 cases += [dict(length=192,source=0x8cd00000,dest=0x10000000,capacity=64,reject_word=n) for n in (0,3,8,13)]
 for spec in cases:
  length=spec['length'];ram=bytearray(0x1000000);source=spec['source']&0x1fffffe0;offset=source-0xc000000
  payload=bytes((i*37+11)&255 for i in range(length))
  if 0<=offset<=len(ram)-len(payload):ram[offset:offset+len(payload)]=payload
  before=hashlib.sha256(ram).digest();view=(ctypes.c_uint8*len(ram)).from_buffer(ram);memory=submit.SubmitMemory();frame_bus=Bus();lib.sa_submit_register_bus_init(ctypes.byref(memory),view,ctypes.byref(frame_bus));memory.frame.packet_capacity=spec['capacity']
  registers={};native_writes=[];word_calls=0;raw_write=Write(ctypes.cast(frame_bus.write,ctypes.c_void_p).value)
  @Read
  def read(_ctx,a,n,out):
   if n!=4 or a&3:return -3
   out[0]=registers.get(a,0);return 0
  @Write
  def write(_ctx,a,value,n):
   nonlocal word_calls
   if n!=4 or a&3:return -3
   if 0x10000000<=a<0x10800000:
    index=word_calls;word_calls+=1
    if index==spec.get('reject_word') :return -3
    return raw_write(frame_bus.context,a,value,n)
   registers[a]=value;native_writes.append((a,value));return 0
  @Cache
  def cache(_ctx,a,op):return -2
  parent=Bus(None,read,write,cache);out=Bus();state=State();pending=ctypes.c_uint32()
  seed=[spec['source'],0,(length+31)//32,0x12c1,spec.get('dmor',0x8201),spec['dest'],length,0,0,0];seed_array=(ctypes.c_uint32*10)(*seed)
  require(lib.sa_pvr_dma_bus_init(ctypes.byref(state),view,ctypes.byref(parent),ctypes.byref(out),ctypes.byref(pending),seed_array,1)==0,'native DMA initialization')
  ref=model.Reference(dict(pvr_dma_seed=seed));sink=types.SimpleNamespace(packets=[],packet_capacity=spec['capacity'],events=[],ta_packet=None);expected_registers={};expected_writes=[];ref_words=0
  def reference_write(a,value):
   nonlocal ref_words
   if 0x10000000<=a<0x10800000:
    index=ref_words;ref_words+=1
    if index==spec.get('reject_word'):return -3
    return model.command_word(sink,a,value)
   expected_registers[a]=value;expected_writes.append((a,value));return 0
  if spec.get('active'):state.active=ref.active=1
  rc=out.write(out.context,model.ADDRESSES[7],1,4)
  # The parent sees the initiating MMIO write before execution,as in the
  # retained wrapper;the independent transfer oracle observes it separately.
  expected_registers[model.ADDRESSES[7]]=1;expected_writes.append((model.ADDRESSES[7],1))
  expected_rc=ref.written(model.ADDRESSES[7],1,4,ram,reference_write,None)
  require(rc==expected_rc,('typed return',spec,rc,expected_rc));require(model.snapshot(state)==model.snapshot(ref),'all DMA registers,counters,signature,errors andpending state')
  require(registers==expected_registers and native_writes==expected_writes,'every ordered MMIO write')
  packets=[(p.destination,p.queue,tuple(p.words)) for p in memory.frame.packets[:memory.frame.packet_count]]
  require(packets==sink.packets,'complete ordered native/reference packets')
  exact(hashlib.sha256(ram).digest()==before,'entire source RAM unchanged')
  if rc==0:
   expected=[((spec['dest']&~31)+pos,2,tuple(int.from_bytes(payload[pos+j:pos+j+4],'little') for j in range(0,32,4))) for pos in range(0,length,32)]
   exact(packets==expected,'independent complete32-byte payload chunking/order/destination')
   exact(state.progress==length and state.started==state.completed==1 and not state.active and pending.value==state.pending==1<<19,'independent untimed DMA completion19,without invented list IRQ')
  elif spec.get('active'):exact(rc==-4 and not packets and state.started==0,'active transfer rejection before restart')
  elif 'reject_word' in spec:exact(rc==-3 and state.progress==spec['reject_word']*4 and state.active==1 and not state.completed,'independent exact partial progress onrejected sink word')
  elif length%32 or not 0xc000000<=source<=0xd000000 or length>0xd000000-source or spec['dest']>=0x10800000 or length>0x10800000-(spec['dest']&~31) or spec.get('dmor',0x8201)!=0x8201:exact(rc==-2 and not packets and state.progress==state.completed==0,'invalid descriptor rejects before data')
  else:exact(rc==-4 and len(packets)==spec['capacity'] and state.progress==32*spec['capacity'],'independent capacity exhaustion atpacket boundary')
  partial=sink.ta_packet
  if partial:
   slot=memory.frame.packets[memory.frame.packet_count];require(slot.destination==partial[0] and slot.queue==0x80000000|len(partial[1]) and list(slot.words[:len(partial[1])])==partial[1],'partial packet state retained exactly onfault')
  exact(memory.frame.graphics.vertex.prefetches==memory.frame.graphics.vertex.flushes==memory.frame.graphics.vertex.invalidates==0,'DMA doesnot invent CPU store-queue operations')
  exact(not any(memory.frame.graphics.vertex.queue) and not any(memory.frame.graphics.vertex.qacr),'DMA leaves CPU storequeue words andQACR unchanged')
  records.append(dict(input=spec,status=rc,packets=len(packets),progress=state.progress,started=state.started,completed=state.completed,pending=state.pending))
 files=[Path(__file__),P/'renderer_frame_ta_dma_reference.py',P/'source/pvr_ta_dma_bus.c',P/'source/frame_ta_command_bus.c',R/'work/sonic-adventure/build/renderer_frame_ta.dll']
 report=dict(status='TA_DMA_packet_capture_contract_matches_reference_and_independent_payload_progress_oracles',cases=len(records),checks=checks,independent_exact_oracles=oracles,cases_detail=records,physical_timing_or_TA_list_interrupts_or_pixels_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,scope='Authored untimed TA DMA input endpoint. Exact ordered32-byte packets,all transfer fields/MMIO writes,partial faults,capacity andboundary rejection. Doesnot claimreal original game execution orTA/rasterizer/list-complete IRQ semantics.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('TA DMA CONTRACT',len(records),checks,oracles,flush=True)


if __name__=='__main__':main()
