import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer.dll')).sa_engine_transfer_descriptor_initialize
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 cases=0
 for flag0,flag1,flag2,initial,bank in itertools.product([0,1,2],[0,1,2],[0,1,2],[0,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0xa00014,initial)
  struct.pack_into('<5I',ram,0xf00000,flag0,flag1,flag2,0x8c668f40,0x8ca01000)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10f9f4;s.r[4]=0x8ca00000;s.r[5]=0xa080123f;s.r[6]=0x8ca0203f;s.r[7]=0x87654321
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  expected=initial
  expected=(expected|1) if flag1==1 else (expected&0xfffffffd)
  expected=(expected|4) if flag2==1 else (expected&0xfffffffe)
  expected &= 7
  assert r[0]==0
  for offset,value in [(4,0x00801220),(8,0x0ca02020),(12,0x81654320),(16,flag0&1),(20,expected),(28,0x8c668f40),(32,0x8ca01000),(48,0)]:
   assert struct.unpack_from('<I',native,0xa00000+offset)[0]==value,(offset,hex(value))
  cases+=1
 (out/'analysis/engine_transfer_descriptor_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=47,scope='Full retail RAM/CPU/FPU comparisons for descriptor initialization with independent masked addresses, flags, callback/payload and cleared status checks. Tests 0/1/2 flag inputs, initial all-zero/all-one flags and both FPU banks. Preserves asymmetric retail bit clearing rather than assuming boolean setters. No external calls. Transfer submission is separately compiled but not runtime verified.'),indent=2)+'\n')
 print(cases,'transfer descriptor initialization comparisons passed')
if __name__=='__main__':main()
