import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,dllname,name in [(0x8c66b5ac,'sound_channel_count','sa_sound_channel_count'),(0x8c6694b8,'sound_driver_channel_count_wrapper','sa_sound_driver_channel_count_wrapper'),(0x8c66b374,'sound_pan_request','sa_sound_pan_request'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]


def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for presence,maximum,level,bank,capacity in itertools.product(['valid','missing_context','missing_handle'],[0,1,2,3],[-129,-128,-1,0,1,2,3,127,128,255],[0,0x200000],[0,1]):
 answer=0
 sr0=0x70000063;entry=0x8c66846a;canonical_entry=0x8c66b5ac;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_channel_count.dll')).sa_sound_driver_channel_count
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 division_target=0x8c10ca5c;level_target=0x8c66b374;error_target=0x8c65acac
 division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=fn.argtypes
 struct.pack_into('<I',ram,lit(0x8c66b650),0 if presence=='missing_context' else 0x8ca0b000);ram[0xa08001]=0x42
 voice=0xa0b000+struct.unpack_from('<h',binary,0x66b64c-0x10000)[0]+0x42*44
 ram[voice+4]=maximum;ram[voice+5]=0x55;levels=[-127,17]
 struct.pack_into('<2i',ram,voice+28,*levels)
 ring=0xa0b000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,256 if capacity else 0,0,0)
 packet=lit(0x8c66b478);template=bytes(ram[packet:packet+20])
 struct.pack_into('<I',ram,0xa07028,0 if presence=='missing_handle' else handle);struct.pack_into('<I',ram,0xa07040,0x77)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];pan_calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==level_target:pan_calls.append(tuple(r[i] for i in range(4,8)))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==division_target:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca07000;s.r[5]=level&0xffffffff;s.r[6]=0x12345678;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 converted=ctypes.c_int8(level).value
 valid=presence=='valid' and 1<=converted<=2 and converted<=maximum
 result=0 if valid else struct.unpack_from('<I',binary,{'missing_context':0x66b654,'missing_handle':0x66b658,'valid':0x66b65c}[presence]-0x10000)[0]
 wanted=[(level_target,handle,i,levels[i]&0xffffffff,0) for i in range(converted)] if valid else [(error_target,entry,result,0x12345678,0)]
 assert calls==events
 if valid:assert pan_calls==[x[1:] for x in wanted]
 else:assert len(calls)==2 and calls[0][:3]==(error_target,canonical_entry,result) and calls[1][0]==0x8c667b7a
 assert r[0]==result and native[voice+5]==(converted if valid else 0x55)
 assert struct.unpack_from('<2i',native,voice+28)==tuple(levels)
 assert struct.unpack_from('<I',native,0xa08040)[0]==0x55
 expected=bytearray(template);records=[];bias=struct.unpack_from('<h',binary,0x66b468-0x10000)[0]
 if valid:
  for channel in range(converted):
   struct.pack_into('<I',expected,0,0x26);expected[4]=ram[lit(0x8c66b480)+channel]|0x42
   value=levels[channel]
   if converted==2:
    value=value+bias if channel==0 else min(value-bias,-2)
    value=abs(value)//2*(-1 if value<0 else 1)
   value+=bias
   for i in range(6):expected[5+i+i//3]=0
   expected[5+4*channel]=value&255;records.append(bytes(expected))
 assert native[packet:packet+20]==expected
 queued=converted if valid and not capacity else 0
 assert struct.unpack_from('<3I',native,ring+20)==(256 if capacity else queued,queued,0)
 for i,record in enumerate(records if not capacity else []):
  assert native[0xa10000+28*i:0xa10014+28*i]==record
  assert struct.unpack_from('<I',native,0xa10014+28*i)[0]==1
 if not queued:assert native[0xa10000:0xa10038]==bytes(56)
 assert struct.unpack_from('<i',native,0xa07008)[0]==converted
 assert struct.unpack_from('<I',native,0xa07040)[0]==0x77
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_channel_count_native_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native23-instruction outer setter66846a and native3-instruction wrapper6694b8 connected; independent outer signed count update persists on canonical rejection. Native80-instruction channel-count66b5ac. Context/handle presence,maximum channels0/1/2/3,signed-byte boundary inputs,both banks and lower pan returns. Full retail RAM/CPU/FPU/SR/PR/MACL and service traces matched. Independent validation/error codes,count update,per-channel saved-pan arguments,unchanged invalid state and returned result checked. Valid path ignores lower pan error returns and still returns zero. Native pan66b374/commit65a464/loaded copy/copy20 connected; independent exact ordered20-byte command26 records,stereo conversion,packet state,tags/cursors/full queue preservation checked. Diagnostic services modeled; real audio unverified.'),indent=2)+'\n')
print(cases,'driver channel count native enqueue comparisons passed')
