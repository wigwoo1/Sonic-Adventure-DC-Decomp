import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_handle_metadata.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
size=lib.sa_file_size_get;size.argtypes=abi;close=lib.sa_file_close;close.argtypes=abi
cases=0
for present,active,kind,length,bank in itertools.product([0,1],[0,1,0xffff],[0,1,2,3,4,5,0xffff],[0,0x12345678],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa00000:0xa00060]=bytes((i*13+7)&255 for i in range(96))
 struct.pack_into('<I',ram,0xa0000c,length);struct.pack_into('<H',ram,0xa0004c,active);struct.pack_into('<H',ram,0xa00050,kind)
 before=bytes(ram[0xa00000:0xa00060]);native_cleanup=[];reference_cleanup=[]
 def hook(m,regs,status):reference_cleanup.append(regs[4]);regs[0]=0x13579;return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target!=0x8c6013d2:return -9
  native_cleanup.append(regs[4]);regs[0]=0x13579;return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000 if present else 0;s.r[5]=0x8cb00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600e2c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 for pc,fn in [(0x8c600e2c,size),(0x8c600df4,close),(0x8c600df4,close)]:
  s.pc=pc;s.pr=pr.value=0x8c000100;s.r[4]=r[4]=0x8ca00000 if present else 0;s.r[5]=r[5]=0x8cb00000
  reference(ram,s,fpu=f,special=special,hooks={0x8c6013d2:hook})
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  if pc==0x8c600e2c:
   assert r[0]==(1 if present and active else 0)
   assert struct.unpack_from('<I',native,0xb00000)[0]==(length if present and active else 0)
 expected=bytearray(before)
 if present and active:struct.pack_into('<H',expected,0x4c,0)
 assert native[0xa00000:0xa00060]==expected
 assert native_cleanup==reference_cleanup==([0x8ca00000] if present and active and kind in [2,3,4] else [])
 cases+=1
(root/'analysis/file_handle_metadata_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native size and close compared against retail full RAM/CPU/FPU/SR/PR/MACL. Null handles, active flag0/1/ffff, types0..5/ffff, lengths, FPU banks, repeated close. Independent size, active flag clearing and other-byte preservation. Type-specific cleanup recorded by explicit hook; cleanup internals not verified.'},indent=2)+'\n')
print(cases,'file metadata lifecycle comparisons passed')
