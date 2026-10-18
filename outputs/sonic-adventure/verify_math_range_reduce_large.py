import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_range_reduce.dll')).sa_math_range_reduce
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def f32(x):return number(raw(x))
inputs=[v|sign for v in [0x4effffff,0x4f000000,0x4f000001,0x4f7fffff,0x4f800000,0x4f800001,0x50000000] for sign in [0,0x80000000]]
divisors=[0x3f800000]
cases=0
for value,divisor,bank in itertools.product(inputs,divisors,[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa2000c]=bytes([0x55])*12
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    f=Fpu();f.fpscr=bank<<21;f.banks[bank][4:6]=[value,divisor];nf=Fpu.from_buffer_copy(f)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca20004;s.r[15]=0x8cf00000;s.pc=0x8c10fbb8;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0}
    @Call
    def call(*args):raise AssertionError('Unexpected call')
    reference(ram,s,fpu=f,special=special)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
    assert code==0,(hex(value),hex(divisor),code)
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    if value&0x7f800000==0x7f800000 or (divisor&0x7f800000==0x7f800000 and divisor&0x7fffff) or divisor&0x7fffffff==0:
        index=0;answer=0x7f800001
    elif divisor&0x7f800000==0x7f800000:
        index=0;answer=value
    else:
        x=abs(number(value));y=abs(number(divisor));q=f32(x/y)
        if q<4294967296.:q=f32(float(max(-2147483648,min(2147483647,int(q)))))
        converted=f32(q-4294967296.) if q>2147483648. else q
        signed_index=max(-2147483648,min(2147483647,int(converted)))
        index=signed_index&0xffffffff
        reconstructed=f32(float(signed_index))
        if signed_index<0:reconstructed=f32(reconstructed+4294967296.)
        answer=raw(f32(x-f32(reconstructed*y)))|(value&0x80000000)
    assert struct.unpack_from('<I',native,0xa20004)[0]==index and nf.banks[bank][0]==answer,(hex(value),hex(divisor),index,hex(answer))
    assert native[0xa20000:0xa20004]==bytes([0x55])*4 and native[0xa20008:0xa2000c]==bytes([0x55])*4 and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/math_range_reduce_large_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native range reducer large-quotient branch.28 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons,no modeled calls. Signed inputs around2^31/2^32 and2^33,divisor1,both banks. Independent retail threshold (>2^31),binary32 subtraction of2^32,saturating signed truncation,stored uint32 index,unsigned reconstruction and signed remainder. Preserves retail saturation quirks at exactly2^31 and beyond uint32 range. PR/SZ/RM0; denormals,other modes/traps,caller integration/fullgame unverified.'),indent=2)+'\n')
print(cases,'large quotient range reduction comparisons passed')
