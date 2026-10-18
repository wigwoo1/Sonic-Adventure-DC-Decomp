import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_update.dll')).sa_sound_stream_work_update
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for phase,count,bank,right in itertools.product([0,1,2,3,0xffffffff],[0,1,2,127,128,255,0xffffffff],[0,1],[0x8ca40000,0]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa20004,phase);ram[0xa20001]=count&255
    struct.pack_into('<I',ram,0xf00000,right);original=bytes(ram[0xa20000:0xa20040])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c65bb88;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    actual=[];expected=[]
    def service(address,r,trace):
        assert address==(0x8c65bb4e if count&255==2 else 0x8c65b7a8)
        assert r[4]==0x8ca20000 and r[15]==0x8cf00000
        trace.append(address);r[0]=0x12345678
    @Call
    def call(ctx,address,mem,f,sr,r,pr):service(address,r,actual);return 0
    def hook(address):
        def run(mem,r,sr):service(address,r,expected);return sr
        return run
    reference(ram,s,{a:hook(a) for a in [0x8c65bb4e,0x8c65b7a8]},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    assert actual==expected==([0x8c65b7a8] if count&255==1 else [0x8c65bb4e] if count&255==2 else [])
    assert regs[0]==(0x12345678 if count&255 in [1,2] else ctypes.c_int8(count&255).value&0xffffffff)
    assert native[0xa20000:0xa20040]==wanted and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_stream_work_update_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of public work update65bb88..65bb9c. Seven state bytes0/1/2/127/128/255,five neighboring phase fixtures,banks,varied stack. Independent signed-byte load, state1 initialization/state2 tick tail calls, other states no call and all64 bytes unchanged. Subordinate initialization/tick modeled; fullgame unverified.'),indent=2)+'\n')
print(cases,'sound_stream_work_update comparisons passed')
