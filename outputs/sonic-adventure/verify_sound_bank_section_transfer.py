import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_transfer.dll'));fn=lib.sa_sound_bank_section_transfer
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lookup_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_lookup.dll'));lookup=lookup_lib.sa_sound_bank_section_lookup;lookup.argtypes=fn.argtypes
support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'));error=support.sa_engine_error_record;error.argtypes=fn.argtypes
kinds=[struct.unpack_from('<I',binary,a-0x8c010000)[0] for a in range(0x8c65a9bc,0x8c65a9d8,4)]+[0xdeadbeef]
cases=0
for kind,index,context,length,address in itertools.product(kinds,[0,15,16,255],[0,1],[0,16,0xffffffff],[0,0x1200]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,0x8ca00000 if context else 0)
 for offset in [0x1e4,0x264,0x2e4,0x364,0x3e4,0x464,0x46c]:
  for slot in range(16 if offset<0x3e4 else 1):struct.pack_into('<II',ram,0xa00000+offset+slot*8,address,length)
 native=bytearray(ram);native_submissions=[];reference_submissions=[]
 def hook(m,regs,status):reference_submissions.append((regs[4],regs[5]));regs[0]=0;return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c65a5dc:native_submissions.append((regs[4],regs[5]));regs[0]=0;return 0
  helper={0x8c65a900:lookup,0x8c65acac:error}.get(target)
  if helper is None:return -9
  return helper(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8cb01000;s.r[5]=kind;s.r[6]=index;s.r[7]=0x12345678;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c643904
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,special=special,hooks={0x8c65a5dc:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'],(hex(kind),index,context,length,address)
 assert native_submissions==reference_submissions
 signed=(index&255)-256 if index&128 else index&255
 expected=context and kind in kinds[:7] and 0<=signed<=(15 if kind in kinds[:4] else 0) and address!=0 and length==16
 assert native_submissions==([(0x8cb01000,address)] if expected else []),(hex(kind),index,context,length,address)
 cases+=1
(root/'analysis/sound_bank_section_transfer_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native transfer wrapper, lookup and error helper; full RAM/CPU/FPU/SR/PR/MACL equality against retail and ordered queue consumer arguments. Queue consumer replaced by recording hook; no actual queue transfer or audio. Types, invalid indices, null context, zero/positive/negative lengths and zero/nonzero addresses.'},indent=2)+'\n')
print(cases,'bank section transfer comparisons passed (queue recorded)')
