import ctypes, itertools, json, struct
from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_result_error_wrapper.dll')).sa_math_result_error_wrapper
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
values=[0,0x80000000,1,0x800001,0x3f800000,0xbf800000,0x7f7fffff,0x7f800000,0xff800000,0x7fc00000,0x7f800001,0xffc00001]
for result,bank,error in itertools.product(values,[0,1],[0,0x12345678,0xffffffff]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0x7ac104:0x7ac110]=bytes([0x55])*12;struct.pack_into('<I',ram,0x7ac108,error)
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;f.banks[bank][4]=0x3f000000;nf=Fpu.from_buffer_copy(f)
    expected,actual=[],[]
    def hook(mem,regs,sr):
        expected.append(f.banks[bank][4]);f.banks[bank][0]=result;return sr
    @Call
    def call(ctx,address,mem,fp,sr,regs,pr):
        assert address==0x8c10fcbc
        actual.append(fp.contents.banks[bank][4]);fp.contents.banks[bank][0]=result;return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000
    s.pc=0x8c10f90c;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr)
    fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    reference(ram,s,{0x8c10fcbc:hook},fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    wanted=(1101 if result&0x7fffff else 1100) if result&0x7f800000==0x7f800000 else error
    assert struct.unpack_from('<I',native,0x7ac108)[0]==wanted
    assert native[0x7ac104:0x7ac108]==bytes([0x55])*4 and native[0x7ac10c:0x7ac110]==bytes([0x55])*4
    assert nf.banks[bank][0]==result and regs[15]==0x8cf00000 and actual==expected==[0x3f000000]
    cases+=1
(out/'analysis/math_result_error_wrapper_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native29-instruction result-error wrapper10f90c..10f946.72 full retail RAM/CPU/FPU/SR/PR/MACL/service-trace comparisons: signed zeros, subnormal, normal boundaries, signed infinity, quiet/signaling NaN patterns, both banks, three prior error words. Independent exponent/mantissa classification: infinity1100,NaN1101,finite preserves global7ac108; exact result bits,guards,stack and argument preservation. Lower routine10fcbc modeled,PR/SZ0 only; trigonometric numerics,coefficient integration,physical audio/fullgame unverified.'),indent=2)+'\n')
print(cases,'math result error wrapper comparisons passed')
