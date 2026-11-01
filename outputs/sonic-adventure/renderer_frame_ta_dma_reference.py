"""Independent untimed TA packet-transfer contract; no game-callee hooks."""
from pvr_dma_reference import *
import pvr_dma_reference as previous


class Reference(previous.Reference):
 def written(self,a,value,width,ram,write,runtime):
  if a!=ADDRESSES[7] or width!=4 or not value&1 or not 0x10000000<=self.registers[5]<0x10800000:
   return super().written(a,value,width,ram,write,runtime)
  self.registers[7]=value
  if self.active:return -4
  self.active=1;self.started+=1;self.progress=0;self.status=0
  r=self.registers;self.source=r[0]&0x1fffffe0;self.length=r[6];offset=r[5]&0x7fffe0;self.destination=0x10000000+offset
  if not self.enabled:return 0
  valid=(r[4]&0xffff8201)==0x8201 and 0xc000000<=self.source<=0xd000000 and self.length<=0xd000000-self.source and not self.length&31 and self.length<=0x800000-offset
  if not valid:self.status=-2;return -2
  payload=bytes(ram[self.source-0xc000000:self.source-0xc000000+self.length])
  for pos in range(0,len(payload),4):
   value=int.from_bytes(payload[pos:pos+4],'little');rc=write(self.destination+pos,value)
   if rc:self.status=rc;return rc
   for word in (self.source+pos,self.destination+pos,value):self.signature=((self.signature^word)*16777619)&0xffffffff
   self.progress+=4
  for index,value in [(3,r[3]|2),(2,0),(5,self.destination+self.length),(6,0),(7,0)]:
   rc=write(ADDRESSES[index],value)
   if rc:self.status=rc;return rc
   r[index]=value
  self.active=0;self.completed+=1;self.pending|=1<<19
  if runtime is not None:runtime.pending|=1<<19
  return 0


def command_word(ref,address,value):
 if address&3:return -3
 lane=(address>>2)&7;base=address&~31
 if len(ref.packets)>=ref.packet_capacity:return -4
 if lane==0:ref.ta_packet=(base,[])
 part=getattr(ref,'ta_packet',None)
 if part is None or part[0]!=base or len(part[1])!=lane:return -3
 part[1].append(value&0xffffffff)
 if len(part[1])==8:
  ref.packets.append((base,2,tuple(part[1])));ref.events.append((9,base,2,32));ref.ta_packet=None
 return 0
