import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_request_layout_initialize.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for address,name in [(0x8c10cd44,'sa_engine_record_copy_loaded_dispatch'),(0x8c10cd7a,'sa_engine_record_copy28_tail'),(0x8c10d31c,'sa_memory_fill'),(0x8c65acac,'sa_engine_error_record')]:
  fn=getattr(lib,name);fn.argtypes=abi;functions[address]=fn
 cases=0
 for pool,present,count,bank,mask in itertools.product([True],[True,False],[0,1,64],[0,0x200000],[0,7,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca01000 if present else 0)
  offset=0x164;size=28;start=0x8c6669c4
  ram[0xa02000:0xa02820]=bytes([0x55])*0x820
  template=bytes(ram[0x67c334:0x67c334+size])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   return functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=start;s.r[4]=0x8ca02000;s.r[5]=count
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={"macl":0};reference(ram,s,fpu=f,special=special)
  fn=lib.sa_engine_frame_request_pool_initialize;fn.argtypes=abi
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special["macl"]
  assert r[0]==(0 if present else 0x0d000001)
  if present:
   assert struct.unpack_from('<5I',native,0xa01000+offset)==(0x8ca02000,count,0,0,0)
   assert native[0xa02000:0xa02000+count*size]==template*count
   assert native[0xa02000+count*size:0xa02820]==bytes([0x55])*(0x820-count*size)
  else:assert struct.unpack_from('<2I',native,0x67c1c8)==(start,0x0d000001)
  cases+=1
 (root/'analysis/engine_frame_request_pool_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native request pool initialization, zero fill, loaded copy dispatcher and 28-byte copy tail versus full retail CPU/RAM/FPU state. Independent request context fields, copied record templates, untouched tail and missing-context errors; count 0/1/64, FPU banks and interrupt masks. Frame context is fixture; full frame initializer not yet integrated.'),indent=2)+'\n')
 print(cases,'request pool initialization comparisons passed')
if __name__=='__main__':main()
