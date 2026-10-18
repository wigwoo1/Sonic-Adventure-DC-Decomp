import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_mono_request.dll')).sa_sound_stream_mono_request
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=fn.argtypes
setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_mono_request.dll')).sa_file_decode_mono_request;setup.argtypes=fn.argtypes
activate=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_activate.dll')).sa_file_decode_request_activate;activate.argtypes=fn.argtypes
cases=0
for phase,count,bank,right in itertools.product([0,1,2,3,0xffffffff],[0,1,2,15,0xffffffff],[0,1],[0,7]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa2000c,phase)
    struct.pack_into('<I',ram,0xf00000,right);original=bytes(ram[0xa20000:0xa20040])
    ram[0xa60000:0xa60100]=bytes([0x66])*256;struct.pack_into('<2I',ram,0xa60004,count,0x8ca20000);struct.pack_into('<I',ram,0xa60050,0x8ca10000);struct.pack_into('<I',ram,0xa60064,0x8ca30000);struct.pack_into('<I',ram,0xa60070,right);stream_before=bytes(ram[0xa60000:0xa60100])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca60000,count,0x22340006,0x22340007];s.r[15]=0x8cf00000;s.pc=0x8c6677e2;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target={0x8c644c1a:setup,0x8c644c56:activate}[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    if phase==0:
        struct.pack_into('<2I',wanted,0xc,1,0);struct.pack_into('<5I',wanted,0x14,1,0x8ca10000,count,0x8ca30000+right*2,0)
    assert native[0xa20000:0xa20040]==wanted and regs[15]==0x8cf00000
    stream_expected=bytearray(stream_before)
    assert native[0xa60000:0xa60100]==stream_expected
    cases+=1
(out/'analysis/sound_stream_mono_request_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR native stream mono request6677e2..66780e comparisons connected to native setup644c1a and activation644c56; no modeled calls. Five decoder phases, five unsigned counts including0xffffffff, output offsets0/7 and both FPU banks. Independent handle oracle verifies idle request fields, output base+2*sample offset, phase1/result0, busy handle preservation; all256 stream bytes preserved. No audio decoding in this test; full stream scheduling/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stream mono request comparisons passed')
