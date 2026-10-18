import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_backend_initialize.dll')).sa_input_device_backend_initialize
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];cases=0
 for count,bank,align in itertools.product([0,1,2],[0,0x200000],[0,1,31]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000)
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  struct.pack_into('<2I',ram,0xa20000,0,3);struct.pack_into('<2I',ram,0xa30000,0x8ca50000+align,0x8ca60000+align)
  struct.pack_into('<I',ram,0x8a83ac,0x8ca70000)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   assert t in [0x8c65f558,0x8c6603b0,0x8c660794];r[0]=0;return 0
  def hook(mem,r,sr):r[0]=0;return sr
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca20000;s.r[5]=0x8ca30000;s.r[6]=0x8ca40000+align;s.r[7]=count;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65eccc
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook for t in [0x8c65f558,0x8c6603b0,0x8c660794]},fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<I',native,0x8a83b4)[0]==0x8c8a72b8
  cases+=1
 (out/'analysis/input_device_backend_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,instruction_count=245,scope='Full retail RAM/CPU/FPU comparison; Backend initializer full retail RAM/CPU/FPU comparison; three lower targets injected. Buffer alignment, zero/one/two channel counts and banks tested. Global state pointer independently checked.'),indent=2)+'\n')
 print(cases,'packet mode comparisons passed')
if __name__=='__main__':main()


