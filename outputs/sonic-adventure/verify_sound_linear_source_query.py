import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_linear_source_query.dll')).sa_sound_linear_source_query
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for mode,available,position,limit,callback,bank in itertools.product([0,1,2,0xffffffff],[0,12,80,0x80000000,0xffffffff],[0,60],[0,7,100,0x80000000,0xffffffff],[0,0x8c65e16e],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    obj=0x8ca40000;dest=0x8ca20004
    ram[0xa40000:0xa40040]=bytes([0x55])*64;ram[0xa20000:0xa20010]=bytes([0x66])*16
    struct.pack_into('<6I',ram,0xa4000c,available,position,0x8ca10000,80,callback,obj)
    before=bytes(ram[0xa40000:0xa40040]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    dependencies={}
    for address,dep in [(0x8c65e08c,'sound_linear_source_lock_enter'),(0x8c65e0a4,'sound_linear_source_lock_exit'),(0x8c65e16e,'sound_linear_source_identity_get')]:
        fun=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dep}.dll')),'sa_'+dep);fun.argtypes=fn.argtypes;dependencies[address]=fun
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address in dependencies,hex(address)
        actual.append(address)
        if address==0x8c65e16e:assert (r[4],r[5])==(obj,0xfffffffd)
        return dependencies[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[obj,mode,limit,dest];s.r[15]=0x8cf00000;s.pc=0x8c65e1d0;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(before)
    if mode==1:
        signed=lambda v:ctypes.c_int32(v).value
        count=available if signed(available)<signed(limit) else limit;pointer=(0x8ca10000+position)&0xffffffff
        struct.pack_into('<2I',wanted,0xc,(available-count)&0xffffffff,(position+count)&0xffffffff)
    else:count=pointer=0
    assert native[0xa40000:0xa40040]==wanted
    assert native[0xa20000:0xa20010]==bytes([0x66])*4+struct.pack('<2I',pointer,count)+bytes([0x66])*4
    assert regs[15]==0x8cf00000 and (sr.value&0xf0)==0x60
    assert actual==[0x8c65e08c]+([callback] if mode not in [0,1] and callback else [])+[0x8c65e0a4]
    cases+=1
scope='800 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source query65e1d0..65e242. Modes0/1 and invalid2/ffffffff,five available/request patterns including signed extremes,two positions,optional controlled native callback,both FPU banks. Native interrupt lock enter/exit and identity-getter callback; no modeled calls. Independent complete64-byte source window and descriptor guards: mode1 signed minimum request/capacity,base+offset pointer,offset advances and capacity decreases modulo32;other modes empty descriptor,invalid optionally callback(userdata,-3). Original interrupt mask restored. Pool construction,connected reservation return and game callers unverified.'
(out/'analysis/sound_linear_source_query_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,'linear source query comparisons passed')
