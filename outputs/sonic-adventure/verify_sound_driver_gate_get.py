import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_gate_get.dll')).sa_sound_driver_gate_get;fn.argtypes=abi
@Call
def call(*args):raise AssertionError('Unexpected external call')
cases=0
for value,bank,dest in itertools.product([0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0x8ca20004,0x8ca08000,0x8ca08040,0x8ca0808c]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08090]=bytes([0x55])*144;ram[0xa20000:0xa20010]=bytes([0x66])*16
 struct.pack_into('<I',ram,dest&0xffffff,value);wanted=bytearray(ram);struct.pack_into('<I',wanted,dest&0xffffff,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=dest;s.r[15]=0x8cf00000;s.pc=0x8c6680e2;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram==wanted and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert r[0]==r[4]==0 and r[15]==0x8cf00000;cases+=1
scope='56 independent whole16MiB RAM and retail CPU/FPU/SR/PR comparisons of zero gate getter6680e2. Seven previous output words,both banks,four valid destinations including three overlapping driver-object fields. Exactly one32-bit zero write,zero R4/result,unchanged stack,all other RAM preserved. No services. Host-invalid pointers and game lifetime behavior unverified.'
(p/'analysis/sound_driver_gate_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'zero gate getter comparisons passed')
