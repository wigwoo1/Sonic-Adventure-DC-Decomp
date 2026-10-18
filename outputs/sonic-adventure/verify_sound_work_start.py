import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_start.dll')).sa_sound_work_start
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_reset.dll')).sa_sound_work_reset
reset.argtypes=fn.argtypes
cases=0
for phase,count,bank,right in itertools.product([0,1,2,4,0xffffffff],[0,1,2,127,128,255,0xffffffff],[0,1],[0x8ca40000,0]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    ram[0xa20000:0xa20100]=bytes([0x55])*256;struct.pack_into('<H',ram,0xa200a8,phase&0xffff);struct.pack_into('<H',ram,0xa200aa,count&0xffff)
    struct.pack_into('<I',ram,0xf00000,right);original=bytes(ram[0xa20000:0xa20100])
    for off in [0x2c,0x30,0x34,0x38,0x3c,0x40,0x44]:struct.pack_into('<I',ram,0xa20000+off,(phase*0x1234567+count)&0xffffffff)
    ram[0xa20003]=count&255;original=bytes(ram[0xa20000:0xa20100])
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,0x8ca10000,count,0x8ca30000];s.r[15]=0x8cf00000;s.pc=0x8c65b778;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,m,f,srp,r,prp):
        assert address==0x8c65b606
        return reset(m,f,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    wanted=bytearray(original)
    wanted[1]=1;wanted[3]=0
    for off,value in [(0x2c,0),(0x30,0),(0x34,0),(0x38,struct.unpack_from('<I',binary,0x65b710-0x10000)[0]),(0x3c,0xffffffff),(0x40,0),(0x44,0)]:struct.pack_into('<I',wanted,off,value)
    assert native[0xa20000:0xa20100]==wanted and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_work_start_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of work start65b778..65b78e with native reset65b606. Varied prior counters/stop bytes,both banks and stack data. Independent entire256-byte object: counters2c/30/34/40/44 zero,limit38 copied from retail literal65b710,remaining3c=-1,stop byte3 cleared; state byte1 set to1; all other bytes preserved. Reset callback uses recovered native source; no modeled services. Full constructor/producer/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'sound_work_start comparisons passed')
