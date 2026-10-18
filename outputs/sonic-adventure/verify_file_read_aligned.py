import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_read_aligned.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cleanup=lib.sa_file_read_aligned;cleanup.argtypes=abi
@Call
def call(*args):return -9
cases=0
for present,active,busy,position,request,alignment,token,bank in itertools.product([0,1],[0,1],[0,1],[0,8,10],[0,4,20],[0,1],[0,0x123],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa00000:0xa00060]=bytes((i*13+7)&255 for i in range(96))
 for offset,value in [(0,0x8ca01000),(8,100),(0x10,10),(0x14,position)]:struct.pack_into('<I',ram,0xa00000+offset,value)
 struct.pack_into('<H',ram,0xa0004c,active);struct.pack_into('<H',ram,0xa0004e,3)
 struct.pack_into('<I',ram,0xa0101c,0x8cb00000 if busy else 0);struct.pack_into('<I',ram,0xa0100c,0x8ca02000)
 struct.pack_into('<I',ram,0xa02004,0x8cb10000);struct.pack_into('<I',ram,0xa02040,0x8cb20000)
 before=bytes(ram[0xa00000:0xa00060]);native_calls=[];reference_calls=[]
 def device(target,m,regs,events):
  events.append((target,tuple(regs[i] for i in range(4,8))))
  regs[0]=token if target==0x8cb10000 else 0
 def read_hook(m,regs,status):device(0x8cb10000,m,regs,reference_calls);return status
 def start_hook(m,regs,status):device(0x8cb20000,m,regs,reference_calls);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target not in [0x8cb10000,0x8cb20000]:return -9
  device(target,m,regs,native_calls);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000 if present else 0;s.r[5]=request;s.r[6]=0x8cb00000+alignment;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600e2c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 s.pc=0x8c601084
 reference(ram,s,fpu=f,special=special,hooks={0x8cb10000:read_hook,0x8cb20000:start_hook})
 assert cleanup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 expected=bytearray(before);effective=min(request,10-position) if position<10 else 0
 eligible=present and active and not busy and not alignment
 submitted=eligible and effective>0
 if eligible:
  if effective==0:struct.pack_into('<I',expected,0x18,0);struct.pack_into('<H',expected,0x50,1)
  elif token:
   struct.pack_into('<I',expected,0x3c,token);struct.pack_into('<I',expected,0x1c,effective)
   for offset,value in [(0x50,2),(0x4a,1),(0x48,1)]:struct.pack_into('<H',expected,offset,value)
   struct.pack_into('<I',expected,0x18,0)
  else:struct.pack_into('<H',expected,0x50,1)
 assert native[0xa00000:0xa00060]==expected
 assert r[0]==(0xfffffff6 if not present or not active else 0xfffffff2 if not busy and alignment else effective if submitted and token else 0)
 assert native_calls==reference_calls and len(native_calls)==(2 if submitted and token else 1 if submitted else 0)
 if submitted:assert native_calls[0][1]==(100+position,effective,0x8cb00000,3),native_calls[0]
 assert struct.unpack_from('<I',native,0xa0101c)[0]==(0x8ca00000 if submitted and token else 0x8cb00000 if busy else 0)
 cases+=1
(root/'analysis/file_read_aligned_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native aligned read submission against full retail RAM/CPU/FPU/SR/PR/MACL, explicit device read/start hooks. Null/inactive handles, busy device, destination alignment, EOF/clamping, zero/positive requests, zero/nonzero device token and banks. Independent request arguments, handle fields, ownership and return assertions; device read internals and data copying not executed.'},indent=2)+'\n')
print(cases,'aligned file read submission comparisons passed')
