import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();cases=0
for kind,mask,bits,bank in itertools.product(['enter','exit'],range(16),[0,3,0x70008303,0xffffffff],[0,1]):
    fn=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/('sound_linear_source_lock_'+kind+'.dll'))),'sa_sound_linear_source_lock_'+kind)
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    @Call
    def call(*args):raise AssertionError('Unexpected dependency')
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[5]=0xabcdef00|mask;s.r[15]=0x8cf00000;s.pc=0x8c65e08c if kind=='enter' else 0x8c65e0a4;s.pr=0x8c000100;s.sr=(bits&~0xf0)|(mask<<4)
    before_sr=s.sr
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    reference(ram,s,{},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert sr.value==((before_sr&0xffffff0f)|(0xf0 if kind=='enter' else mask<<4))
    assert regs[0]==(mask if kind=='enter' else sr.value) and regs[15]==0x8cf00000
    cases+=1
(out/'analysis/sound_linear_source_locks_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='256 native C/retail SH4 comparisons: enter65e08c..65e0a4 and exit65e0a4..65e0bc. All16 interrupt masks, four other SR bit patterns and both FPU banks. Full RAM/CPU/SR/PR/FPU match plus independent mask/result checks. No modeled dependencies. Native host concurrency/interrupt hardware not implemented by this register-ABI recovery.'),indent=2)+'\n')
print(cases,'sound source interrupt mask comparisons passed')
