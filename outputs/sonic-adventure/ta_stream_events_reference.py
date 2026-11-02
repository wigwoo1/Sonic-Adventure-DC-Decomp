"""Independent accepted-packet TA-list oracle; no game RAM or rasterization."""
import ctypes
FIELDS=('cursor','pending','list','active','headers','vertices','ends','rejected','enabled')
EVENTS={0:128,1:256,2:512,3:1024,4:2097152}


def state_type(Bus):
 class State(ctypes.Structure):
  _fields_=[('parent',Bus),('frame',ctypes.c_void_p),('interrupt_pending',ctypes.POINTER(ctypes.c_uint32))]+[(n,ctypes.c_uint32) for n in FIELDS]
 return State


def snapshot(s):return tuple(getattr(s,n) for n in FIELDS)


class Reference:
 def __init__(self,seed=None,enabled=True):
  for n,v in zip(FIELDS,seed if seed is not None else (0,)*9):setattr(self,n,v)
  self.enabled=int(bool(enabled))
 def snapshot(self):return snapshot(self)
 def consume(self,packets,runtime=None):
  if not self.enabled:return 0
  if len(packets)>0xffffffff or self.cursor>len(packets):return -6
  for _,_,words in packets[self.cursor:]:
   tag=words[0]//(2**29)
   if tag in (4,5):
    channel=(words[0]//(2**24))%8
    if channel not in EVENTS or (tag==4 and words[0]&0xd0 and words[0]%256 not in (16,18)):self.rejected+=1;return -2
    self.list=channel;self.active=1 if tag==4 and channel in (0,2,4) else 2;self.headers+=1
   elif tag==7:
    if self.active!=1:self.rejected+=1;return -2
    self.vertices+=1
   elif tag==0:
    if words[0] or not self.active or self.list not in EVENTS:self.rejected+=1;return -2
    self.pending|=EVENTS[self.list]
    if runtime is not None:runtime.pending|=EVENTS[self.list]
    self.active=0;self.ends+=1
   elif tag!=1:self.rejected+=1;return -2
   self.cursor+=1
  return 0
 def acknowledge(self,value,runtime=None):
  self.pending&=~value
  if runtime is not None:runtime.pending&=~value


def declare(lib,Bus,State):
 lib.sa_frame_events_bus_init.argtypes=[ctypes.POINTER(State),ctypes.c_void_p,ctypes.POINTER(Bus),ctypes.POINTER(Bus),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32]
 lib.sa_frame_events_bus_init.restype=ctypes.c_int
