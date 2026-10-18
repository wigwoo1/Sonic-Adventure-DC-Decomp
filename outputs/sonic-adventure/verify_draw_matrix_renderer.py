"""Bind established renderer executor to the separate complete-matrix build."""
import ctypes,json
from pathlib import Path
import verify_main_dispatch_mesh_families as previous
PROJECT=Path(__file__).resolve().parent
DLL=previous.base.ROOT/'work/sonic-adventure/build/draw_matrix_renderer.dll'
LIB=ctypes.CDLL(str(DLL));SPECS=previous.SPECS
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for key,fn in FUNCTIONS.items():fn.argtypes=previous.FUNCTIONS[key].argtypes;fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 method=getattr(LIB,name);method.argtypes=getattr(previous.LIB,name).argtypes;method.restype=getattr(previous.LIB,name).restype
def setup():
 connection=json.loads((PROJECT/'analysis/draw_matrix_renderer_connection.json').read_text())
 for p,h in connection['sha256'].items():assert previous.digest(Path(p))==h,p
 baseline,allowed,execute=previous.setup()
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS)
 return baseline,allowed,execute
