import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for a,stem in [(0x8c10f9bc,'engine_channel_control_restore'),(0x8c10fa60,'engine_channel_control_install'),(0x8c1101f0,'engine_channel_status_get')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helper.argtypes=abi;helpers[a]=helper
cases=0
for stem,entry,lower,flag in [('engine_transfer_control_restore',0x8c10ee20,0x8c10f9bc,0),('engine_transfer_control_install',0x8c10ee9c,0x8c10fa60,1)]:
 fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);fn.argtypes=abi
 for initial,arg,bank,mask in itertools.product([0,1,0x80000000,0xffffffff],[0,0xffffffff],[0,0x200000],[0,15,1,2,4,8,5,10]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca08000);struct.pack_into('<3I',ram,0x7ac8bc,0x12345678,initial,0x87654321);ram[0xa08000:0xa08100]=bytes([0x55])*256
  struct.pack_into('<I',ram,0xa08000,0x8ca10000);ram[0xa10000:0xa10080]=bytes([0x66])*128
  for i,index in enumerate([3,1,0,2]):
   struct.pack_into('<I',ram,0xa08004+i*0x34,index);struct.pack_into('<I',ram,0xa10018+index*32,0x80000000|((mask>>i)&1))
  before=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
  @Call
  def call(ctx,a,m,f,sr,r,pr):
   assert a in helpers
   if a==lower:actual.append((a,r[4],r[5]))
   return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[5]=arg;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x70000063
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  reference(ram,s,{},fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert actual==[(lower,0x8ca08004+i*0x34,arg if i==0 else 0x8cefffe0) for i in range(4)]
  context=bytearray(before[0xa08000:0xa08100]);table=bytearray(before[0xa10000:0xa10080])
  for i,index in enumerate([3,1,0,2]):
   if mask&(1<<i):
    struct.pack_into('<I',table,index*32+28,flag)
    if flag:struct.pack_into('<I',context,4+i*0x34+24,2)
  assert native[0xa08000:0xa08100]==context and native[0xa10000:0xa10080]==table
  assert struct.unpack_from('<3I',native,0x7ac8bc)==(0x12345678,flag,0x87654321) and r[0]==0 and r[15]==0x8cf00000
  cases+=1
scope='256 full retail RAM/CPU/FPU/SR/PR comparisons of transfer restore10ee20/install10ee9c connected through native channel controls10f9bc/10fa60 and status query1101f0. Eight four-channel readiness masks,permuted channel indices,four initial global flags,two initial R5 values,both banks. Independent four channel calls and native R5 clobbering,only ready-channel table flag writes,install channel state2,global flag0/1,256-byte context/128-byte table guards,status0 and stack restoration. No modeled calls. Physical hardware/full game integration unverified.'
(p/'analysis/engine_transfer_control_native_channels_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'transfer control comparisons passed')
