"""Independent channel-2 data/register/pending-IRQ controls; no game hooks."""
import ctypes,hashlib,itertools,json,struct,types
from pathlib import Path
import pvr_dma_reference as model
import vram_aperture_reference as aperture
from verify_vram_aperture_bus import Bus,Read,Write,Cache,U
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def cases():
 for length,mode,port,source in itertools.product((0,32,64,32768),(0,1),(0,1),(0x0c900000,0x8c900000)):
  yield dict(length=length,mode=mode,port=port,source=source,offset=0x1000,enabled=True)
 base=dict(length=64,mode=0,port=0,source=0x8c900000,offset=0x1000,enabled=True)
 for name,extra in (('disabled',dict(enabled=False)),('bad-dmaor',dict(dmaor=0)),('bad-mode',dict(mode=2)),('unaligned-length',dict(length=4)),('source-bound',dict(source=0x8d000000)),('vram-bound',dict(offset=0x7fffe0)),('polygon-unsupported',dict(raw_destination=0x10001000)),('word-failure',dict(fail_word=3)),('completion-failure',dict(fail_completion=True))):yield base|extra|dict(control=name)

def main():
 dll=ROOT/'work/sonic-adventure/build/movie_buffer_initialize.dll';lib=ctypes.CDLL(str(dll));State=model.state_type(Bus);Aperture=aperture.state_type(Bus);model.declare(lib,Bus,State);aperture.declare(lib,Bus,Aperture)
 ram=bytearray(0x1000000);ram[0x900000:0x910000]=bytes(((i*73)^(i>>3)^0x5a)&255 for i in range(0x10000));initial_hash=hashlib.sha256(ram).hexdigest();ram_view=(ctypes.c_uint8*len(ram)).from_buffer(ram);records=[]
 for number,c in enumerate(cases(),1):
  native=bytearray([0xa5])*0x800000;expected=bytearray(native);registers={};expected_registers={};actual_events=[];wanted=[];phase=[False];native_words=[0];reference_words=[0];pending=U(0);runtime=types.SimpleNamespace(pending=0)
  def parent_write(a,value,width,reference=False):
   events=wanted if reference else actual_events;memory=expected if reference else native;regs=expected_registers if reference else registers;count=reference_words if reference else native_words;events.append((a,value,width))
   if 0x05000000<=a&0x1fffffff<0x05800000:
    if c.get('fail_word')==count[0]:return -7
    count[0]+=1;offset=a&0x7fffff;memory[offset:offset+4]=value.to_bytes(4,'little');return 0
   if c.get('fail_completion') and phase[0] and a==0xffa0002c:return -7
   if a&0x1fffffff==0x5f6900:regs[a]=regs.get(a,0)&~value
   else:regs[a]=value
   return 0
  @Read
  def read(ctx,a,width,out):out[0]=registers.get(a,0);return 0
  @Write
  def write(ctx,a,value,width):return parent_write(a,value,width)
  @Cache
  def cache(ctx,a,op):return 0
  parent=Bus(None,read,write,cache);ap=Bus();ap_state=Aperture();ap_trace=(aperture.Event*0x10000)();assert lib.sa_vram_aperture_bus_init(ctypes.byref(ap_state),ctypes.byref(parent),ctypes.byref(ap),ap_trace,len(ap_trace))==0
  state=State();bus=Bus();seed=(U*10)();assert lib.sa_pvr_dma_bus_init(ctypes.byref(state),ram_view,ctypes.byref(ap),ctypes.byref(bus),ctypes.byref(pending),seed,int(c['enabled']))==0
  ref=model.Reference(dict(pvr_dma_enabled=c['enabled']));ap_ref=aperture.Reference(0x10000)
  def expected_write(a,value):
   if aperture.inside(a):
    ap_ref.attempts+=1;mapped=aperture.mapping(a);status=parent_write(mapped,value,4,True)
    if not status:ap_ref.record(a,mapped,value,2)
    return status
   return parent_write(a,value,4,True)
  destination=c.get('raw_destination',(0x13000000 if c['port'] else 0x11000000)+c['offset'])
  configured=((0xffa00040,c.get('dmaor',0x8201)),(0xffa00020,c['source']),(0xffa00024,0),(0xffa00028,c['length']//32),(0xffa0002c,0x12c1),(0xa05f6800,destination),(0xa05f6804,c['length']),(0xa05f6884,c['mode'] if not c['port'] else 0),(0xa05f6888,c['mode'] if c['port'] else 0))
  for a,value in configured:
   assert bus.write(bus.context,a,value,4)==0;assert expected_write(a,value)==0;assert ref.written(a,value,4,ram,expected_write,runtime)==0
  phase[0]=True;actual=bus.write(bus.context,0xa05f6808,1,4);assert expected_write(0xa05f6808,1)==0;result=ref.written(0xa05f6808,1,4,ram,expected_write,runtime)
  assert actual==result,(number,c,actual,result)
  assert model.snapshot(state)==ref.snapshot(),(number,c,model.snapshot(state),ref.snapshot())
  assert pending.value==runtime.pending and actual_events==wanted and native==expected and registers==expected_registers,(number,c,'complete data,registers orordering')
  assert aperture.snapshot(ap_state)==ap_ref.snapshot()
  assert ctypes.string_at(ap_state.trace,ap_state.count*16)==b''.join(struct.pack('<IIII',*e) for e in ap_ref.events[:ap_ref.capacity])
  if result==0 and c['enabled']:
   assert state.completed==1 and state.progress==c['length'] and pending.value==1<<19 and not state.active
   out=U(0);assert bus.read(bus.context,0xa05f6900,4,ctypes.byref(out))==0 and out.value==1<<19
   assert bus.write(bus.context,0xa05f6900,1<<19,4)==0;assert expected_write(0xa05f6900,1<<19)==0;ref.acknowledge(1<<19,runtime)
   assert pending.value==runtime.pending==state.pending==0 and model.snapshot(state)==ref.snapshot()
  elif not c['enabled']:assert state.progress==state.completed==state.pending==pending.value==0 and state.active==1 and registers[0xa05f6808]==1
  else:assert not state.completed and not state.pending and not pending.value and state.active==1
  assert hashlib.sha256(ram).hexdigest()==initial_hash
  records.append(dict(input=c,status=result,state=model.snapshot(state),vram_sha256=hashlib.sha256(native).hexdigest(),parent_operations=len(actual_events),source_bytes_unchanged=True));print(number,c.get('control','copy'),result,state.progress,flush=True)
 files=[Path(__file__),Path(model.__file__),P/'vram_aperture_reference.py',P/'verify_vram_aperture_bus.py',P/'source/pvr_dma_bus.c',P/'source/pvr_dma_bus.h',P/'source/vram_aperture_bus.c',P/'source/vram_aperture_bus.h',dll]
 report=dict(cases_passed=len(records),original_recovery_credit=0,platform_backend_only=True,cases=records,full_vram_and_source_bytes_compared=True,complete_register_trace_and_pending_irq_compared=True,scope='Untimed channel2 directtexture ports11/13,64/32bitviews,DRAMaliases,length0/32/64/32768,disabled/unsupportedprotocol/range/mode/failureprefixcontrols. CompleteVRAM/sourceRAM/registerwriteorder/device/shadow/aperture/pendingIRQ state compare with independent byte-payload oracle. Deviceonly writeshardwarestate;gamequeue retirement andIRQdelivery are separateoriginal-code tests. No polygon/YUV DMA,wrappingoutofrange,physicaltiming orfullhardwareclaim.',dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files})
 (P/'analysis/pvr_dma_bus_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('PVR DMA backend',len(records),'cases passed')

if __name__=='__main__':main()
