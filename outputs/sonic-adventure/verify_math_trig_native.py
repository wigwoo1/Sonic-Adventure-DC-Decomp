import ctypes,itertools,json,math,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from sh4_binary32 import multiply,multiply_add,divide,add_sub
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
functions={}
for address,dll,name in [(0x8c10fcbc,'math_trig_octant_dispatch','sa_math_trig_octant_dispatch'),(0x8c10fbb8,'math_range_reduce','sa_math_range_reduce'),(0x8c110218,'math_trig_polynomials','sa_math_cosine_polynomial'),(0x8c110238,'math_trig_polynomials','sa_math_sine_polynomial')]:
    functions[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dll}.dll')),name);functions[address].argtypes=abi
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
values=[0.,-0.,.125,-.125,.5,-.5,1.,-1.,2.,-2.,3.,-3.,4.,5.,6.,7.,8.,10.,100.,1000.]
cases=0;max_error=0.
for value,bank in itertools.product(values,[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;f.banks[bank][4]=raw(value);nf=Fpu.from_buffer_copy(f)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c10fcbc;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};calls=[]
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address in functions;calls.append(address)
        return functions[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,fpu=f,special=special)
    assert functions[0x8c10fcbc](view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    x=raw(abs(value));quarter=0x3f490fdb;index=int(number(divide(x,quarter)[0]));remainder=add_sub(x,multiply(raw(float(index)),quarter)[0],True)[0]
    octant=index&7
    argument=add_sub(remainder,0xbf490fdb)[0] if octant&1 else remainder
    cosine=octant in [0,3,4,7];table=0x8c160444 if cosine else 0x8c160460
    coefficients=struct.unpack_from('<7I',binary,table-0x8c010000);square=multiply(argument,argument)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if not cosine:answer=multiply(argument,answer)[0]
    if octant in [1,2,3,4]:answer^=0x80000000
    assert nf.banks[bank][0]==answer and calls==[0x8c10fbb8,0x8c110218 if cosine else 0x8c110238] and regs[15]==0x8cf00000
    max_error=max(max_error,abs(number(answer)-math.cos(value)));cases+=1
(out/'analysis/math_trig_native_verification.json').write_text(json.dumps(dict(cases_passed=cases,observed_max_absolute_error_vs_host_cos=max_error,scope='40 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons with native range reduction,octant dispatch,cosine/sine polynomials and numerical helpers; no modeled calls. Signed zero and finite angles through1000,both banks. Explicit binary32 quotient/remainder and original-table Horner output checks,helper selection,nested stack restoration. Host cosine error recorded as diagnostic,not bit-exact retail oracle. PR/SZ/RM0; nonfinite/denormal,large-angle modes/traps,audio coefficient integration/fullgame unverified.'),indent=2)+'\n')
print(cases,'fully native trig comparisons passed; max host cosine error',max_error)
