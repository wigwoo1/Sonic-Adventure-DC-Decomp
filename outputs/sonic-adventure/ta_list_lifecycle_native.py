"""A separate guest-strobed list collector lifecycle; submitted jobs are immutable."""
import ctypes,hashlib
from pathlib import Path
U=ctypes.c_uint32;Q=ctypes.c_uint64
COUNTERS=('inits','retired','discarded','soft_resets');FIELDS=('opb_base','isp_base','last_count','enabled')
class Packet(ctypes.Structure):_fields_=[('destination',U),('queue',U),('words',U*8)]
class State(ctypes.Structure):_fields_=[(n,Q) for n in COUNTERS]+[(n,U) for n in FIELDS]+[('last',Packet*64)]
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/ta_list_lifecycle.dll';LIB=ctypes.CDLL(str(DLL))
LIB.sa_ta_list_lifecycle_init.argtypes=[ctypes.POINTER(State),U];LIB.sa_ta_list_lifecycle_init.restype=None
def declare(Bus):
 class Binding(ctypes.Structure):_fields_=[('state',ctypes.POINTER(State)),('frame',ctypes.c_void_p),('events',ctypes.c_void_p),('job',ctypes.c_void_p),('parent',Bus)]
 LIB.sa_ta_list_lifecycle_bind.argtypes=[ctypes.POINTER(Binding),ctypes.POINTER(State),ctypes.c_void_p,ctypes.c_void_p,ctypes.c_void_p,ctypes.POINTER(Bus),ctypes.POINTER(Bus)];LIB.sa_ta_list_lifecycle_bind.restype=ctypes.c_int
 return Binding
def serialize(s):
 return dict(**{n:getattr(s,n) for n in COUNTERS+FIELDS},last=[[p.destination,p.queue,list(p.words)] for p in s.last[:s.last_count]])
def restore(seed=None,enabled=True):
 s=State();LIB.sa_ta_list_lifecycle_init(ctypes.byref(s),enabled)
 if seed is not None:
  for n in COUNTERS+FIELDS:setattr(s,n,seed[n])
  assert len(seed['last'])==s.last_count<=64
  for p,(a,q,w) in zip(s.last,seed['last']):p.destination=a;p.queue=q;p.words[:]=w
 return s
def snapshot(s):return tuple(getattr(s,n) for n in COUNTERS+FIELDS),tuple((p.destination,p.queue,tuple(p.words)) for p in s.last)
def observation(s):return tuple(getattr(s,n) for n in COUNTERS+FIELDS),hashlib.sha256(bytes(s.last)).digest()
