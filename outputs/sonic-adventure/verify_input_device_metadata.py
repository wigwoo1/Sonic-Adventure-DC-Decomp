import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_metadata.dll')).sa_input_device_metadata
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for present,a,b,c,d,bank in itertools.product([0,1],[0,255],[0,255],[0,0x8f,0xf1,255],[0,128],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  ram[0xa20000:0xa20004]=bytes([a,b,c,d])
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   assert t==0x8c646b42 and r[4]==0x8ca30000 and r[5]==2
   r[0]=0x8ca20000 if present else 0;return 0
  def locate(mem,r,sr):
   assert r[4]==0x8ca30000 and r[5]==2
   r[0]=0x8ca20000 if present else 0;return sr
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[5]=0x8ca30000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c646bbc
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{0x8c646b42:locate},fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==(0 if present else 0xffffffff)
  if present:assert struct.unpack_from('<5H',native,0xa10000)==(a+1,(b+1)*32,c>>4,c&15,d>>7)
  count+=1
 (out/'analysis/input_device_metadata_verification.json').write_text(json.dumps(dict(cases_passed=count,instruction_count=50,scope='Full retail RAM/CPU/FPU comparison; metadata locator injected. Independent five halfword output and null-result checks across boundary bytes and banks.'),indent=2)+'\n')
 print(count,'device metadata comparisons passed')
if __name__=='__main__':main()

