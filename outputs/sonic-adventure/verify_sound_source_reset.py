import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_reset.dll')).sa_sound_source_reset
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for phase,count,bank,right in itertools.product([0,1,2,3,0xffffffff],[0,1,2,127,128,255,0xffffffff],[0,1],[0x8ca40000,0]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa20004,phase);struct.pack_into('<I',ram,0xa20020,count)
    struct.pack_into('<I',ram,0xf00000,right);original=bytes(ram[0xa20000:0xa20040])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c65e454;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(*args):raise AssertionError('Unexpected service')
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    for offset,value in [(0xc,0),(0x10,count),(0x14,0),(0x18,0)]:struct.pack_into('<I',wanted,offset,value)
    assert native[0xa20000:0xa20040]==wanted and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_source_reset_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of native source reset65e454..65e462. Independent entire64-byte object: counter+c zero,capacity+10 receives ring size+20,read+14 and write+18 zero;all other bytes preserved. Seven ring-size patterns,five neighboring fixtures,both banks,two stack fixtures. No modeled calls. Constructor/lifecycle integration separately verified;fullgame unverified.'),indent=2)+'\n')
print(cases,'sound_source_reset comparisons passed')
