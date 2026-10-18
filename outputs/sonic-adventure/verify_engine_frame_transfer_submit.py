import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_transfer_submit.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 entries={0x8c10d770:('sa_engine_transfer_copy_strided_io',True),0x8c10ee9c:('sa_engine_transfer_channels_arm',False),0x8c10fa60:('sa_engine_transfer_channel_arm_io',True),0x8c10eee0:('sa_engine_transfer_ready_wait',False),0x8c10fb48:('sa_engine_transfer_ready_get_io',True),0x8c10ebac:('sa_engine_transfer_fifo_wait',False),0x8c10f9a8:('sa_engine_transfer_fifo_ready_get_io',True),0x8c10ee20:('sa_engine_transfer_channels_release',False),0x8c10f9bc:('sa_engine_transfer_channel_release_io',True),0x8c1101f0:('sa_engine_transfer_busy_get_io',True),0x8c65acac:('sa_engine_error_record',False)}
 funcs={}
 for address,(name,hasio) in entries.items():
  f=getattr(lib,name);f.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];funcs[address]=(f,hasio)
 fn=lib.sa_engine_frame_transfer_submit;fn.argtypes=abi
 cases=0
 for length,callback,mode,bitmap,bank in itertools.product([0,4,36],[0,0xffffffff,0x8c64241a],[0,1],[0,15],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200);struct.pack_into('<I',ram,0xa00200,0xa05f7800)
  struct.pack_into('<I',ram,0xa002d4,mode)
  for channel in range(4):struct.pack_into('<I',ram,0xa00204+channel*52,channel)
  data=bytes((i*17+9)&255 for i in range(64));ram[0xa02000:0xa02040]=data
  struct.pack_into('<I',ram,0xf00000,0x12345678)
  mmio={0xa05f688c:0}
  for channel in range(4):
   mmio[0xa05f7818+channel*32]=(bitmap>>channel)&1
   mmio[0xa05f781c+channel*32]=0xabcdef01
  for offset in range(0,64,4):mmio[0xa0801200+offset]=0x55555555
  native=bytearray(ram);registers=dict(mmio);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[];callbacks=[]
  @Read
  def read(ctx,a,n):
   if n!=4 or a not in registers:errors.append(('read',a,n));return 0
   return registers[a]
  @Write
  def write(ctx,a,v,n):
   if n!=4 or a not in registers:errors.append(('write',a,v,n));return
   registers[a]=v
  io=IO(None,read,write)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c64241a:callbacks.append(r[4]);return 0
   if t not in funcs:errors.append(t);return -9
   function,hasio=funcs[t]
   args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return function(*args,None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c669088;s.r[4]=0x8ca02000;s.r[5]=0x1200;s.r[6]=length;s.r[7]=callback
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,mmio=mmio)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  copied=b''.join(struct.pack('<I',registers[0xa0801200+i]) for i in range(0,64,4))
  assert copied==data[:length]+bytes([0x55])*(64-length)
  assert native[0xa02000:0xa02040]==data and r[0]==0
  assert callbacks==([0x12345678] if callback==0x8c64241a else [])
  for channel in range(4):assert registers[0xa05f781c+channel*32]==(0 if bitmap&(1<<channel) else 0xabcdef01)
  cases+=1
 (out/'analysis/engine_frame_transfer_submit_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Retail CPU/RAM/FPU/MMIO comparison of aligned synchronous root-to-G2 word copy and native arm/wait/release helpers. Independent data, preserved source/tail and callback payload checks. Channel and bus fixtures are synthetic; callback is retail no-op, not real frame services. Unaligned paths, busy delays and physical hardware not covered.'),indent=2)+'\n')
 print(cases,'synchronous frame submission comparisons passed')
if __name__=='__main__':main()
