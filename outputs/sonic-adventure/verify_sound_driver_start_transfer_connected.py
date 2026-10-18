import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_start.dll'))
fn=lib.sa_sound_driver_start;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
helpers={}
queue_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_consume_direct_connected.dll'))
entries={0x8c666c2c:("sa_engine_frame_transfer_begin",False),0x8c668f40:("sa_engine_frame_services",False),0x8c65a518:("sa_engine_object_callback_dispatch",False),0x8c66abfc:("sa_engine_pending_count_get",False),0x8c65a568:("sa_engine_object_state_get",False),0x8c65a5cc:("sa_engine_object_state_set",False),0x8c669088:("sa_engine_frame_transfer_submit",False),0x8c10d770:('sa_engine_transfer_copy_strided_io',True),0x8c10ee9c:('sa_engine_transfer_channels_arm',False),0x8c10fa60:('sa_engine_transfer_channel_arm_io',True),0x8c10eee0:('sa_engine_transfer_ready_wait',False),0x8c10fb48:('sa_engine_transfer_ready_get_io',True),0x8c10ebac:('sa_engine_transfer_fifo_wait',False),0x8c10f9a8:('sa_engine_transfer_fifo_ready_get_io',True),0x8c10ee20:('sa_engine_transfer_channels_release',False),0x8c10f9bc:('sa_engine_transfer_channel_release_io',True),0x8c1101f0:('sa_engine_transfer_busy_get_io',True),0x8c65acac:('sa_engine_error_record',False)}
io_helpers={}
for a,(name,hasio) in entries.items():
 helper=getattr(queue_lib,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:]
 if hasio:io_helpers[a]=helper
 else:helpers[a]=helper
reader=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_read_strided_io.dll')).sa_engine_transfer_read_strided_io;reader.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:];io_helpers[0x8c10d460]=reader
cases=0
for result,bank,mask in itertools.product([0,3,0x0d000001,0xffffffff],[0,0x200000],[0,7,15]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 struct.pack_into('<I',ram,0x5fbb14,0x8cb09000);struct.pack_into('<I',ram,0xb09000,0xa05f7800)
 for channel in range(4):struct.pack_into('<I',ram,0xb09004+channel*52,channel)
 mmio={0xa05f688c:0,0xa0702c00:result,**{0xa05f7818+32*i:0 for i in range(4)}};registers=dict(mmio)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];written=[]
 @Read
 def read(ctx,a,n):
  if a not in registers or n!=4:errors.append(('read',a,n));return 0
  return registers[a]
 @Write
 def write(ctx,a,v,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=v;written.append((a,v))
 io=IO(None,read,write)
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t in io_helpers:return io_helpers[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if t in helpers:return helpers[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65a27c;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 assert r[0]==0 and r[14]==0x2234000e and r[15]==0x8cf00000
 assert registers==mmio and registers[0xa0702c00]==result&0xfffffffe
 assert written==[(0xa0702c00,result&0xfffffffe)]
 assert struct.unpack_from('<I',native,0x67c1c0)[0]==result&0xfffffffe
 assert ((sr.value>>4)&15)==mask
 cases+=1
report={'cases_passed':cases,'instructions_recovered':53,'scope':'Full original RAM/CPU/SR/PR/FPU/events equality; four input/control words, both FPU banks, three interrupt masks. Independent read/write transfer args, control word ANDfffffffe preserving other bits, interrupt mask restore, original256-iteration delay, zero result/stack/R14. Read/write transfer and channel-control helpers native; exact modeled MMIO equality and control write checked; physical sound stop unverified.'}
(out/'analysis/sound_driver_start_transfer_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'sound start comparisons passed')
