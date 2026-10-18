import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_reserved_handle.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cleanup=lib.sa_file_reserved_handle_acquire;cleanup.argtypes=abi
@Call
def call(*args):return -9
cases=0
for owned,length,present,bank in itertools.product([0,1,2],[0,2047,2048,2049,0xffffffff],[0,1],[0,0x200000]):
 active=1;kind=7
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa00000:0xa00060]=bytes((i*13+7)&255 for i in range(96))
 struct.pack_into('<I',ram,0xa00000,0x8ca01000);struct.pack_into('<I',ram,0xa0101c,0x8ca00000 if owned==1 else 0x8ca03000 if owned==2 else 0);struct.pack_into('<I',ram,0xa0000c,length);struct.pack_into('<H',ram,0xa0004c,active);struct.pack_into('<H',ram,0xa00050,kind)
 before=bytes(ram[0xa00000:0xa00060])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000 if present else 0;s.r[5]=7;s.r[6]=0x12345678;s.r[7]=length;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600e2c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 s.pc=0x8c600d82
 reference(ram,s,fpu=f,special=special)
 assert cleanup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 expected=bytearray(before)
 if present and owned!=1:
  rounded=(length+2047)&0xffffffff;signed=rounded if rounded<0x80000000 else rounded-0x100000000
  for offset,value in [(4,7),(8,0x12345678),(0xc,length),(0x10,(signed>>11)&0xffffffff)]+[(o,0) for o in [0x14,0x18,0x1c,0x20,0x3c,0x44]]:struct.pack_into('<I',expected,offset,value)
  for offset,value in [(0x48,0),(0x4a,0),(0x4c,1),(0x50,0),(0x52,0)]:struct.pack_into('<H',expected,offset,value)
 assert native[0xa00000:0xa00060]==expected
 assert r[0]==(0x8ca00000 if present and owned!=1 else 0)
 cases+=1
(root/'analysis/file_reserved_handle_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native reserved handle acquire with full retail RAM/CPU/FPU/SR/PR/MACL equality and independent whole handle fields. Null/present, free/self/other ownership, boundary sizes and banks; no hooks.'},indent=2)+'\n')
print(cases,'file handle initialization comparisons passed')
