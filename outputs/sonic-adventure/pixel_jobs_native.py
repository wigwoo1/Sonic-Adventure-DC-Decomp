"""Native immutable command capture and completion after actual raster work."""
import ctypes,hashlib
from dataclasses import dataclass,field
from pathlib import Path
import pvr_pixels_native as p
import render_completion_native as r
U=p.U;Q=p.Q;B=p.B;Bus=r.Bus
class State(ctypes.Structure):_fields_=[('job_id',Q),('processed',Q),('last_job',Q),('cursor',U),('count',U),('ready',U),('words',U*512),('stats',p.Stats)]
class Binding(ctypes.Structure):_fields_=[('state',ctypes.POINTER(State)),('render',ctypes.POINTER(r.State)),('frame',ctypes.c_void_p),('parent',Bus)]
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/pixel_jobs.dll';LIB=ctypes.CDLL(str(DLL));p.declare(LIB);r.declare(LIB)
LIB.sa_pixel_job_init.argtypes=[ctypes.POINTER(State)];LIB.sa_pixel_job_init.restype=None
LIB.sa_pixel_job_bind.argtypes=[ctypes.POINTER(Binding),ctypes.POINTER(State),ctypes.POINTER(r.State),ctypes.c_void_p,ctypes.POINTER(Bus),ctypes.POINTER(Bus)];LIB.sa_pixel_job_bind.restype=ctypes.c_int
LIB.sa_pixel_job_execute.argtypes=[ctypes.POINTER(State),ctypes.POINTER(r.State),ctypes.POINTER(B),U,ctypes.POINTER(U),ctypes.POINTER(U),Q,ctypes.POINTER(p.Options)];LIB.sa_pixel_job_execute.restype=ctypes.c_int
def snapshot(s):return s.job_id,s.processed,s.last_job,s.cursor,s.count,s.ready,tuple(s.words),p.snapshot(s.stats)
def restored(seed):return State.from_buffer_copy(seed.state)
@dataclass(frozen=True,repr=False)
class Seed:
 state:bytes=field(repr=False)
 pixels:bytes=field(repr=False)
 depth:bytes=field(repr=False)
 options:tuple
 def summary(self):
  s=State.from_buffer_copy(self.state)
  return dict(job_id=s.job_id,processed=s.processed,last_job=s.last_job,cursor=s.cursor,queued_packets=s.count,ready=s.ready,stats=p.snapshot(s.stats),pixels_sha256=hashlib.sha256(self.pixels).hexdigest(),depth_sha256=hashlib.sha256(self.depth).hexdigest(),options=self.options)
 def __repr__(self):return 'PixelSeed('+repr(self.summary())+')'
def initial(width=640,height=480,textures=True,level=0):
 s=State();LIB.sa_pixel_job_init(ctypes.byref(s));color=(U*(width*height))(*([0xff101018]*(width*height)))
 return Seed(bytes(s),bytes(color),bytes(width*height*4),(width,height,640,480,int(textures),level,3,128))
def image_digests(pixels,depth):return hashlib.sha256(pixels).digest(),hashlib.sha256(depth).digest()
