import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_default_input.dll')).sa_sound_stream_default_input
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];cases=0
for position,capacity,total,bank in itertools.product([0,1,64,0xffffffff],[0,64,0xffffffff],[0,80,0xffffffff],[0,1]):
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;ram[0xa20000:0xa20100]=bytes([0x55])*256
    for offset,value in [(0x9c,position),(0x98,capacity),(0x40,(capacity+7)&0xffffffff),(0x18,total),(0x3c,1234)]:struct.pack_into('<I',ram,0xa20000+offset,value)
    ram[0xa10000:0xa10014]=bytes([0x66])*20;before=bytes(ram[0xa20000:0xa20100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,0x8ca10004,0x8ca10008,0x8ca1000c];s.r[15]=0x8cf00000;s.pc=0x8c667384;s.pr=0x8c000100;s.sr=0x70000063
    regs=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
    @Call
    def call(*args):raise AssertionError('Unexpected dependency')
    reference(ram,s,fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert native[0xa20000:0xa20100]==before
    assert native[0xa10000:0xa10014]==bytes([0x66])*4+struct.pack('<3I',position,(capacity+7-position)&0xffffffff,(total-capacity)&0xffffffff)+bytes([0x66])*4
    assert regs[0]==1234 and regs[15]==0x8cf00000;cases+=1
(p/'analysis/sound_stream_default_input_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='72 full retail RAM/CPU/FPU/SR/PR comparisons of default stream input callback667384..6673a2. Four position values,three consumed counters,three totals,both FPU banks. Independent position from+9c,capacity from+40 minus+9c,remaining from total+18 minus consumed+98,uint32 wraparound,returned stream+3c value,full256-byte unchanged stream and output guards. No modeled dependencies. Native lifecycle with default input/full playback/fullgame unverified.'),indent=2)+'\n');print(cases,'default input comparisons passed')
