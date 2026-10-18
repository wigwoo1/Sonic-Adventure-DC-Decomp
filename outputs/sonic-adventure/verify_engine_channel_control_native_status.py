import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
query=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_channel_status_get.dll')).sa_engine_channel_status_get;query.argtypes=abi
cases=0
for stem,entry,flag in [('engine_channel_control_restore',0x8c10f9bc,0),('engine_channel_control_install',0x8c10fa60,1)]:
 fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);fn.argtypes=abi
 for index,status,bank,answer in itertools.product([0,1,3,0xffffffff],[0,1,2,0xffffffff],[0,0x200000],[0,0xffffffff]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  ram[0xa08000:0xa08040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa08000,index)
  struct.pack_into('<I',ram,0x5fbb14,0x8ca09000);struct.pack_into('<I',ram,0xa09000,0x8ca10000);ram[0xa0ffe0:0xa100a0]=bytes([0x66])*192
  struct.pack_into('<I',ram,((0x8ca10000+index*32+24)&0xffffff),status)
  struct.pack_into('<I',ram,((0x8ca10000+index*32+28)&0xffffff),answer)
  before=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
  @Call
  def call(ctx,a,m,f,sr,r,pr):
   assert a==0x8c1101f0;actual.append((r[4],r[5]));return query(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  def hook(mem,r,sr):expected.append((r[4],r[5]));struct.pack_into('<I',mem,r[5]&0xffffff,status);r[0]=answer;return sr
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x70000063
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  reference(ram,s,{},fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert actual==[(0x8ca08000,0x8ceffff4)]
  channel=bytearray(before[0xa08000:0xa08040]);table=bytearray(before[0xa0ffe0:0xa100a0])
  if status&1:
   address=(0x8ca10000+index*32+28)&0xffffffff;struct.pack_into('<I',table,address-0x8ca0ffe0,flag)
   if flag:struct.pack_into('<I',channel,24,2)
  assert native[0xa08000:0xa08040]==channel and native[0xa0ffe0:0xa100a0]==table and r[0]==0 and r[15]==0x8cf00000
  cases+=1
scope='128 full retail RAM/CPU/FPU/SR/PR comparisons of channel control restore10f9bc/install10fa60. Four channel indices including unsigned address wrap,four hardware-table status words,both banks,two initial control-word values. Independent channel+stack status arguments,only bit0 set writes hardware-table flag at32*index+1c,install also writes channel+18=2,64-byte channel and192-byte table guards,status0 and stack restoration. Native status query1101f0 connected;independent table address32*index+18 and bit0 output. No modeled calls;physical hardware/full game integration unverified.'
(p/'analysis/engine_channel_control_native_status_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'channel control comparisons passed')
