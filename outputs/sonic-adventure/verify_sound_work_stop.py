import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_stop.dll')).sa_sound_work_stop
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=fn.argtypes
reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_handle_reset.dll')).sa_file_decode_handle_reset;reset.argtypes=fn.argtypes
stream_reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_decode_reset.dll')).sa_sound_stream_decode_reset;stream_reset.argtypes=fn.argtypes
cases=0
for phase,count,bank,right in itertools.product([0,1,2,3,0xffffffff],[0,1,2,15,0xffffffff],[0,1],[0x8ca40000,0]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa2000c,phase)
    struct.pack_into('<I',ram,0xf00000,right);original=bytes(ram[0xa20000:0xa20040])
    ram[0xa60000:0xa60100]=bytes([0x66])*256;struct.pack_into('<2I',ram,0xa60004,count,0x8ca20000);stream_before=bytes(ram[0xa60000:0xa60100])
    ram[0xa80000:0xa80100]=bytes([0x77])*256;ram[0xa80001]=count&255;struct.pack_into('<I',ram,0xa80004,0x8ca60000);work_before=bytes(ram[0xa80000:0xa80100])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca80000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c65b78e;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address==0x8c66777c:return stream_reset(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        if address==0x8c644c68:return reset(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        assert address==0x8c10d31c
        return fill(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    struct.pack_into('<I',wanted,0xc,0);wanted[0x28:0x30]=bytes(8)
    assert native[0xa20000:0xa20040]==wanted and regs[15]==0x8cf00000
    stream_expected=bytearray(stream_before);struct.pack_into('<I',stream_expected,4,0)
    assert native[0xa60000:0xa60100]==stream_expected
    work_expected=bytearray(work_before);work_expected[1]=0
    assert native[0xa80000:0xa80100]==work_expected
    cases+=1
(out/'analysis/sound_work_stop_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR comparisons of work stop65b78e..65b7a8 with native stream reset66777c connected to native handle reset644c68 and byte fill10d31c. Idle and four busy states, varied count/register/stack inputs and both FPU banks. Independent full64-byte handle oracle: every input phase becomes0, both predictor histories cleared, all other handle bytes preserved; stream phase+4 independently cleared and all other256 stream bytes preserved. Independent entire256-byte work object:state byte1 zero,all other bytes preserved. No modeled calls; original fill body executed by reference. Connected scheduling and fullgame unverified.'),indent=2)+'\n')
print(cases,'stream decode reset comparisons passed')
