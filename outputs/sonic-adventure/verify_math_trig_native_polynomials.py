import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_trig_octant_dispatch.dll')).sa_math_trig_octant_dispatch
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
from sh4_binary32 import multiply,multiply_add
polydll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_trig_polynomials.dll'))
poly={}
for address,name in [(0x8c110218,'sa_math_cosine_polynomial'),(0x8c110238,'sa_math_sine_polynomial')]:
    poly[address]=getattr(polydll,name);poly[address].argtypes=fn.argtypes
cases=0
for input_value,octant,bank in itertools.product([0.,0.25,-0.25],range(16),[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;f.banks[bank][4]=raw(input_value);nf=Fpu.from_buffer_copy(f)
    actual=[];expected=[]
    def apply(address,mem,fp,regs,trace):
        trace.append((address,fp.banks[bank][4],fp.banks[bank][5]))
        if address==0x8c10fbb8:
            pointer=regs[4]&0xffffff
            for i,b in enumerate(octant.to_bytes(4,'little')):mem[pointer+i]=b
            fp.banks[bank][0]=raw(0.125)
        else:
            assert address in [0x8c110218,0x8c110238]
            fp.banks[bank][0]=raw(0.75 if address==0x8c110218 else 0.5)
    def hook(address):
        def run(mem,regs,sr):apply(address,mem,f,regs,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,sr,regs,pr):
        if address in poly:return poly[address](mem,fp,sr,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        apply(address,mem,fp.contents,regs,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000
    s.pc=0x8c10fcbc;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    reference(ram,s,{a:hook(a) for a in [0x8c10fbb8]},fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    index=octant&7; chosen=0x8c110218 if index in [0,3,4,7] else 0x8c110238
    arg=raw(f32(0.125+struct.unpack('<f',struct.pack('<I',0xbf490fdb))[0])) if index&1 else raw(0.125)
    assert actual==[(0x8c10fbb8,raw(abs(input_value)),0x3f490fdb)],(input_value,octant,actual)
    table=0x8c160444 if chosen==0x8c110218 else 0x8c160460
    coefficients=struct.unpack_from('<7I',binary,table-0x8c010000)
    square=multiply(arg,arg)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if chosen==0x8c110238:answer=multiply(arg,answer)[0]
    if index in [1,2,3,4]:answer^=0x80000000
    assert nf.banks[bank][0]==answer and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/math_trig_native_polynomials_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native104-instruction dispatch10fcbc..10fd8c.96 full retail RAM/CPU/FPU/SR/PR/MACL and service traces: zero/+0.25/-0.25,16 injected indices including high-bit masking, both banks. Independent absolute input,quarter-pi forwarding,odd-index adjustment,all8 helper selections and result signs,stack/PR restoration. Native polynomial helpers110218/110238 connected with native FMAC/multiplication; only reducer10fbb8 modeled. Independent original-table Horner evaluation checks all8 result selections/signs. Finite PR/SZ/RM0 only; NaN/Infinity,actual reduction,coefficient integration/fullgame unverified.'),indent=2)+'\n')
print(cases,'trig dispatcher with native polynomials comparisons passed')
