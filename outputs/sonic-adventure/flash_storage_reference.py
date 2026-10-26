"""Independent explicit host flash policy; neither a BIOS dump nor game code."""
import ctypes
U=ctypes.c_uint32
class Partition(ctypes.Structure):
 _fields_=[('start',U),('size',U)]
class State(ctypes.Structure):
 _fields_=[('bytes',ctypes.POINTER(ctypes.c_uint8))]+[(n,U) for n in ('size','configured','writable','partition_count')]+[('partitions',Partition*5),('calls',ctypes.c_uint64*4)]+[(n,ctypes.c_uint64) for n in ('read_bytes','programmed_bytes','erased_bytes')]

def declare(lib):
 lib.sa_flash_storage_size.restype=U
 assert lib.sa_flash_storage_size()==ctypes.sizeof(State)
 lib.sa_flash_storage_init.argtypes=[ctypes.POINTER(State)]
 lib.sa_flash_storage_configure.argtypes=[ctypes.POINTER(State),ctypes.POINTER(ctypes.c_uint8),U,ctypes.POINTER(Partition),U,U]
 lib.sa_flash_storage_service.argtypes=[ctypes.POINTER(State),ctypes.POINTER(ctypes.c_uint8),U,U,U,U,ctypes.POINTER(U)]
 for name in ('init','configure','service'):getattr(lib,'sa_flash_storage_'+name).restype=ctypes.c_int

def snapshot(s):
 return (s.size,s.configured,s.writable,s.partition_count,tuple((p.start,p.size) for p in s.partitions),tuple(s.calls),s.read_bytes,s.programmed_bytes,s.erased_bytes)

class Reference:
 def __init__(self,data,parts,writable=True):
  assert 0<len(data) and 0<=len(parts)<=5
  occupied=set()
  for first,size in parts:
   assert 0<size and 0<=first and first+size<=len(data)
   span=set(range(first,first+size));assert not occupied&span;occupied|=span
  self.data=bytearray(data);self.parts=list(parts);self.writable=int(writable)
  self.calls=[0]*4;self.read_bytes=self.programmed_bytes=self.erased_bytes=0
 def snapshot(self):
  return (len(self.data),1,self.writable,len(self.parts),tuple(self.parts+[(0,0)]*(5-len(self.parts))),tuple(self.calls),self.read_bytes,self.programmed_bytes,self.erased_bytes)
 def service(self,ram,selector,first,second,count):
  if selector not in range(4):return -8,None
  answer=0xffffffff
  def output(size):
   if second>>29 not in (0,4,5):return None
   physical=second&0x1fffffff
   if not 0xc000000<=physical<=0xd000000 or physical+size>0xd000000:return None
   return physical-0xc000000
  if selector==0 and first<len(self.parts):
   at=output(8)
   if at is None:return -3,None
   ram[at:at+8]=b''.join(x.to_bytes(4,'little') for x in self.parts[first]);answer=0
  elif selector in (1,2) and count<0x80000000 and first+count<=len(self.data):
   at=output(count)
   if at is None:return -3,None
   if selector==1:
    ram[at:at+count]=self.data[first:first+count];self.read_bytes+=count;answer=count
   elif self.writable:
    old=self.data[first:first+count];wanted=ram[at:at+count]
    if all((previous|desired)==previous for previous,desired in zip(old,wanted)):
     self.data[first:first+count]=wanted;self.programmed_bytes+=count;answer=count
  elif selector==3 and self.writable:
   partition=next(((a,n) for a,n in self.parts if a==first),None)
   if partition:
    a,n=partition;self.data[a:a+n]=bytes([255])*n;self.erased_bytes+=n;answer=0
  self.calls[selector]=(self.calls[selector]+1)&0xffffffffffffffff
  return 0,answer
