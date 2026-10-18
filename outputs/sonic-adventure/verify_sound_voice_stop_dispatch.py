import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_dispatch.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for slot,bank in itertools.product([-1,0,15,31,32,36],[0,0x200000]):
 entry=0x8c093b70;name='sa_sound_voice_stop_dispatch'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 mapping=lit(0x8c093bb4)
 for index in range(2,7):struct.pack_into('<I',ram,mapping+index*4,0x8ca08000+index*8)
 expected_target=struct.unpack_from('<I',binary,0x093bc0-0x10000)[0] if slot>=32 else 0x8c0933ba
 expected_argument=0x8ca08000+(slot-30)*8 if slot>=32 else slot&0xffffffff
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],pr[0]));r[0]=123;return 0
 def hook(m,r,sr):events.append((expected_target,r[4],0x8c000100));r[0]=123;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=slot&0xffffffff;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={expected_target:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events==[(expected_target,expected_argument,0x8c000100)] and r[0]==123
 cases+=1
(out/'analysis/sound_voice_stop_dispatch_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='16-instruction native stop dispatcher; signed slot-1,ordinary0/15/31,reserved32/36,both banks. Full retail RAM/CPU/FPU/SR/PR, independent target/handle/ordinary slot/original PR and tail return propagation checks. Lower stop/note-off calls modeled; caller integration pending.'),indent=2)+'\n')
print(cases,'voice stop dispatcher comparisons passed')
