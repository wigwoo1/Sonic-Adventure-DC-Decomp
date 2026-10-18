import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_readback.dll')).sa_sound_backend_readback;fn.argtypes=abi
diagnostic=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_error_record.dll')).sa_engine_error_record;diagnostic.argtypes=abi
specs=[(0x8c66ba2c,0x14),(0x8c6691d0,0x18),(0x8c66bdd4,2),(0x8c66bb4c,3),(0x8c66bc24,6),(0x8c66bcb4,8),(0x8c66babc,4),(0x8c66b9b0,5),(0x8c66accc,0x10),(0x8c66bd44,0xc)]
query=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_descriptor_byte_query.dll')).sa_sound_descriptor_byte_query;query.argtypes=abi
byte_read=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_byte_read.dll')).sa_sound_byte_read;byte_read.argtypes=abi
word_read=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/device_word_read.dll')).sa_device_word_read;word_read.argtypes=abi
transfer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_read_strided_io.dll')).sa_engine_transfer_read_strided_io;transfer.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
volume_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_volume_get.dll')).sa_sound_backend_volume_get;volume_get.argtypes=abi
volume_byte=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_byte_read.dll')).sa_sound_volume_byte_read;volume_byte.argtypes=abi
pan_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_pan_get.dll')).sa_sound_backend_pan_get;pan_get.argtypes=abi
field_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field_get.dll')).sa_sound_backend_field_get;field_get.argtypes=abi
field_word=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_field_word_read.dll')).sa_sound_field_word_read;field_word.argtypes=abi
field8_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field8_get.dll')).sa_sound_backend_field8_get;field8_get.argtypes=abi
fieldc_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_fieldc_get.dll')).sa_sound_backend_fieldc_get;fieldc_get.argtypes=abi
field14_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field14_get.dll')).sa_sound_backend_field14_get;field14_get.argtypes=abi
field4_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field4_get.dll')).sa_sound_backend_field4_get;field4_get.argtypes=abi
field5_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field5_get.dll')).sa_sound_backend_field5_get;field5_get.argtypes=abi
field10_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field10_get.dll')).sa_sound_backend_field10_get;field10_get.argtypes=abi
lookup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_resource_lookup.dll')).sa_sound_resource_lookup;lookup.argtypes=abi
cases=0
for (enabled,present),value,bank,answer,mode,channels,fmt in itertools.product([(1,1)],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,0xffffffff],[0,1],[1,2],range(4)):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08090]=bytes([0x55])*144;ram[0xa20000:0xa20028]=bytes([0x66])*40
 struct.pack_into('<I',ram,0x67c1d0,0x8ca09000 if enabled else 0);struct.pack_into('<2I',ram,0x67c1c8,0x12345678,0x9abcdef0)
 struct.pack_into('<I',ram,0xa091b8,0x8ca30000);ram[0xa09000+0xa8+0x55*44+5]=channels
 struct.pack_into('<I',ram,0x5fbb14,0x8cb08000);struct.pack_into('<I',ram,0xb080d4,mode)
 signed_selector=ctypes.c_int8(value&255).value;device=(0xa0800000+0x8ca30400+0x55*64+signed_selector*32)&0xffffffff
 struct.pack_into('<I',ram,0xa09000+0xa8+0x55*44,fmt)
 struct.pack_into('<2I',ram,0xa09364,0x12345678,0x87654321)
 ram[(0xa09000+0xa8+0x55*44+7+signed_selector)&0xffffff]=0
 payload=int.from_bytes(bytes((value+i)&255 for i in range(4)),'little');field_payload=int.from_bytes(bytes((value+4+i)&255 for i in range(4)),'little');field8_payload=int.from_bytes(bytes((value+8+i)&255 for i in range(4)),'little');fieldc_payload=int.from_bytes(bytes((value+12+i)&255 for i in range(4)),'little');field14_payload=int.from_bytes(bytes((value+16+i)&255 for i in range(4)),'little');field4_payload=int.from_bytes(bytes((value+20+i)&255 for i in range(4)),'little');field10_payload=int.from_bytes(bytes((value+24+i)&255 for i in range(4)),'little');mmio={(device+24)&0xffffffff:field10_payload,(device+16)&0xffffffff:field4_payload,(device+28)&0xffffffff:field14_payload,(device+20)&0xffffffff:fieldc_payload,device:payload,(device+8)&0xffffffff:field_payload,(device+4)&0xffffffff:field8_payload}
 before=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];reads=[];volume_reads=[];field_reads=[];lookup_calls=[]
 errors=[]
 @Read
 def read(ctx,a,n):
  if a not in mmio or n!=4:errors.append((a,n));return 0
  reads.append(a);return mmio[a]
 @Write
 def write(ctx,a,v,n):errors.append(('write',a,n))
 io=IO(None,read,write)
 def service(a,m,r):
  i=next(i for i,x in enumerate(specs) if x[0]==a);m[r[6]&0xffffff]=(value+i)&255;r[0]=answer
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c6671c4:return byte_read(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66910c:return word_read(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10d460:return transfer(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if a in [0x8c10ee9c,0x8c10eee0,0x8c10ee20]:r[0]=0;return 0
  if a==0x8c667188:
   volume_reads.append(r[4]);return volume_byte(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c667150:
   field_reads.append(r[4]);return field_word(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c65a900:
   lookup_calls.append(tuple(r[i] for i in [4,5,6,7]));return lookup(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in dict(specs);calls.append((a,r[4],r[5],r[6]))
  if a==0x8c6691d0:return query(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66bdd4:return volume_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66bb4c:return pan_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66bc24:return field_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66bcb4:return field8_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66bd44:return fieldc_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66ba2c:return field14_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66babc:return field4_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66b9b0:return field5_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c66accc:return field10_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  raise AssertionError(hex(a))
 def poll(m,r,sr):r[0]=0;return sr
 def volume_hook(m,r,sr):m[r[5]&0xffffff]=(value+2)&255;r[0]=answer;return sr
 def field_hook(m,r,sr):
  for i in range(4):m[(r[5]&0xffffff)+i]=(value+4+i)&255
  r[0]=answer;return sr
 def hook(a):
  def run(m,r,sr):service(a,m,r);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000 if present else 0;s.r[5]=value;s.r[6]=0x8ca20004;s.r[15]=0x8cf00000;s.pc=0x8c66adc4;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{**{a:hook(a) for a,o in specs if a not in [0x8c6691d0,0x8c66bdd4,0x8c66bb4c,0x8c66bc24,0x8c66bcb4,0x8c66bd44,0x8c66ba2c,0x8c66babc,0x8c66b9b0,0x8c66accc]},**{a:poll for a in [0x8c10ee9c,0x8c10eee0,0x8c10ee20]}},fpu=f,special={'macl':0},mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert volume_reads==[(0x8ca30400+0x55*64+signed_selector*32+i)&0xffffffff for i in [1,2,16,17]]
 assert macl.value==0x55*44
 assert field_reads==[(0x8ca30400+0x55*64+signed_selector*32+i)&0xffffffff for i in [8,4,24]]
 assert lookup_calls==[(0x52535053,0,0x8cefffcc,0x8cefffd0)],lookup_calls
 assert not errors
 assert calls==[(a,0x8ca08000,value,0x8ca20004+o) for a,o in specs]
 output=bytearray(before[0xa20000:0xa20028]);output[4:6]=bytes(2)
 for i,(a,o) in enumerate(specs):
  if a==0x8c6691d0:struct.pack_into('<I',output,4+o,value&255)
  elif a==0x8c66bdd4:output[4+o]=(value+1)&255
  elif a==0x8c66bb4c:
   result=ctypes.c_int8((value+2)&255).value
   if channels==2:
    if (value&255)==0:result+=64
    elif (value&255)==1:result=(result if result else -1)-64
    result*=2
   output[4+o]=result&255
  elif a==0x8c66bc24:output[4+o:6+o]=bytes([(value+4)&255,(value+5)&255])
  elif a==0x8c66bcb4:output[4+o:6+o]=bytes([(value+8)&255,(value+9)&255])
  elif a==0x8c66bd44:struct.pack_into('<I',output,4+o,fieldc_payload)
  elif a==0x8c66ba2c:struct.pack_into('<I',output,4+o,field14_payload)
  elif a==0x8c66babc:output[4+o]=(value+20)&255
  elif a==0x8c66b9b0:output[4+o]=(value+21)&255
  elif a==0x8c66accc:
   signed=ctypes.c_int32(field10_payload).value
   adjusted=(field10_payload*2)&0xffffffff if fmt==1 else (abs(signed)//2*(-1 if signed<0 else 1))&0xffffffff if fmt==2 else field10_payload
   struct.pack_into('<I',output,4+o,adjusted)
  else:raise AssertionError(hex(a))
 signed_selector=ctypes.c_int8(value&255).value
 assert reads==[(device+28)&0xffffffff]+[device]*3+[(device+8)&0xffffffff,(device+4)&0xffffffff,(device+16)&0xffffffff,(device+16)&0xffffffff,(device+24)&0xffffffff,(device+20)&0xffffffff]
 assert native[0xa20000:0xa20028]==output and native[0xa08000:0xa08090]==before[0xa08000:0xa08090]
 assert struct.unpack_from('<2I',native,0x67c1c8)==(0x12345678,0x9abcdef0) and r[0]==0 and r[15]==0x8cf00000
 cases+=1
scope='448 complete retail RAM/CPU/FPU/SR/PR comparisons of backend readback66adc4 with all ten field readers native,including667150/667188/6671c4/66910c and transfer10d460. Seven selector/payload fixtures,both banks,two transfer context modes,mono/stereo channels,four voice formats. Independent signed addressing,ordered ten MMIO reads,byte lanes,16/32-bit field widths,pan conversion,format doubling/wrap and signed halving,40-byte packet guards,144-byte handle preservation,MACL and diagnostic globals. Native resource lookup65a900 connected with independent tag/index/stack output pointers and distinct table values. Only hardware polling/control services10ee9c/10eee0/10ee20 remain modeled;device data supplied by controlled MMIO. No modeled field readers. Actual hardware and full game integration unverified.'
(p/'analysis/backend_readback_native_all_fields_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'backend error comparisons passed')
