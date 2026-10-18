import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();stem='sound_linear_source_pool_counter_decrement'
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);fn.argtypes=abi;cases=0
for count,bank,sr_initial,stack in itertools.product([0,1,2,0x7fffffff,0x80000000,0xfffffffe,0xffffffff],[0,1],[0,0x70000063,0x700000f2],[0,0x8ca40000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x67c22c,count);struct.pack_into('<I',ram,0xf00000,stack)
 ram[0x8a6138:0x8a65b8]=bytes([0x55])*1152
 original=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65e0dc;s.pr=0x8c000100;s.sr=sr_initial
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(*args):raise AssertionError('Unexpected subordinate call')
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 expected=(count-1)&0xffffffff;wanted=bytearray(original);struct.pack_into('<I',wanted,0x67c22c,expected)
 assert native==wanted and r[2]==r[3]==expected and r[4]==0x8c67c22c
 assert sr.value==(sr_initial&~1)|int(ctypes.c_int32(expected).value>0) and r[15]==0x8cf00000
 cases+=1
(p/f'analysis/{stem}_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='84 full retail RAM/CPU/FPU/SR/PR comparisons of source-pool counter decrement65e0dc..65e0ea. Seven counters including underflow/signed boundaries,both banks,three SR fixtures,two stack fixtures. Independent entire16MiB RAM: only global67c22c decremented modulo32bits,source pool unchanged;R2/R3 result,R4 global address,T signed-positive result in RTS delay slot,other SR bits preserved. Export opcodes checked against retail. No modeled calls. Caller counter lifecycle/fullgame remain unverified.'),indent=2)+'\n')
print(cases,'source pool counter decrement comparisons passed')
