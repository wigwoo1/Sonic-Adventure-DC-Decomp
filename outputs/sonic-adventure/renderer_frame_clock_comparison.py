"""Keep all complete comparisons; additionally compare clock state and accesses."""
import frame_clock_native as model
from frame_clock_reference import FrameClock
ADDRESSES=(0xffd80000,0xffd80004,0xffd80008,0xffd8000c,0xffd80010)
CONFIGS=(0x5f8044,0x5f80d0,0x5f80d8,0x5f80cc,0x5f80dc)

def watched(parent):
 def build(*args):
  ref=parent(*args);seed=args[-1].get('frame_clock_seed')
  ref.frame_clock=None if seed is None else FrameClock.restore(seed)
  class WithClock(type(ref)):
   def __contains__(self,a):return bool(self.frame_clock is not None and (a in ADDRESSES or a&0x1fffffff==0x5f810c)) or super().__contains__(a)
   def __getitem__(self,a):
    clock=self.frame_clock
    if clock is not None and (a in ADDRESSES or a&0x1fffffff==0x5f810c):
     rc,value=clock.read(a,self.access_width);assert rc==0,('frame clock read',hex(a),rc)
     if a==0xffd8000c:self.clock_cursor+=1;self.clock_reads.append((a,value,self.access_width))
     return value
    value=super().__getitem__(a)
    if clock is not None and a&0x1fffffff==0x5f6900:
     value|=clock.pending;clock.trace.append((3,a,value,self.access_width))
    return value
   def __setitem__(self,a,value):
    clock=self.frame_clock
    if clock is not None and (a in ADDRESSES or a&0x1fffffff==0x5f810c):
     rc=clock.write(a,value,self.access_width);assert rc==0,('frame clock write',hex(a),rc)
     return
    if clock is not None and a&0x1fffffff in CONFIGS:
     trial=FrameClock.restore(clock.serializable());trial.trace=list(clock.trace)
     rc=trial.write(a,value,self.access_width);assert rc==0,('frame clock configuration',hex(a),rc)
     result=super().__setitem__(a,value);self.frame_clock=trial;return result
    result=super().__setitem__(a,value)
    if clock is not None and a&0x1fffffff==0x5f6900:
     clock.pending&=~value;clock.trace.append((4,a,value,self.access_width))
    return result
  ref.__class__=WithClock
  return ref
 return build

def extend(code):
 def change(a,b):
  nonlocal code
  assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
 marker=" old.runtime_attached=bool(c.get('runtime_attached'))"
 change(marker," old.native_frame_clock=frame_model.restore(c['frame_clock_seed'],lib=effects.LIB) if c.get('frame_clock_seed') is not None else None\n"+marker)
 marker=' def observe(ctx,target,mem,fp,sr,r,pr):'
 sync="native_clock[:]=[(a,v,w) for k,a,v,w in frame_model.events(old.native_frame_clock) if k==1 and a==TIMER]"
 change(marker,marker+'\n  if old.native_frame_clock is not None:'+sync)
 marker='old.native_flash_snapshot()))'
 change(marker,'old.native_flash_snapshot(),frame_model.snapshot(old.native_frame_clock) if old.native_frame_clock is not None else None))')
 marker=' journal.check()'
 change(marker,' if old.native_frame_clock is not None:\n  '+sync+"\n  assert frame_model.snapshot(old.native_frame_clock)==ref.frame_clock.snapshot(),'complete persistent frame-clock state and ordered accesses'\n"+marker)
 change(' return dict('," return dict(frame_clock_out=frame_model.serialize(old.native_frame_clock) if old.native_frame_clock is not None else None,frame_clock_trace=frame_model.events(old.native_frame_clock) if old.native_frame_clock is not None else (),")
 return code
