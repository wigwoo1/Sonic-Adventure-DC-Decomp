"""Independent untimed native-device oracle and ctypes layout; no game execution."""
import ctypes
from verify_g2_transfer_host import Job
U=ctypes.c_uint32
TAIL=('pending','polls','transfers','bytes','deliveries','blocked','in_irq','enabled')
class State(ctypes.Structure):
 _fields_=[('registers',U*32),('progress',U*4),('active',U*4),('jobs',Job*4)]+[(n,U) for n in TAIL]
def snapshot(s):
 return (tuple(s.registers),tuple(s.progress),tuple(s.active),tuple(tuple(getattr(j,n) for n,_ in Job._fields_) for j in s.jobs))+tuple(getattr(s,n) for n in TAIL)
def seed(s,values=None,enabled=True):
 ctypes.memset(ctypes.byref(s),0,ctypes.sizeof(s))
 if values is None:s.enabled=bool(enabled);return
 s.registers[:]=values[0];s.progress[:]=values[1];s.active[:]=values[2]
 for j,row in zip(s.jobs,values[3]):
  for (name,_),value in zip(Job._fields_,row):setattr(j,name,value)
 for name,value in zip(TAIL,values[4:]):setattr(s,name,value)

class Device:
 def __init__(self,values=None,enabled=True):
  self.registers=[0]*32;self.progress=[0]*4;self.active=[0]*4;self.jobs=[[0]*5 for _ in range(4)]
  for n in TAIL:setattr(self,n,0)
  self.enabled=int(bool(enabled))
  if values is not None:
   self.registers=list(values[0]);self.progress=list(values[1]);self.active=list(values[2]);self.jobs=[list(j) for j in values[3]]
   for name,value in zip(TAIL,values[4:]):setattr(self,name,value)
 def snapshot(self):return (tuple(self.registers),tuple(self.progress),tuple(self.active),tuple(tuple(j) for j in self.jobs))+tuple(getattr(self,n) for n in TAIL)
 def written(self,a,value,n=4):
  p=a&0x1fffffff
  if p==0x5f6900 and n==4:self.pending&=~value
  if not (n==4 and not a&3 and 0x5f7800<=p<0x5f7880):return 0
  word=(p-0x5f7800)//4;channel,index=divmod(word,8);self.registers[word]=value
  if index in (5,6) and not value&1:self.active[channel]=0;self.progress[channel]=0
  if index==6 and value&1 and self.registers[channel*8+5]&1 and not self.registers[channel*8+4]&2:
   row=self.registers[channel*8:channel*8+8]
   if row[3]>1:return -2
   self.jobs[channel]=[channel,row[0],row[1],row[2],row[3]];self.progress[channel]=0;self.active[channel]=1
  return 0
 def tick(self,ram,read,write):
  if not self.enabled or self.in_irq:return 0
  self.polls+=1
  for channel in range(4):
   if not self.active[channel]:continue
   _,g2,root,raw,direction=self.jobs[channel];length=raw&0x7fffffff;pos=self.progress[channel]
   if pos>length:return -6
   if length&31 or direction>1 or length>0xffffffff-root or length>0xffffffff-g2:return -2
   source,destination=(g2,root) if direction else (root,g2)
   while pos<length:
    data=[]
    for offset in range(0,32,4):
     a=source+pos+offset
     if 0xc000000<=a<=0xcfffffc:value=int.from_bytes(ram[a-0xc000000:a-0xc000000+4],'little')
     elif 0x800000<=a<=0x9ffffc:
      status,value=read(a)
      if status:return status
     else:return -2
     data.append(value)
    for offset,value in zip(range(0,32,4),data):
     a=destination+pos+offset
     if 0xc000000<=a<=0xcfffffc:ram[a-0xc000000:a-0xc000000+4]=value.to_bytes(4,'little')
     elif 0x800000<=a<=0x9ffffc:
      status=write(a,value)
      if status:return status
     else:return -2
    pos+=32;self.progress[channel]=pos;self.bytes+=32
   base=0xa05f7800+channel*32
   status=write(base+24,0)
   if status:return status
   self.registers[channel*8+6]=0
   if raw&0x80000000:
    status=write(base+20,0)
    if status:return status
    self.registers[channel*8+5]=0
   self.active[channel]=0;self.pending|=1<<(15+channel);self.transfers+=1
  return 0
