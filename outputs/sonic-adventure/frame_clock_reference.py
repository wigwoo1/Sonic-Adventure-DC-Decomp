"""Independent line-by-line discrete-event oracle; no game-callee hooks."""
from dataclasses import asdict
from tmu0_clock_reference import Clock,ADDRESSES,MAX64
CAPACITY=1024

class FrameClock:
 def __init__(self,pclk,video,fb,control,load,interrupts,blank):
  assert 0<pclk<=1000000000 and 0<video<=500000000 and not control&32
  self.timer=Clock();self.fraction=self.pixels=self.in_events=self.out_events=self.wraps=self.host_frames=self.idle_waits=0
  self.pclk_hz=pclk;self.video_hz=video;self.fb_control=fb;self.spg_control=control;self.load=load;self.vblank_int=interrupts;self.vblank=blank
  self.hpos=self.line=self.field=self.pending=0;self.enabled=1;self.timer_reads=self.scan_reads=0;self.trace=[]
  assert blank&1023<self.vtotal and (blank>>16)&1023<self.vtotal

 @property
 def htotal(self):return (self.load&1023)+1
 @property
 def vtotal(self):return ((self.load>>16)&1023)+1
 @property
 def rate(self):return (self.video_hz//(1 if self.fb_control&(1<<23) else 2))*(2 if self.spg_control&16 else 1)

 def begin_entry(self):self.trace=[]
 def advance(self,cycles):
  if not self.enabled:return 0
  if len(self.trace)>=CAPACITY:return -6
  if self.timer.peripheral_cycles+cycles>MAX64:return -6
  # Independent small-step oracle for qualified test intervals.
  pixels,fraction=divmod(self.fraction+cycles*self.rate,self.pclk_hz)
  if self.pixels+pixels>MAX64:return -6
  elapsed=self.timer.advance(cycles)
  if elapsed:return elapsed
  self.fraction=fraction;self.pixels+=pixels
  while pixels:
   count=min(pixels,self.htotal-self.hpos);pixels-=count;self.hpos+=count
   if self.hpos!=self.htotal:continue
   self.hpos=0;self.line+=1
   if self.line==self.vtotal:
    self.line=0;self.wraps+=1;self.field^=int(bool(self.spg_control&16))
   if self.line==self.vblank_int&1023:self.pending|=8;self.in_events+=1
   if self.line==(self.vblank_int>>16)&1023:self.pending|=16;self.out_events+=1
  self.trace.append((5,cycles&0xffffffff,cycles>>32,0));return 0

 def next_in(self,reason):
  if reason not in (0,1):return -6
  if not self.enabled:return 0
  if len(self.trace)>CAPACITY-2:return -6
  target=self.vblank_int&1023
  if target>=self.vtotal:return -4
  # Search line boundaries, rather than native modular-distance arithmetic.
  h=self.hpos;line=self.line;distance=0
  while True:
   distance+=self.htotal-h;h=0;line=(line+1)%self.vtotal
   if line==target:break
  numerator=distance*self.pclk_hz-self.fraction
  cycles=(numerator+self.rate-1)//self.rate
  result=self.advance(cycles)
  if result:return result
  if reason:self.idle_waits+=1
  else:self.host_frames+=1
  self.trace.append((6,reason,0,0));return 0

 def status(self):
  start=self.vblank&1023;end=(self.vblank>>16)&1023
  blank=start<=self.line<end if start<end else start>end and (self.line>=start or self.line<end)
  return self.line|(self.field<<10)|(int(blank)<<13)

 def read(self,a,width):
  if len(self.trace)>=CAPACITY:return -6,None
  if a in ADDRESSES:
   rc,value=self.timer.read(a,width)
   if rc:return rc,None
   if a==0xffd8000c:self.timer_reads+=1
  elif a&0x1fffffff==0x5f810c:
   if width!=4:return -3,None
   value=self.status();self.scan_reads+=1
  else:return -2,None
  self.trace.append((1,a,value,width));return 0,value

 def write(self,a,value,width):
  if len(self.trace)>=CAPACITY:return -6
  if a&0x1fffffff==0x5f810c:return -3
  configs={0x5f8044:('fb_control',1<<23),0x5f80d0:('spg_control',0x30),0x5f80d8:('load',0xffffffff),0x5f80cc:('vblank_int',0xffffffff),0x5f80dc:('vblank',0xffffffff)}
  if a&0x1fffffff in configs:
   name,mask=configs[a&0x1fffffff]
   if width!=4:return -3
   if (getattr(self,name)^value)&mask:return -2
   setattr(self,name,value);self.trace.append((7,a,value,width));return 0
  if a not in ADDRESSES:return -2
  rc=self.timer.write(a,value,width)
  if rc:return rc
  self.trace.append((2,a,value&((1<<(width*8))-1),width));return 0

 def core_snapshot(self):
  return (self.timer.snapshot(),)+(tuple(getattr(self,n) for n in ('fraction','pixels','in_events','out_events','wraps','host_frames','idle_waits')))+(tuple(getattr(self,n) for n in ('pclk_hz','video_hz','fb_control','spg_control','load','vblank_int','vblank','hpos','line','field','pending','enabled')))

 def snapshot(self):return self.core_snapshot(),self.timer_reads,self.scan_reads,tuple(self.trace)

 def serializable(self):
  return dict(timer=asdict(self.timer),**{name:getattr(self,name) for name in ('fraction','pixels','in_events','out_events','wraps','host_frames','idle_waits','pclk_hz','video_hz','fb_control','spg_control','load','vblank_int','vblank','hpos','line','field','pending','enabled','timer_reads','scan_reads')})

 @classmethod
 def restore(cls,d):
  result=cls(*(d[n] for n in ('pclk_hz','video_hz','fb_control','spg_control','load','vblank_int','vblank')))
  for name,value in d.items():
   if name=='timer':result.timer=Clock(**value)
   else:setattr(result,name,value)
  return result
