import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_info.dll'))
fn=lib.sa_sound_driver_info;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
logger=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_frame_debug_log;logger.argtypes=abi
getters=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_getters.dll'))
helpers={0x8c667188:getters.sa_sound_driver_byte_get,0x8c667150:getters.sa_sound_driver_word_get,0x8c66910c:ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/device_word_read.dll')).sa_device_word_read}
for helper in helpers.values():helper.argtypes=abi
queue_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_consume_direct_connected.dll'))
entries={0x8c666c2c:("sa_engine_frame_transfer_begin",False),0x8c668f40:("sa_engine_frame_services",False),0x8c65a518:("sa_engine_object_callback_dispatch",False),0x8c66abfc:("sa_engine_pending_count_get",False),0x8c65a568:("sa_engine_object_state_get",False),0x8c65a5cc:("sa_engine_object_state_set",False),0x8c669088:("sa_engine_frame_transfer_submit",False),0x8c10d770:('sa_engine_transfer_copy_strided_io',True),0x8c10ee9c:('sa_engine_transfer_channels_arm',False),0x8c10fa60:('sa_engine_transfer_channel_arm_io',True),0x8c10eee0:('sa_engine_transfer_ready_wait',False),0x8c10fb48:('sa_engine_transfer_ready_get_io',True),0x8c10ebac:('sa_engine_transfer_fifo_wait',False),0x8c10f9a8:('sa_engine_transfer_fifo_ready_get_io',True),0x8c10ee20:('sa_engine_transfer_channels_release',False),0x8c10f9bc:('sa_engine_transfer_channel_release_io',True),0x8c1101f0:('sa_engine_transfer_busy_get_io',True),0x8c65acac:('sa_engine_error_record',False)}
io_helpers={}
for a,(name,hasio) in entries.items():
 helper=getattr(queue_lib,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:]
 if hasio:io_helpers[a]=helper
 else:helpers[a]=helper
reader=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_read_strided_io.dll')).sa_engine_transfer_read_strided_io;reader.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:];io_helpers[0x8c10d460]=reader
cases=0
for bank,mask,version in itertools.product([0,0x200000],[0,7,15],[(1,0),(1,1),(0,0)]):
 result=0
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 struct.pack_into('<I',ram,0x67c1d0,0x8cb08000)
 def lit(a):return struct.unpack_from('<h',binary,a-0x8c010000)[0]
 struct.pack_into('<I',ram,0xb08000+lit(0x8c65ab62),0x1000)
 values={0x1020:version[0],0x1021:version[1],0x1022:0x7f,0x1023:0x80}
 word_pairs=[(0x8c65ac6a,0x8c65ac68),(0x8c65ac6e,0x8c65ac6c),(0x8c65ac72,0x8c65ac70),(0x8c65ac76,0x8c65ac74),(0x8c65ac7a,0x8c65ac78),(0x8c65ac7e,0x8c65ac7c)]
 for i,(a,b) in enumerate(word_pairs):values[0x1020+lit(a)]=0x89abcdef+i
 struct.pack_into('<I',ram,0x5fbb14,0x8cb09000);struct.pack_into('<I',ram,0xb09000,0xa05f7800)
 for channel in range(4):struct.pack_into('<I',ram,0xb09004+channel*52,channel)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 device_words={0x1020:version[0]|(version[1]<<8)|(0x7f<<16)|(0x80<<24)}
 for a,b in word_pairs:device_words[0x1020+lit(a)]=values[0x1020+lit(a)]
 mmio={0xa0800000+a:v for a,v in device_words.items()};mmio.update({0xa05f688c:0,**{0xa05f7818+32*i:0 for i in range(4)}});registers=dict(mmio)
 @Read
 def read(ctx,a,n):
  if a not in registers or n!=4:errors.append(('read',a,n));return 0
  if a!=0xa05f688c and not 0xa05f7800<=a<0xa05f7880:actual.append(a)
  return registers[a]
 @Write
 def write(ctx,a,v,n):errors.append(('unexpected write',a,v,n))
 io=IO(None,read,write)
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c65a2fc:return logger(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t in helpers:return helpers[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t in io_helpers:return io_helpers[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65aaf4;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<2I',native,0x67c1c8)==(0xabcdef01,0x12345678)
 third=0x8c65ab68 if version==(1,0) else 0x8c65ac64
 wanted=[(0x8c667188,0x1020,0x8cb08000+lit(0x8c65ab64)),(0x8c667188,0x1021,0x8cb08000+lit(0x8c65ab66)),(0x8c667188,0x1023 if version==(1,0) else 0x1022,0x8cb08000+lit(third))]
 wanted += [(0x8c667150,0x1020+lit(a),0x8cb08000+lit(b)) for a,b in word_pairs]
 assert actual==[0xa0801020]*3+[0xa0801020+lit(a) for a,b in word_pairs]
 for t,a,b in wanted:
  n=1 if t==0x8c667188 else 4
  assert int.from_bytes(native[b&0xffffff:(b&0xffffff)+n],'little')==values[a]
 assert struct.unpack_from('<I',native,0xb08000+lit(0x8c65ab62)-28)[0]==0x1020
 assert native[0xb08000+lit(0x8c65ac66)]==0
 cases+=1
report={'cases_passed':cases,'instructions_recovered':167,'scope':'Full original RAM/CPU/SR/PR/FPU/getter-event equality for context present, three version combinations/both FPU banks/three interrupt masks. Native disabled logger; independent info base, three byte/six word read sources/destinations, version branch, values, reset byte, zero return, stack/R14. Native byte/word getters and reader66910c connected; device offsets corrected to1000 base rather than MMIO aliases. Native read transfer10d460 and channel control connected; exact modeled MMIO equality and nine payload reads checked; no routine service substitutions; hardware unverified.'}
(out/'analysis/sound_driver_info_transfer_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'driver info context comparisons passed')
