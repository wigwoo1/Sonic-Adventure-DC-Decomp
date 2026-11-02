"""Native page-backed immutable pixel jobs; serialized seeds contain no pointers."""
import ctypes,hashlib
from dataclasses import dataclass
import ta_job_stream_native as stream
import pvr_pixels_native as p
import render_completion_native as r
U=p.U;Q=p.Q;B=p.B;V=ctypes.c_void_p;Bus=r.Bus;LIB=stream.LIB;DLL=stream.DLL
p.declare(LIB);r.declare(LIB)
class State(ctypes.Structure):
 _fields_=[('job_id',Q),('processed',Q),('last_job',Q),('cursor',U),('count',U),('ready',U),('stats',p.Stats),('snapshot',V)]
class Binding(ctypes.Structure):_fields_=[('state',ctypes.POINTER(State)),('render',ctypes.POINTER(r.State)),('stream',V),('parent',Bus)]
LIB.sa_stream_pixel_job_bind.argtypes=[ctypes.POINTER(Binding),ctypes.POINTER(State),ctypes.POINTER(r.State),V,ctypes.POINTER(Bus),ctypes.POINTER(Bus)];LIB.sa_stream_pixel_job_bind.restype=ctypes.c_int
LIB.sa_stream_pixel_job_dispose.argtypes=[ctypes.POINTER(State)];LIB.sa_stream_pixel_job_dispose.restype=None
LIB.sa_stream_pixel_job_execute.argtypes=[ctypes.POINTER(State),ctypes.POINTER(r.State),ctypes.POINTER(B),U,ctypes.POINTER(U),ctypes.POINTER(U),Q,ctypes.POINTER(p.Options)];LIB.sa_stream_pixel_job_execute.restype=ctypes.c_int
LIB.sa_stream_banked_pixel_job_execute.argtypes=LIB.sa_stream_pixel_job_execute.argtypes[:4]+[ctypes.POINTER(B),U]+LIB.sa_stream_pixel_job_execute.argtypes[4:];LIB.sa_stream_banked_pixel_job_execute.restype=ctypes.c_int
LIB.sa_ta_stream_restore_packet.argtypes=[V,U,ctypes.POINTER(stream.Packet)];LIB.sa_ta_stream_restore_packet.restype=ctypes.c_int
LIB.sa_ta_stream_set_cursor.argtypes=[V,U];LIB.sa_ta_stream_set_cursor.restype=ctypes.c_int
_states=[]
def packets(state):
 if not state.snapshot:return ()
 out=[]
 for i in range(LIB.sa_ta_stream_count(state.snapshot,0)):
  packet=stream.Packet();assert LIB.sa_ta_stream_packet(state.snapshot,0,i,ctypes.byref(packet))==0;out.append((packet.destination,packet.queue,tuple(packet.words)))
 return tuple(out)
def words(state):return tuple(w for _,_,row in packets(state) for w in row)
def snapshot(state):return state.job_id,state.processed,state.last_job,state.cursor,state.count,state.ready,words(state),p.snapshot(state.stats)
def native_observation(state):
 data=(U*(state.count*8))(*words(state));return state.job_id,state.processed,state.last_job,state.cursor,state.count,state.ready,hashlib.sha256(bytes(data)).digest(),p.snapshot(state.stats)
def image_digests(pixels,depth):return hashlib.sha256(pixels).digest(),hashlib.sha256(depth).digest()
@dataclass(frozen=True,repr=False)
class Seed:
 state:bytes
 pixels:bytes
 depth:bytes
 options:tuple
 packets:tuple=()
 def summary(self):
  s=State.from_buffer_copy(self.state);assert not s.snapshot
  return dict(job_id=s.job_id,processed=s.processed,last_job=s.last_job,cursor=s.cursor,queued_packets=s.count,ready=s.ready,stats=p.snapshot(s.stats),pixels_sha256=hashlib.sha256(self.pixels).hexdigest(),depth_sha256=hashlib.sha256(self.depth).hexdigest(),options=self.options)
 def __repr__(self):return 'StreamPixelSeed('+repr(self.summary())+')'
def make_seed(state,pixels,depth,options):
 copy=State.from_buffer_copy(bytes(state));copy.snapshot=None;return Seed(bytes(copy),pixels,depth,options,packets(state))
def initial(width=640,height=480,textures=True,level=0):
 return Seed(bytes(State()),bytes.fromhex('181010ff')*(width*height),bytes(width*height*4),(width,height,640,480,int(textures),level,3,128))
def restored(seed):
 s=State.from_buffer_copy(seed.state);assert not s.snapshot and len(seed.packets)==s.count;_states.append(s)
 if seed.packets:
  s.snapshot=LIB.sa_ta_stream_create_win32();assert s.snapshot
  for a,q,row in seed.packets:
   packet=stream.Packet(a,q,(U*8)(*row));assert LIB.sa_ta_stream_append(s.snapshot,ctypes.byref(packet))==0
 return s
def dispose_all():
 for state in _states:LIB.sa_stream_pixel_job_dispose(ctypes.byref(state))
 _states.clear()
