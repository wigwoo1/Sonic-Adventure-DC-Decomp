import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_linear_source_capacity.dll')).sa_sound_linear_source_capacity
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for mode,count,callback,bank,stack in itertools.product([0,1,2,0xffffffff,128],[0,1,127,128,255,0x80000000,0xffffffff],[0,0x8c65e16e],[0,1],[0x8cf00000,0x8cef0000]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20040]=bytes([0x55])*64
    struct.pack_into('<2I',ram,0xa2000c,count,count^0xabcdef01)
    struct.pack_into('<2I',ram,0xa2001c,callback,0x8ca20000);before=bytes(ram[0xa20000:0xa20040])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_linear_source_identity_get.dll')).sa_sound_linear_source_identity_get;getter.argtypes=fn.argtypes
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address==0x8c65e16e and (r[4],r[5])==(0x8ca20000,0xfffffffd)
        actual.append((r[4],r[5]))
        return getter(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca20000;s.r[5]=mode;s.r[15]=stack;s.pc=0x8c65e182;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert regs[0]==(count if mode==1 else 0)
    assert bool(actual)==bool(callback and mode not in [0,1])
    assert native[0xa20000:0xa20040]==before and regs[15]==stack
    cases+=1
(p/'analysis/sound_linear_source_capacity_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='280 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source capacity query65e182..65e1b2. Modes0/1 and invalid2/ffffffff/128,seven full-word capacity patterns,optional controlled native identity-getter callback,both banks,two stacks. Independent mode1 returns remaining+c,mode0 returns0,invalid returns0 and optionally callback(userdata,-3);all64 object-plus-guard bytes preserved. Callback executes natively,no modeled calls;retail default handler and connected reservation/fullgame unverified.'),indent=2)+'\n')
print(cases,'source capacity comparisons passed')
