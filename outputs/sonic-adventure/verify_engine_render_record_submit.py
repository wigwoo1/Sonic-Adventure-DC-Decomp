import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_render_record_submit.dll')).sa_engine_render_record_submit;fn.argtypes=abi;cases=0
for count,flag,result,bank in itertools.product([0,1,2,3],[0,1],[0,3],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x67c1d0,0x8cb08000)
 ram[0xb08000:0xb08400]=bytes([0xa5])*1024
 for offset,value in [(344,0x100000+flag),(348,count),(376,0x8cb10000),(452,0x8cb02000)]:struct.pack_into('<I',ram,0xb08000+offset,value)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(target,m,r,events):
  entry=(target,tuple(r[i] for i in range(4,8)))
  if target==0x8c643624:entry+=(struct.unpack('<I',bytes(m[(r[15]&0xffffff):(r[15]&0xffffff)+4]))[0],)
  elif target!=0x8c65a5dc:raise AssertionError(hex(target))
  events.append(entry);r[0]=result
 hooks={}
 for target in [0x8c643624,0x8c65a5dc]:
  def hook(m,r,sr,target=target):service(target,m,r,expected);return sr
  hooks[target]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  try:service(t,m,r,actual)
  except Exception as exc:errors.append(repr(exc));return -9
  return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c666a68;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,hooks=hooks,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
 assert r[0]==0 and r[15]==0x8cf00000
 if count:
  adjusted=count+(count&1)
  assert struct.unpack_from('<II',native,0xb08000+348)==(adjusted,1)
  assert actual[0]==(0x8c643624,(0x8cb10000,0x100000+flag,adjusted*16,0x8c666ad0),0x8cb08158)
  assert actual[1][0]==0x8c65a5dc and actual[1][1][:2]==(0x8cb10000,0x8cb02000)
 else:assert not actual and native[0xb08000:0xb08400]==ram[0xb08000:0xb08400]
 cases+=1
report={'cases_passed':cases,'instructions_recovered':52,'scope':'Full retail RAM/CPU/FPU/SR/PR/MACL/event equality for counts0/1/2/3, source low bit0/1, service returns0/3, both FPU banks. Independent active flag, count rounded up to an even value, 16-byte transfer size, completion callback/context and queue arguments; return0 preserved. Object configure643624 and queue65a5dc modeled; real completion/device use unverified.'}
(root/'analysis/engine_render_record_submit_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'record submission comparisons passed')
