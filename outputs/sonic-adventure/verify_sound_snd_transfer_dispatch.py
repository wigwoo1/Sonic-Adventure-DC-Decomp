import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_snd_transfer_dispatch.dll')).sa_sound_snd_transfer_dispatch;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
pending=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_snd_transfer_dispatch.dll')).sa_file_pending_state_get;pending.argtypes=abi
lower={}
for address,name in [(0x8c66a13c,'sa_sound_snd_transfer8_step'),(0x8c66a01c,'sa_sound_snd_transfer16_step'),(0x8c66a244,'sa_sound_snd_table_transfer_step')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_snd_transfer_dispatch.dll')),name);lower[address].argtypes=abi
cases=0
for format_flag,phase,channels,frames,ready,bank,capacity in itertools.product([0,1,2,0xffff,0x8001],[0,1,2,3],[1,2],[0,1,3,128],[0,1],[0,0x200000],[2,128]):
 sr0=0x70000063;ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 state=0xa20000;struct.pack_into('<2I',ram,state+4,phase,0x8ca50000);ram[state+14]=channels;struct.pack_into('<H',ram,state+0xaa,format_flag)
 for off,val in [(0x50,0x8ca30000),(0x54,capacity),(0x64,0x8ca40000),(0x68,frames+1),(0x6c,256),(0x70,1),(0x74,frames),(0x78,99),(0x88,0x8c000200),(0x8c,0x8ca60000),(0x90,0x8c000202),(0x94,0x8ca70000),(0xa0,5),(0xa4,20)]:struct.pack_into('<I',ram,state+off,val)
 struct.pack_into('<I',ram,0xa5000c,ready)
 samples=([0x00ff,0x8001,0x1234,0xffff,0xabcd,0x0100]*43)[:256]
 if format_flag in (1,2):samples=list(range(256))
 data=bytes(samples) if format_flag in (1,2) else struct.pack('>256H',*samples);ram[0xa30000:0xa30000+len(data)]=data;ram[0xa40000:0xa40400]=bytes([0x55])*1024
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 def effect(mem,r,t):
  trace=(t,tuple(r[i] for i in range(4,8)))
  if t==0x8c644c16:r[0]=ready
  elif t==0x8c000200:
   for ptr,val in zip((r[5],r[6],r[7]),(1,frames,99)):struct.pack_into('<I',mem,ptr&0xffffff,val)
   r[0]=123
  else:r[0]=456
  return trace
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in lower:return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c644c16:return pending(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in (0x8c000200,0x8c000202),a
  actual.append(effect(native,r,a));return 0
 def hook(t):
  def body(mem,r,sr):expected.append(effect(mem,r,t));return sr
  return body
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca20000;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66a348
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,{t:hook(t) for t in (0x8c000200,0x8c000202)},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert actual==expected and macl.value==special['macl']
 converted=phase==1 and ready==0;n=min(frames,capacity)
 target=bytearray([0x55])*1024
 def convert(value):
  if format_flag==2:return struct.unpack_from('<H',binary,0x66c3ec+value*2)[0]
  return value<<8 if format_flag==1 else value
 if converted:
  for i in range(n):
   struct.pack_into('<H',target,2*(1+i),convert(samples[channels*i]))
   if channels==2:struct.pack_into('<H',target,2*(257+i),convert(samples[2*i+1]))
 assert native[0xa40000:0xa40400]==target
 assert native[0xa30000:0xa30000+len(data)]==data
 assert struct.unpack_from('<I',native,state+4)[0]==(3 if converted or phase==2 else phase)
 if converted:assert actual[-1][1][:3]==(0x8ca70000,n*channels*(1 if format_flag in (1,2) else 2),n)
 if phase==2:assert actual[-1][1][:3]==(0x8ca70000,20,5)
 cases+=1
(out/'analysis/sound_snd_transfer_dispatch_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native14-instruction SND dispatcher66a348 with native130-instruction table transfer66a244,125-instruction8-bit66a13c,137-instruction16-bit66a01c and file pending getter.1280 full retail RAM/CPU/FPU/SR/PR/MACL and callback-trace comparisons: selector0/1/2/ffff/8001,phases0..3,mono/stereo,frames0/1/3/128,pending0/1,banks,capacity2/128. Independent exact output using separately read retail table67c3ec for selector2,raw byte shift for1,BE16 conversion otherwise. All256 table indexes exercised by stereo128-frame fixture. Offset1,separation256,buffer/source guards,phase/count/submission-length checks. Query/submission callbacks modeled; physical audio/fullgame unverified.'),indent=2)+'\n')
print(cases,'SND dispatch comparisons passed')
