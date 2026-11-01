"""Native lifecycle state; backend completion is explicit and never means pixels."""
import ctypes
from pathlib import Path
from tmu0_clock_native import U,Q,Bus,READ,WRITE,CACHE
OFFSETS=(0x20,0x2c,0x30,0x48,0x4c,0x60,0x64,0x68,0x6c,0x74,0x78,0x7c,0x80,0x84,0x88,0x8c)
class State(ctypes.Structure):_fields_=[(n,Q) for n in ('submitted','completed','cancelled','active_id')]+[('pending',U),('enabled',U),('parameters',U*16),('job',U*16)]
class Binding(ctypes.Structure):_fields_=[('state',ctypes.POINTER(State)),('parent',Bus)]
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/render_completion.dll';LIB=ctypes.CDLL(str(DLL))
def declare(lib):
 for name,args,result in (('init',[ctypes.POINTER(State),ctypes.POINTER(U)],None),('finish',[ctypes.POINTER(State),Q],ctypes.c_int),('bind',[ctypes.POINTER(Binding),ctypes.POINTER(State),ctypes.POINTER(Bus),ctypes.POINTER(Bus)],ctypes.c_int)):
  f=getattr(lib,'sa_render_completion_'+name);f.argtypes=args;f.restype=result
declare(LIB)
def snapshot(s):return tuple(getattr(s,n) for n in ('submitted','completed','cancelled','active_id','pending','enabled'))+(tuple(s.parameters),tuple(s.job))
def serialize(s):return dict(zip(('submitted','completed','cancelled','active_id','pending','enabled','parameters','job'),snapshot(s)))
def initialized(parameters=None,lib=LIB):
 s=State();a=None if parameters is None else (U*16)(*parameters);lib.sa_render_completion_init(ctypes.byref(s),a);return s
def restore(d,lib=LIB):
 s=initialized(d['parameters'],lib)
 for name,value in d.items():
  if name in ('parameters','job'):getattr(s,name)[:]=value
  else:setattr(s,name,value)
 return s
