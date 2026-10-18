import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_callback_register.dll')).sa_engine_callback_register
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_uint32,ctypes.POINTER(IO),ctypes.c_void_p,Call]
cases=0
for callback,initial_sr,vbr,bank in itertools.product([0,0x8c652210],[0,0x70000063,0x700000f3,0xffffffff],[0x8c000000,0x8ca08000],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 mmio={a:0x12345678 for a in [0xa05f6930,0xa05f6938,0xa05f6900,0xa05f6908]};registers=dict(mmio);writes=[];errors=[]
 @Read
 def read(ctx,a,n):errors.append(('read',a,n));return 0
 @Write
 def write(ctx,a,value,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  writes.append((a,value));registers[a]=value
 @Call
 def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
 io=IO(None,read,write);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=callback;s.r[15]=0x8cf00000;s.pc=0x8c653640;s.pr=0x8c000100;s.sr=initial_sr
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,mmio=mmio,vbr=vbr)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),vbr,ctypes.byref(io),None,call)==0 and not errors,errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
 assert struct.unpack_from('<I',native,(vbr&0xffffff)+0x224)[0]==callback
 assert writes==[(0xa05f6930,0),(0xa05f6938,0),(0xa05f6900,0xffffffff),(0xa05f6908,0xffffffff),(0xa05f6930,0x2807ec),(0xa05f6938,14)]
 assert sr.value==initial_sr and r[15]==0x8cf00000
 cases+=1
report={'cases_passed':cases,'instructions_recovered':38,'entry':'0x8c653640','scope':'Full retail RAM/CPU/FPU/SR/PR/MMIO comparison with independent VBR+224 callback, ordered interrupt-register writes and restored SR checks. Two callbacks/VBR bases/FPU banks, four SR masks. Hardware MMIO modeled; no physical interrupt execution.'}
(root/'analysis/engine_callback_register_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'engine callback registrar comparisons passed')
