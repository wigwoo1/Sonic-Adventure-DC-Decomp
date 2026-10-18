import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
def load(stem,name):return getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),name)
fn=load('sound_stream_work_initialize','sa_sound_stream_work_initialize')
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for a,stem,name in [(0x8c668af4,'sound_driver_split_range','sa_sound_driver_split_range'),(0x8c10cd38,'engine_frame_pool_queue_initialize','sa_engine_record_copy_dispatch'),(0x8c10cd8e,'engine_frame_pool_queue_initialize','sa_engine_record_copy8_tail'),(0x8c10d25c,'byte_move','sa_byte_move')]:helpers[a]=load(stem,name);helpers[a].argtypes=fn.argtypes
cases=0
for length,choice,fmt,bank in itertools.product([0,15,16,64,4096],[0,1,2,3],[0,2,4],[0,1]):
    result=[0,8,length,length+1][choice];success=length>=16 and 0<result<=length
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa60000:0xa60100]=bytes([0x55])*256;ram[0xa60001]=1
    struct.pack_into('<2I',ram,0xa60004,0x8ca70000,0x8ca80000)
    struct.pack_into('<I',ram,0xa80000,0x8ca90000)
    for off,a in [(0x18,0x8c010100),(0x1c,0x8c010200),(0x20,0x8c010300)]:struct.pack_into('<I',ram,0xa90000+off,a)
    payload=bytes((i*17+3)&255 for i in range(4096));ram[0xa10000:0xa11000]=payload
    before=bytes(ram[0xa60000:0xa60100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
    def service(a,mem,r,trace):
        args=tuple(r[i] for i in range(4,8));trace.append((a,args))
        if a==0x8c010100:
            assert args[:3]==(0x8ca80000,1,4096)
            for i,v in enumerate(struct.pack('<2I',0x8ca10000,length)):mem[(r[7]&0xffffff)+i]=v
        elif a==0x8c667596:assert args[:3]==(0x8ca70000,0x8ca10000,length);r[0]=result
        elif a==0x8c6676a4:assert r[4]==0x8ca70000;r[0]=fmt
        else:
            assert r[4]==0x8ca80000
            pos=r[6]&0xffffff;desc=tuple(sum(int(mem[pos+i*4+j])<<(j*8) for j in range(4)) for i in range(2))
            if a==0x8c010300:assert success and r[5]==0 and desc==(0x8ca10000,result)
            else:assert a==0x8c010200 and r[5]==1 and desc==((0x8ca10000+result,length-result) if success else (0x8ca10000,length))
    def hook(a):
        def run(mem,r,sr):service(a,mem,r,expected);return sr
        return run
    @Call
    def call(ctx,a,mem,f,sr,r,pr):
        if a in helpers:return helpers[a](mem,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        service(a,mem,r,actual);return 0
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca60000;s.r[15]=0x8cf00000;s.pc=0x8c65b7a8;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{a:hook(a) for a in [0x8c010100,0x8c010200,0x8c010300,0x8c667596,0x8c6676a4]},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
    wanted=bytearray(before)
    if success:
        wanted[1]=2
        if fmt==4:wanted[3]=1
        if fmt==2:wanted[0x50:0x50+min(length,64)]=payload[:min(length,64)]
    assert native[0xa60000:0xa60100]==wanted and regs[15]==0x8cf00000
    assert native[0xa10000:0xa11000]==payload
    assert [a for a,args in actual]==[0x8c010100]+([0x8c667596] if length>=16 else [])+([0x8c6676a4,0x8c6676a4,0x8c010300,0x8c010200] if success else [0x8c010200])
    cases+=1
(p/'analysis/sound_stream_work_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='120 full retail RAM/CPU/FPU/SR/PR and call trace comparisons65b7a8..65b88c. Reservation lengths0/15/16/64/4096,initializer result0/8/length/oversize,formats0/2/4,banks. Native split/copy and byte move; query/header initializer/format/commit/return modeled. Independent head/tail descriptors, state2 success, format4 stop flag, format2 min(length,64) copy, complete256 work bytes and4096 source bytes preserved otherwise. Full native header/source integration and fullgame unverified.'),indent=2)+'\n')
print(cases,'audio work initialization comparisons passed')
