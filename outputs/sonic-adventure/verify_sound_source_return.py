import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(dll,name):
    f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);f.argtypes=abi;return f
fn=load('sound_source_return.dll','sa_sound_source_return');locks={0x8c65e368:load('sound_source_lock_enter.dll','sa_sound_source_lock_enter'),0x8c65e380:load('sound_source_lock_exit.dll','sa_sound_source_lock_exit')};cases=0
locks[0x8c10cbb8]=load('integer_remainder.dll','sa_integer_remainder')
for mode,offset,count,callback,bank,mismatch in itertools.product([0,1,2],[0,4,7,60],[0,1,4,16],[0,0x8c010100],[0,1],[False,True]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b
    ram[0xa40000:0xa40040]=bytes([0x55])*64
    struct.pack_into('<8I',ram,0xa4000c,5,20,offset,offset,0x8ca10000,64,8,callback);struct.pack_into('<I',ram,0xa4002c,0x12345678)
    ram[0xa10000:0xa10080]=bytes(range(128));struct.pack_into('<2I',ram,0xa20004,0x8ca10000+((offset+64-count)%64+int(mismatch))%64,count)
    before=bytes(ram[0xa40000:0xa40040]);buffer_before=bytes(ram[0xa10000:0xa10080]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(address,mem,r,trace):
        trace.append((address,tuple(r[i] for i in range(4,8))))
        if address==0x8c10d25c:
            assert r[6]<=16
            for i in range(r[6]):mem[(r[4]&0xffffff)+i]=mem[(r[5]&0xffffff)+i]
        else:assert address==callback and r[4]==0x12345678 and r[5]==0xfffffffd
    def hook(address):
        def run(mem,r,sr):service(address,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        if address in locks:return locks[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        service(address,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca40000,mode,0x8ca20004,0x22340007];s.r[15]=0x8cf00000;s.pc=0x8c65e644;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in [0x8c010100]},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    wanted=bytearray(before);wanted_buffer=bytearray(buffer_before)
    if mode in [0,1] and not mismatch:
        struct.pack_into('<I',wanted,0x14 if mode==0 else 0x18,(offset+64-count)%64)
        struct.pack_into('<I',wanted,0x10 if mode==0 else 0xc,(20 if mode==0 else 5)+count)
    assert native[0xa40000:0xa40040]==wanted and native[0xa10000:0xa10080]==wanted_buffer
    assert native[0xa20004:0xa2000c]==(struct.pack('<2I',0x8ca10000+((offset+64-count)%64+int(mismatch))%64,count) if mode in [0,1] else bytes(8))
    assert len(actual)==int(bool(callback and (mode==2 or mismatch)))
    assert sr.value&~1==0x70000062 and regs[15]==0x8cf00000;cases+=1
(p/'analysis/sound_source_return_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='384 retail RAM/CPU/FPU/SR/PR/service-trace comparisons for source return65e644..65e712. Native interrupt save/restore and signed remainder; only error callback modeled. Modes0/1/invalid,positions0/4/7/60,counts0/1/4/16,optional callback,both banks,matching/mismatched returned descriptor. Independent rollback position modulo64 and counter restoration only for matching descriptors; mismatches preserve object and report error; invalid modes clear descriptor. Full64-byte object,128-byte buffer,SR mask/stack preserved. Connected virtual buffer commits and full playback/fullgame unverified.'),indent=2)+'\n');print(cases,'source return comparisons passed')
