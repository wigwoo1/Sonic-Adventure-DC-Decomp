import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_aiff_transfer8_step.dll')).sa_sound_aiff_transfer8_step;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
pending=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_aiff_transfer8_step.dll')).sa_file_pending_state_get;pending.argtypes=abi
cases=0
for phase,channels,frames,ready,bank in itertools.product([0,1,2,3],[1,2],[0,1,3],[0,1],[0,0x200000]):
 sr0=0x70000063;ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 state=0xa20000;struct.pack_into('<2I',ram,state+4,phase,0x8ca50000);ram[state+14]=channels
 for off,val in [(0x50,0x8ca30000),(0x54,2),(0x64,0x8ca40000),(0x68,frames+1),(0x6c,8),(0x70,1),(0x74,frames),(0x78,99),(0x88,0x8c000200),(0x8c,0x8ca60000),(0x90,0x8c000202),(0x94,0x8ca70000),(0xa0,5),(0xa4,20)]:struct.pack_into('<I',ram,state+off,val)
 struct.pack_into('<I',ram,0xa5000c,ready)
 samples=[0,127,128,255,1,254]
 data=bytes(samples);ram[0xa30000:0xa30000+len(data)]=data;ram[0xa40000:0xa40040]=bytes([0x55])*64
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
  if a==0x8c644c16:return pending(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in (0x8c000200,0x8c000202),a
  actual.append(effect(native,r,a));return 0
 def hook(t):
  def body(mem,r,sr):expected.append(effect(mem,r,t));return sr
  return body
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca20000;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c669af2
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,{t:hook(t) for t in (0x8c000200,0x8c000202)},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert actual==expected and macl.value==special['macl']
 converted=phase==1 and ready==0;n=min(frames,2)
 target=bytearray([0x55])*64
 if converted:
  for i in range(n):
   struct.pack_into('<H',target,2*(1+i),samples[channels*i]<<8)
   if channels==2:struct.pack_into('<H',target,2*(9+i),samples[2*i+1]<<8)
 assert native[0xa40000:0xa40040]==target
 assert native[0xa30000:0xa30000+len(data)]==data
 assert struct.unpack_from('<I',native,state+4)[0]==(3 if converted or phase==2 else phase)
 if converted:assert actual[-1][1][:3]==(0x8ca70000,n*channels*2,n)
 if phase==2:assert actual[-1][1][:3]==(0x8ca70000,20,5)
 cases+=1
(out/'analysis/sound_aiff_transfer8_step_native_pending_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native126-instruction8-bit audio transfer step669af2.96 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Phases0/1/2/3,mono/stereo,frames0/1/3,ready0/1,banks. Native pending-state getter644c16 reads file+12; buffer query/submission callbacks modeled. Independent exact raw8-bit samples shifted left8 to16-bit little-endian mono or separated stereo buffers,clamp2,offset1,separation8,source/tail preservation,submission lengths/count and phase transitions. External callback integration/physical audio/full game unverified.'),indent=2)+'\n')
print(cases,'AIFF transfer step comparisons passed')
