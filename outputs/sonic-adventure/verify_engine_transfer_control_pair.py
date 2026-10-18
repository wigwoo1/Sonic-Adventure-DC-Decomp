import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for stem,entry,lower,flag in [('engine_transfer_control_restore',0x8c10ee20,0x8c10f9bc,0),('engine_transfer_control_install',0x8c10ee9c,0x8c10fa60,1)]:
 fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);fn.argtypes=abi
 for initial,arg,bank,answer in itertools.product([0,1,0x80000000,0xffffffff],[0,1,0x8c10eee0,0xffffffff],[0,0x200000],[0,0xffffffff]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca08000);struct.pack_into('<3I',ram,0x7ac8bc,0x12345678,initial,0x87654321);ram[0xa08000:0xa08100]=bytes([0x55])*256
  before=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
  @Call
  def call(ctx,a,m,f,sr,r,pr):
   assert a==lower;actual.append((a,r[4],r[5]));r[0]=answer;return 0
  def hook(mem,r,sr):expected.append((lower,r[4],r[5]));r[0]=answer;return sr
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[5]=arg;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x70000063
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  reference(ram,s,{lower:hook},fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert actual==expected==[(lower,0x8ca08004+i*0x34,arg) for i in range(4)]
  assert struct.unpack_from('<3I',native,0x7ac8bc)==(0x12345678,flag,0x87654321) and native[0xa08000:0xa08100]==before[0xa08000:0xa08100] and r[0]==0 and r[15]==0x8cf00000
  cases+=1
scope='128 full retail RAM/CPU/FPU/SR/PR comparisons of transfer control restore10ee20/install10ee9c. Four initial flags,four forwarded R5 values,both banks,two lower returns. Independent four calls to10f9bc or10fa60 at context+4+i*34,unchanged R5,flag0/1 at7ac8c0,neighbor guards,256-byte context preservation,ignored lower returns,status0 and stack restoration. Lower channel-control functions modeled;physical hardware/full game integration unverified.'
(p/'analysis/engine_transfer_control_pair_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'transfer control comparisons passed')
