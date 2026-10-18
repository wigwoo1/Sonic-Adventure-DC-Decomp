import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_buffer_read.dll')).sa_sound_buffer_read;fn.argtypes=abi
error_record=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll')).sa_engine_error_record;error_record.argtypes=abi
from verify_engine_cache_writeback import IO,Read,Write
support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
reader=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
helpers={}
for a,name,hasio,owner in [(0x8c10d460,'sa_engine_transfer_read_strided_io',True,reader),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support)]:
 helper=getattr(owner,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];helpers[a]=(helper,hasio)
def word(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]
lower=word(0x8c669078);error=word(0x8c669084);result_address=word(0x8c66907c)&0xffffff;cases=0
for source,length,misaligned,bitmap,bank,sr0 in itertools.product([0x1200,0x1234],[0,4,5,256],[False,True],[0,15],[0,0x200000],[0,0x70000063]):
 answer=0
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 dest=0x8ca20000+int(misaligned);ram[0xa20000:0xa20104]=bytes([0xa5])*260;struct.pack_into('<I',ram,result_address,0xdeadbeef)
 struct.pack_into('<I',ram,0x5fbb14,0x8ca01000);struct.pack_into('<I',ram,0xa01000,0xa05f7800);struct.pack_into('<I',ram,0xa010d4,0)
 for channel in range(4):struct.pack_into('<I',ram,0xa01004+channel*52,channel)
 mmio={0xa05f688c:0}
 for channel in range(4):
  mmio[0xa05f7818+channel*32]=(bitmap>>channel)&1
  mmio[0xa05f781c+channel*32]=0xabcdef01
 payload=bytes((i*17+9)&255 for i in range(256))
 for offset in range(0,256,4):mmio[0xa0800000+source+offset]=struct.unpack_from('<I',payload,offset)[0]
 registers=dict(mmio);errors=[]
 @Read
 def read(ctx,a,n):
  if n!=4 or a not in registers:errors.append(('read',a,n));return 0
  return registers[a]
 @Write
 def write(ctx,a,v,n):
  if n!=4 or a not in registers:errors.append(('write',a,v,n));return
  registers[a]=v
 io=IO(None,read,write)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];stacks=[];refstacks=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==error:return error_record(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==lower:
   calls.append((a,r[4],r[5],r[6],r[7]))
   stacks.append(tuple(sum(int(m[(r[15]&0xffffff)+4*k+j])<<(8*j) for j in range(4)) for k in range(2)))
  assert a in helpers,a
  helper,hasio=helpers[a];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if hasio:args.append(ctypes.byref(io))
  return helper(*args,None,call)
 def hook(m,r,sr):
  events.append((lower,r[4],r[5],r[6],r[7]));refstacks.append(struct.unpack_from('<2I',m,r[15]&0xffffff));r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=source;s.r[5]=dest;s.r[6]=length;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c669014
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 rejected=misaligned or length%4!=0
 assert not errors and mmio==registers
 if rejected:
  assert not calls and r[0]==0x7fffffff
  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c669014,0x7fffffff)
  assert struct.unpack_from('<I',native,result_address)[0]==0xdeadbeef
 else:
  assert calls==[(lower,source+0xa0800000,dest,4,(length+3)//4)] and stacks==[(1,1)]
  assert r[0]==0 and struct.unpack_from('<I',native,result_address)[0]==answer
 expected=bytearray(bytes([0xa5])*260)
 if not rejected:expected[:length]=payload[:length]
 assert native[0xa20000:0xa20104]==expected
 for channel in range(4):assert registers[0xa05f781c+channel*32]==(0 if not rejected and bitmap&(1<<channel) else 0xabcdef01)
 cases+=1
(out/'analysis/sound_buffer_read_native_bus_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native45-instruction G2 read wrapper/error recorder and strided reader/arm/ready/busy/release connected. Full retail RAM/CPU/FPU/SR/PR and MMIO match. Independent exact0/4/256-byte payload,preserved tail,source bias/stride/word count/stack flags,channel release,alignment error/result record checked. G2 registers simulated; physical audio/full game unverified.'),indent=2)+'\n')
print(cases,'G2 buffer read native bus comparisons passed')
