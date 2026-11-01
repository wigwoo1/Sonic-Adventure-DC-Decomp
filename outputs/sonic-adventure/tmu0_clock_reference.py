"""Independent edge-by-edge TMU0 oracle in explicitly supplied PCLK cycles."""
from dataclasses import dataclass
DIVISORS=(4,16,64,256,1024)
ADDRESSES={0xffd80000:('tocr',1),0xffd80004:('tstr',1),0xffd80008:('tcor',4),0xffd8000c:('tcnt',4),0xffd80010:('tcr',2)}
MAX64=(1<<64)-1

@dataclass
class Clock:
 peripheral_cycles:int=0
 underflows:int=0
 tcor:int=0xffffffff
 tcnt:int=0xffffffff
 tcr:int=0
 tstr:int=0
 tocr:int=0

 def read(self,address,width):
  item=ADDRESSES.get(address)
  if item is None:return -2,None
  name,n=item
  return (0,getattr(self,name)) if width==n else (-3,None)

 def write(self,address,value,width):
  item=ADDRESSES.get(address)
  if item is None:return -2
  name,n=item
  if width!=n:return -3
  value&=(1<<(8*n))-1
  if name=='tocr':
   if value not in (0,1):return -2
  elif name=='tstr':
   if value not in (0,1) or (value and self.tcr&7>=5):return -2
  elif name in ('tcor','tcnt'):
   if self.tstr:return -2
  elif name=='tcr':
   if value&~0x13f:return -2
   if self.tstr and value&0x3f!=self.tcr&0x3f:return -2
   if value&self.tcr&0x100:return -2
   value=(value&0x3f)|(value&self.tcr&0x100)
  setattr(self,name,value);return 0

 def advance(self,cycles):
  if cycles<0 or self.peripheral_cycles+cycles>MAX64:return -6
  if self.tstr and self.tcr&7>=5:return -2
  if not self.tstr:self.peripheral_cycles+=cycles;return 0
  divisor=DIVISORS[self.tcr&7]
  # Deliberately simulate each selected clock edge instead of native division
  # over reload periods. Small randomized tests independently check the formula.
  next_edge=(self.peripheral_cycles//divisor+1)*divisor
  finish=self.peripheral_cycles+cycles;counter=self.tcnt;wraps=0
  for _ in range(next_edge,finish+1,divisor):
   if counter==0:counter=self.tcor;wraps+=1
   else:counter-=1
  if self.underflows+wraps>MAX64:return -6
  self.peripheral_cycles=finish;self.tcnt=counter;self.underflows+=wraps
  if wraps:self.tcr|=0x100
  return 0

 def pending(self):return int(self.tcr&0x120==0x120)

 def snapshot(self):return tuple(getattr(self,name) for name in self.__dataclass_fields__)
