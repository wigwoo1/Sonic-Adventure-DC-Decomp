import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_system_initialize_native.dll')).sa_input_system_initialize_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for mask,bank,kind,typecode in itertools.product([0,1,2,3,0x80000000,0xffffffff],[0,0x200000],[0,1],[15,33,65,122,0]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000)
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  struct.pack_into('<I',ram,0x67c27c,0x87654321)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   assert t in [0x8c647f4c];r[0]=0x1234;return 0
  def hook(mem,r,sr):r[0]=0x1234;return sr
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=typecode;s.r[5]=mask;s.r[6]=0x8ca20000 if kind else 0;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c647e16
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<I',native,0x89c54c)[0]==0x8c89c538
  assert struct.unpack_from('<I',native,0x8a1458)[0]==(mask&255).bit_count()
  assert struct.unpack_from('<I',native,0x89c540)[0]==(0 if kind else 1)
  assert r[0]==0
  if not kind or typecode==0:assert native[0x89c550:0x89cf90]==bytes(0xa40)
  assert struct.unpack_from('<I',native,0x89c53c)[0]=={15:0,33:1,65:2,122:3}.get(typecode,0)
  assert struct.unpack_from('<I',native,0x894114)[0]==10
  assert struct.unpack_from('<I',native,0x67c27c)[0]==0
  if kind and typecode in [15,33,65,122]:
   selected=0
   for channel in range(8):
    record=0x89c550+channel*0x148
    enabled=bool(mask&(1<<channel));assert native[record+2]==enabled
    if enabled:
     assert struct.unpack_from('<I',native,record+28)[0]==0x8c8a0fb0+channel*76
     assert struct.unpack_from('<I',native,record+44)[0]==0x8ca20000+selected*(typecode*512+32)
     selected+=1
  count+=1
 (out/'analysis/input_system_initialize_verification.json').write_text(json.dumps(dict(cases_passed=count,instruction_count=97,scope='Full retail RAM/CPU/FPU comparison; Native system initializer, fill and packet mode setter; alternate configuration and backend reset native; no injected calls; retry-limit setter native; simple type setter native. Independent global pointer, channel count, cleared records and mode checks.'),indent=2)+'\n')
 print(count,'packet mode comparisons passed')
if __name__=='__main__':main()


