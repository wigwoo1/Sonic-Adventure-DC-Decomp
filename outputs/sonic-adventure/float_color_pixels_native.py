"""Native texture view for the existing bank-linear game-memory backend."""
import ctypes
from pathlib import Path
from pixel_jobs_native import *
import pixel_jobs_native as previous
ROOT=Path(__file__).resolve().parents[2];DLL=ROOT/'work/sonic-adventure/build/float_color_pixels.dll';LIB=ctypes.CDLL(str(DLL))
p.declare(LIB);r.declare(LIB)
for name in ('sa_pixel_job_init','sa_pixel_job_bind','sa_pixel_job_execute'):
 getattr(LIB,name).argtypes=getattr(previous.LIB,name).argtypes
 getattr(LIB,name).restype=getattr(previous.LIB,name).restype
LIB.sa_pixel_vram64_view.argtypes=[ctypes.POINTER(B),U,ctypes.POINTER(B),U];LIB.sa_pixel_vram64_view.restype=ctypes.c_int
LIB.sa_banked_pixel_job_execute.argtypes=previous.LIB.sa_pixel_job_execute.argtypes[:4]+[ctypes.POINTER(B),U]+previous.LIB.sa_pixel_job_execute.argtypes[4:]
LIB.sa_banked_pixel_job_execute.restype=ctypes.c_int
