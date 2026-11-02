"""Opaque native owned TA pages; the legacy bounded structs retain their ABI."""
import ctypes,hashlib
from pathlib import Path
from ta_list_lifecycle_native import COUNTERS,FIELDS
U=ctypes.c_uint32;Q=ctypes.c_uint64;V=ctypes.c_void_p
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/model_ta_jobs.dll';LIB=ctypes.CDLL(str(DLL))
class Packet(ctypes.Structure):_fields_=[('destination',U),('queue',U),('words',U*8)]
LIB.sa_ta_stream_create_win32.argtypes=[];LIB.sa_ta_stream_create_win32.restype=V
LIB.sa_ta_stream_destroy.argtypes=[V];LIB.sa_ta_stream_destroy.restype=None
for name in ('count','pages'):
 f=getattr(LIB,'sa_ta_stream_'+name);f.argtypes=[V,U];f.restype=U
LIB.sa_ta_stream_packet.argtypes=[V,U,U,ctypes.POINTER(Packet)];LIB.sa_ta_stream_packet.restype=ctypes.c_int
LIB.sa_ta_stream_append.argtypes=[V,ctypes.POINTER(Packet)];LIB.sa_ta_stream_append.restype=ctypes.c_int
LIB.sa_ta_stream_snapshot.argtypes=[V];LIB.sa_ta_stream_snapshot.restype=ctypes.c_int
LIB.sa_ta_stream_events.argtypes=[V,ctypes.POINTER(U)];LIB.sa_ta_stream_events.restype=ctypes.c_int
LIB.sa_ta_stream_lifecycle.argtypes=[V,ctypes.POINTER(Q)];LIB.sa_ta_stream_lifecycle.restype=ctypes.c_int
current=None
def declare(Bus):
 LIB.sa_ta_stream_bind.argtypes=[V,V,ctypes.POINTER(Bus),ctypes.POINTER(Bus),ctypes.POINTER(U),ctypes.POINTER(U),ctypes.POINTER(Q)];LIB.sa_ta_stream_bind.restype=ctypes.c_int
 return object
def packets(which=0):
 assert current;out=[]
 for i in range(LIB.sa_ta_stream_count(current,which)):
  p=Packet();assert LIB.sa_ta_stream_packet(current,which,i,ctypes.byref(p))==0;out.append((p.destination,p.queue,tuple(p.words)))
 return tuple(out)
def events():
 out=(U*9)();assert LIB.sa_ta_stream_events(current,out)==0;return tuple(out)
class LifecycleSeed:
 def __init__(self,seed,enabled):
  self.values=tuple(seed[n] if seed is not None else (int(enabled) if n=='enabled' else 0) for n in COUNTERS+FIELDS)
  self.last=[] if seed is None else seed['last']
def restore(seed=None,enabled=True):return LifecycleSeed(seed,enabled)
def snapshot(s):
 out=(Q*8)();assert LIB.sa_ta_stream_lifecycle(current,out)==0
 last=packets(1);return tuple(out),last+((0,0,(0,)*8),)*max(0,64-len(last))
def observation(s):
 counters,last=snapshot(s);raw=b''.join(bytes(Packet(a,q,(U*8)(*w))) for a,q,w in last)
 return counters,hashlib.sha256(raw).digest()
def serialize(s):
 values,_=snapshot(s);return dict(zip(COUNTERS+FIELDS,values),last=[[a,q,list(w)] for a,q,w in packets(1)])
def scoped(fn):
 def compare(*args):
  global current
  assert current is None
  try:return fn(*args)
  finally:
   from stream_pixel_jobs_native import dispose_all
   dispose_all()
   if current:LIB.sa_ta_stream_destroy(current)
   current=None
 return compare
