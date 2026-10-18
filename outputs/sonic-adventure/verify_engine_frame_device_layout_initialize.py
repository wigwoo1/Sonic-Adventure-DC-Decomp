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
 for bank,mask in itertools.product([0,0x200000],[0,7,15]):
  present=True;count=0;pool=True
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca01000 if present else 0)
  offset=0;size=8;start=0x8c667240
  ram[0xa02000:0xa02820]=bytes([0x55])*0x820
  template=bytes(ram[0x67c334:0x67c334+size])
  ram[0xa01000:0xa010c0]=bytes([0x55])*0xc0
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   return functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=start;s.r[4]=0x8ca02000;s.r[5]=count
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={"macl":0};reference(ram,s,fpu=f,special=special)
  fn=lib.sa_engine_frame_device_layout_initialize;fn.argtypes=abi
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special["macl"]
  assert r[0]==(0 if present else 0x0d000001)
  expected=bytearray([0x55]*0xc0)
  for entry in range(21):
   expected[entry*8+1]=0;struct.pack_into('<I',expected,entry*8+4,0)
  expected[0]=9
  for base,number,kind in [(8,8,1),(0x48,8,2),(0x88,4,3)]:
   for index in range(number):expected[base+index*8]=kind;expected[base+index*8+1]=index
  assert native[0xa01000:0xa010c0]==expected
  cases+=1
 (root/'analysis/engine_frame_device_layout_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Full retail CPU/RAM/FPU comparisons of device layout initializer across FPU banks and interrupt masks. Independent root type 9, eight type-1/eight type-2/four type-3 indexed slots, cleared word fields and preserved untouched bytes. Caller must supply valid frame context.'),indent=2)+'\n')
 print(cases,'device layout initialization comparisons passed')
if __name__=='__main__':main()
