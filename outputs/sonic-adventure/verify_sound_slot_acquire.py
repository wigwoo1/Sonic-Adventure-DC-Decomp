import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_slot_setup.dll'))
 support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for address,name in [(0x8c65a2fc,'sa_engine_frame_debug_log'),(0x8c65acac,'sa_engine_error_record')]:
  helper=getattr(support,name);helper.argtypes=abi;functions[address]=helper
 fn=lib.sa_sound_slot_acquire;fn.argtypes=abi;cases=0
 for kind,pattern,bank in itertools.product([0,1,2,3,4,0xffffffff],['empty','full','last','fragmented'],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca03000)
  slots=[]
  for pool,base,capacity in [(1,8,8),(2,0x48,8),(3,0x88,4)]:
   for i in range(capacity):
    flag=0 if pattern=='empty' else 1 if pattern=='full' else int(i!=capacity-1) if pattern=='last' else (1 if i%3!=1 else 2)
    offset=0xa03000+base+i*8;struct.pack_into('<2I',ram,offset,pool|(i<<8),flag);slots.append((pool,offset,flag))
  struct.pack_into('<I',ram,0xa02000,0xabcdef12)
  before=bytes(ram[0xa03000:0xa030a8])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   return functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca02000;s.r[5]=kind;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65aa30
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  selected=next((offset for pool,offset,flag in slots if pool==kind and flag!=1),None)
  expected=bytearray(before)
  if kind not in (1,2,3):result=0x7fffffff;out=0xabcdef12
  elif selected is None:result=0x07000101;out=0
  else:result=0;out=0x8c000000|selected;struct.pack_into('<I',expected,selected-0xa03000+4,1)
  assert r[0]==result and struct.unpack_from('<I',native,0xa02000)[0]==out,(kind,pattern,r[0],result)
  assert native[0xa03000:0xa030a8]==expected
  if kind in (1,2,3):
   remaining=[offset for pool,offset,flag in slots if pool==kind and flag!=1 and offset!=selected]
   for next_slot in remaining+[None]:
    s.pc=0x8c65aa30;s.pr=pr.value=0x8c000100;s.r[4]=r[4]=0x8ca02000;s.r[5]=r[5]=kind
    reference(ram,s,fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
    assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    if next_slot is None:assert r[0]==0x07000101 and struct.unpack_from('<I',native,0xa02000)[0]==0
    else:
     assert r[0]==0 and struct.unpack_from('<I',native,0xa02000)[0]==(0x8c000000|next_slot)
     struct.pack_into('<I',expected,next_slot-0xa03000+4,1)
    assert native[0xa03000:0xa030a8]==expected
  cases+=1
 (root/'analysis/sound_slot_acquire_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native typed slot allocator/error handler with full retail RAM/CPU/FPU/SR/PR/MACL equality. Types1/2/3, invalid0/4/-1, empty/full/last-free/fragmented pools, flag2 availability, FPU banks; independent first-available pointer, occupation flag, preserved other pools/fields, output and error values. Sequential allocations consume every remaining free slot followed by repeated exhaustion, checking full state after each call. Fixture sound context; null context/output outside contract not tested.'},indent=2)+'\n')
 print(cases,'typed sound slot comparisons passed')
if __name__=='__main__':main()
