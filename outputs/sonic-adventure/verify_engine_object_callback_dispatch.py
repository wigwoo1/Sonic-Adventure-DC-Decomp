import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_object_callback_dispatch.dll')).sa_engine_object_callback_dispatch
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 cases=0
 for callback,bank,mask in itertools.product([0,0xffffffff,0x8c64241a],[0,0x200000],[0,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<3I',ram,0xa00010,callback,0x8ca00100,0)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c64241a:return 0
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c65a518;s.r[4]=0x8ca00000
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==0
  assert struct.unpack_from('<I',native,0xa00018)[0]==(1 if callback==0xffffffff else 0)
  cases+=1
 (out/'analysis/engine_object_callback_dispatch_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=33,scope='Complete RAM/CPU/FPU comparison for inactive, sentinel and retail RTS callback paths (native dispatcher models its no-op body; reference executes retail instructions) across both FPU banks and interrupt masks. Independent return and sentinel flag checks. Error-report path and arbitrary callback bodies remain unverified.'),indent=2)+'\n')
 print(cases,'object callback dispatch comparisons passed')
if __name__=='__main__':main()

