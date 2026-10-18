import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_helpers.dll'))
 fn=lib.sa_engine_pending_count_get
 error_fn=lib.sa_engine_error_record
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 error_fn.argtypes=fn.argtypes
 cases=0
 for count,bank,mask in itertools.product([None,0,1,0xffffffff,0x7fffffff],[0,0x200000],[0,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0 if count is None else 0x8ca00200)
  struct.pack_into('<I',ram,0xa003a4,count or 0)
  struct.pack_into('<I',ram,0xa00000,0x12345678)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c65acac:return error_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c66abfc;s.r[4]=0x8ca00000
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==(0x0d000001 if count is None else 0)
  assert struct.unpack_from('<I',native,0xa00000)[0]==(0x12345678 if count is None else count)
  if count is None:assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66abfc,0x0d000001)
  cases+=1
 (out/'analysis/engine_pending_count_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=28,scope='Full retail RAM/CPU/FPU comparisons for present and missing engine contexts with native error recording. Independent output/error globals/return checks across counts, FPU banks and interrupt masks. No injected service bodies.'),indent=2)+'\n')
 print(cases,'pending count comparisons passed')
if __name__=='__main__':main()
