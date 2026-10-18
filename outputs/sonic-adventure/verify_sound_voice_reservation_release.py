import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_reservation_release.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for group,count,match,slot,bank in itertools.product([-1,0,34],[-1,0,1,4],[-1,0,3],[0,36],[0,0x200000]):
 entry=0x8c09550e;name='sa_sound_voice_reservation_release'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 identities=lit(0x8c0955ac);metadata=lit(0x8c0955a0);reservations=lit(0x8c0955a4);table=lit(0x8c0955a8)
 for i in range(35):
  struct.pack_into('<h',ram,identities+i*2,-100+i)
  struct.pack_into('<h',ram,metadata+i*8,count)
  struct.pack_into('<4I',ram,reservations+i*16,51,52,53,54)
 identity=-999 if group<0 else -100+group
 struct.pack_into('<I',ram,table+slot*52+16,identity&0xffffffff);struct.pack_into('<I',ram,table+slot*52+8,42)
 if group>=0 and match>=0:struct.pack_into('<I',ram,reservations+group*16+match*4,43)
 expected_memory=bytearray(ram)
 if group>=0 and 0<=match<count:struct.pack_into('<I',expected_memory,reservations+group*16+match*4,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):errors.append(hex(a));return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=slot;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 expected_memory[0xeffff4:0xf00000]=native[0xeffff4:0xf00000]
 assert native==expected_memory and macl.value==slot*52
 cases+=1
(out/'analysis/sound_voice_reservation_release_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='64-instruction native reservation release; group missing/first/last,signed identities,counts-1/0/1/4,matching key missing/first/last,slots0/36,both banks. Full retail RAM/CPU/FPU/SR/PR and independent expected single clear/no-op across RAM excluding12-byte stack scratch plus MACL check. Explicit synthetic valid tables; caller integration pending.'),indent=2)+'\n')
print(cases,'voice reservation release comparisons passed')
