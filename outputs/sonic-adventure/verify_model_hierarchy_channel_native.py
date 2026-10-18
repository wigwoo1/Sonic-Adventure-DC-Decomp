import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);P=ctypes.POINTER(ctypes.c_uint32)
 CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P)
 dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_channel_native.dll'));dispatch=dll.sa_model_hierarchy_channel_dispatch;dispatch.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P];fallback=dll.sa_model_hierarchy_channel_native;VC=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu));fallback.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,ctypes.c_uint,ctypes.c_void_p,VC,CB,P];counts=[0,0]
 for bank,sr0,mode,channel,flags in itertools.product(range(2),[0,0x700083f3],[2,3],range(4),range(8)):
  f=Fpu();f.fpscr=0x40000|(bank<<21);f.fpul=0xabcdef12;rng=random.Random(counts[1])
  for k in range(2):f.banks[k][:]=[struct.unpack("<I",struct.pack("<f",rng.randrange(-16,17)/4))[0] for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[13]=0x8ca04000;state.r[14]=0x8ca00100;state.r[15]=0x8cefffc0;state.r[9]=channel;state.r[10]=2;state.r[11]=0x8c639cf4;state.pr=0x8c000100;state.sr=sr0;state.pc=0x8c057c44 if mode==2 else 0x8c057e30;r=(ctypes.c_uint32*16)(*state.r)
  for m in (old,native):struct.pack_into('<I',m,0xa04000,flags);struct.pack_into('<3I',m,0xa04014,11,22,33);m[0xefffc0:0xefffc4]=bytes([0x55])*4;struct.pack_into('<3f',m,0xa04008,2,-3,4);struct.pack_into('<3f',m,0xa04020,0.5,-2,4)
  expected=[];actual=[];end=0x8c057dfe if mode==2 else 0x8c05808a
  def stop(m,regs,s):state.pr=0x8c000100;return s
  def hook(target):
   def apply(m,regs,s):expected.append((target,state.pr,[regs[i] for i in range(16)],s,bytes(f)));regs[0]=0x13579bdf;return s
   return apply
  ev=[];av=[]
  def vec(word,fp,log):
   log.append((word,bytes(fp)));active=(fp.fpscr>>21)&1;fp.banks[active][4:8]=[0x3f800000,0x40000000,0x40400000,0x3f800000]
  def original_vec(word,fp):vec(word,fp,ev)
  @VC
  def native_vec(ctx,word,fp):vec(word,fp.contents,av);return 0
  reference(old,state,{end:stop,0x8c639cf4:hook(0x8c639cf4)},f,vector_instruction=original_vec)
  @CB
  def call(ctx,target,pr,m,fp,srp,regs):actual.append((target,pr,[regs[i] for i in range(16)],srp[0],bytes(fp.contents)));regs[0]=0x13579bdf;return 0
  sr=ctypes.c_uint32(sr0);nextpc=ctypes.c_uint32();assert fallback(view,ctypes.byref(nf),ctypes.byref(sr),r,mode,None,native_vec,call,ctypes.byref(nextpc))==0
  assert ev==av and actual==expected and list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and old[0xefffc0:0xefffc4]==native[0xefffc0:0xefffc4] and nextpc.value==end,(mode,channel,flags)
  counts[1]+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_channel_native_verification.json').write_text(json.dumps(dict(cases_passed=counts[1],source_sha256=hashlib.sha256((out/'source/model_hierarchy_channel_native.c').read_bytes()).hexdigest(),scope='Null-channel fallback integrated with native scale and translation; rotation remains service and FTRV results injected. Full integer/FPU/SR/scratch and vector/rotation input ABI compared. Interpolated channels and complete generator pending.'),indent=2)+'\n');print(counts[1],'native hierarchy fallback comparisons passed')
if __name__=='__main__':main()
