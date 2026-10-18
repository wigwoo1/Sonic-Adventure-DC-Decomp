import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_buffer_size.dll')).sa_engine_render_buffer_size;fn.argtypes=abi
literal=lambda a:struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
table=literal(0x8c665eec);indices=literal(0x8c665ef0);second=literal(0x8c665ef4)
cases=0
for values,argument,bank in itertools.product([(0,1,2,3,4),(4,3,2,1,0),(2,2,2,2,2)],[0,1,120,0xffffffff],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 for a,v in zip([indices,second,indices+8,indices+12,indices+16],values):struct.pack_into('<I',ram,a,v)
 weights=[0,4,0x12345678,0xffffffff,0x80000000]
 for i,w in enumerate(weights):struct.pack_into('<I',ram,table+4*i,w)
 expected=(sum(weights[v] for v in values)*argument*4)&0xffffffff
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 @Call
 def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=argument;s.r[15]=0x8cf00000;s.pc=0x8c665e00;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32(0xabcdef12);f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':macl.value}
 reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert r[0]==expected
 cases+=1
report={'cases_passed':cases,'instructions_recovered':30,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL equality; three table-index patterns, four multipliers including zero and overflow, both FPU banks. Independent weighted size formula checked; no external services.'}
(root/'analysis/engine_render_buffer_size_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'render buffer size comparisons passed')
