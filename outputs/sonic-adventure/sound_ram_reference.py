"""Independent declared RAM-window policy, not ARM/device execution."""
import ctypes
BASE=0x00800000;SIZE=0x200000

def inside(address):return BASE<=address&0x1fffffff<BASE+SIZE
def valid(address,width):return width in (1,2,4) and not address&(width-1) and BASE<=address&0x1fffffff<=BASE+SIZE-width

class Event(ctypes.Structure):_fields_=[(n,ctypes.c_uint32) for n in ('kind','address','value','width')]

def memory_type(Bus):
 class Memory(ctypes.Structure):
  _fields_=[('parent',Bus),('ram',ctypes.POINTER(ctypes.c_uint8)),('trace',ctypes.POINTER(Event))]+[(n,ctypes.c_uint32) for n in ('bytes','trace_capacity','trace_count','trace_dropped')]+[(n,ctypes.c_uint64) for n in ('reads','writes','read_bytes','write_bytes')]
 return Memory

def declare(lib,Bus,Memory):
 lib.sa_sound_ram_bus_init.argtypes=[ctypes.POINTER(Memory),ctypes.POINTER(Bus),ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32,ctypes.POINTER(Event),ctypes.c_uint32,ctypes.POINTER(Bus)]
 lib.sa_sound_ram_bus_init.restype=ctypes.c_int

def snapshot(m):return tuple(getattr(m,n) for n in ('reads','writes','read_bytes','write_bytes','trace_count','trace_dropped'))

class Reference:
 def __init__(self,seed=0xa5,configured=True,capacity=0x90000):
  assert 0<=seed<=255 and 0<=capacity<=0x1000000
  self.ram=bytearray([seed])*SIZE;self.configured=configured;self.capacity=capacity;self.events=[];self.reads=self.writes=self.read_bytes=self.write_bytes=0
 def read(self,address,width):
  if not valid(address,width):return -3,None
  if not self.configured:return -2,None
  offset=(address&0x1fffffff)-BASE;value=int.from_bytes(self.ram[offset:offset+width],'little')
  self.reads+=1;self.read_bytes+=width;self.events.append((32,address,value,width));return 0,value
 def write(self,address,value,width):
  if not valid(address,width):return -3
  if not self.configured:return -2
  offset=(address&0x1fffffff)-BASE;value&=(1<<(width*8))-1
  self.ram[offset:offset+width]=value.to_bytes(width,'little');self.writes+=1;self.write_bytes+=width;self.events.append((33,address,value,width));return 0
 def snapshot(self):return self.reads,self.writes,self.read_bytes,self.write_bytes,min(len(self.events),self.capacity),max(0,len(self.events)-self.capacity)
