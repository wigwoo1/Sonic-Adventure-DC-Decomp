import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_work_complete.dll')).sa_sound_stream_work_complete
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
services=[0x8c6676f6,0x8c6677b6,0x8c6677d8,0x8c668af4,0x8c6676ae,0x8c667796,0x8c010100,0x8c010200,0x8c010300];cases=0
for total,consumed,channels,limit,callback,bank in itertools.product([0,5,32,100],[0,4,40],[0,1,2],[0xffffffff,0,50],[0,0x8c010300],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;work=0x8ca60000
    ram[0xa60000:0xa60090]=bytes([0x55])*144
    struct.pack_into('<4I',ram,0xa60004,0x8ca70000,0x8ca80000,0x8ca80100,0x8ca80200)
    for offset,value in [(0x2c,7),(0x30,9),(0x34,consumed),(0x3c,limit),(0x40,11),(0x44,13),(0x48,callback),(0x4c,0x12345678)]:struct.pack_into('<I',ram,0xa60000+offset,value)
    for address in [0xa80000,0xa80100,0xa80200]:struct.pack_into('<I',ram,address,0x8ca90000)
    struct.pack_into('<2I',ram,0xa9001c,0x8c010100,0x8c010200)
    before=bytes(ram[0xa60000:0xa60090]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    chosen=min(total-consumed,32);splits=[]
    def service(address,mem,r,trace):
        trace.append((address,tuple(r[i] for i in range(4,8))))
        if address==0x8c6676f6:r[0]=total
        elif address==0x8c6677b6:r[0]=36
        elif address==0x8c6677d8:r[0]=32
        elif address==0x8c6676ae:r[0]=channels
        elif address==0x8c668af4:
            assert r[4] in [work+0x14,work+0x1c,work+0x24]
            assert r[5]==(36 if r[4]==work+0x14 else chosen*2&0xffffffff)
            for pointer in [r[6],r[7]]:
                offset=pointer&0xffffff
                for i,v in enumerate(struct.pack('<2I',r[4],r[5])):mem[offset+i]=v
        elif address==0x8c010300:assert r[4]==0x12345678
        else:assert address in [0x8c010100,0x8c010200,0x8c667796]
    def hook(address):
        def run(mem,r,sr):service(address,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):service(address,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=work;s.r[15]=0x8cf00000;s.pc=0x8c65ba22;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in services},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    wanted=bytearray(before)
    for offset,value in [(0x2c,7+chosen),(0x30,45),(0x34,consumed+chosen),(0x40,11+chosen),(0x44,49)]:struct.pack_into('<I',wanted,offset,value&0xffffffff)
    assert native[0xa60000:0xa60090]==wanted and regs[15]==0x8cf00000
    signed_limit=ctypes.c_int32(limit).value;notify=bool(callback and signed_limit>=0 and 11+chosen>=signed_limit)
    assert [a for a,args in actual]==[0x8c6676f6,0x8c6677b6,0x8c6677d8,0x8c668af4,0x8c010200,0x8c010100]+[a for index in range(channels) for a in [0x8c6676ae,0x8c668af4,0x8c010200,0x8c010100]]+[0x8c6676ae]+([0x8c010300] if notify else [])+[0x8c667796]
    cases+=1
(p/'analysis/stream_work_complete_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='432 retail RAM/CPU/FPU/SR/PR/service-trace comparisons for higher-level completion65ba22..65bb4e. Independent signed minimum(decoded32,total-consumed),per-channel byte range counts,complete144-byte work and progress counters,optional finite-limit notification/order,stack checked. Totals0/5/32/100,consumed0/4/40,channels0/1/2,limits-1/0/50,callback presence,both banks. Getters,range splitter,virtual commits,ack and callback modeled. Retail negative remaining propagates signed negative count; no clamping introduced. Native dependency integration,actual asset final-sample trimming,full playback/fullgame unverified.'),indent=2)+'\n');print(cases,'stream work completion comparisons passed')
