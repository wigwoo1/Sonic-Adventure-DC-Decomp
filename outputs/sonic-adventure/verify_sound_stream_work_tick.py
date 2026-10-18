import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_tick.dll')).sa_sound_stream_work_tick
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for initial,after,bank in itertools.product([0,1,2,3,0xffffffff],[0,1,2,3,0xffffffff],[0,1]):
    for stack in [0x8cf00000,0x8cef0000]:
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        ram[0xa60000:0xa60100]=bytes([0x55])*256;struct.pack_into('<I',ram,0xa60004,0x8ca70000)
        ram[0xa70000:0xa70100]=bytes([0x77])*256;struct.pack_into('<I',ram,0xa70004,initial)
        native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
        def service(address,mem,r,trace):
            trace.append((address,r[4],r[15]))
            if address==0x8c667718:
                assert r[4]==0x8ca70000
                r[0]=sum(int(mem[0xa70004+i])<<(8*i) for i in range(4))
            elif address==0x8c667ad4:
                assert r[4]==0x8ca70000
                for i in range(4):mem[0xa70004+i]=(after>>(8*i))&255
            else:
                assert address in [0x8c65b930,0x8c65ba22] and r[4]==0x8ca60000
                if address==0x8c65ba22:assert r[15]==stack
        def hook(address):
            def run(mem,r,sr):service(address,mem,r,expected);return sr
            return run
        @Call
        def call(ctx,address,mem,f,sr,r,pr):service(address,mem,r,actual);return 0
        s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca60000;s.r[15]=stack;s.pc=0x8c65bb4e;s.pr=0x8c000100;s.sr=0x70000063
        regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
        reference(ram,s,{a:hook(a) for a in [0x8c667718,0x8c667ad4,0x8c65b930,0x8c65ba22]},fpu=f,special={'macl':0})
        assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
        assert [a for a,r4,sp in actual]==[0x8c667718]+([0x8c65b930] if initial==0 else [])+[0x8c667ad4,0x8c667718]+([0x8c65ba22] if after==3 else [])
        assert regs[15]==stack and struct.unpack_from('<I',native,0xa70004)[0]==after
        cases+=1
(p/'analysis/sound_stream_work_tick_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR and ordered call trace comparisons of work dispatcher65bb4e..65bb88. Five initial phases, five post-update phases, both banks and two stacks. Independent feed-only-on-idle, unconditional update, completion-only-on-phase3 and restored-stack tail-call assertions. Phase getter/feed/update/completion modeled; integrated native decoding/full playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stream work tick comparisons passed')
