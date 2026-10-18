import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_magic_state_initialize.dll')).sa_sound_magic_state_initialize;fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_memory_fill;fill.argtypes=abi
cases=0
for first,second,channels,stride,offset,bank in itertools.product([0,0xffffffff],[0,0xffffffff],[1,128],[2,255],[24,32768],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;state=0xa20000;ram[state:state+256]=bytes([0x55])*256
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 def effect(mem,r,t):
  sp=r[15]&0xffffff;stack=struct.unpack_from('<6I',mem,sp)
  trace=(t,tuple(r[i] for i in range(4,8)))
  def put(ptr,value,size):mem[ptr&0xffffff:(ptr&0xffffff)+size]=value.to_bytes(size,'little')
  if t==0x8c65ae44:
   put(r[6],offset,2);put(r[7],255,1)
   for ptr,value,size in zip(stack,[16,stride,channels,22050,19,1],[1,1,1,4,4,4]):put(ptr,value,size)
   r[0]=first
  elif t==0x8c65af64:put(r[6],0xfffe,2);r[0]=second
  elif t==0x8c644be6:r[0]=0xffffffff
  elif t==0x8c65afca:
   for ptr,value,size in [(r[6],101,4),(r[7],102,2)]+list(zip(stack[:5],[103,104,105,106,107],[2,4,4,4,4])):put(ptr,value,size)
   r[0]=0xffffffff
  return trace
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10d31c:return fill(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in (0x8c65ae44,0x8c65af64,0x8c644be6,0x8c65afca),a
  actual.append(effect(native,r,a));return 0
 def hook(t):
  def body(mem,r,sr):expected.append(effect(mem,r,t));return sr
  return body
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca20000,0x8ca10000,64];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c667488
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,{t:hook(t) for t in (0x8c65ae44,0x8c65af64,0x8c644be6,0x8c65afca)},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==expected
 target=bytearray([0x55])*256;struct.pack_into('<H',target,2,1);target[12:16]=bytes([255,16,channels,stride]);struct.pack_into('<3I',target,16,1,22050,19)
 if first==0:struct.pack_into('<H',target,0x1c,0xfffe)
 accepted=first==0 and second==0
 if accepted:
  struct.pack_into('<I',target,0x20,101);struct.pack_into('<2H',target,0x24,102,103);struct.pack_into('<4I',target,0x28,104,105,106,107)
  struct.pack_into('<3I',target,0x58,ctypes.c_int8(channels).value&0xffffffff,ctypes.c_int8(stride).value&0xffffffff,1)
  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)
  struct.pack_into('<I',target,0x80,0x8ca2004c);struct.pack_into('<I',target,0x9c,0);struct.pack_into('<H',target,0xa8,0);struct.pack_into('<I',target,0x4c,0)
 assert native[state:state+256]==target,(first,second,channels,stride,offset,[(hex(i),native[state+i],target[i]) for i in range(256) if native[state+i]!=target[i]])
 assert r[0]==(ctypes.c_int16(offset).value&0xffffffff if accepted else 0)
 assert [t for t,args in actual]==([0x8c65ae44]+([0x8c65af64] if first==0 else [])+([0x8c644be6,0x8c65afca] if accepted else []))
 cases+=1
(out/'analysis/sound_magic_state_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native special magic8000 stream-state initializer667488.64 full retail RAM/CPU/FPU/SR/PR/MACL/service-trace comparisons: metadata/channel success/failure,channels1/128,stride2/255,offset24/32768,banks. Independent full256-byte partial failure/success state,signed extensions,format0,copied defaults,internal pointer,fill and returned signed offset. Metadata65ae44/channel65af64/configure644be6/parameter65afca modeled; configure/parameter errors deliberately ignored as retail. Memory fill native. Real encoded files/decoder/physical audio/fullgame unverified.'),indent=2)+'\n')
print(cases,'magic audio state comparisons passed')
