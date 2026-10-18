import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fn.argtypes=abi;cases=0
for count,value,offset,bank in itertools.product([0,1,28,224],[0,0x7f,0x1234ffff],[0,1],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xb00000:0xb00100]=bytes([0xa5])*256
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 @Call
 def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8cb00004+offset;s.r[5]=value;s.r[6]=count;s.r[15]=0x8cf00000;s.pc=0x8c10d31c;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert r[0]==0x8cb00004+offset
 assert native[0xb00000:0xb00100]==bytes([0xa5])*(4+offset)+bytes([value&255])*count+bytes([0xa5])*(252-offset-count)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':12,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL equality; counts0/1/28/224, three fill values including high bits, aligned/unaligned addresses and both FPU banks. Independent low-byte fill, return pointer and outside-range guards. No external services.'}
(root/'analysis/byte_fill_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'byte fill comparisons passed')
