import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_word_copy.dll')).sa_engine_word_copy;fn.argtypes=abi
cases=0
for destination,size,bank in itertools.product([0x8cb01000,0xa5000080],[4,7,24,128],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;payload=bytes((i*37+11)&255 for i in range(160));ram[0xb00000:0xb000a0]=payload
 is_vram=destination>>24==0xa5
 if not is_vram:ram[0xb00ffc:0xb010a4]=bytes([0xa5])*168
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 mmio={destination+i:0xa5a5a5a5 for i in range(-4,164,4)} if is_vram else {};registers=dict(mmio);writes=[];errors=[]
 @Read
 def read(ctx,a,n):errors.append(('unexpected read',a,n));return 0
 @Write
 def write(ctx,a,v,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=v;writes.append((a,v))
 io=IO(None,read,write)
 @Call
 def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=destination;s.r[5]=0x8cb00000;s.r[6]=size;s.r[15]=0x8cf00000;s.pc=0x8c65253e;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and registers==mmio
 count=(size>>2)*4
 assert r[4]==destination+count and r[5]==0x8cb00000+count and r[6]==0
 if is_vram:
  assert writes==[(destination+i,int.from_bytes(payload[i:i+4],'little')) for i in range(0,count,4)]
  assert all(v==0xa5a5a5a5 for a,v in registers.items() if not destination<=a<destination+count)
 else:assert native[0xb00ffc:0xb010a4]==bytes([0xa5])*4+payload[:count]+bytes([0xa5])*(164-count)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':8,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/MMIO equality for RAM and modeled VRAM destinations, sizes4/7/24/128 and both FPU banks. Independent payload, rounded word count, source/destination advance and untouched guards. No external services; sizes below4 retain retail pathological loop and are not exercised; actual GPU rendering unverified.'}
(root/'analysis/engine_word_copy_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'native word copy comparisons passed')
