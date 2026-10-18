import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,dllname,name in [(0x8c6683be,'sound_driver_pitch','sa_sound_driver_pitch'),(0x8c667b7a,'sound_error_report','sa_sound_error_report'),(0x8c65acac,'engine_error_record','sa_engine_error_record'),(0x8c66b504,'sound_pitch_request','sa_sound_pitch_request'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]


def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,coarse,fine,bank,capacity in itertools.product([0,1,3],[-101,-100,-99,0,99,100,101],[-101,-100,-99,0,99,100,101],[0,0x200000],[0,1]):
 answer=0
 sr0=0x70000063;entry=0x8c65ddb2;handle=0x8ca06000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_pitch.dll')).sa_sound_subordinate_pitch
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa06000:0xa06090]=bytes([0x55])*144
 struct.pack_into('<I',ram,0xa06040,0x8ca08000);upper_before=bytes(ram[0xa06000:0xa06090])
 struct.pack_into('<2I',ram,0x67c37c,0,0)
 struct.pack_into('<2I',ram,0x67c1c8,0x12345678,0x9abcdef0)
 struct.pack_into('<I',ram,0xa08008,channels);struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 division_target=0x8c10ca5c;wrapper_target=0x8c6694a4;level_target=0x8c66b504;error_target=0x8c667b7a
 division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=fn.argtypes
 wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_pitch_wrapper.dll')).sa_sound_driver_pitch_wrapper;wrapper.argtypes=fn.argtypes
 struct.pack_into('<I',ram,lit(0x8c66b57c),0x8ca0b000);ram[0xa09001]=0x42
 ring=0xa0b000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,256 if capacity else 0,0,0)
 packet=lit(0x8c66b590);expected=bytearray(ram[packet:packet+20])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];canonical_calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==level_target:canonical_calls.append(tuple(r[i] for i in range(4,8)))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==wrapper_target:return wrapper(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==division_target:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  raise AssertionError(hex(a))
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=coarse&0xffffffff;s.r[6]=fine&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 coarse_value=max(-100,min(100,coarse))*256
 fine_value=-256 if fine<=-100 else 256 if fine>=100 else (abs(fine*256)//100)*(-1 if fine<0 else 1)
 converted=(coarse_value+fine_value)&0xffffffff
 wanted=[(level_target,0x8ca09000,ctypes.c_int16(converted).value&0xffffffff,0,0) for i in range(channels)]
 assert canonical_calls==[x[1:] for x in wanted]
 minimum,maximum=struct.unpack_from('<2h',binary,0x66b578-0x10000);signed_level=ctypes.c_int16(converted).value
 valid=minimum<=signed_level<=maximum
 result=struct.unpack_from('<I',binary,(0x65a508 if capacity else 0x66b588)-0x10000)[0] if (capacity or not valid) else 0
 if not valid:result=struct.unpack_from('<I',binary,0x66b588-0x10000)[0]
 assert r[0]==(result if channels else 0x2234000d)
 queued=channels if valid and not capacity else 0
 assert struct.unpack_from('<3I',native,ring+20)==(256 if capacity else queued,queued,0)
 if valid and channels:
  struct.pack_into('<I',expected,0,0x28);expected[4]=0x42
  biased=signed_level+struct.unpack_from('<I',binary,0x66b58c-0x10000)[0]
  expected[6:10]=bytes([biased&255,(biased>>8)&255,0,0])
 assert native[packet:packet+20]==expected
 for i in range(queued):
  assert native[0xa10000+28*i:0xa10014+28*i]==expected
  assert struct.unpack_from('<I',native,0xa10014+28*i)[0]==1
 if not queued:assert native[0xa10000:0xa10054]==bytes(84)
 wanted_upper=bytearray(upper_before);struct.pack_into('<2I',wanted_upper,0x80,coarse&0xffffffff,fine&0xffffffff)
 assert native[0xa06000:0xa06090]==wanted_upper
 assert r[15]==0x8cf00000
 assert struct.unpack_from('<I',native,0xa08040)[0]==0x55
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_subordinate_pitch_native_queue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='588 connected native pitch setter/driver/wrapper/conversion/request/queue/copies/error reporter/diagnostic comparisons. Channels0/1/3, seven signed coarse/fine thresholds, both FPU banks, empty/full queues. Complete retail RAM/CPU/FPU/SR/PR/MACL agreement without modeled services. Independent144-byte upper object cache, pitch conversion, range rejection,20-byte packet/record tags/cursors and queue rejection preservation. Callback unset; hardware playback and game caller integration remain unverified.'),indent=2)+'\n')
print(cases,'subordinate pitch native queue comparisons passed')
