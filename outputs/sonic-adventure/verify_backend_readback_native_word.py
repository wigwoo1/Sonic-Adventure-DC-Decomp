import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_readback.dll')).sa_sound_backend_readback;fn.argtypes=abi
diagnostic=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_error_record.dll')).sa_engine_error_record;diagnostic.argtypes=abi
specs=[(0x8c66ba2c,0x14),(0x8c6691d0,0x18),(0x8c66bdd4,2),(0x8c66bb4c,3),(0x8c66bc24,6),(0x8c66bcb4,8),(0x8c66babc,4),(0x8c66b9b0,5),(0x8c66accc,0x10),(0x8c66bd44,0xc)]
query=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_descriptor_byte_query.dll')).sa_sound_descriptor_byte_query;query.argtypes=abi
byte_read=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_byte_read.dll')).sa_sound_byte_read;byte_read.argtypes=abi
word_read=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/device_word_read.dll')).sa_device_word_read;word_read.argtypes=abi
cases=0
for (enabled,present),value,bank,answer in itertools.product([(1,1)],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,0xffffffff]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08090]=bytes([0x55])*144;ram[0xa20000:0xa20028]=bytes([0x66])*40
 struct.pack_into('<I',ram,0x67c1d0,0x8ca09000 if enabled else 0);struct.pack_into('<2I',ram,0x67c1c8,0x12345678,0x9abcdef0)
 struct.pack_into('<I',ram,0xa091b8,0x8ca30000)
 before=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];reads=[]
 def service(a,m,r):
  i=next(i for i,x in enumerate(specs) if x[0]==a);m[r[6]&0xffffff]=(value+i)&255;r[0]=answer
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c6671c4:return byte_read(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66910c:return word_read(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10d460:
   assert r[6]==4 and r[7]==1 and all(m[(r[15]&0xffffff)+i]==0 for i in range(8))
   reads.append(r[4])
   for i in range(4):m[(r[5]&0xffffff)+i]=(value+i)&255
   r[0]=answer;return 0
  assert a in dict(specs);calls.append((a,r[4],r[5],r[6]))
  if a==0x8c6691d0:return query(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  service(a,m,r);return 0
 def read_hook(m,r,sr):
  for i in range(4):m[(r[5]&0xffffff)+i]=(value+i)&255
  r[0]=answer;return sr
 def hook(a):
  def run(m,r,sr):service(a,m,r);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000 if present else 0;s.r[5]=value;s.r[6]=0x8ca20004;s.r[15]=0x8cf00000;s.pc=0x8c66adc4;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{**{a:hook(a) for a,o in specs if a!=0x8c6691d0},0x8c10d460:read_hook},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert calls==[(a,0x8ca08000,value,0x8ca20004+o) for a,o in specs]
 output=bytearray(before[0xa20000:0xa20028]);output[4:6]=bytes(2)
 for i,(a,o) in enumerate(specs):
  if a==0x8c6691d0:struct.pack_into('<I',output,4+o,value&255)
  else:output[4+o]=(value+i)&255
 signed_selector=ctypes.c_int8(value&255).value
 assert reads==[(0xa0800000+0x8ca30400+0x55*64+signed_selector*32)&0xffffffff]
 assert native[0xa20000:0xa20028]==output and native[0xa08000:0xa08090]==before[0xa08000:0xa08090]
 assert struct.unpack_from('<2I',native,0x67c1c8)==(0x12345678,0x9abcdef0) and r[0]==0 and r[15]==0x8cf00000
 cases+=1
scope='28 complete retail RAM/CPU/FPU/SR/PR comparisons of backend readback66adc4 successful dispatch. Seven selector/payload fixtures,both banks,zero/ffffffff service returns. Native descriptor query6691d0 connected with independent signed selector address and unsigned32-bit output. Native byte reader6671c4 connected;native device-word reader66910c connected;independent device alias,4-byte count,stride1 and zero stack arguments. Transfer10d460 and nine other field readers modeled with distinct controlled single-byte writes;independent exact call order,handle/selector/output offsets,first two packet bytes zeroed,40-byte guarded packet and144-byte handle preservation. Final status remains zero despite service errors;diagnostic globals preserved. Actual field widths/values and native field readers/hardware/game integration unverified. Together with42 native error-path comparisons covers routine control paths within these scopes.'
(p/'analysis/backend_readback_native_word_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'backend error comparisons passed')
