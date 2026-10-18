import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_alternate_bank_predicate.dll')).sa_sound_alternate_bank_predicate
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
ranges={0:[(0x2d2,0x2e0),(0x457,0x478)],3:[(0x2e0,0x2ef),(0x478,0x490)],6:[(0x308,0x31b),(0x4a2,0x4af)]}
ids=sorted({0,0xffffffff,*[n for pairs in ranges.values() for lo,hi in pairs for n in (lo-1,lo,lo+1,hi-1,hi,hi+1)]})
cases=0
for identity,character,enabled,bank in itertools.product(ids,[-1,0,1,3,6,7],[0,1],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c0956f0),character&0xffffffff)
 struct.pack_into('<I',ram,lit(0x8c0956f4),enabled)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append(a);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=identity;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c095612
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not calls
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 signed=ctypes.c_int32(identity).value
 assert r[0]==int(bool(enabled) and any(lo<signed<hi for lo,hi in ranges.get(character,[])))
 cases+=1
(out/'analysis/sound_alternate_bank_predicate_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='86 retail instructions; full native/reference RAM/CPU/FPU/SR/PR equality plus independent strict open range predicate across all six interval endpoints and adjacent values, character-1/0/1/3/6/7, enabled0/1, both FPU banks. No external services.'),indent=2)+'\n')
print(cases,'alternate bank predicate comparisons passed')
