"""ctypes entry points for the authored host texture/raster profile."""
import ctypes
from pathlib import Path
U=ctypes.c_uint32;Q=ctypes.c_uint64;B=ctypes.c_uint8
class Texture(ctypes.Structure):_fields_=[(n,U) for n in ('width','height','address','format','linear')]
class Options(ctypes.Structure):_fields_=[(n,U) for n in ('width','height','input_width','input_height','textures','mip_level','cull_area','alpha_ref')]
class Stats(ctypes.Structure):_fields_=[(n,Q) for n in ('triangles','fragments','depth_passed','texture_samples','pixels')]
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/pvr_pixels.dll';LIB=ctypes.CDLL(str(DLL))
def declare(lib):
 for name,args in (('texture_info',[U,U,U,ctypes.POINTER(Texture)]),('texture_decode',[ctypes.POINTER(B),U,U,U,U,ctypes.POINTER(U),U,ctypes.POINTER(Texture)]),('draw',[ctypes.POINTER(U),U,ctypes.POINTER(B),U,ctypes.POINTER(U),ctypes.POINTER(U),Q,ctypes.POINTER(Options),ctypes.POINTER(Stats)])):
  f=getattr(lib,'sa_pixel_'+name);f.argtypes=args;f.restype=ctypes.c_int
declare(LIB)
def snapshot(s):return tuple(getattr(s,n) for n,_ in s._fields_)
def draw(words,vram,options,pixels=None,depth=None,lib=LIB):
 assert len(words)%8==0
 count=options.width*options.height
 pixels=(U*count)() if pixels is None else (U*count)(*pixels);depth=(U*count)() if depth is None else (U*count)(*depth);packets=(U*len(words))(*words)
 storage=None if vram is None else (B*len(vram)).from_buffer_copy(vram);stats=Stats()
 rc=lib.sa_pixel_draw(packets,len(words)//8,storage,0 if storage is None else len(storage),pixels,depth,count,ctypes.byref(options),ctypes.byref(stats))
 return rc,bytes(pixels),bytes(depth),snapshot(stats)
