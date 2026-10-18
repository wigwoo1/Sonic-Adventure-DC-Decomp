import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(name):
    fn=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/f'{name}.dll')),'sa_'+name);fn.argtypes=abi;return fn
copy=load('sound_stream_sample_copy');cases=0
for kind,count,offset,bank in itertools.product(['mono','stereo'],[0,1,2,16,-1],[0,3,20],[0,1]):
    name='sound_stream_'+kind+'_compact';fn=load(name);entry=0x8c66792c if kind=='stereo' else 0x8c667960
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa20000:0xa20200]=bytes(range(256))*2
    wanted=bytearray(ram[0xa20000:0xa20200])
    for channel in range(2 if kind=='stereo' else 1):
        for index in range(max(count,0)):
            dest=16+channel*128+index*2;source=dest+offset*2;wanted[dest:dest+2]=wanted[source:source+2]
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20010,offset,64,count&0xffffffff];s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address==0x8c667900
        return copy(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert native[0xa20000:0xa20200]==wanted and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_stream_compaction_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='60 full retail RAM/CPU/FPU/SR/PR mono/stereo buffer compaction comparisons667960..667978 and66792c..667960 with native sample copy667900,no modeled calls. Counts0/1/2/16/-1,source sample offsets0/3/20,both FPU banks,64-sample planar channel span. Independent chronological sample copy oracle, all512 buffer bytes and stack checked. Continuous scheduling/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'stream compaction comparisons passed')
