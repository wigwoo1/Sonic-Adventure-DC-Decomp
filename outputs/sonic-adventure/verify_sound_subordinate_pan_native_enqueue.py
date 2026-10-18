import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,dllname,name in [(0x8c66837c,'sound_driver_pan','sa_sound_driver_pan'),(0x8c667b7a,'sound_error_report','sa_sound_error_report'),(0x8c10ca5c,'signed_division','sa_signed_division'),(0x8c669498,'sound_driver_pan_wrapper','sa_sound_driver_pan_wrapper'),(0x8c66b374,'sound_pan_request','sa_sound_pan_request'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]

def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,selector,level,bank,capacity in itertools.product([1,2],[0,1,2,3],[-32768,-16,-15,-1,0,1,15,16,32767],[0,0x200000],[0,123]):
 if channels==1 and selector==1:continue
 answer=0
 sr0=0x70000063;entry=0x8c65dcca
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_pan.dll')).sa_sound_subordinate_pan
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c66b46c),0x8ca09000);ram[0xa0b001]=0x42
 ram[0xa06000:0xa06080]=bytes([0x55])*128
 struct.pack_into('<I',ram,0xa06040,0x8ca08000)
 upper_before=bytes(ram[0xa06000:0xa06080])
 struct.pack_into('<2I',ram,0x67c37c,0,0)
 struct.pack_into('<I',ram,0xa08028,0x8ca0b000);struct.pack_into('<I',ram,0xa08040,0x55)
 voice=0xa09000+struct.unpack_from('<h',binary,0x66b466-0x10000)[0]+0x42*44
 ram[voice+5]=channels
 target=0x8c65a464;error_target=0x8c65acac
 packet=lit(0x8c66b478);expected=bytearray(ram[packet:packet+20]);struct.pack_into('<I',expected,0,0x26)
 expected[4]=ram[lit(0x8c66b480)+(selector&3)]|0x42
 clamped=max(-15,min(15,level));converted=(abs(127*clamped)//15)*(-1 if clamped<0 else 1);signed_level=ctypes.c_int8(converted).value;bias=struct.unpack_from('<h',binary,0x66b468-0x10000)[0]
 auxiliary=0;levels=[signed_level]*channels
 if channels==2:
  if selector==0:levels[0]+=bias
  if selector==1:levels[1]=min(levels[1]-bias,-2)
  levels=[abs(v)//2*(-1 if v<0 else 1) for v in levels]
 levels=[v+bias for v in levels]
 if selector in (0,1):
  for i in range(6):expected[5+i+(i//3)]=0
  off=5+4*selector
  expected[off:off+3]=bytes([levels[selector]&255,auxiliary&255,(auxiliary>>8)&255])
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,256 if capacity else 0,0,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];canonical_calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c66b374:canonical_calls.append(tuple(r[i] for i in range(4,8)))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca06000;s.r[5]=selector&0xffffffff;s.r[6]=level&0xffffffff;s.r[7]=auxiliary;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={error_target:hook(error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 assert canonical_calls==[(0x8ca0b000,selector,converted&0xffffffff,0)]
 assert struct.unpack_from('<I',native,0xa08040)[0]==0x55
 assert native[packet:packet+20]==expected
 assert struct.unpack_from('<i',native,voice+28+4*selector)[0]==signed_level
 assert struct.unpack_from('<3I',native,ring+20)==((256,0,0) if capacity else (1,1,0))
 assert struct.unpack_from('<I',native,0xa10014)[0]==(0 if capacity else 1)
 if not capacity:assert native[0xa10000:0xa10014]==expected
 else:assert native[0xa10000:0xa1001c]==bytes(28)
 wanted_upper=bytearray(upper_before);struct.pack_into('<i',wanted_upper,0x70+4*selector,clamped)
 assert native[0xa06000:0xa06080]==wanted_upper
 assert all(event[0]==error_target for event in calls)
 cases+=1
(out/'analysis/sound_subordinate_pan_native_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='252 full retail RAM/CPU/FPU/SR/PR/MACL comparisons of upper pan65dcca through native driver66837c,division10ca5c,wrapper669498,canonical command26 builder66b374,commit65a464,copy20 and loaded copy. Nine signed levels including clamps,mono/stereo,selectors0..3,both banks,empty/full256-entry queue. Mono selector1 excluded because the retail builder reads an uninitialized second-channel stack word. Independent complete128-byte upper object/clamped cache70+4*channel,canonical forwarded arguments,converted signedbyte/voice cache,entire20-byte command/stereo/lookup/aux fields,accepted queue record/tag/counters and full-queue preservation. Error reporter667b7a executes natively with callback unset. Only queue diagnostic65acac modeled. Physical audio/game caller integration/fullgame remain unverified.'),indent=2)+'\n')
print(cases,'upper pan native enqueue comparisons passed')
