import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_create.dll')).sa_sound_source_create;fn.argtypes=abi
reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_reset.dll')).sa_sound_source_reset;reset.argtypes=abi
cases=0;pool=0x8a65b8
for slot,size,bank in itertools.product(range(65),[0,1,2,127,128,255,0xffffffff],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[pool-8:pool+3080]=bytes([0x55])*3088
 for i in range(64):struct.pack_into('<I',ram,pool+i*48+4,0 if i>=slot else [1,2,128,0xffffffff][i%4])
 original=bytes(ram[pool:pool+3072]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca40000,size,16];s.r[15]=0x8cf00000;s.pc=0x8c65e3c6;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  assert a==0x8c65e454,hex(a)
  return reset(m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 wanted=bytearray(original);selected=0x8c000000|pool+slot*48
 if slot<64:
  offset=slot*48
  for off,value in [(0,0x8c67c234),(4,1),(8,0x8c672434),(0xc,0),(0x10,size),(0x14,0),(0x18,0),(0x1c,0x8ca40000),(0x20,size),(0x24,16),(0x28,0x8c65e398),(0x2c,selected)]:struct.pack_into('<I',wanted,offset+off,value)
 assert native[pool:pool+3072]==wanted and r[0]==(selected if slot<64 else 0)
 assert native[pool-8:pool]==bytes([0x55])*8 and native[pool+3072:pool+3080]==bytes([0x55])*8 and r[15]==0x8cf00000
 cases+=1
(p/'analysis/sound_source_create_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='910 full retail RAM/CPU/FPU/SR/PR comparisons of source constructor65e3c6..65e42c with native reset65e454, no modeled calls. All64 candidate slots and full pool,signed/noncanonical nonzero occupied flags,seven ring-size patterns,both banks. Independent complete3072-byte pool and8-byte guards,first-free selection,returned pointer or null,all12 constructed fields including method table8c67c234,type identity8c672434,buffer/ring/mirror,default handler65e398/self userdata and initial counter/cursors. Other slots preserved. Buffer contents not touched. Pool initialization/source producer/refill/lifecycle integration/fullgame remain unverified.'),indent=2)+'\n')
print(cases,'native source constructor comparisons passed')
