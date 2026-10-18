import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_backend_check.dll')).sa_input_device_backend_check
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 mutations=[(None,0)]+[(off,v) for off in [8,12,20,52,56] for v in [0,1,0xffffffff,2047,2048]]+[(16,65),(24,65)]
 @Call
 def call(*args):return -9
 for size,mutation,mode,bank in itertools.product([0,1,255,511,1023,2047,2048],mutations,[0,1],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;record=0xa00000;device=0xa01000
  struct.pack_into('<I',ram,record+4,0x12345678);struct.pack_into('<2H',ram,record+16,64,64);struct.pack_into('<I',ram,record+28,0x8ca01000);struct.pack_into('<I',ram,0x89c54c,0x8ca02000);struct.pack_into('<I',ram,0xa02008,mode)
  for off,value in [(0,size),(8,size),(12,0),(16,1),(20,0),(24,1),(52,0),(56,128)]:struct.pack_into('<I',ram,device+off,value)
  if mutation[0] is not None:struct.pack_into('<I',ram,device+mutation[0],mutation[1])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c64b4e4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  fields=[struct.unpack_from('<i',ram,device+k)[0] for k in [0,8,12,20,52,56,16,24]];n,mirror,a,b,c,d,w,h=fields
  valid=1<=n<=2047 and n==mirror and 0<=a<n and 0<=b<n and 0<=c<=n and 0<=d<=n and (mode or (64>=w and 64>=h)) and n in [255,511,1023,2047]
  assert r[0]==(0 if valid else 0xffffffff) and struct.unpack_from('<I',native,record+4)[0]==([255,511,1023,2047].index(n) if valid else 0x12345678)
  count+=1
 report=dict(cases_passed=count,instruction_count=80,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_device_backend_check.c').read_bytes()).hexdigest(),scope='Full retail RAM/register/FPU comparisons plus independent signed-bound, mirrored-size, dimension and type mapping checks across valid/invalid sizes, mutated fields, modes and FPU banks. No injected services.')
 (out/'analysis/input_device_backend_check_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'backend record checks passed')
if __name__=='__main__':main()
