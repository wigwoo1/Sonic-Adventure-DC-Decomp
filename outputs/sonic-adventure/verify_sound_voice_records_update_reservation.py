import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_records_update.dll')).sa_sound_voice_records_update
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
table=lit(0x8c094a18);gate=lit(0x8c094a00);mode=lit(0x8c094a04)
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_reservation_acquire.dll')).sa_sound_voice_reservation_acquire;acquire.argtypes=fn.argtypes
release=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_reservation_release.dll')).sa_sound_voice_reservation_release;release.argtypes=fn.argtypes
helpers={0x8c095402:acquire,0x8c09550e:release}
native_calls=[]
cases=0
for phase,capacity,bank in itertools.product(['start','stop'],[0,1,4],[0,0x200000]):
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
 if phase=='stop':struct.pack_into('<I',ram,reservations,43)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);events=[];actual=[]
 def service(address,regs,log):log.append((address,tuple(regs[i] for i in range(4,8))));regs[0]=0
 hooks={}
 for address in addresses:
  if address in (0x8c0948a0,*helpers):continue
  def hook(m,regs,status,address=address):service(address,regs,events);return status
  hooks[address]=hook
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in helpers:
   native_calls.append((phase,a,r[4]));return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  service(a,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c0948a0
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events
 assert native_calls[before_calls:]==[(phase,0x8c095402 if phase=='start' else 0x8c09550e,0)]
 expected_key=(43 if capacity>0 else 0) if phase=='start' else (0 if capacity>0 else 43)
 assert struct.unpack_from('<4I',native,reservations)==(expected_key,0,0,0)
 assert struct.unpack_from('<I',native,table+4)[0]==(lifetime if phase=='start' else 0)
 cases+=1
(out/'analysis/sound_voice_records_update_reservation_verification.json').write_text(json.dumps({'cases_passed':cases,'native_reservation_invocations':len(native_calls),'scope':'Native updater acquisition/release branches, explicit start flag1001/lifetime5 and stop flag1010/lifetime1, capacity0/1/4,both banks. Native97-instruction acquisition and64-instruction release exercised with call observations, full retail state/service trace and independent reservation/lifetime checks. Other subordinate calls including panning/playback modeled; no hardware audio proven.'},indent=2)+'\n')
print(cases,'updater reservation comparisons passed')
