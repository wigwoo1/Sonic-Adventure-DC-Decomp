import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from sh4_binary32 import multiply,multiply_add
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_trig_polynomials.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
values=[0,0x80000000,0x3d800000,0xbd800000,0x3e000000,0xbe000000,0x3e800000,0xbe800000,0x3f000000,0xbf000000,0x3f490fdb,0xbf490fdb]
cases=0;tables=[]
for name,address,table in [('sa_math_cosine_polynomial',0x8c110218,0x8c160444),('sa_math_sine_polynomial',0x8c110238,0x8c160460)]:
    fn=getattr(dll,name);fn.argtypes=abi
    coefficients=struct.unpack_from('<7I',binary,table-0x8c010000)
    tables.append(dict(name=name,address=hex(table),words=[hex(v) for v in coefficients],sha256=hashlib.sha256(binary[table-0x8c010000:table-0x8c010000+28]).hexdigest()))
    for value,bank,mode in itertools.product(values,[0,1],[0,1]):
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
        f=Fpu();f.fpscr=(bank<<21)|mode;f.banks[bank][4]=value;nf=Fpu.from_buffer_copy(f)
        s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=address;s.pr=0x8c000100;s.sr=0x70000063
        regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
        @Call
        def call(*args):raise AssertionError('Unexpected external call')
        reference(ram,s,fpu=f,special=special)
        assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
        square=multiply(value,value,mode)[0];answer=coefficients[0]
        for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient,mode)[0]
        if name=='sa_math_sine_polynomial':answer=multiply(value,answer,mode)[0]
        assert nf.banks[bank][0]==answer and regs[4]==0 and regs[5]==table+28 and regs[15]==0x8cf00000
        cases+=1
(out/'analysis/math_trig_polynomials_verification.json').write_text(json.dumps(dict(cases_passed=cases,tables=tables,scope='Native cosine14/sine16 instruction polynomial helpers110218..110234/110238..110258.96 full retail RAM/CPU/FPU/SR/PR/MACL comparisons, signed zero and finite reduced arguments through +/-pi/4, both banks, RM0/1. No external calls; native multiplication and fused multiply-add. Explicit seven-coefficient Horner evaluation with exact dyadic binary32 oracle, final sine multiplication, original coefficient table hashes, unchanged RAM, register/stack/PR checks. PR/SZ0; nonfinite/denormal inputs,range reduction,full trigonometric accuracy and caller integration unverified.'),indent=2)+'\n')
print(cases,'trigonometric polynomial comparisons passed')
