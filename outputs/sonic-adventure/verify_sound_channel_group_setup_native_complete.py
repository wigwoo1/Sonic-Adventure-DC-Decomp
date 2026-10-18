import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_channel_group_setup.dll')).sa_sound_channel_group_setup;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
slot=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_slot_setup.dll')).sa_sound_slot_acquire;slot.argtypes=abi
debug=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_frame_debug_log;debug.argtypes=abi
control=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_group_control_set.dll')).sa_sound_group_control_set;control.argtypes=abi
helpers={}
for address,dllname,name in [(0x8c66b374,'sound_pan_request','sa_sound_pan_request'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
for address,dllname,name in [(0x8c66b5ac,'sound_channel_count','sa_sound_channel_count'),(0x8c10ca5c,'sound_volume_dispatch','sa_signed_division')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
cases=0
for present,count,channel,free,bank,sr0 in itertools.product([False,True],[0,1,2,3],[0,15,16,255],[0,3,4],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,0x8ca08000 if present else 0)
 struct.pack_into('<I',ram,0xa10000,0x8ca11000);ram[0xa11001]=1
 for i in range(4):struct.pack_into('<2I',ram,0xa08088+8*i,3|(i<<8),0 if i==free else 1)
 ram[0xa080a8:0xa080a8+176]=bytes([0x55])*176
 struct.pack_into('<3I',ram,0xf00000,count,channel,7)
 ring=0xa08000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca40000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca30000,256,0,0,0)
 packet=struct.unpack_from('<I',binary,0x66b478-0x10000)[0]&0xffffff;template=bytes(ram[packet:packet+20])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 def effect(t,r):
  trace=(t,r[4],r[5]);r[0]=456;return trace
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c65aa30:return slot(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c65a2fc:return debug(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66b1a0:return control(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  raise AssertionError(hex(a))
 def hook(t):
  def body(mem,r,sr):expected.append(effect(t,r));return sr
  return body
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c669284
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 code=0x0d000001 if not present else 0x03000001 if count not in (1,2) or channel>15 else 0x07000101 if free==4 else 0
 assert r[0]==code and actual==expected
 record=0xa080a8+44*free
 if not code:
  assert struct.unpack_from('<I',native,0xa10000)[0]==0x8ca08088+8*free
  assert struct.unpack_from('<I',native,0xa0808c+8*free)[0]==1
  assert native[record+5]==count
  assert struct.unpack_from('<3I',native,ring+20)==(count,count,0)
  command=bytearray(template);bias=struct.unpack_from('<h',binary,0x66b468-0x10000)[0]
  channel_table=struct.unpack_from('<I',binary,0x66b480-0x10000)[0]&0xffffff
  for i in range(count):
   struct.pack_into('<I',command,0,0x26);command[4]=ram[channel_table+i]|free
   pan=ctypes.c_int8(0x55).value
   if count==2:
    pan=pan+bias if i==0 else min(pan-bias,-2)
    pan=(abs(pan)//2)*(-1 if pan<0 else 1)
   pan+=bias
   for j in range(6):command[5+j+j//3]=0
   command[5+4*i]=pan&255
   assert native[0xa30000+28*i:0xa30014+28*i]==command
   assert struct.unpack_from('<I',native,0xa30014+28*i)[0]==1
  assert native[record+6]==255
  assert struct.unpack_from('<I',native,record)[0]==1 and native[record+4]==count
  assert native[record+7:record+7+count]==bytes([channel,7][:count])
  for i in range(count):
   assert struct.unpack_from('<I',native,record+12+4*i)[0]==1
   assert struct.unpack_from('<I',native,record+20+4*i)[0]==0
   assert struct.unpack_from('<I',native,record+36+4*i)[0]==0
 else:
  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66aec8,code)
  assert native[0xa080a8:0xa080a8+176]==bytes([0x55])*176
 cases+=1
(out/'analysis/sound_channel_group_setup_native_complete_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native channel-group setup with native type3 slot allocator/debug/error recorder.384 full retail RAM/CPU/FPU/SR/PR comparisons. First/last/free exhaustion slots, counts0/1/2/3,channels0/15/16/255,banks/SR. Independent allocated handle/flag, descriptor selected by slot tag, record and error fields. Native control-byte setter66b1a0 added, byte6=FF independently checked; native channel-count/pan request/packet commit/copy and signed division added; no modeled subordinate calls. Request ring is a fixture; consumption and real audio unverified; full game unverified.'),indent=2)+'\n')
print(cases,'channel group comparisons passed')
