import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_slot_release.dll'))
 support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for address,name in [(0x8c65a2fc,'sa_engine_frame_debug_log'),(0x8c65acac,'sa_engine_error_record')]:
  helper=getattr(support,name);helper.argtypes=abi;functions[address]=helper
 validator=lib.sa_sound_slot_validate;validator.argtypes=abi;functions[0x8c669194]=validator
 fn=lib.sa_sound_slot_release;fn.argtypes=abi;cases=0
 for valid,flag,bank in itertools.product([0,1],[0,1,2,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  ram[0xa00000:0xa00020]=bytes((i*17+3)&255 for i in range(32));struct.pack_into('<I',ram,0xa00004,flag)
  before=bytes(ram[0xa00000:0xa00020])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   return functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000 if valid else 0;s.r[5]=0x8ca02000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c66b888
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  expected=bytearray(before)
  if valid and flag==1:struct.pack_into('<I',expected,4,0)
  assert native[0xa00000:0xa00020]==expected
  assert r[0]==(0x07000103 if not valid else 0 if flag==1 else 0x07000102)
  if valid and flag==1:
   s.pc=0x8c66b888;s.pr=pr.value=0x8c000100;s.r[4]=r[4]=0x8ca00000
   reference(ram,s,fpu=f,special=special)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
   assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
   assert r[0]==0x07000102 and native[0xa00000:0xa00020]==expected
  cases+=1
 (root/'analysis/sound_slot_release_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native slot release/validate/error routines, full retail RAM/CPU/FPU/SR/PR/MACL equality; valid/null pointers, occupation flags0/1/2/-1, FPU banks. Independent success flag clearing with other bytes preserved, invalid/error preservation and repeated-release rejection. Allocation reuse not yet integrated.'},indent=2)+'\n')
 print(cases,'sound slot release comparisons passed')
if __name__=='__main__':main()
