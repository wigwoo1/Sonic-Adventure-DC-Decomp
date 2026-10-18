import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_setup_copy.dll')).sa_input_setup_copy
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for length,bank,offset in itertools.product([0,1,3,4,9],[0,0x200000],[-8,0,1,8]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000)
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  ram[0xa10060:0xa100c0]=bytes(range(96));shadow=bytearray(ram[0xa10060:0xa100c0])
  for i in range(length):shadow[32+offset+i]=shadow[32+i]
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):raise AssertionError(hex(t))
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[0]=length;s.r[1]=0x8ca10080+offset;s.r[2]=0x8ca10080;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10fb5c
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert native[0xa10060:0xa100c0]==shadow
  count+=1
 (out/'analysis/input_setup_copy_verification.json').write_text(json.dumps(dict(cases_passed=count,instruction_count=28,scope='Full retail RAM/CPU/FPU comparison; Byte copy full retail state plus independent forward-copy content and boundary check, including overlap and lengths around unrolled-loop boundaries. No injected calls.'),indent=2)+'\n')
 print(count,'packet mode comparisons passed')
if __name__=='__main__':main()

