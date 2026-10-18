import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_irq_callback.dll'))
fn=lib.sa_sound_driver_irq_callback;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
sound_service=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_service.dll')).sa_sound_driver_service;sound_service.argtypes=abi
cases=0
for countdown,mode,packed,bank,enabled,pair in itertools.product([0,1,2],[0,4,5,9],[0,0x102,0x203],[0,0x200000],[0,1],[(1,2),(2,1)]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x754ec0,countdown);struct.pack_into('<I',ram,0x760970,mode)
 struct.pack_into('<I',ram,0x67c1d0,0x8cb08000);struct.pack_into('<I',ram,0x67c1d4,enabled)
 struct.pack_into('<I',ram,0xb0816c,1);struct.pack_into('<I',ram,0xb081ac,0x8c7ac7a4)
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 row=packed>>8;column=packed&255;table_target=0x8cb70000
 struct.pack_into('<I',ram,0x19e3e4+row*32+column*4,table_target)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[];errors=[]
 targets=[0x8c04f7e0,table_target,0x8c643f98,0x8c602cb8,0x8c09ce40,0x8c65a378,0x8c10d428,0x8c10d444]
 def service(t,m,r,events):
  events.append(t)
  if t in targets[6:]:
   v=pair[0] if t==targets[6] else pair[1]
   for i in range(4):m[(r[5]&0xffffff)+i]=(v>>(8*i))&255
  r[0]=packed if t==targets[0] else 0
 hooks={}
 for t in targets:
  if t==0x8c643f98:continue
  def hook(m,r,sr,t=t):service(t,m,r,expected);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c643f98:return sound_service(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t not in targets:errors.append(t);return -9
  service(t,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c051810
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,hooks=hooks,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 wanted=[]
 if countdown==1:
  wanted=[targets[0]]
  if mode in [4,5,9]:wanted+=[table_target]
  if enabled:wanted+=targets[5:]
 wanted+=targets[3:5]
 assert actual==wanted and struct.unpack_from('<I',native,0x754ec0)[0]==(countdown-1)&0xffffffff
 failed=countdown==1 and enabled and pair[1]<pair[0]
 assert struct.unpack_from('<2I',native,0x67c1c8)==((0x8c643f98,0x7fffffff) if failed else (0xabcdef01,0x12345678))
 assert r[15]==0x8cf00000
 cases+=1
report={'cases_passed':cases,'instructions_recovered':55,'scope':'Original RAM/CPU/SR/PR/FPU/service order equality across countdown0/1/2, modes0/4/5/9, three packed table indices and both FPU banks. Countdown wrap, exact conditional service/table dispatch and final tail call independently checked. Native sound-service643f98/error recorder connected, enable0/1 and status ordering pairs tested; request processor65a378/status getters and other callback services modeled. Independent error globals. Queue/interrupt delivery scheduling not yet connected.'}
(out/'analysis/sound_driver_irq_callback_service_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'driver IRQ callback comparisons passed')
