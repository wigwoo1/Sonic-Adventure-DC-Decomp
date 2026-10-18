import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_slot_select.dll')).sa_sound_voice_slot_select
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def literal(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
table=literal(0x8c094078);reserved_cursor=literal(0x8c09407c);ordinary_cursor=literal(0x8c094220);cases=0
ids=[0,1,0x42,0x7c,0x308,0x309,0x30d,0x30e,0x4be,0x4bf,0x4c6,0x4c7,0x4ef,0x4f0,0x4f7,0x4f8,0x4f9]
for identity,pattern,bank in itertools.product(ids,['free','busy','eligible'],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 reserved=identity in [0,0x42,0x43,0x7c,0xa1,0xa0,0xef,0x229,0x22a,0x423,0x40f,0x438,0x29b,0x403,0x405,0x4f9] or 0x308<identity<0x30e or 0x4be<identity<0x4c7 or 0x4ef<identity<0x4f8
 base=32 if reserved else 0
 struct.pack_into('<I',ram,reserved_cursor,32);struct.pack_into('<I',ram,ordinary_cursor,0)
 for slot in range(37):struct.pack_into('<2I',ram,table+slot*52,10,0 if pattern=='free' else 1)
 if pattern=='eligible':struct.pack_into('<I',ram,table+(base+2)*52,1)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=5;s.r[5]=identity;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c093f02
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 errors=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):errors.append(a);return -9
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 expected=base if pattern=='free' else base+2 if pattern=='eligible' else 0xffffffff
 assert r[0]==expected,(identity,pattern,r[0],expected)
 cases+=1
(out/'analysis/sound_voice_slot_select_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native/reference full RAM/CPU/SR/PR/FPU comparisons. Independent reserved/ordinary selection for boundary identities, free/busy/priority eligible tables and both FPU banks. All 37 table entries initialized; valid cursor starts32/0 only. Function name provisional; audio playback not exercised.'},indent=2)+'\n')
print(cases,'sound slot selector comparisons passed')
