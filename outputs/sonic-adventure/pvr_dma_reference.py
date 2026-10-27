"""Independent untimed transfer oracle; no retail instruction execution or hooks."""
import ctypes
U=ctypes.c_uint32
ADDRESSES=(0xffa00020,0xffa00024,0xffa00028,0xffa0002c,0xffa00040,0xa05f6800,0xa05f6804,0xa05f6808,0xa05f6884,0xa05f6888)
TAIL=('source','destination','length','progress','started','completed','pending','signature','status','enabled','active')
ERROR=('error_status','error_reads','error_writes','error_signature','error_count','error_dropped')
def state_type(Bus):
 class State(ctypes.Structure):_fields_=[('parent',Bus),('ram',ctypes.POINTER(ctypes.c_uint8)),('interrupt_pending',ctypes.POINTER(U)),('registers',U*10)]+[(n,ctypes.c_int32 if n=='status' else U) for n in TAIL]+[(n,U) for n in ERROR]+[('error_trace',(U*3)*256)]
 return State
def declare(lib,Bus,State):
 lib.sa_pvr_dma_bus_init.argtypes=[ctypes.POINTER(State),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Bus),ctypes.POINTER(Bus),ctypes.POINTER(U),ctypes.POINTER(U),U];lib.sa_pvr_dma_bus_init.restype=ctypes.c_int
def snapshot(s):return (tuple(s.registers),)+tuple(getattr(s,n) for n in TAIL)+(tuple(getattr(s,n) for n in ERROR),)
def seed(c):
 result=list(c.get('pvr_dma_seed',[0]*10));assert len(result)==10
 result[4]=c.get('dma_operation',result[4]);return result

class Reference:
 def __init__(self,c):
  self.registers=seed(c)
  for n in TAIL:setattr(self,n,0)
  self.signature=2166136261;self.enabled=int(c.get('pvr_dma_enabled',True))
  for n in ERROR:setattr(self,n,0)
  self.error_signature=2166136261;self.error_events=[]
 def snapshot(self):return snapshot(self)
 def error_access(self,a,value,op,width=4):
  if width!=4 or a&3:return -3,None
  if op==1:value=self.error_status;self.error_reads+=1
  else:self.error_status&=~value;self.error_writes+=1
  for word in (a,value,op):self.error_signature=((self.error_signature^word)*16777619)&0xffffffff
  if len(self.error_events)<256:self.error_events.append((a,value,op));self.error_count+=1
  else:self.error_dropped+=1
  return 0,value
 def acknowledge(self,value,runtime):
  self.pending&=~value
  if runtime is not None:runtime.pending&=~value
 def written(self,a,value,width,ram,write,runtime):
  if a not in ADDRESSES or width!=4:return 0
  index=ADDRESSES.index(a);self.registers[index]=value
  if index!=7:return 0
  if not value&1:self.active=0;return 0
  if self.active:return -4
  self.active=1;self.started+=1;self.progress=0;self.status=0
  r=self.registers;self.source=r[0]&0x1fffffe0;self.length=r[6];offset=r[5]&0x7fffe0;mode=r[9 if r[5]&0x2000000 else 8];self.destination=(0xa4000000 if mode==0 else 0xa5000000)+offset
  if not self.enabled:return 0
  valid=(r[4]&0xffff8201)==0x8201 and mode in (0,1) and r[5]&0xfc000000==0x10000000 and r[5]&0x1000000 and 0xc000000<=self.source<=0xd000000 and self.length<=0xd000000-self.source and not self.length&31 and self.length<=0x800000-offset
  if not valid:self.status=-2;return -2
  payload=bytes(ram[self.source-0xc000000:self.source-0xc000000+self.length])
  for pos in range(0,len(payload),4):
   value=int.from_bytes(payload[pos:pos+4],'little');status=write(self.destination+pos,value)
   if status:self.status=status;return status
   for word in (self.source+pos,self.destination+pos,value):self.signature=((self.signature^word)*16777619)&0xffffffff
   self.progress+=4
  completion={3:r[3]|2,2:0,5:self.destination+self.length,6:0,7:0}
  for index,value in completion.items():
   status=write(ADDRESSES[index],value)
   if status:self.status=status;return status
   r[index]=value
  self.active=0;self.completed+=1;self.pending|=1<<19
  if runtime is not None:runtime.pending|=1<<19
  return 0
