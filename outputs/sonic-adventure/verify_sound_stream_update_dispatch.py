import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_update_dispatch.dll')).sa_sound_stream_update_dispatch
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for phase,count,bank,right in itertools.product([0,1,2,3,0xffffffff],[0,1,2,3,4,15,0xffffffff],[0,1],[0x8ca40000,0]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64;struct.pack_into('<2I',ram,0xa2000c,phase,count)
    struct.pack_into('<I',ram,0xf00000,right);original=bytes(ram[0xa20000:0xa20040])
    struct.pack_into('<H',ram,0xa200a8,count&65535);actual=[];expected=[]
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c667ad4;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):actual.append((address,r[4]));return 0
    def hook(address):
        def run(mem,r,sr):expected.append((address,r[4]));return sr
        return run
    reference(ram,s,{a:hook(a) for a in [0x8c667a5c,0x8c66ab5e,0x8c66a524,0x8c669bfc,0x8c66a348]},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    target={0:0x8c667a5c,1:0x8c66ab5e,2:0x8c66a524,3:0x8c669bfc,4:0x8c66a348}.get(count&65535)
    assert actual==expected==([] if target is None else [(target,0x8ca20000)])
    assert native[0xa20000:0xa20040]==wanted and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_stream_update_dispatch_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='140 full retail RAM/CPU/FPU/SR/PR update format dispatcher comparisons667ad4..667b16. Formats0/1/2/3/4/15/ffff,varied phase/register/stack inputs,both banks. All five downstream update bodies modeled; independent target selection and unchanged handle bytes,stack checked. Shared format0 body reached through external conditional branch. Native update integration/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stream update dispatcher comparisons passed')
