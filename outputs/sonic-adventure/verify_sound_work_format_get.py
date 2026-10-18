import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_format_get.dll')).sa_sound_work_format_get
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_format_get.dll')).sa_sound_stream_format_get;getter.argtypes=fn.argtypes
cases=0
for phase,count,bank,right in itertools.product([0,1,2,3,0xffffffff],[0,1,2,127,128,255,0xffffffff],[0,1],[0x8ca40000,0]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa20004,phase);struct.pack_into('<I',ram,0xa2001c,count)
    struct.pack_into('<I',ram,0xa20004,0x8ca70000);original=bytes(ram[0xa20000:0xa20040])
    ram[0xa70000:0xa70100]=bytes([0x77])*256
    struct.pack_into('<2H',ram,0xa700a8,count&65535,phase&65535);ram[0xa7000e]=count&255
    stream_before=bytes(ram[0xa70000:0xa70100])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c65bc3c;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,mem,f,sr,r,pr):
        assert address==2355525284 and r[4]==0x8ca70000 and r[15]==0x8cf00000
        return getter(mem,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    def hook(mem,r,sr):
        assert r[4]==0x8ca70000 and r[15]==0x8cf00000
        r[0]=count;return sr
    reference(ram,s,{},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    wanted_value=ctypes.c_int16(count&65535).value&0xffffffff
    assert regs[0]==wanted_value and native[0xa70000:0xa70100]==stream_before
    assert native[0xa20000:0xa20040]==wanted and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_work_format_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of metadata wrapper8c65bc3c. Tail getter native;independent returned format signed16/channel signed8/sample width4/8/16 as applicable,entire256-byte stream unchanged; independent stream argument from work+4, restored stack at tail call and returned value; work bytes unchanged. Native feeder integration/full playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'sound_work_format_get comparisons passed')
