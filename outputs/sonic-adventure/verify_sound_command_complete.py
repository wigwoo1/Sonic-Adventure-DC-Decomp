import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
 writer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_set.dll'))
 support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 functions={}
 for address,name,hasio,owner in [(0x8c669144,'sa_sound_status_byte_set',False,writer),(0x8c66ac3c,'sa_sound_status_word_set',False,writer),(0x8c10d770,'sa_engine_transfer_copy_strided_io',True,support),(0x8c10ebac,'sa_engine_transfer_fifo_wait',False,support),(0x8c10f9a8,'sa_engine_transfer_fifo_ready_get_io',True,support),(0x8c66910c,'sa_sound_status_word_get',False,lib),(0x8c10d460,'sa_engine_transfer_read_strided_io',True,lib),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support)]:
  fn=getattr(owner,name);fn.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];functions[address]=(fn,hasio)
 fn=writer.sa_sound_command_complete;fn.argtypes=abi;cases=0
 for address,mode,bank,count in itertools.product([0x1200,0x1201,0x1202,0x1203],[0,1],[0,0x200000],[0,1,32]):
  value=1
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca01000);struct.pack_into('<I',ram,0xa01000,0xa05f7800);struct.pack_into('<I',ram,0xa010d4,mode)
  for slot in range(4):struct.pack_into('<I',ram,0xa01004+slot*52,slot)
  ram[0xa00000:0xa00004]=bytes([0x55])*4
  mmio={0xa05f688c:0,0xa0801200:0x1234abff}
  for slot in range(4):mmio[0xa05f7818+slot*32]=0
  struct.pack_into('<I',ram,0x67c1d0,0x8ca01000)
  struct.pack_into('<2I',ram,0xa0115c,count,1)
  struct.pack_into('<I',ram,0xa011bc,address);struct.pack_into('<I',ram,0xa01158,0x8ca02000)
  ram[0xa02000:0xa02200]=bytes([0x55])*512
  native=bytearray(ram);registers=dict(mmio);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Read
  def read(ctx,a,n):
   if a not in registers or n!=4:errors.append(('read',a,n));return 0
   return registers[a]
  @Write
  def write(ctx,a,v,n):
   if a!=0xa0801200 or n!=4:errors.append(('write',a,v,n))
   registers[a]=v
  io=IO(None,read,write)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   function,hasio=functions[t];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return function(*args,None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c666ad0;s.r[4]=0x8ca011c4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,mmio=mmio)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  expected=0x1234abff if address==0x200000 else (0x1234abff&~(0xff<<((address&3)*8)))|(value<<((address&3)*8))
  assert registers[0xa0801200]==expected and native[0xa00000:0xa00004]==bytes([0x55])*4
  expected_buffer=bytearray([0x55]*512)
  for index in range(count):expected_buffer[index*16]=0
  assert native[0xa02000:0xa02200]==expected_buffer
  assert struct.unpack_from('<2I',native,0xa0115c)==(0,0)
  cases+=1
 (root/'analysis/sound_command_complete_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Retail full CPU/RAM/FPU/MMIO comparisons for sound status byte read-modify-write via native word getter/setter, G2-to-root/root-to-G2 copy and arm/ready/FIFO/release helpers. Independent selected byte update and other byte preservation across offsets, bounds, values 0/ff, modes and banks. Native command completion performs status byte write then clears each packed work flag and resets pending/busy counters; independent buffer/tail/counter checks for 0/1/32 entries. Synthetic hardware/table/context inputs.'),indent=2)+'\n')
 print(cases,'sound command completion comparisons passed')
if __name__=='__main__':main()
