import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for bank,answer,diagnostic_answer,sr0 in itertools.product([0,0x200000],[0,123,0xffffffff],[0,7,0xffffffff],[0,0x70000063]):
 level=17
 entry=0x8c66849c;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_flush.dll')).sa_sound_driver_flush
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 division_target=0x8c10ca5c;level_target=0x8c65a378;error_target=0x8c65a2fc
 division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=fn.argtypes
 host=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_host_flush.dll')).sa_sound_host_flush;host.argtypes=fn.argtypes
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];logger_args=[];reference_logger_args=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c6694e8:return host(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==division_target:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==error_target:logger_args.append(tuple(sum(int(m[(r[15]&0xffffff)+4*i+j])<<(8*j) for j in range(4)) for i in range(3)))
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=diagnostic_answer if a==error_target else answer;return 0
 def hook(a):
  def run(m,r,sr):
   if a==error_target:reference_logger_args.append(struct.unpack_from('<3I',m,r[15]&0xffffff))
   events.append((a,r[4],r[5],r[6],r[7]));r[0]=diagnostic_answer if a==error_target else answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=7;s.r[6]=level&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (level_target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)

 assert calls==events and [x[0] for x in calls]==[error_target,level_target,error_target]
 assert logger_args==reference_logger_args
 filename=0x8c000000+lit(0x8c669514)
 assert logger_args[0][:2]==(0x8c000000+lit(0x8c669518),filename)
 assert logger_args[1]==(0x8c000000+lit(0x8c669524),filename,0)
 assert r[0]==0
 assert struct.unpack_from('<I',native,0xa08040)[0]==0x55
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_flush_native_host_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native16-instruction outer wrapper66849c connected to native22-instruction host flush6694e8. Banks/SR values,consumer zero/nonzero/highbit returns and independent logger returns. Full retail RAM/CPU/FPU/SR/PR/MACL and traces match; independent entry log/consumer/exit log sequence,stack format/filename/exit-zero argument and unconditional zero result checked. Logger and consumer modeled; actual transfer/audio unverified.'),indent=2)+'\n')
print(cases,'driver flush native host comparisons passed')
