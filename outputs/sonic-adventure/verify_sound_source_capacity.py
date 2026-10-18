import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_capacity.dll')).sa_sound_source_capacity
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for mode,count,callback,bank,stack in itertools.product([0,1,2,0xffffffff,128],[0,1,127,128,255,0x80000000,0xffffffff],[0,0x8c010300],[0,1],[0x8cf00000,0x8cef0000]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64
    struct.pack_into('<2I',ram,0xa2000c,count,count^0xabcdef01)
    struct.pack_into('<2I',ram,0xa20028,callback,0x12345678);before=bytes(ram[0xa20000:0xa20040])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(mem,r,trace):
        assert (r[4],r[5])==(0x12345678,0xfffffffd)
        trace.append((r[4],r[5]));r[0]=0xdeadbeef
    def hook(mem,r,sr):service(mem,r,expected);return sr
    @Call
    def call(ctx,address,mem,f,sr,r,pr):
        assert address==0x8c010300;service(mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca20000;s.r[5]=mode;s.r[15]=stack;s.pc=0x8c65e462;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{0x8c010300:hook},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    assert regs[0]==(count if mode==1 else count^0xabcdef01 if mode==0 else 0)
    assert bool(actual)==bool(callback and mode not in [0,1])
    assert native[0xa20000:0xa20040]==before and regs[15]==stack
    cases+=1
(p/'analysis/sound_source_capacity_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='280 full retail RAM/CPU/FPU/SR/PR and error callback comparisons65e462..65e49a. Modes0/1/invalid2/ffffffff/128; seven full-word counter patterns; optional callback,banks,two stacks. Independent mode1 free counter+c,mode0 available counter+10,invalid returns0 and optionally callback(user,-3); all64 object bytes unchanged. Callback modeled; native feeder integration/full playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'source capacity comparisons passed')
