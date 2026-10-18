import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_busy.dll')).sa_engine_transfer_busy_get
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 cases=0
 for channel,flags,bank in itertools.product([0,1,7],[0,1,2,3,0x80000000,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200)
  struct.pack_into('<I',ram,0xa00200,0x8ca00400)
  struct.pack_into('<I',ram,0xa00400+channel*32+24,flags)
  struct.pack_into('<I',ram,0xa00000,channel)
  struct.pack_into('<I',ram,0xa00100,0x12345678)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c1101f0;s.r[4]=0x8ca00000;s.r[5]=0x8ca00100
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==0
  assert struct.unpack_from('<I',native,0xa00100)[0]==(flags&1)
  cases+=1
 (out/'analysis/engine_transfer_busy_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=18,scope='Complete retail RAM/CPU/FPU comparisons for transfer channel busy getter. Independent low-bit output and zero return checks across three channels, six flag patterns and both FPU banks. No external calls. Invalid channel indices/pointers not covered.'),indent=2)+'\n')
 print(cases,'transfer busy comparisons passed')
if __name__=='__main__':main()
