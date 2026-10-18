import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    fn=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);fn.argtypes=abi;return fn
fn=load('sound_stream_source_set.dll','sa_sound_stream_source_set');division=load('signed_division.dll','sa_signed_division');cases=0
for fmt,width,channels,stride,count,bank in itertools.product([0,1,0xffff],[8,16,0xf8],[1,2],[18,36],[0,288,0xffffffff],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;ram[0xa20000:0xa20100]=bytes([0x55])*256
    struct.pack_into('<H',ram,0xa200a8,fmt);ram[0xa2000d:0xa20010]=bytes([width,channels,stride]);before=bytes(ram[0xa20000:0xa20100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c66771c;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address==0x8c10ca5c
        return division(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,{},fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    signed_width=ctypes.c_int8(width).value;bytes_per_sample=abs(signed_width)//8*(-1 if signed_width<0 else 1)
    denominator=stride if fmt==0 else bytes_per_sample*channels;numerator=ctypes.c_int32(count).value
    quotient=abs(numerator)//abs(denominator)*(-1 if (numerator<0)!=(denominator<0) else 1)
    wanted=bytearray(before);struct.pack_into('<2I',wanted,0x50,0x8ca10000,quotient&0xffffffff)
    for offset in [0x84,0xa0,0xa4]:struct.pack_into('<I',wanted,offset,0)
    assert native[0xa20000:0xa20100]==wanted and regs[15]==0x8cf00000;cases+=1
(p/'analysis/sound_stream_source_set_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='216 retail RAM/CPU/FPU/SR/PR/MACL comparisons of source setter66771c..66776e with native signed division,no modeled calls. Magic and nonzero formats,8/16/-8 bits,1/2 channels,18/36 encoded block sizes,zero/positive/negative byte counts,both FPU banks. Independent source pointer,derived unit count and reset fields84/a0/a4,entire256-byte stream preservation,stack checked. Repeated audio updates/physical playback/fullgame unverified.'),indent=2)+'\n');print(cases,'stream source setter comparisons passed')
