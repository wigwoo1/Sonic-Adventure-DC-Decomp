"""Platform contract tests: real packet data, exact status, acknowledgments/faults."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import write_watch_renderer_frame_events as w
import frame_events_packed_reference as model
Bus=w.effects.Bus;State=w.EventsState;P=Path(__file__).resolve().parent;R=P.parents[1]
Memory=w.previous.namespace['RtcMemory']
Frame=type(Memory().frame)
READ,WRITE,CACHE=(dict(Bus._fields_)[n] for n in ('read','write','cache'))


def packet(word,*rest):return (0x10000000,2,(word,)+tuple(rest)+(0,)*(7-len(rest)))


def main():
 target=P/'analysis/frame_events_bus_verification.json';assert not target.exists()
 cases=[];checks=0
 def exact(ok):
  nonlocal checks
  assert ok;checks+=1
 def run(label,packets,expected_status=0,expected_pending=None,seed=None,enabled=True,fail_parent=False,count=None):
  frame=Frame();frame.packet_count=len(packets) if count is None else count;frame.packet_capacity=64
  for out,(destination,queue,words) in zip(frame.packets,packets):out.destination=destination;out.queue=queue;out.words[:]=words
  parent_status=1<<17;calls=[]
  @READ
  def read(ctx,a,n,out):out[0]=parent_status;calls.append(('read',a,n));return -3 if fail_parent else 0
  @WRITE
  def write(ctx,a,value,n):calls.append(('write',a,value,n));return -3 if fail_parent else 0
  @CACHE
  def cache(ctx,a,op):calls.append(('cache',a,op));return -3 if fail_parent else 0
  parent=Bus(None,read,write,cache);out=Bus();state=State();pending=ctypes.c_uint32(1<<17)
  values=(ctypes.c_uint32*9)(*seed) if seed is not None else None
  status=w.effects.LIB.sa_frame_events_bus_init(ctypes.byref(state),ctypes.byref(frame),ctypes.byref(parent),ctypes.byref(out),ctypes.byref(pending),values,int(enabled))
  ref=model.Reference(seed,enabled);runtime=type('Pending',(),{})();runtime.pending=(1<<17)|ref.pending
  expected=ref.consume(packets,runtime) if count is None else -6
  exact(status==expected_status==expected)
  exact(model.snapshot(state)==ref.snapshot());exact(pending.value==runtime.pending)
  if status==0:
   if expected_pending is not None:exact(state.pending==expected_pending)
   value=ctypes.c_uint32();rc=out.read(out.context,0xa05f6900,4,ctypes.byref(value))
   exact(rc==(-3 if fail_parent else 0))
   if not fail_parent:exact(value.value==(parent_status|ref.pending))
   before=model.snapshot(state);rc=out.cache(out.context,0xe0000000,1)
   exact(rc==(-3 if fail_parent else 0));exact(model.snapshot(state)==before)
   before=model.snapshot(state);rc=out.write(out.context,0xa05f6900,0xffffffff,4)
   exact(rc==(-3 if fail_parent else 0))
   if fail_parent:exact(model.snapshot(state)==before)
   else:
    ref.acknowledge(0xffffffff,runtime);exact(model.snapshot(state)==ref.snapshot());exact(pending.value==0)
  cases.append(dict(label=label,status=status,input_packets=len(packets),enabled=enabled,expected_pending=expected_pending))
 for channel,kind,vertices in itertools.product(range(5),(4,5),(0,1,4)):
  supported=vertices==0 or (kind==4 and channel in (0,2,4))
  run(f'list{channel}_header{kind}_vertices{vertices}',[packet((kind<<29)|(channel<<24))]+[packet(0xe0000000)]*vertices+[packet(0)],expected_status=0 if supported else -2,expected_pending=(128,256,512,1024,2097152)[channel] if supported else None)
 for flags in (0x10,0x30,0x40,0x80):run('unsupported_vertex_header_'+hex(flags),[packet(0x80000000|flags)],expected_status=-2)
 run('actual_textured_intensity32_header',[packet(0x828c0028),packet(0xe0000000),packet(0xf0000000),packet(0)],expected_pending=512)
 run('incomplete_list_no_completion',[packet(0x82000000),packet(0xf0000000)],expected_pending=0)
 run('disabled_does_not_consume',[packet(0x80000000),packet(0)],expected_pending=0,enabled=False)
 run('all_five_lists_combined',[p for channel in range(5) for p in (packet(0x80000000|(channel<<24)),packet(0))],expected_pending=0x200780)
 run('all64_packet_slots',[packet(0x80000000)]+[packet(0xe0000000)]*62+[packet(0)],expected_pending=128)
 run('clipping_command_retains_current_list',[packet(0x20000000),packet(0x82000000),packet(0x20000000),packet(0)],expected_pending=512)
 for label,packets in [('orphan_vertex',[packet(0xe0000000)]),('orphan_end',[packet(0)]),('nonzero_end',[packet(0x80000000),packet(0,1)]),('unknown_list',[packet(0x85000000)]),('unsupported_objectlist',[packet(0x40000000)]),('unsupported_parameter3',[packet(0x60000000)]),('unsupported_parameter6',[packet(0xc0000000)]),('duplicate_end',[packet(0x80000000),packet(0),packet(0)])]:run(label,packets,-2)
 run('packet_count_above_capacity',[],expected_status=-6,count=65)
 run('cursor_above_packet_count',[],expected_status=-6,seed=(1,0,0,0,0,0,0,0,1))
 run('parent_failure_preserved',[packet(0x80000000),packet(0)],expected_pending=128,fail_parent=True)
 files=[Path(__file__),P/'frame_events_packed_reference.py',P/'source/frame_events_bus_packed.c',P/'source/frame_events_bus.h',R/'work/sonic-adventure/build/renderer_frame_events_packed.dll']
 report=dict(status='untimed_TA_list_event_contract_verified',cases_passed=len(cases),checks=checks,cases=cases,game_code_executed=False,game_RAM_written=False,physical_timing=False,rasterization=False,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Frame event contracts',len(cases),'cases',checks,'checks',flush=True)


if __name__=='__main__':main()
