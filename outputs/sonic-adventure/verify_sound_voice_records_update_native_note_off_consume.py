import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_tikal_position_native import Vector
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_records_update.dll')).sa_sound_voice_records_update
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
table=lit(0x8c094a18);gate=lit(0x8c094a00);mode=lit(0x8c094a04)
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_reservation_acquire.dll')).sa_sound_voice_reservation_acquire;acquire.argtypes=fn.argtypes
release=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_reservation_release.dll')).sa_sound_voice_reservation_release;release.argtypes=fn.argtypes
helpers={0x8c095402:acquire,0x8c09550e:release}
pan=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_pan.dll')).sa_sound_voice_pan;pan.argtypes=fn.argtypes
helpers[0x8c0955c0]=pan
transform=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_position_transform.dll')).sa_sound_position_transform;transform.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,ctypes.c_void_p,Vector]
normalize=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_normalize.dll')).sa_model_vector_normalize;normalize.argtypes=transform.argtypes
stop=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_dispatch.dll')).sa_sound_voice_stop_dispatch;stop.argtypes=fn.argtypes
helpers[0x8c093b70]=stop
note_off=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_low_index;note_off.argtypes=fn.argtypes
helpers[0x8c0933ba]=note_off
pack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_midi_pack;pack.argtypes=fn.argtypes
helpers[0x8c643c18]=pack
enqueue=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_midi_enqueue;enqueue.argtypes=fn.argtypes
helpers[0x8c643b9c]=enqueue
producer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_commit.dll'))
copylib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
ring_helpers={}
for address,owner,name in [(0x8c65a464,producer,'sa_sound_request_commit'),(0x8c10cd82,producer,'sa_sound_request_copy20'),(0x8c10cd44,copylib,'sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(owner,name);helper.argtypes=fn.argtypes;ring_helpers[address]=helper
consumer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_process.dll')).sa_sound_request_process;consumer.argtypes=fn.argtypes
packing=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_request_enqueue.dll'))
for address,name in [(0x8c666ba8,'sa_engine_frame_request_enqueue'),(0x8c10d25c,'sa_memory_move')]:
 helper=getattr(packing,name);helper.argtypes=fn.argtypes;ring_helpers[address]=helper
ring_helpers[0x8c65a378]=consumer
packet_errors=[]
native_calls=[]
cases=0
for phase,capacity,bank,(x,z) in itertools.product(['start','stop'],[0,1,4],[0,0x200000],[(-2.,0.),(-1.,0.),(0.,1.),(0.,2.),(1.,0.),(2.,0.)]):
 disabled=0;lifetime=5 if phase=='start' else 1;flags=0x1001 if phase=='start' else 0x1010
 before_calls=len(native_calls)
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,gate,disabled);struct.pack_into('<I',ram,mode,0)
 for slot in range(38):struct.pack_into('<13I',ram,table+slot*52,5,0,42,0,0x42,0,0,0,0,0,0,0,0xffffffff)
 struct.pack_into('<13I',ram,table,5,lifetime,42,flags,0x42,0,0,127,0,0,0,0,0)
 identities=lit(0x8c095470);metadata=lit(0x8c0955a0);reservations=lit(0x8c0955a4)
 for i in range(35):
  struct.pack_into('<h',ram,identities+2*i,0x42+i)
  struct.pack_into('<4h',ram,metadata+8*i,capacity,1,0,0x42+i)
  struct.pack_into('<4I',ram,reservations+16*i,0,0,0,0)
 struct.pack_into('<3f',ram,table+36,x,4.,z)
 devices=lit(0x8c093448);packet=lit(0x8c093444)
 struct.pack_into('<2I',ram,devices,0x8ca08000,0x8ca08008);ram[0xa08001]=0;ram[0xa08009]=1
 struct.pack_into('<I',ram,lit(0x8c643d18),0x8ca09000)
 struct.pack_into('<I',ram,lit(0x8c093440),60)
 ram[packet:packet+8]=bytes([0x55])*8
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,0,0,0)
 request_base=lit(0x8c643c00);expected_request=bytearray(ram[request_base:request_base+20]);struct.pack_into('<I',expected_request,0,0x86);expected_request[6:10]=bytes([0,60,0,0])
 pan_address=lit(0x8c094a28);initial_pan=ram[pan_address]
 if phase=='stop':struct.pack_into('<I',ram,reservations,43)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);events=[];actual=[]
 def service(address,regs,log,m):
  log.append((address,tuple(regs[i] for i in range(4,8))))
  if address==0x8c6671c4:m[regs[5]&0xffffff]=0
  regs[0]=0
 hooks={}
 for address in addresses:
  if address in (0x8c0948a0,*helpers,*ring_helpers,0x8c02d528,0x8c63a94c):continue
  def hook(m,regs,status,address=address):service(address,regs,events,m);return status
  hooks[address]=hook
 def instruction(word,fp):
  b=(fp.fpscr>>21)&1
  def raw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
  if word==0xf1fd:assert list(fp.banks[b][:4])==[raw(x),raw(4.),raw(z),raw(1.)]
  elif word==0xf0ed:
   assert list(fp.banks[b][:4])==[raw(x),raw(0.),raw(z),raw(0.)];fp.banks[b][3]=raw(x*x+z*z)
  elif word==0xf37d:
   assert fp.banks[b][3]==raw(x*x+z*z);fp.banks[b][3]=raw(1./max(abs(x),abs(z)))
  else:raise AssertionError(hex(word))
 @Vector
 def vector(ctx,word,fp):instruction(word,fp.contents);return 0
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c02d528:return transform(m,f,r,None,vector)
  if a==0x8c63a94c:return normalize(m,f,r,None,vector)
  if a in ring_helpers:return ring_helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a in helpers:
   native_calls.append((phase,a,r[4]));guard=bytes(m[packet+i] for i in range(4,8))
   code=helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if a==0x8c643c18 and bytes(m[packet+i] for i in range(8))!=bytes([0,60,0,0])+guard:packet_errors.append('packet/guard mismatch');return -9
   return code
  service(a,r,actual,m);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c0948a0
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,hooks=hooks,vector_instruction=instruction)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 nf.fpul=fpul.value
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events,(phase,capacity,x,z,ram==native,list(r)==list(s.r),sr.value==s.sr,pr.value==s.pr,bytes(f)==bytes(nf),macl.value,special['macl'],actual,events)
 wanted=[(phase,0x8c095402 if phase=='start' else 0x8c09550e,0)]
 if phase=='start' and capacity>0:wanted.append((phase,0x8c0955c0,0))
 if phase=='stop':wanted=[(phase,0x8c0955c0,0),(phase,0x8c093b70,0),(phase,0x8c0933ba,0),(phase,0x8c643c18,0x8ca08000),(phase,0x8c643b9c,0x8ca08000),*wanted]
 assert native_calls[before_calls:]==wanted,(phase,capacity,x,z,native_calls[before_calls:],wanted)
 pan_expected=0 if x<0 else 124 if x>0 else 60
 assert native[pan_address]==(pan_expected if phase=='stop' or capacity>0 else initial_pan),(phase,capacity,x,z,hex(pan_address),native[pan_address],pan_expected,initial_pan)
 expected_key=(43 if capacity>0 else 0) if phase=='start' else (0 if capacity>0 else 43)
 assert struct.unpack_from('<4I',native,reservations)==(expected_key,0,0,0)
 assert struct.unpack_from('<I',native,table+4)[0]==(lifetime if phase=='start' else 0)
 assert struct.unpack_from('<2I',native,ring+20)==((1,1) if phase=='stop' else (0,0))
 if phase=='stop':
  assert native[0xa10000:0xa10014]==expected_request
  assert struct.unpack_from('<I',native,0xa10014)[0]==1
 assert not packet_errors
 if phase=='stop':assert native[packet:packet+4]==bytes([0,60,0,0])
 s.pc=0x8c65a378;s.pr=0x8c000100;pr.value=s.pr
 reference(ram,s,fpu=f,special=special,hooks=hooks,vector_instruction=instruction)
 assert consumer(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 nf.fpul=fpul.value
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events
 assert struct.unpack_from('<I',native,ring+20)[0]==0
 assert struct.unpack_from('<I',native,ring+28)[0]==int(phase=='stop')
 assert struct.unpack_from('<I',native,0xa10014)[0]==0
 assert struct.unpack_from('<I',native,ring+4)[0]==int(phase=='stop')
 if phase=='stop':assert native[0xa20000:0xa20010]==bytes([0x86,0])+expected_request[4:18]
 cases+=1
(out/'analysis/sound_voice_records_update_native_note_off_consume_verification.json').write_text(json.dumps({'cases_passed':cases,'native_helper_invocations':len(native_calls),'native_pan_invocations':sum(a==0x8c0955c0 for _,a,_ in native_calls),'native_stop_invocations':sum(a==0x8c093b70 for _,a,_ in native_calls),'native_note_off_invocations':sum(a==0x8c0933ba for _,a,_ in native_calls),'native_midi_pack_invocations':sum(a==0x8c643c18 for _,a,_ in native_calls),'scope':'Native updater acquisition/release branches, explicit start flag1001/lifetime5 and stop flag1010/lifetime1, capacity0/1/4,both banks. Native97-instruction acquisition and64-instruction release exercised with call observations, full retail state/service trace and independent reservation/lifetime checks. Native panning/transform/normalization connected with six axis vector fixtures and independent stored pan byte0/60/124; FTRV/FIPR/FSRRA instruction results supplied, native stop dispatcher connected with per-case stop sequence checks; existing native41-instruction note-off0933ba connected; native MIDI packing connected with independently checked note-off packet/neighbor guards; native enqueue/commit/copy20 connected, independent20-byte ring record and tag/cursor/count checks; native consumer65a378 and packed builder666ba8/memory move connected, independent packed16-byte record/drained count/read cursor/cleared tag checks; readiness/work-dispatch and remaining playback subordinates modeled; no hardware audio proven.'},indent=2)+'\n')
print(cases,'updater native note-off consumption comparisons passed')
