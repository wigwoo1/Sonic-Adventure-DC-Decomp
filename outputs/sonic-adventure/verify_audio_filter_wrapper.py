import ctypes, itertools, json, math, struct
from fractions import Fraction
from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/audio_filter_coefficients.dll')).sa_audio_filter_coefficients
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def val(word):
    if word==0:return Fraction(0)
    exponent=(word>>23)-150; significand=(word&0x7fffff)|0x800000
    return Fraction(significand)*Fraction(2)**exponent
def sqrt_service(word,fp):
    assert word&0xf0ff==0xf06d
    bank=(fp.fpscr>>21)&1; n=(word>>8)&15; x=val(fp.banks[bank][n])
    candidate=raw(math.sqrt(float(x)))
    while val(candidate)**2>x:candidate-=1
    while val(candidate+1)**2<=x:candidate+=1
    inexact=val(candidate)**2!=x
    if inexact:
        midpoint=(val(candidate)+val(candidate+1))/2
        if x>midpoint**2 or (x==midpoint**2 and candidate&1):candidate+=1
    fp.banks[bank][n]=candidate
    fp.fpscr=(fp.fpscr&~0x3f000)|(0x1004 if inexact else 0)
wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_result_error_wrapper.dll')).sa_math_result_error_wrapper
wrapper.argtypes=fn.argtypes
cases=0
for extension,rate,trig,bank in itertools.product([0,500,0xfffffe0c],[22050,32000,44100],[0.,0.5,1.],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20010]=bytes([0x55])*16
    struct.pack_into('<I',ram,0x7ac108,0x12345678)
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;f.banks[bank][12]=raw(0.25);nf=Fpu.from_buffer_copy(f)
    expected,actual=[],[]
    def hook(mem,regs,sr):
        expected.append(f.banks[bank][4]);f.banks[bank][0]=raw(trig);return sr
    @Call
    def call(ctx,address,mem,fp,sr,regs,pr):
        if address==0x8c10f90c:
            return wrapper(mem,fp,sr,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        assert address==0x8c10fcbc
        actual.append(fp.contents.banks[bank][4]);fp.contents.banks[bank][0]=raw(trig);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)]
    s.r[4:8]=[extension,rate,0x8ca20004,0x8ca20008];s.r[15]=0x8cf00000
    s.pc=0x8c65adc0;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr)
    fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    reference(ram,s,{0x8c10fcbc:hook},fpu=f,special=special,vector_instruction=sqrt_service)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert code==0,code
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    signed=ctypes.c_int32(extension).value
    pi2=struct.unpack('<f',struct.pack('<I',0x40c90fda))[0]
    assert actual==expected==[raw(f32(f32(f32(float(signed))*pi2)/f32(float(rate))))]
    root2=f32(math.sqrt(2.)); center=f32(root2-trig); denominator=f32(root2-1.)
    coefficient=f32(f32(center-f32(math.sqrt(f32(f32(center-denominator)*f32(center+denominator)))))/denominator)
    a=int(f32(coefficient*8192.))&65535;b=int(-f32(f32(coefficient*coefficient)*4096.))&65535
    target=bytearray([0x55])*16;struct.pack_into('<H',target,4,a);struct.pack_into('<H',target,8,b)
    assert native[0xa20000:0xa20010]==target
    assert struct.unpack_from('<I',native,0x7ac108)[0]==0x12345678
    assert regs[15]==0x8cf00000 and nf.banks[bank][12]==raw(0.25)
    cases+=1
(out/'analysis/audio_filter_wrapper_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native66-instruction coefficient routine65adc0..65ae44.54 full retail RAM/CPU/FPU/SR/PR/MACL comparisons, signed extension0/+500/-500, rates22050/32000/44100, injected trig0/.5/1, both banks. Native float conversion/add/sub/multiply/divide/sqrt/truncate helpers. Independent binary32 coefficient formula/output guards/angle forwarding and exact rational FSQRT oracle. Native result-error wrapper10f90c connected; only lower trigonometric routine10fcbc modeled; prior error field preserved for finite results; PR/SZ/RM0 only, general exceptional inputs/full initializer integration/physical audio/fullgame unverified.'),indent=2)+'\n')
print(cases,'audio filter with native wrapper comparisons passed')
