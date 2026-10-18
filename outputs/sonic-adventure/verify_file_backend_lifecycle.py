import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_backend_lifecycle.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for install,present,result,bank in itertools.product([False,True],[0,0x8c66c4b4,0x8cb04000],[0,0xfffffff2],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x673420,present);struct.pack_into('<I',ram,0xb04014,0x8cb10000)
 target=0x8c600038 if install else 0x8c600240 if present==0x8c66c4b4 else 0x8cb10000
 expected=[];actual=[]
 def observe(m,regs,events):
  events.append((tuple(regs[i] for i in range(4,8)),struct.unpack('<I',bytes(m[(regs[15]&0xffffff):(regs[15]&0xffffff)+4]))[0]))
  regs[0]=result
 def hook(m,regs,status):observe(m,regs,expected);return status
 @Call
 def call(ctx,t,m,f,status,regs,pr):
  assert t==target;observe(m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c601618 if install else 0x8c60164a
 initialargs=tuple(s.r[i] for i in range(4,8))
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 fn=lib.sa_file_backend_install if install else lib.sa_file_backend_shutdown_dispatch;fn.argtypes=abi
 reference(ram,s,fpu=f,hooks={target:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert actual==expected and len(actual)==(1 if install or present else 0)
 if actual:assert actual[0][0]==initialargs
 if install:assert actual[0][1]==0x8c66c4b4
 assert struct.unpack_from('<I',native,0x673420)[0]==(0x8c66c4b4 if install else 0)
 cases+=1
report={'cases_passed':cases,'scope':'Native backend install/shutdown dispatch compared with retail RAM/CPU/FPU/SR/PR. Null/retail/alternate previous backend, callback results and FPU banks. Independent callback arguments, stack table pointer, dispatch count and final backend global. Initialization/shutdown callbacks modeled; filesystem startup itself remains pending.'}
(root/'analysis/file_backend_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'backend lifecycle comparisons passed')
