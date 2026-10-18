import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);f.argtypes=abi;return f
create=load('sound_stream_create.dll','sa_sound_stream_create');destroy=load('sound_stream_destroy.dll','sa_sound_stream_destroy');fill=load('byte_fill.dll','sa_byte_fill');cases=0
pool=0x8c8d61ac;size=0xac
for free,handle,bank in itertools.product(range(9),[0,0x8ca60000],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[pool&0xffffff:(pool&0xffffff)+size*8]=bytes([0x55])*(size*8)
    for index in range(8):struct.pack_into('<H',ram,(pool&0xffffff)+size*index,0 if index==free else 1)
    before=bytes(ram[pool&0xffffff:(pool&0xffffff)+size*8]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(address,r,trace):
        trace.append((address,r[4]))
        if address==0x8c644b6e:r[0]=handle
        else:assert address==0x8c644bfe and r[4]==0
    def hook(address):
        def run(mem,r,sr):service(address,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target={0x8c10d31c:fill,0x8c667440:destroy}.get(address)
        if target:return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        service(address,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];args=[0x8ca10000,512,0x8ca20000,65536];s.r[4:8]=args;s.r[15]=0x8cf00000;s.pc=0x8c6673b4;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in [0x8c644b6e,0x8c644bfe]},fpu=f,special=special)
    assert create(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
    wanted=bytearray(before)
    if free<8:
        obj=bytearray(size)
        if handle:
            struct.pack_into('<H',obj,0,1);struct.pack_into('<I',obj,8,handle);struct.pack_into('<4I',obj,0x38,*args)
            struct.pack_into('<4I',obj,0x88,0x8c667384,pool+free*size,0x8c6673a2,pool+free*size)
        wanted[free*size:(free+1)*size]=obj
    assert native[pool&0xffffff:(pool&0xffffff)+size*8]==wanted
    assert regs[0]==(pool+free*size if free<8 and handle else 0) and regs[15]==0x8cf00000
    assert [a for a,v in actual]==([] if free==8 else [0x8c644b6e]+([] if handle else [0x8c644bfe]))
    cases+=1
(p/'analysis/stream_allocation_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='36 full retail RAM/CPU/FPU/SR/PR/MACL stream constructor6673b4..667440 comparisons. First free slots0..7 and exhausted pool,successful/failed file handle allocation,both FPU banks. Native zero-fill and destructor667440..667464 on failure; handle allocator/release modeled. Independent entire eight-slot pool,callback/default userdata,arguments,active flag,handle,failure clearing,return pointer/stack checked. Standalone destruction and native file handle allocator/release integration unverified.'),indent=2)+'\n')
print(cases,'stream allocation comparisons passed')
