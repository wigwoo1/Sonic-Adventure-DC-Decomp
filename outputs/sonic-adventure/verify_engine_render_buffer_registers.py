import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_registers.dll')).sa_engine_render_buffer_registers;fn.argtypes=abi
writer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_graphics_register_io.dll')).sa_engine_graphics_register_write;writer.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
literal=lambda a:struct.unpack_from('<I',binary,a-0x8c010000)[0]
word=lambda a:struct.unpack_from('<h',binary,a-0x8c010000)[0]&0xffffffff
base=literal(0x8c6534c0);assert literal(0x8c665ef8)==0x8c653444
cases=0
for hardware,pattern,index,dimensions,bank in itertools.product([1,6],[0,0xffffffff],[0,1],[(1,1),(80,60),(0,0)],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 config_literals=[0x8c665ef0,0x8c665ef4,0x8c665f0c,0x8c665f10,0x8c665f18,0x8c665f1c,0x8c665fe8]
 for a in config_literals:struct.pack_into('<I',ram,literal(a)&0xffffff,pattern)
 struct.pack_into('<I',ram,literal(0x8c665f14)&0xffffff,hardware)
 buffer_values=[0x10203040,0x55667788,0x89abcdef,0xffffffff]
 for a,v in zip([0x8c665efc,0x8c665f00,0x8c665f04,0x8c665f08],buffer_values):struct.pack_into('<I',ram,(literal(a)+index*32)&0xffffff,v)
 width,height=dimensions
 packed=(pattern&3)|((pattern&3)<<4)|((pattern&3)<<8)|((pattern&3)<<12)
 packed|=((pattern&1)<<20)|((pattern&3)<<16) if hardware==6 else (pattern&1)<<16
 expected_writes=[(base+word(a),v) for a,v in zip([0x8c665ee0,0x8c665ee2,0x8c665ee4,0x8c665ee6],buffer_values)]
 expected_writes += [(base+word(0x8c665ee8),(((height-1)<<16)+width-1)&0xffffffff),(base+word(0x8c665fe0),packed),(base+word(0x8c665fe2),literal(0x8c665fec))]
 mmio={a:0 for a,v in expected_writes};registers=dict(mmio);writes=[];errors=[]
 @Read
 def read(ctx,a,n):errors.append(('unexpected read',a,n));return 0
 @Write
 def write(ctx,a,v,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=v;writes.append((a,v))
 io=IO(None,read,write)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target!=0x8c653444:errors.append(target);return -9
  return writer(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=width;s.r[5]=height;s.r[6]=index;s.r[15]=0x8cf00000;s.pc=0x8c665e3c;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and registers==mmio
 assert writes==expected_writes,(writes,expected_writes)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':93,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/MMIO equality with native graphics register writer. Two hardware classes, zero/all-one flags, both buffer indices, three dimensions including zero, both FPU banks. Seven ordered register writes independently checked. MMIO hardware modeled; real rendering unverified.'}
(root/'analysis/engine_render_buffer_registers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'render buffer register comparisons passed')
