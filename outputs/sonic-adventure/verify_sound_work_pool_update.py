import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_pool_update.dll')).sa_sound_work_pool_update
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
base=struct.unpack_from('<I',binary,0x65bcf0-0x10000)[0];assert base==0x8c8a3910
cases=0
for mask,bank in itertools.product(range(256),[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    pos=base&0xffffff;ram[pos:pos+0x480]=bytes([0x55])*0x480
    for i in range(8):ram[pos+i*0x90]=1 if mask&(1<<i) else [0,2,128,255][i%4]
    before=bytes(ram[pos:pos+0x480]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(r,trace):trace.append(r[4]);assert base<=r[4]<base+0x480 and (r[4]-base)%0x90==0
    def hook(mem,r,sr):service(r,expected);return sr
    @Call
    def call(ctx,a,mem,f,sr,r,pr):assert a==0x8c65bb88;service(r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65bbd4;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{0x8c65bb88:hook},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert actual==expected==[base+i*0x90 for i in range(8) if mask&(1<<i)]
    assert native[pos:pos+0x480]==before and regs[15]==0x8cf00000
    cases+=1
(p/'analysis/sound_work_pool_update_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='512 full retail RAM/CPU/FPU/SR/PR and ordered calls comparisons65bbd4..65bc02. All256 active masks across8 work slots(stride90,total480) at original pool8c8a3910,both banks. Independent calls exactly once in address order only for active byte1; inactive0/2/128/255 skipped; entire pool unchanged,stack restored. Public update modeled as no-op; engine scheduling/native per-work integration/fullgame unverified.'),indent=2)+'\n')
print(cases,'audio work pool comparisons passed')
