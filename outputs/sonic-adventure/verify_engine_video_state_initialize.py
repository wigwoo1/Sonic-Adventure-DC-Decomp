import ctypes,json,struct,time
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_video_state_initialize.dll')).sa_engine_video_state_initialize
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
identity=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_video_state_initialize.dll')).sa_engine_video_state_identity
identity.argtypes=fn.argtypes[:7]+fn.argtypes[8:]
ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
mmio={0xa05f689c:0};events=[];actual=[];counts=[0,0];errors=[]
def service(target,m,r,log,which):
 if target==0x8c65253e:
  index=counts[which];assert r[4]==0xa5000000+4*index and r[6]==4
  assert struct.unpack('<I',bytes(m[(r[5]&0xffffff):(r[5]&0xffffff)+4]))[0]==0
  counts[which]+=1;r[0]=0;return
 log.append((target,tuple(r[i] for i in range(4,8))))
 if target==0x8c653476:r[0]=0x17fd11db if r[4]==0 else 0x11
 elif target==0x8c653444:r[0]=0
 else:raise AssertionError(hex(target))
hooks={}
for target in [0x8c653476,0x8c653444,0x8c65253e]:
 def hook(m,r,sr,target=target):service(target,m,r,events,0);return sr
 hooks[target]=hook
@Read
def read(ctx,a,n):
 if a!=0xa05f689c or n!=4:errors.append((a,n));return 0
 return 0
@Write
def write(ctx,a,v,n):errors.append(('write',a,v,n))
@Call
def call(ctx,target,m,f,sr,r,pr):
 if target==0x8c6513ee:return identity(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 if target not in hooks:errors.append(target);return -9
 service(target,m,r,actual,1);return 0
io=IO(None,read,write);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c6656e2;s.pr=0x8c000100;s.sr=0x70000063
r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();nf=Fpu.from_buffer_copy(f)
reference(ram,s,fpu=f,hooks=hooks,mmio=mmio)
assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0 and not errors,errors
assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
assert counts==[0x200000,0x200000] and r[0]==1
assert struct.unpack_from('<I',native,0x8a2d58)[0]==6
assert struct.unpack_from('<I',native,0x8a3060)[0]==0x800408
assert struct.unpack_from('<I',native,0x8a2d5c)[0]==0
report={'cases_passed':1,'instructions_recovered':234,'video_clear_word_count':counts[1],'scope':'Full retail CPU/RAM/FPU/SR/PR and register-service event equality for hardware ID17fd11db revision17 and bus mode0. All2097152 four-byte clear requests independently check sequential destinations a5000000..a57ffffc and zero payload. Register reads/writes and VRAM service modeled; no real rendering. Other bus modes and unsupported-ID halt branches unverified.'}
(root/'analysis/engine_video_state_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print('Full video initializer comparison passed;2097152 clear requests verified')
