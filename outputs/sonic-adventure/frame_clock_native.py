"""Shared declarations for the native TMU/scan clock and its persistent state."""
import ctypes
from pathlib import Path
import tmu0_clock_native as t
U=t.U;Q=t.Q;Bus=t.Bus
Q_NAMES=('fraction','pixels','in_events','out_events','wraps','host_frames','idle_waits')
U_NAMES=('pclk_hz','video_hz','fb_control','spg_control','load','vblank_int','vblank','hpos','line','field','pending','enabled')
class Core(ctypes.Structure):_fields_=[('timer',t.State)]+[(n,Q) for n in Q_NAMES]+[(n,U) for n in U_NAMES]
class Event(ctypes.Structure):_fields_=[(n,U) for n in ('kind','address','value','width')]
class State(ctypes.Structure):_fields_=[('core',Core),('timer_reads',Q),('scan_reads',Q),('trace_count',U),('trace',Event*1024)]
class Binding(ctypes.Structure):_fields_=[('state',ctypes.POINTER(State)),('parent',Bus)]
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/frame_clock.dll';LIB=ctypes.CDLL(str(DLL))

def declare(lib):
 for name,args in {
  'init':[ctypes.POINTER(State)]+[U]*7,
  'begin_entry':[ctypes.POINTER(State)],
  'advance':[ctypes.POINTER(State),Q],
  'next_in':[ctypes.POINTER(State),U],
  'status':[ctypes.POINTER(State)],
  'read':[ctypes.POINTER(State),U,U,ctypes.POINTER(U)],
  'write':[ctypes.POINTER(State),U,U,U],
  'bind':[ctypes.POINTER(Binding),ctypes.POINTER(State),ctypes.POINTER(Bus),ctypes.POINTER(Bus)]}.items():
  fn=getattr(lib,'sa_frame_clock_'+name);fn.argtypes=args;fn.restype=U if name=='status' else ctypes.c_int
declare(LIB)

def initialized(pclk,video,fb,control,load,interrupts,blank,lib=LIB):
 s=State();assert lib.sa_frame_clock_init(ctypes.byref(s),pclk,video,fb,control,load,interrupts,blank)==0;return s

def events(s):return tuple(tuple(getattr(e,n) for n,_ in Event._fields_) for e in s.trace[:s.trace_count])
def core_snapshot(s):return (t.snapshot(s.core.timer),)+tuple(getattr(s.core,n) for n in Q_NAMES+U_NAMES)
def snapshot(s):return core_snapshot(s),s.timer_reads,s.scan_reads,events(s)

def serialize(s):
 return dict(timer={n:getattr(s.core.timer,n) for n,_ in t.State._fields_},**{n:getattr(s.core,n) for n in Q_NAMES+U_NAMES},timer_reads=s.timer_reads,scan_reads=s.scan_reads)

def restore(d,lib=LIB):
 s=initialized(*(d[n] for n in U_NAMES[:7]),lib=lib)
 for n in Q_NAMES+U_NAMES:setattr(s.core,n,d[n])
 for n,v in d['timer'].items():setattr(s.core.timer,n,v)
 s.timer_reads=d['timer_reads'];s.scan_reads=d['scan_reads'];return s
