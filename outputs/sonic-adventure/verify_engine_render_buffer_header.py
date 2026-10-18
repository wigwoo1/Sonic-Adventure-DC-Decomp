import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_header.dll')).sa_engine_render_buffer_header;fn.argtypes=abi
cases=0
for destination,size,bank in itertools.product([0x80,0x180],[4,24,128],[0,0x100000,0x200000,0x300000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;payload=bytes((i*37+11)&255 for i in range(160));ram[0xb00000:0xb000a0]=payload
 is_vram=True;alias=struct.unpack_from("<I",binary,0x665ff0-0x10000)[0];address=alias|destination
 if not is_vram:ram[0xb00ffc:0xb010a4]=bytes([0xa5])*168
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 mmio={address+i:0xa5a5a5a5 for i in range(-4,164,4)} if is_vram else {};registers=dict(mmio);writes=[];errors=[]
 @Read
 def read(ctx,a,n):errors.append(('unexpected read',a,n));return 0
 @Write
 def write(ctx,a,v,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=v;writes.append((a,v))
 io=IO(None,read,write)
 @Call
 def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=destination;s.r[5]=(destination-size)&0xffffffff;s.r[6]=size;s.r[15]=0x8cf00000;s.pc=0x8c665f42;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank
 for b in range(2):
  for i in range(16):f.banks[b][i]=0x12340000+b*0x100+i
 nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0 and not errors,errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and registers==mmio
 assert r[0]==destination
 literal=lambda a:struct.unpack_from('<I',binary,a-0x8c010000)[0]
 assert registers[address]==((size//4+2)|literal(0x8c665ff4))
 assert registers[address+4]==literal(0x8c665ff8)
 assert registers[address+8]==literal(0x8c665ffc)
 assert registers[address+12]==literal(0x8c666000)
 assert all(v==0xa5a5a5a5 for a,v in registers.items() if not address<=a<address+68)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':51,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/MMIO equality, two destinations, three header distances, both FPU banks and FPSCR.SZ modes. Independent header count/constants, return pointer and untouched guards. Hardware MMIO modeled; actual rendering unverified.'}
(root/'analysis/engine_render_buffer_header_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'buffer header comparisons passed')


