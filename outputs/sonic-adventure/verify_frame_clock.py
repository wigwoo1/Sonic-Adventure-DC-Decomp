"""Independent raster-line and timer tests for the discrete-event clock domain."""
import ctypes,hashlib,json,random
from pathlib import Path
import frame_clock_native as n
from frame_clock_reference import FrameClock,CAPACITY
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/frame_clock_verification.json';assert not target.exists()
 rng=random.Random(0x6050b0);checks=operations=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def write(s,a,value,width):return n.LIB.sa_frame_clock_write(ctypes.byref(s),a,value,width)
 def read(s,a,width):
  out=n.U(0xabcdef12);rc=n.LIB.sa_frame_clock_read(ctypes.byref(s),a,width,ctypes.byref(out));return rc,out.value
 for case in range(240):
  h=rng.randrange(4,65);v=rng.randrange(4,33);i=rng.randrange(v);o=rng.randrange(v)
  args=(50000000,27000000,(case&1)<<23,(case&2)<<3,((v-1)<<16)|(h-1),(o<<16)|i,((v-2)<<16)|1)
  s=n.initialized(*args);r=FrameClock(*args)
  for a,value,width in ((0xffd80010,2,2),(0xffd80008,7,4),(0xffd8000c,3,4),(0xffd80004,1,1)):
   require(write(s,a,value,width)==r.write(a,value,width)==0,'timer setup')
  for step in range(28):
   before=n.core_snapshot(s)
   choice=rng.randrange(4)
   if choice==0:
    reason=rng.randrange(2);rc=n.LIB.sa_frame_clock_next_in(ctypes.byref(s),reason);wanted=r.next_in(reason)
   elif choice==1:
    a=rng.choice((0xffd8000c,0xa05f810c));rc,value=read(s,a,4);wanted,expected=r.read(a,4)
    require(value==(expected if not wanted else 0xabcdef12),'exact register result')
    require(n.core_snapshot(s)==before,'reads never advance time or physical counters')
   else:
    cycles=rng.randrange(4000);rc=n.LIB.sa_frame_clock_advance(ctypes.byref(s),cycles);wanted=r.advance(cycles)
   require(rc==wanted and n.snapshot(s)==r.snapshot(),'line-by-line oracle including exact native clock trace')
   require(n.LIB.sa_frame_clock_status(ctypes.byref(s))==r.status(),'scan/field/vertical-window projection')
   operations+=1
 profile=(50000000,27000000,0x800004,0x100,0x20c0359,0x150208,0x280208)
 s=n.initialized(*profile);r=FrameClock(*profile)
 for frame in range(12):
  require(n.LIB.sa_frame_clock_next_in(ctypes.byref(s),0)==r.next_in(0)==0,'original configured VBlank advance')
  require(n.snapshot(s)==r.snapshot() and s.core.line==520 and s.core.hpos==0 and s.core.in_events==frame+1,'actual display-register period and interrupt line')
  if frame:require(s.core.wraps==frame,'steady frame wrap count')
 for _ in range(400):
  a=n.initialized(*profile);b=n.State.from_buffer_copy(a);elapsed=rng.randrange(1<<48);part=rng.randrange(elapsed+1)
  require(n.LIB.sa_frame_clock_advance(ctypes.byref(a),elapsed)==n.LIB.sa_frame_clock_advance(ctypes.byref(b),part)==n.LIB.sa_frame_clock_advance(ctypes.byref(b),elapsed-part)==0 and n.core_snapshot(a)==n.core_snapshot(b),'large intervals are independent of host chunking')
 s=n.initialized(*profile);saved=bytes(s)
 require(n.LIB.sa_frame_clock_write(ctypes.byref(s),0xa05f80d8,0,4)==-2 and bytes(s)==saved,'unsupported mode change is atomic')
 require(n.LIB.sa_frame_clock_write(ctypes.byref(s),0xa05f810c,0,4)==-3 and bytes(s)==saved,'scan status is read-only')
 require(read(s,0xa05f810c,2)==(-3,0xabcdef12) and bytes(s)==saved,'invalid status width leaves output/state unchanged')
 s.core.timer.peripheral_cycles=(1<<64)-1;saved=bytes(s)
 require(n.LIB.sa_frame_clock_advance(ctypes.byref(s),1)==-6 and bytes(s)==saved,'time overflow preserves the whole state')
 s=n.initialized(*profile);s.trace_count=CAPACITY;saved=bytes(s)
 require(n.LIB.sa_frame_clock_next_in(ctypes.byref(s),0)==-6 and read(s,0xffd8000c,4)==(-6,0xabcdef12) and bytes(s)==saved,'bounded trace never silently loses qualified accesses')
 s=n.initialized(*profile);s.core.enabled=0;saved=bytes(s)
 require(n.LIB.sa_frame_clock_next_in(ctypes.byref(s),1)==n.LIB.sa_frame_clock_advance(ctypes.byref(s),1000000)==0 and bytes(s)==saved,'disabled clock does not advance or create events')
 s=n.initialized(*profile);calls=[]
 @n.t.READ
 def parent_read(ctx,a,width,out):calls.append(('read',a,width));out[0]=0x80;return 0
 @n.t.WRITE
 def parent_write(ctx,a,value,width):calls.append(('write',a,value,width));return -8 if value==0xdeadbeef else 0
 @n.t.CACHE
 def parent_cache(ctx,a,op):return -9
 parent=n.Bus(None,parent_read,parent_write,parent_cache);bound=n.Bus();binding=n.Binding()
 require(n.LIB.sa_frame_clock_bind(ctypes.byref(binding),ctypes.byref(s),ctypes.byref(parent),ctypes.byref(bound))==0,'binding')
 require(n.LIB.sa_frame_clock_next_in(ctypes.byref(s),0)==0 and s.core.pending==24,'VBlank events are latched, not written into guest RAM')
 out=n.U();require(bound.read(bound.context,0xa05f6900,4,ctypes.byref(out))==0 and out.value==0x98,'clock events merge with parent status')
 require(bound.write(bound.context,0xa05f6900,8,4)==0 and s.core.pending==16,'acknowledge only selected pending event')
 saved=s.core.pending;require(bound.write(bound.context,0xa05f6900,0xdeadbeef,4)==-8 and s.core.pending==saved,'parent failure preserves pending clock events')
 require(bound.write(bound.context,0xa05f6900,16,4)==0 and s.core.pending==0,'remaining event acknowledgement')
 require(bound.cache(bound.context,0x8c100000,2)==-9,'parent cache status is preserved')
 files=[Path(__file__),P/'frame_clock_native.py',P/'frame_clock_reference.py',P/'source/frame_clock.c',P/'source/frame_clock.h',P/'source/tmu0_clock.c',P/'source/tmu0_clock.h',P/'build_frame_clock.ps1',n.DLL]
 result=dict(status='native_discrete_event_frame_clock_contract_verified',random_operations=operations,profiles=240,checks=checks,large_interval_partition_cases=400,original_configured_periods=12,reads_advance_time=False,events_from_register_comparisons=True,automatic_host_and_idle_boundary_time=True,guest_instruction_cycle_accuracy_qualified=False,physical_board_timing_qualified=False,physical_GPU_pixels_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,scope='Independent line-by-line oracle versus native modular-count arithmetic; shared elapsed PCLK drives the existing TMU0 core and scan counters. Tests progressive/interlaced projected fields, rate division, rational remainder, complete trace order, latched VBlank events, ack merging, disabled clock, explicit unsupported mode changes and bounded trace faults. Clock profile rates/reset phase are declared; hsync/electrical blank, physical phase, HBlank events and pixels are not qualified.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('VERIFIED frame clock',operations,'operations;',checks,'checks',flush=True)

if __name__=='__main__':main()
