import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_process.dll')).sa_file_decode_request_process
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for phase,channels,result,coefficients,bank in itertools.product([0,1,2,3],[0,1,2],[0,2,3,0xffffffff,0xfffffffe],[(1000,-500),(-300,200)],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa20000:0xa20040]=bytes([0x55])*64
    struct.pack_into('<6I',ram,0xa2000c,phase,0x55555555,channels,0x8ca10000,4,0x8ca30000)
    struct.pack_into('<I',ram,0xa20024,0x8ca40000);struct.pack_into('<2h',ram,0xa20030,*coefficients)
    original=bytes(ram[0xa20000:0xa20040]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    actual=[];expected=[]
    def service(address,mem,regs,trace):
        if address==0x8c644c90:trace.append((address,));return
        assert address in [0x8c65b120,0x8c65b238]
        count=2 if address==0x8c65b120 else 4
        stack=tuple(int.from_bytes(bytes(mem[(regs[15]&0xffffff)+i*4:(regs[15]&0xffffff)+i*4+4]),'little') for i in range(count))
        trace.append((address,tuple(regs[i] for i in range(4,8)),stack,int.from_bytes(bytes(mem[0xa2000c:0xa20010]),'little')));regs[0]=result
    def hook(address):
        def run(mem,regs,sr):service(address,mem,regs,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,sr,regs,pr):service(address,mem,regs,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca20000;s.r[15]=0x8cf00000;s.pc=0x8c644cb0;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);special={'macl':0}
    reference(ram,s,{a:hook(a) for a in [0x8c65b120,0x8c65b238,0x8c644c90]},fpu=f,special=special)
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    target=bytearray(original);trace=[]
    if phase in [1,2]:
        struct.pack_into('<2I',target,12,3,result)
        mono=channels==1;address=0x8c65b120 if mono else 0x8c65b238
        stack=tuple(v&0xffffffff for v in coefficients)
        if not mono:stack=(0x8ca40000,0x8ca2002c)+stack
        trace=[(address,(0x8ca10000,4,0x8ca30000,0x8ca20028),stack,2)]
        if not mono and result<0x80000000 and result&1:trace.append((0x8c644c90,))
    assert actual==trace and native[0xa20000:0xa20040]==target and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/file_decode_request_process_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='240 full retail RAM/CPU/FPU/SR/PR/MACL/service-trace comparisons of78-instruction file decoder dispatcher644cb0..644d4c. Phases0/1/2/3,channel selector0/1/2,five signed result patterns,two signed coefficient pairs,both banks. Independent phase1->2->3,mono iff selector1 otherwise stereo,source/count/output/history pointers,signed stack coefficients,stored result and positive odd stereo notification. Decoder kernels65b120/65b238 and flag setter644c90 modeled. Native kernel integration/physical playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'file decoder dispatcher comparisons passed')
