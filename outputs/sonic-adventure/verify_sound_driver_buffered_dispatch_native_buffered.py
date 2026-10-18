import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_buffered_dispatch.dll')).sa_sound_driver_buffered_dispatch;fn.argtypes=abi
helpers={}
for a,dll,name in [(0x8c10ca5c,'sound_volume_dispatch','sa_signed_division'),(0x8c10cbb8,'integer_remainder','sa_integer_remainder'),(0x8c6687bc,'sound_driver_power_two_factor','sa_sound_driver_power_two_factor')]:
 f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(dll+'.dll'))),name);f.argtypes=abi;helpers[a]=f
buffered_fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_buffered_transfer.dll')).sa_sound_driver_buffered_transfer;buffered_fn.argtypes=abi
get=0x8c668ffc;setlevel=0x8c6436d0;buffered=0x8c6687f4;cases=0
for length,channels,bank,sr0 in itertools.product([0,1,255,256,257,512],[0,1,2],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;handle=0x8ca08000;buffer=0x8ca30000
 struct.pack_into('<2I',ram,0xa08030,0x1200,0x1200)
 struct.pack_into('<I',ram,0xa08008,channels);struct.pack_into('<I',ram,0xa0803c,0xabcdef01)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in (get,setlevel,buffered,0x8c669014,0x8c6684c4)
  stack=sum(int(m[(r[15]&0xffffff)+j])<<(8*j) for j in range(4)) if a in (buffered,0x8c6684c4) else None
  calls.append((a,r[4],r[5],r[6],r[7],stack))
  if a==buffered:return buffered_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==get:
   for j in range(4):m[(r[4]&0xffffff)+j]=(13>>(j*8))&255
  r[0]=77;return 0
 def hook(a):
  def run(m,r,sr):
   events.append((a,r[4],r[5],r[6],r[7],struct.unpack_from('<I',m,r[15]&0xffffff)[0] if a in (buffered,0x8c6684c4) else None))
   if a==get:struct.pack_into('<I',m,r[4]&0xffffff,13)
   r[0]=77;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=length;s.r[6]=buffer;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c668916
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={a:hook(a) for a in (get,setlevel,0x8c669014,0x8c6684c4)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and [x for x in calls if x[0]!=buffered]==events
 blocks=(length+255)//256;factor=min(blocks&-blocks,128) if blocks else 0
 assert [x[1] for x in calls if x[0]==setlevel]==[0,13]
 assert [x for x in calls if x[0]==buffered]==[(buffered,handle,i,blocks,factor,buffer) for i in range(channels)]
 assert struct.unpack_from('<I',native,0xa0803c)[0]==(256-length%256 if channels else 0xabcdef01)
 assert r[0]==77
 expected=[]
 for channel in range(channels):
  active=buffer;other=buffer+256
  for outer in range(factor):
   ring=(blocks+outer)%64
   expected.append((0x8c669014,0x1200+ring*256,active,256,None))
   for inner in range(64//factor):
    next_ring=(ring-blocks+64)%64;dest=0x1200+next_ring*256
    expected.append((0x8c669014,dest,other,256,None))
    expected.append((0x8c6684c4,handle,channel,dest,active,256))
    ring=next_ring;active,other=other,active
 actual=[x for x in calls if x[0] in (0x8c669014,0x8c6684c4)]
 assert len(actual)==len(expected)
 for observed,wanted in zip(actual,expected):
  if wanted[0]==0x8c669014:assert observed[:4]==wanted[:4]
  else:assert observed==wanted
 cases+=1
(out/'analysis/sound_driver_buffered_dispatch_native_buffered_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native68-instruction buffered dispatcher668916 with native buffered subordinate,signed division,remainder and factor helper. Length0/1/255/256/257/512,channels0/1/2,banks/SR. Full retail state/traces match. Independent ceil(length/256),power-two factor,per-channel arguments/fifth buffer,cached remaining bytes and prior-level restoration checked. Independent per-channel nested read/transfer call counts,ring offsets,buffer alternation and stack length256 checked. Level getter/setter and lower G2 read/block transfer modeled returning77 without payload effects; full game unverified.'),indent=2)+'\n')
print(cases,'buffered dispatcher native buffered comparisons passed')
