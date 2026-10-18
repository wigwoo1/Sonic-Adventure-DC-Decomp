import ctypes,itertools,json,struct,hashlib
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_interrupt_constructor import IO,Read,Write
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_load_initialize.dll')).sa_sound_driver_load_initialize;fn.argtypes=abi
objects=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
helpers={}
for a,name in [(0x8c6435bc,'sa_engine_frame_object_release'),(0x8c643624,'sa_engine_frame_object_configure'),(0x8c65a568,'sa_engine_object_state_get'),(0x8c65a5cc,'sa_engine_object_state_set'),(0x8c65a5ac,'sa_engine_object_address_set'),(0x8c65a5d4,'sa_engine_object_length_set'),(0x8c65a5b4,'sa_engine_object_callback_set'),(0x8c65acac,'sa_engine_error_record')]:
 helpers[a]=getattr(objects,name);helpers[a].argtypes=abi
callback=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_callback_register.dll')).sa_setup_callback_register;callback.argtypes=abi;helpers[0x8c605234]=callback
constructor=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_constructor.dll')).sa_interrupt_callback_create;constructor.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
handler=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/task_delete_native.dll')).sa_interrupt_handler_primary_native;handler.argtypes=abi;helpers[0x8c64135a]=handler
removal=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_remove.dll')).sa_interrupt_callback_remove;removal.argtypes=constructor.argtypes
pool_count_offset=struct.unpack_from('<h',binary,0x64360c-0x10000)[0]
cases=0
for failure,size,bank,exhausted,prior in itertools.product(['none','initialize','allocate','configure'],[1,2048,2049],[0,0x200000],[0,1],[0,2]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,0x8cb08000)
 ram[0xb10000:0xb10040]=bytes([0x99])*64;struct.pack_into('<2I',ram,0xb10000,1,2 if failure=='configure' else 1)
 struct.pack_into('<I',ram,0xb08000+pool_count_offset,1)
 struct.pack_into('<I',ram,0x8900fc,prior)
 struct.pack_into('<I',ram,0x67c004,0x8ca00000)
 for i in range(11):struct.pack_into('<2I',ram,0xa00230+8*i,0xdead0000+i,0xbeef0000+i)
 ram[0x892840:0x894040]=bytes(0x1800)
 for i in range(256):struct.pack_into('<I',ram,0x892840+16*i+12,0x80000000)
 if not exhausted:struct.pack_into('<I',ram,0x89285c,0)
 if prior:
  struct.pack_into('<4I',ram,0x892860,0x8c010000,0,0xabcdef01,0x80000020)
  struct.pack_into('<I',ram,0x893a80,0x8c892860)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(t,m,r,events):
  events.append((t,tuple(r[i] for i in range(4,8))))
  if t==0x8c6016d6:r[0]=0x8cb00000
  elif t==0x8c600e2c:
   a=r[5]&0xffffff
   for i in range(4):m[a+i]=(size>>(8*i))&255
   r[0]=0
  elif t==0x8c6434e0:
   a=r[4]&0xffffff;v=0 if failure=='allocate' else 0x8cb10000
   for i in range(4):m[a+i]=(v>>(8*i))&255
   r[0]=3 if failure=='allocate' else 0
  elif t==0x8c64594c:r[0]=0x8cb30000
  else:r[0]=3 if (t==0x8c643f6c and failure=='initialize') or (t==0x8c643624 and failure=='configure') else 0
 targets=[0x8c6016d6,0x8c600e2c,0x8c600f1e,0x8c600df4,0x8c643f6c,0x8c6436d0,0x8c6434e0,0x8c643624,0x8c643e6c,0x8c6435bc,0x8c605234,0x8c64594c,0x8c64135a,0x8c64137c,0x8c645c54]
 hooks={}
 for t in targets:
  if t in helpers or t in (0x8c64594c,0x8c645c54):continue
  def hook(m,r,sr,t=t):service(t,m,r,expected);return sr
  hooks[t]=hook
 io=IO(0x8ca08000,0,None,Read(),Write())
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c645c54:return removal(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if t==0x8c64594c:
   if sum(m[0x8900fc+i]<<(8*i) for i in range(4))!=0:errors.append("handle not cleared")
   return constructor(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if t in helpers:return helpers[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t not in targets:errors.append(t);return -9
  service(t,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c093e60;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,hooks=hooks,fpu=f,vbr=io.vbr)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 ts=[e[0] for e in actual];wanted=targets[:5]
 if failure!='initialize':
  wanted+=targets[5:7]
  if failure!='allocate':
   if failure=='none':wanted+=[targets[8]]

 wanted+=[targets[8]]
 # Handler installation now executes recovered code without a modeled event.
 assert ts==wanted
 assert actual[0][1][:2]==(0x8c159d60,0)
 assert actual[2][1][:3]==(0x8cb00000,(size+2047)>>11,0x0cd00000)
 assert actual[4][1][:3]==(0,0,0)
 created=2 if exhausted and prior else 0 if exhausted else 1
 assert struct.unpack_from('<I',native,0x8900fc)[0]==created
 if created:
  assert struct.unpack_from('<2I',native,0xa00238)==(0x8c645b1e,0)
 else:
  assert struct.unpack_from('<2I',native,0xa00238)==(0xdead0001,0xbeef0001)
 if created:
  assert struct.unpack_from('<4I',native,0x892840+16*created)==(0x8c051810,0,0,0x80000058)
  assert struct.unpack_from('<I',native,0x893a80)[0]==0x8c892840+16*created
 if prior and created!=2:assert struct.unpack_from('<I',native,0x89286c)[0]==0
 if failure in ('none','configure'):
  if failure=='none':
   assert struct.unpack_from('<5I',native,0xb10004)==(0,0x0cd00000,0,0xffffffff,0)
   assert native[0xb10018:0xb10040]==bytes([0x99])*40
  else:
   initial=bytearray([0x99])*64;struct.pack_into('<2I',initial,0,0,2)
   assert native[0xb10000:0xb10040]==initial
   assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c643624,0x7fffffff)
 assert struct.unpack_from('<I',native,0xb10000)[0]==(0 if failure in ('none','configure') else 1)
 assert struct.unpack_from('<I',native,0xb08000+pool_count_offset)[0]==(0 if failure in ('none','configure') else 1)
 assert r[15]==0x8cf00000 and r[14]==0x2234000e
 cases+=1
report={'cases_passed':cases,'instructions_recovered':81,'scope':'Original-instruction RAM/CPU/SR/PR/FPU/event equality for initializer/allocation/configuration failures and success, three file sizes, both FPU banks; independent call order, file open/sector rounding/driver destination, initialization args, final callback registration, preserved stack/R14. Object configure643624/release6435bc with state getter/setters/error recorder native; independent allocation flag and pool-count decrement checked; Callback register605234 native; Native constructor64594c connected; native removal645c54 connected; prior handle2 absent/present, freed-slot reuse under exhaustion, handle1/2/0, callback node and list head checked; low handler install64135a/set/reset cluster native; callback-table index1 and exhausted-table preservation checked; other subordinate routines modeled; actual driver loading/startup ordering unverified.'}
report['binary_sha256']=hashlib.sha256(binary).hexdigest()
report['test_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
report['dll_sha256']={name:hashlib.sha256((ROOT/'work/sonic-adventure/build'/name).read_bytes()).hexdigest() for name in ['sound_driver_load_initialize.dll','engine_frame_object_configure.dll','sound_driver_callback_register.dll','interrupt_constructor.dll','interrupt_remove.dll','task_delete_native.dll']}
(out/'analysis/sound_driver_load_initialize_native_replace_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'sound driver initialization comparisons passed')



