import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_lookup.dll'));fn=lib.sa_sound_bank_section_lookup
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
@Call
def call(*args):return -9
kinds=[struct.unpack_from('<I',binary,a-0x8c010000)[0] for a in range(0x8c65a9bc,0x8c65a9d8,4)]+[0xdeadbeef]
cases=0
for kind,index,context,pointers in itertools.product(kinds,[0,1,15,16,255,256],[0,1],[0,1,2,3]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,0x8ca00000 if context else 0)
 ram[0xa00000:0xa01000]=bytes((i*13+7)&255 for i in range(4096))
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=kind;s.r[5]=index;s.r[6]=0x8cb00000 if pointers&1 else 0;s.r[7]=0x8cb00004 if pointers&2 else 0;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65a900
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'],(hex(kind),index,context,pointers)
 cases+=1
(root/'analysis/sound_bank_section_lookup_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Full RAM/CPU/FPU/SR/PR/MACL comparison against retail instructions; seven types plus unknown, signed-byte index boundaries and truncation, null context, all output pointer presence combinations. Fixture tables; integrated bank loading remains pending.'},indent=2)+'\n')
print(cases,'bank section lookup comparisons passed')
