import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_decode_acknowledge.dll')).sa_sound_stream_decode_acknowledge
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=fn.argtypes
reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_completion_acknowledge.dll')).sa_file_decode_completion_acknowledge;reset.argtypes=fn.argtypes
cases=0
for phase,count,bank,right in itertools.product([0,1,2,3,0xffffffff],[0,1,2,3,0xffffffff],[0,1],[0x8ca40000,0]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa2000c,phase)
    struct.pack_into('<I',ram,0xf00000,right);original=bytes(ram[0xa20000:0xa20040])
    ram[0xa60000:0xa60100]=bytes([0x66])*256;struct.pack_into('<2I',ram,0xa60004,count,0x8ca20000);stream_before=bytes(ram[0xa60000:0xa60100])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca60000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c667796;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address==0x8c644c7e:return reset(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        assert address==0x8c10d31c
        return fill(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    if count==3 and phase==3:struct.pack_into('<I',wanted,0xc,0)
    assert native[0xa20000:0xa20040]==wanted and regs[15]==0x8cf00000
    stream_expected=bytearray(stream_before)
    if count==3:
        struct.pack_into('<I',stream_expected,4,0);struct.pack_into('<I',stream_expected,0x9c,0)
    assert native[0xa60000:0xa60100]==stream_expected
    cases+=1
(out/'analysis/sound_stream_decode_acknowledge_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR comparisons of16-instruction stream acknowledgment667796..6677b6 connected to native handle acknowledgment644c7e; no modeled calls. Five stream phases crossed with five decoder phases and both FPU banks. Independent256-byte stream/64-byte handle oracle: stream phase3 clears +4 and +9c and acknowledges handle; handle phase3 alone becomes0, other phases and all history/result/request bytes preserved. Full stream scheduling/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stream decode acknowledgment comparisons passed')
