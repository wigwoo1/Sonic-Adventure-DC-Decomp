"""ctypes declarations for the authored peripheral-clock implementation."""
import ctypes
from pathlib import Path
U=ctypes.c_uint32;Q=ctypes.c_uint64
READ=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,ctypes.POINTER(U))
WRITE=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,U)
CACHE=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U)
class Bus(ctypes.Structure):_fields_=[('context',ctypes.c_void_p),('read',READ),('write',WRITE),('cache',CACHE)]
class State(ctypes.Structure):_fields_=[('peripheral_cycles',Q),('underflows',Q)]+[(name,U) for name in ('tcor','tcnt','tcr','tstr','tocr')]
class Binding(ctypes.Structure):_fields_=[('state',ctypes.POINTER(State)),('parent',Bus)]
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/tmu0_clock.dll';LIB=ctypes.CDLL(str(DLL))
LIB.sa_tmu0_init.argtypes=[ctypes.POINTER(State)];LIB.sa_tmu0_init.restype=ctypes.c_int
LIB.sa_tmu0_advance.argtypes=[ctypes.POINTER(State),Q];LIB.sa_tmu0_advance.restype=ctypes.c_int
LIB.sa_tmu0_read.argtypes=[ctypes.POINTER(State),U,U,ctypes.POINTER(U)];LIB.sa_tmu0_read.restype=ctypes.c_int
LIB.sa_tmu0_write.argtypes=[ctypes.POINTER(State),U,U,U];LIB.sa_tmu0_write.restype=ctypes.c_int
LIB.sa_tmu0_irq_pending.argtypes=[ctypes.POINTER(State)];LIB.sa_tmu0_irq_pending.restype=U
LIB.sa_tmu0_bind.argtypes=[ctypes.POINTER(Binding),ctypes.POINTER(State),ctypes.POINTER(Bus),ctypes.POINTER(Bus)];LIB.sa_tmu0_bind.restype=ctypes.c_int
def snapshot(s):return tuple(getattr(s,name) for name,_ in State._fields_)
def initialized():
 s=State();assert LIB.sa_tmu0_init(ctypes.byref(s))==0;return s
