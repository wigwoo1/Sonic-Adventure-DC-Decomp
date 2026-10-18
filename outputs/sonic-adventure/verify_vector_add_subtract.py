import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/vector_add_subtract.dll'));counts={}
 @Call
 def forbidden(*args):return -99
 values=[0,0x80000000,0x3f800000,0xbf800000,0x3f000000,0xbf000000,0x00800000,0x80800000,0x3f800001,0x4b000001]
 for name,address in [('add',0x8c638eac),('subtract',0x8c63a7e4)]:
  fn=getattr(dll,'sa_vector_'+name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for seed,offset,bank,rm in itertools.product(range(64),[0,4,8,-4,32],range(2),range(2)):
   rng=random.Random(seed)
   vals=[rng.choice(values) if seed<32 else ((rng.randrange(2)<<31)|(rng.randrange(40,210)<<23)|rng.getrandbits(23)) for _ in range(12)]
   for mem in (ram,native):struct.pack_into('<12I',mem,0xa00400,*vals)
   f=Fpu();f.fpscr=0x4007c|(bank<<21)|rm;f.fpul=0xdeadbeef
   for b in range(2):f.banks[b][:]=[rng.getrandbits(32) for _ in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=0x8ca00404;s.r[5]=s.r[4]+offset;s.r[15]=0x8cf00000;s.pc=address;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
   reference(ram,s,fpu=f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,forbidden)==0
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and fpul.value==f.fpul and ram==native,(name,seed,offset,bank,rm)
   count+=1
  counts[name]=count
 names=['sa_vector_add.c','sa_vector_subtract.c','sh4_add_sub.c']
 report=dict(cases_passed=counts,instruction_counts={'add':13,'subtract':13},binary_sha256=hashlib.sha256(binary).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/vector_add_subtract.dll').read_bytes()).hexdigest(),scope='Complete retail bodies without injected calls or arithmetic. Native integer binary32 scalar add/subtract, finite normal/signed zero, DN=1, RM0/1, both banks, cancellation and rounding, identical/partially overlapping source/destination buffers. All registers/SR/PR/FPU/FPUL/RAM compared against unhooked retail instruction execution. PR=SZ=0; NaN/Infinity and enabled exception trap paths outside current verification scope.')
 (out/'analysis/vector_add_subtract_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'vector add/subtract comparisons passed')
if __name__=='__main__':main()
