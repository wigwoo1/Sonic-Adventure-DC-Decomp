import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_open_dispatch.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cleanup=lib.sa_file_open_dispatch;cleanup.argtypes=abi
@Call
def call(*args):return -9
cases=0
for present,target,kind,length,bank in itertools.product([0,1],[0x8cb10000,0x8cb20000],[0,1],[0,0x8cb01000],[0,0x200000]):
 active=1
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa00000:0xa00060]=bytes((i*13+7)&255 for i in range(96))
 struct.pack_into('<I',ram,0xa00000,0x8ca01000);struct.pack_into('<I',ram,0xa0101c,0);struct.pack_into('<I',ram,0xa0000c,length);struct.pack_into('<H',ram,0xa0004c,active);struct.pack_into('<H',ram,0xa00050,kind)
 struct.pack_into('<I',ram,0x673420,0x8ca00000 if present else 0);struct.pack_into('<I',ram,0xa0003c,target)
 before=bytes(ram[0xa00000:0xa00060]);native_calls=[];reference_calls=[]
 def hook(m,regs,status):reference_calls.append((regs[4],regs[5]));regs[0]=length;return status
 @Call
 def call(ctx,address,m,f,status,regs,pr):
  assert address==target
  native_calls.append((regs[4],regs[5]));regs[0]=length;return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8cb00000;s.r[5]=kind;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600e2c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 s.pc=0x8c6016d6
 reference(ram,s,fpu=f,special=special,hooks={target:hook})
 assert cleanup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert native[0xa00000:0xa00060]==before
 expected_status=(kind if kind<0x8000 else kind-0x10000)&0xffffffff
 assert r[0]==(length if present else 0)
 assert native_calls==reference_calls==([(0x8cb00000,kind)] if present else [])
 cases+=1
(root/'analysis/file_open_dispatch_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native open dispatcher against full retail RAM/CPU/FPU/SR/PR/MACL; absent/present backend, two tailcall targets, modes0/1 and zero/nonzero callback return, FPU banks. Explicit backend hook; independent arguments and return, source object preservation. Backend internals unverified.'},indent=2)+'\n')
print(cases,'file open dispatcher comparisons passed')
