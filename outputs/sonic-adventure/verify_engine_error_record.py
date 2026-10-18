import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_error_record.dll')).sa_engine_error_record;fn.argtypes=abi;cases=0
values=[0,1,127,128,0x80000000,0xffffffff]
for message,user,bank,sr_initial in itertools.product(values,values,[0,1],[0,0x70000063,0x700000f2]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x67c1c0:0x67c1d8]=bytes([0x55])*24
 wanted=bytearray(ram);struct.pack_into('<2I',wanted,0x67c1c8,message,user)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:6]=[message,user];s.r[15]=0x8cf00000;s.pc=0x8c65acac;s.pr=0x8c000100;s.sr=sr_initial
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(*args):raise AssertionError('Unexpected subordinate call')
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram==wanted and list(r)==list(s.r) and sr.value==s.sr==sr_initial and pr.value==s.pr and bytes(nf)==bytes(f)
 assert r[2]==0x8c67c1cc and r[3]==0x8c67c1c8 and r[15]==0x8cf00000
 cases+=1
scope='216 full retail RAM/CPU/FPU/SR/PR comparisons of five-instruction diagnostic recorder65acac..65acb6. Six full-word message/userdata patterns,both FPU banks,three SR fixtures. Independent entire16MiB RAM: only global67c1c8=R4 and global67c1cc=R5 changed,including R5 store in RTS delay slot;adjacent globals/stack preserved,R2/R3 global addresses,allSRbits preserved. No modeled calls. Existing C translation rebuilt and independently registered,not newly recovered code. Game caller integration/physical audio/fullgame unverified.'
(p/'analysis/engine_error_record_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,'native diagnostic recorder comparisons passed')
