import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_move.dll')).sa_byte_move
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];cases=0
for count,alignment,delta,bank in itertools.product([0,1,2,3,4,7,16,32],range(4),[-8,-2,0,2,8,64],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;ram[0xa10000:0xa10100]=bytes(range(256));before=bytes(ram[0xa10000:0xa10100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    source=0x8ca10040+alignment;dest=source+delta
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[dest,source,count,0x22340007];s.r[15]=0x8cf00000;s.pc=0x8c10d25c;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(*args):raise AssertionError('Unexpected dependency')
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(before);src=0x40+alignment;dst=src+delta;wanted[dst:dst+count]=before[src:src+count]
    assert native[0xa10000:0xa10100]==wanted and regs[0]==dest and regs[15]==0x8cf00000;cases+=1
(p/'analysis/byte_move_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='384 retail RAM/CPU/FPU/SR/PR comparisons of byte move10d25c. Counts0/1/2/3/4/7/16/32,four source alignments,six destination deltas including overlap in both directions and identical pointers,both FPU banks. Independent snapshot memmove oracle over complete256-byte buffer,returned original destination and stack. No modeled functions. Large-count16-bit halfword-loop behavior,invalid pointers and fullgame unverified.'),indent=2)+'\n');print(cases,'byte move comparisons passed')
