import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram)
 view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/effect_angles_native.dll'))
 funcs=[dll.sa_angle_approach,dll.sa_effect_primary_angles_native,dll.sa_effect_secondary_angles_native]
 for fn in funcs:fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 @Call
 def forbidden(*args):return -99
 rng=random.Random(0x55d20);counts=[0,0,0]
 edge=[0,1,0x7ff,0x800,0xfff,0x1000,0x7fff,0x8000,0x8001,0xffff,0xffffffff,0x12348000]
 for kind,fn in enumerate(funcs):
  cases=list(itertools.product(edge,edge,[0,1,0x800,0x1000,0x8000])) if kind==0 else list(itertools.product(edge,edge,range(4)))
  for target,current,variant in cases:
   fpscr=[0,0x180003,0x3f07ff,0x200000][counts[kind]%4]
   for mem in (ram,native):
    mem[0xa00000:0xa01000]=bytes(4096);mem[0xefff80:0xf00000]=bytes(128)
    struct.pack_into('<10I',mem,0xa00400,target,target,current,current,target,target,current,current,current,current)
    bounds=[(0x1000,0xfffff000),(0x7fff,0xffff8000),(0x8000,0xffff7fff),(0xffffffff,1)][variant%4]
    struct.pack_into('<12I',mem,0xa00428,*(bounds*6))
    nodes=[0x8ca00600,0x8ca00700,0x8ca00800] if variant%4!=3 else [0x8ca00600]*3
    struct.pack_into('<3I',mem,0xa00458,*nodes)
   f=Fpu();f.fpscr=fpscr;f.fpul=0xdeadbeef
   for b in range(2):
    for i in range(16):f.banks[b][i]=rng.getrandbits(32)
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)]
   s.r[4]=current if kind==0 else 0x8ca00400;s.r[5]=target;s.r[6]=variant;s.r[15]=0x8cf00000
   s.pc=[0x8c055d20,0x8c07aef8,0x8c07af8c][kind];s.pr=0x8c000100;s.sr=0x700083f3 if counts[kind]&1 else 0
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
   reference(ram,s,fpu=f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,forbidden)==0
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and fpul.value==f.fpul and ram==native,(kind,hex(target),hex(current),variant)
   counts[kind]+=1
 names=['sa_angle_approach.c','sa_effect_primary_angles.c','sa_effect_secondary_angles.c','effect_angles_native.c']
 report=dict(cases_passed=counts,instruction_counts=[23,58,95],binary_sha256=hashlib.sha256(binary).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/effect_angles_native.dll').read_bytes()).hexdigest(),scope='Three complete integer routine bodies, original code unhooked and no injected services. Wrapped angles, signed clamp boundaries including 0x8000, reversed bounds, zero/large step, node aliasing, arbitrary FPU bits. All integer registers, SR, PR, FPU, FPUL, stack and RAM compared.')
 (out/'analysis/effect_angles_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'native angle comparisons passed')
if __name__=='__main__':main()
