import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
 writer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_word_write.dll'))
 support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for address,name,hasio,owner in [(0x8c10d770,'sa_engine_transfer_copy_strided_io',True,support),(0x8c10ebac,'sa_engine_transfer_fifo_wait',False,support),(0x8c10f9a8,'sa_engine_transfer_fifo_ready_get_io',True,support),(0x8c66910c,'sa_sound_status_word_get',False,lib),(0x8c10d460,'sa_engine_transfer_read_strided_io',True,lib),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support)]:
  fn=getattr(owner,name);fn.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];functions[address]=(fn,hasio)
 helper=writer.sa_sound_bank_word_write;helper.argtypes=abi;functions[0x8c667200]=(helper,False)
 config=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_configure.dll'))
 fn=config.sa_sound_bank_section_configure;fn.argtypes=abi;cases=0
 lookup_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_lookup.dll'))
 lookup=lookup_lib.sa_sound_bank_section_lookup;lookup.argtypes=abi
 kinds=[struct.unpack_from('<I',binary,a-0x8c010000)[0] for a in range(0x8c65a7e4,0x8c65a800,4)]
 offsets=[0,0x80,0x100,0x180,0x200,0x280,0x288]
 context_offsets=[0x1e4,0x264,0x2e4,0x364,0x3e4,0x464,0x46c]
 for selection,mode,bank,value in itertools.product([(section,index) for section in range(7) for index in ([0,15] if section<4 else [0])],[0,1],[0,0x200000],[0,0x12345678]):
  section,index=selection
  address=0x1200+offsets[section]+index*8
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca01000);struct.pack_into('<I',ram,0xa01000,0xa05f7800);struct.pack_into('<I',ram,0xa010d4,mode)
  for slot in range(4):struct.pack_into('<I',ram,0xa01004+slot*52,slot)
  struct.pack_into('<I',ram,0x67c1d0,0x8ca02000)
  struct.pack_into('<I',ram,0xa021c0,0x1200)
  mmio={0xa05f688c:0,0xa0801200:0x1234abff}
  for slot in range(4):mmio[0xa05f7818+slot*32]=0
  mmio[0xa0800000+address]=0x1234abff
  mmio[0xa0800004+address]=0x55667788
  native=bytearray(ram);registers=dict(mmio);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Read
  def read(ctx,a,n):
   if a not in registers or n!=4:errors.append(('read',a,n));return 0
   return registers[a]
  @Write
  def write(ctx,a,v,n):
   if a not in [0xa0800000+address,0xa0800004+address] or n!=4:errors.append(('write',a,v,n))
   registers[a]=v
  io=IO(None,read,write)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   function,hasio=functions[t];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return function(*args,None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65a730;s.r[4]=kinds[section];s.r[5]=index;s.r[6]=value;s.r[7]=0x789a
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,mmio=mmio)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert registers[0xa0800000+address]==value and registers[0xa0800004+address]==0x789a and r[0]==0
  assert struct.unpack_from('<II',native,0xa02000+context_offsets[section]+index*8)==(value,0x789a)
  s.pc=0x8c65a900;s.pr=pr.value=0x8c000100
  for register,data in [(4,kinds[section]),(5,index),(6,0x8cb00000),(7,0x8cb00004)]:s.r[register]=r[register]=data
  reference(ram,s,fpu=f,mmio=mmio)
  assert lookup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert struct.unpack_from('<II',native,0xb00000)==(value,0x789a) and r[0]==0
  cases+=1
 (root/'analysis/sound_bank_section_configure_lookup_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native bank configuration, word write and recovered G2 copy/arm/ready/FIFO/release helpers compared with retail CPU/RAM/FPU/MMIO; seven valid section types, indices0/15 for indexed sections, followed by native lookup readback, values, modes and FPU banks. Independent destination word assertion. Hardware readiness modeled; no audio/device execution.'),indent=2)+'\n')
 print(cases,'bank configuration and lookup comparisons passed')
if __name__=='__main__':main()
