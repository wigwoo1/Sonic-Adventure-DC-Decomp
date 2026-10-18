import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_descriptor_byte_query.dll')).sa_sound_descriptor_byte_query;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
functions={}
for address,name,hasio,owner in [(0x8c66910c,'sa_sound_status_word_get',False,lib),(0x8c10d460,'sa_engine_transfer_read_strided_io',True,lib),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support)]:
 fn=getattr(owner,name);fn.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];functions[address]=(fn,hasio)
byteget=lib.sa_sound_status_byte_get;byteget.argtypes=abi;functions[0x8c6671c4]=(byteget,False)
functions[0x8c65acac]=(error,False)
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_descriptor_byte_query.dll')).sa_sound_descriptor_byte_query;fn.argtypes=abi
cases=0
for mode,selector,descriptor,value,bank,sr0 in itertools.product(range(4),[0,1,128,255],[0,127,128,255],[0,255],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 context=0 if mode==0 else 0x8ca08000;inputptr=0 if mode==1 else 0x8ca10000;output=0 if mode==2 else 0x8ca20000
 struct.pack_into('<I',ram,0x67c1d0,context);struct.pack_into('<I',ram,0xa081b8,0x10001)
 ram[0xa10001]=descriptor;struct.pack_into('<2I',ram,0xa20000,0xdeadbeef,0x12345678)
 struct.pack_into('<I',ram,0x5fbb14,0x8ca01000);struct.pack_into('<I',ram,0xa01000,0xa05f7800);struct.pack_into('<I',ram,0xa010d4,0)
 for slot in range(4):struct.pack_into('<I',ram,0xa01004+slot*52,slot)
 address=(0x10401+ctypes.c_int8(descriptor).value*64+ctypes.c_int8(selector).value*32)&0xffffffff
 mmio={0xa05f688c:0,(0xa0800000+(address&0xfffffffc))&0xffffffff:value*0x01010101}
 for slot in range(4):mmio[0xa05f7818+slot*32]=0
 native=bytearray(ram);registers=dict(mmio);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[];actual=[]
 @Read
 def read(ctx,a,n):
  if a not in registers or n!=4:errors.append(('read',a,n));return 0
  return registers[a]
 @Write
 def write(ctx,a,v,n):errors.append(('unexpected write',a,v,n))
 io=IO(None,read,write)
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c6671c4:actual.append(r[4])
  assert t in functions,t
  function,hasio=functions[t];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if hasio:args.append(ctypes.byref(io))
  return function(*args,None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=inputptr;s.r[5]=selector;s.r[6]=output;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c6691d0
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 code=[0x0d000001,0x07000103,0x07000203,0][mode]
 assert r[0]==code and not errors and registers==mmio
 if mode==3:
  address=(0x10401+ctypes.c_int8(descriptor).value*64+ctypes.c_int8(selector).value*32)&0xffffffff
  assert actual==[address]
  assert struct.unpack_from('<2I',native,0xa20000)==(value,0x12345678)
 else:
  assert not actual
  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c6691d0,code)
  assert struct.unpack_from('<2I',native,0xa20000)==(0xdeadbeef,0x12345678)
 cases+=1
(out/'analysis/sound_descriptor_byte_query_native_bus_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native descriptor query6691d0 with native status-byte/word getters, G2 reader, channel arm/ready/release and error recorder.512 full retail RAM/CPU/FPU/SR/PR/MMIO comparisons and independent error/descriptor address/output checks. Lower sound read no longer modeled; G2 register/sound memory fixtures and no physical hardware/full game claim.'),indent=2)+'\n')
print(cases,'descriptor byte query comparisons passed')
