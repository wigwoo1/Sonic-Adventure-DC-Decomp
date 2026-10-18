import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_query.dll')).sa_sound_source_query
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for mode,available,position,limit,callback,bank in itertools.product([0,1,2,0xffffffff],[0,12,80],[0,60],[0,7,100],[0,0x8c010100],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    obj=0x8ca40000;dest=0x8ca20004
    ram[0xa40000:0xa40040]=bytes([0x55])*64;ram[0xa20000:0xa20010]=bytes([0x66])*16
    struct.pack_into('<8I',ram,0xa4000c,available,available,position,position,0x8ca10000,64,8,callback)
    struct.pack_into('<I',ram,0xa4002c,0x12345678)
    before=bytes(ram[0xa40000:0xa40040]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(address,mem,r,trace):
        trace.append((address,tuple(r[i] for i in range(4,8))))
        if address==0x8c65e368:
            assert r[4]==obj;r[0]=0x13579bdf
        elif address==0x8c65e380:
            assert r[4]==obj and r[5]==0x13579bdf;r[0]=0x2468ace0
        elif address==0x8c10cbb8:
            assert r[0]==64
            signed=ctypes.c_int32(r[1]).value;r[0]=(signed-(abs(signed)//64*(-1 if signed<0 else 1))*64)&0xffffffff
        else:
            assert address==callback and r[4]==0x12345678 and r[5]==0xfffffffd;r[0]=0xabcdef12
    def hook(address):
        def run(mem,r,sr):service(address,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):service(address,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[obj,mode,limit,dest];s.r[15]=0x8cf00000;s.pc=0x8c65e4b8;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in [0x8c65e368,0x8c65e380,0x8c10cbb8,0x8c010100]},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    wanted=bytearray(before)
    if mode in [0,1]:
        count=min(available,64-position+8,limit);pointer=0x8ca10000+position
        struct.pack_into('<I',wanted,0x14 if mode==0 else 0x18,(position+count)%64)
        struct.pack_into('<I',wanted,0x10 if mode==0 else 0x0c,(available-count)&0xffffffff)
    else:count=pointer=0
    assert native[0xa40000:0xa40040]==wanted
    assert native[0xa20000:0xa20010]==bytes([0x66])*4+struct.pack('<2I',pointer,count)+bytes([0x66])*4
    assert regs[0]==0x2468ace0 and regs[15]==0x8cf00000
    assert [a for a,args in actual]==[0x8c65e368]+([0x8c10cbb8] if mode in [0,1] else ([callback] if callback else []))+[0x8c65e380]
    cases+=1
(out/'analysis/sound_source_query_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='288 retail instruction/native C comparisons for 65e4b8..65e58a. Both query modes, invalid modes, available counts, ring positions, limits, optional error callback and FPU banks. Full RAM/register/SR/PR/FPU and service traces plus independent object/output guards, capacity minimum and ring position changes. Lock enter/exit, signed remainder and error callback modeled; native dependency integration and continuous playback unverified.'),indent=2)+'\n')
print(cases,'sound source query comparisons passed')
