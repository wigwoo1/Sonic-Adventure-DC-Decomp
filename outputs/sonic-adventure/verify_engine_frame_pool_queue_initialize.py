import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_pool_queue_initialize.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for address,name in [(0x8c10cd38,'sa_engine_record_copy_dispatch'),(0x8c10cd76,'sa_engine_record_copy32_tail'),(0x8c10cd8e,'sa_engine_record_copy8_tail'),(0x8c65acac,'sa_engine_error_record')]:
  fn=getattr(lib,name);fn.argtypes=abi;functions[address]=fn
 cases=0
 for pool,present,count,bank,mask in itertools.product([True,False],[True,False],[0,1,64],[0,0x200000],[0,7,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca01000 if present else 0)
  offset=0x17c if pool else 0x194;size=32 if pool else 8;start=0x8c666e50 if pool else 0x8c666ef0
  ram[0xa02000:0xa02820]=bytes([0x55])*0x820
  template=bytes(ram[(0x67c354 if pool else 0x67c358):(0x67c354 if pool else 0x67c358)+size])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   return functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=start;s.r[4]=0x8ca02000;s.r[5]=count
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  fn=lib.sa_engine_frame_object_pool_initialize if pool else lib.sa_engine_frame_queue_initialize;fn.argtypes=abi
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==(0 if present else 0x0d000001)
  if present:
   assert struct.unpack_from('<6I',native,0xa01000+offset)==(0x8ca02000,count,0,0,0,0)
   assert native[0xa02000:0xa02000+count*size]==template*count
   assert native[0xa02000+count*size:0xa02820]==bytes([0x55])*(0x820-count*size)
  else:assert struct.unpack_from('<2I',native,0x67c1c8)==(start,0x0d000001)
  cases+=1
 (root/'analysis/engine_frame_pool_queue_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native object pool and pending queue initialization, copy dispatcher and 32/8-byte copy tails versus full retail CPU/RAM/FPU state. Independent six context fields, copied record templates, untouched tail and missing-context errors; count 0/1/64, FPU banks and interrupt masks. Frame context is fixture; full frame initializer not yet integrated.'),indent=2)+'\n')
 print(cases,'pool/queue initialization comparisons passed')
if __name__=='__main__':main()
