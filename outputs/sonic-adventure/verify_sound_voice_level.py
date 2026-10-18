import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_level.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for enabled,slot,level,cap,bank in itertools.product([0,1],[0,17,36],[-200,-127,0,90,200],[-50,127],[0,0x200000]):
 entry=0x8c0947ba;name='sa_sound_voice_level'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c094870),0x8ca01000 if enabled else 0)
 record=lit(0x8c09486c)+slot*52
 struct.pack_into('<2i',ram,record+24,level,cap)
 struct.pack_into('<3f',ram,record+36,1.,2.,3.);struct.pack_into('<3f',ram,0xa01020,1.,2.,3.)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):errors.append(hex(a));return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=slot;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert r[0]==(min(max(level,-127),cap)&0xffffffff) and macl.value==slot*52
 cases+=1
(out/'analysis/sound_voice_level_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='83 reachable instruction native voice level routine; null listener and coincident non-null positions, slots0/17/36, signed base levels and caps, both FPU banks. Full retail RAM/CPU/FPU/SR/PR and independent clamped level/MACL checks. Nonzero distance/external numerical helper and other FPU modes untested.'),indent=2)+'\n')
print(cases,'voice level comparisons passed')
