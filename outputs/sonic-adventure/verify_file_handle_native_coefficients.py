import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);f.argtypes=abi;return f
allocate=load('file_decode_handle_allocate.dll','sa_file_decode_handle_allocate');release=load('file_decode_handle_release.dll','sa_file_decode_handle_release');fill=load('byte_fill.dll','sa_byte_fill')
binary=b;lower={}
import math
from fractions import Fraction
from sh4_binary32 import multiply,multiply_add,divide,add_sub
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
def trig_oracle(angle):
    x=angle&0x7fffffff;index=int(struct.unpack('<f',struct.pack('<I',divide(x,0x3f490fdb)[0]))[0])
    remainder=add_sub(x,multiply(raw(float(index)),0x3f490fdb)[0],True)[0];octant=index&7
    argument=add_sub(remainder,0xbf490fdb)[0] if octant&1 else remainder
    cosine=octant in [0,3,4,7];table=0x8c160444 if cosine else 0x8c160460
    coefficients=struct.unpack_from('<7I',b,table-0x8c010000);square=multiply(argument,argument)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if not cosine:answer=multiply(argument,answer)[0]
    if octant in [1,2,3,4]:answer^=0x80000000
    return struct.unpack('<f',struct.pack('<I',answer))[0]
for address,stem,name in [(0x8c644be6,'file_audio_configuration_forward','sa_file_audio_configuration_forward'),(0x8c65adc0,'audio_filter_coefficients','sa_audio_filter_coefficients'),(0x8c10f90c,'math_result_error_wrapper','sa_math_result_error_wrapper'),(0x8c10fcbc,'math_trig_octant_dispatch','sa_math_trig_octant_dispatch'),(0x8c10fbb8,'math_range_reduce','sa_math_range_reduce'),(0x8c110218,'math_trig_polynomials','sa_math_cosine_polynomial'),(0x8c110238,'math_trig_polynomials','sa_math_sine_polynomial')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi

pool=struct.unpack_from('<I',b,0x8c644c9c-base)[0];size=52;cases=0
for free,bank in itertools.product(range(9),[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;start=pool&0xffffff
    ram[start:start+size*8]=bytes([0x55])*(size*8)
    for index in range(8):struct.pack_into('<I',ram,start+size*index,0 if index==free else 1)
    before=bytes(ram[start:start+size*8]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target=fill if address==0x8c10d31c else lower[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    s.pc=0x8c644b6e;reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert allocate(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    wanted=bytearray(before)
    if free<8:
        extension=struct.unpack_from('<h',b,0x8c644c9a-base)[0];rate=struct.unpack_from('<I',b,0x8c644ca4-base)[0];signed=extension;pi2=struct.unpack('<f',struct.pack('<I',0x40c90fda))[0]
        angle=raw(f32(f32(f32(float(signed))*pi2)/f32(float(rate))));trig=trig_oracle(angle)
        root2=f32(math.sqrt(2.));center=f32(root2-trig);denominator=f32(root2-1.)
        coefficient=f32(f32(center-f32(math.sqrt(f32(f32(center-denominator)*f32(center+denominator)))))/denominator)
        coef_a=int(f32(coefficient*8192.))&65535;coef_b=int(-f32(f32(coefficient*coefficient)*4096.))&65535
        obj=bytearray(size);struct.pack_into('<2I',obj,0,1,free);struct.pack_into('<2H',obj,48,coef_a,coef_b);wanted[free*size:(free+1)*size]=obj
    assert native[start:start+size*8]==wanted and regs[0]==(pool+free*size if free<8 else 0)
    handle=regs[0];regs[4]=handle;s.r[4]=handle;s.pc=0x8c644bfe
    reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
    assert release(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    if free<8:wanted[free*size:(free+1)*size]=bytes(size)
    assert native[start:start+size*8]==wanted and regs[15]==0x8cf00000
    cases+=1
(p/'analysis/file_handle_native_coefficients_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='18 persistent allocation/release sequences: all eight free slots and exhausted decode-handle pool,both FPU banks. Full retail RAM/CPU/FPU/SR/PR/MACL compared. Independent 52-byte entry initialization,index,active flag,predictor/history fields,entire pool preservation,returned pointer and release clearing/null no-op. Byte fill, coefficient generation65adc0, trig wrapper/dispatcher/range reducer/polynomials and numerical helpers native. Independent default header-derived binary32 coefficient calculation. No modeled calls; host concurrency and fullgame unverified.'),indent=2)+'\n');print(cases,'native coefficient file handle pool sequences passed')
