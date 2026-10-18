import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_graphics_register_io.dll'));cases=0
base=struct.unpack_from('<I',binary,0x6534c0-0x10000)[0]
for address,name in [(0x8c653440,'sa_engine_graphics_initialize_leaf'),(0x8c653444,'sa_engine_graphics_register_write'),(0x8c653476,'sa_engine_graphics_register_read')]:
 fn=getattr(lib,name);fn.argtypes=abi
 for offset,value,bank in itertools.product([0,4,8,0x200,0x3fc],[0,0x12345678,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  mmio={base+offset:value};registers=dict(mmio);events=[];errors=[]
  @Read
  def read(ctx,a,n):
   if a not in registers or n!=4:errors.append((a,n));return 0
   events.append(('read',a));return registers[a]
  @Write
  def write(ctx,a,v,n):
   if a not in registers or n!=4:errors.append((a,n));return
   events.append(('write',a,v));registers[a]=v
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  io=IO(None,read,write);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=offset;s.r[5]=value;s.r[15]=0x8cf00000;s.pc=address;s.pr=0x8c000100;s.sr=0x70000063
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  reference(ram,s,fpu=f,mmio=mmio)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert events==([] if address==0x8c653440 else [('write',base+offset,value)] if address==0x8c653444 else [('read',base+offset)])
  assert r[0]==(0x22340000 if address==0x8c653440 else 1 if address==0x8c653444 else value)
  cases+=1
report={'cases_passed':cases,'instructions_recovered':10,'scope':'Full retail RAM/CPU/FPU/SR/PR/MMIO comparison of no-op leaf653440, register writer653444 and reader653476. Five offsets, three values and both FPU banks. Independent exact MMIO calls and return values checked. MMIO device modeled, no physical rendering.'}
(root/'analysis/engine_graphics_register_io_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'graphics register primitive comparisons passed')
