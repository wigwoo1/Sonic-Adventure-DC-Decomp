import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_restore_all.dll'))
volume_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
volume=volume_lib.sa_sound_volume_dispatch;volume.argtypes=abi
division=volume_lib.sa_signed_division;division.argtypes=abi
control=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_control.dll')).sa_sound_volume_control;control.argtypes=abi
pack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_midi_pack;pack.argtypes=abi
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for bank,answer,sr0 in itertools.product([0,0x200000],[0,123],[0,0x70000063]):
 entry=0x8c094720;name='sa_sound_voice_restore_all'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 mute_global=lit(0x8c094860);struct.pack_into('<I',ram,mute_global,12345)
 final_target=struct.unpack_from('<I',binary,0x094868-0x10000)[0]
 levels=[(-127,0,127,200,-200,17)[i%6] for i in range(38)]
 table=lit(0x8c094864)
 for i,v in enumerate(levels):struct.pack_into("<i",ram,table+4*i,v)
 handles=lit(0x8c093cc0)
 for slot in range(32,38):struct.pack_into('<I',ram,handles+4*(slot-30),0x8ca08000+8*slot)
 reserved_target=struct.unpack_from('<I',binary,0x093cc4-0x10000)[0]
 struct.pack_into('<I',ram,lit(0x8c643d18),0x8ca09000)
 devices=lit(0x8c093348);packet=lit(0x8c093344)
 struct.pack_into('<2I',ram,devices,0x8ca07000,0x8ca07008)
 ram[0xa07001]=0;ram[0xa07009]=1
 ram[packet:packet+8]=bytes([0x55])*8
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];dispatch_inputs=[];packets=[];reference_packets=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c093be0:
   dispatch_inputs.append((a,r[4],r[5]));return volume(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c093240:return control(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c643c18:return pack(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c643b9c:packets.append(bytes(m[(r[5]&0xffffff)+i] for i in range(4)))
  calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):
   if a==0x8c643b9c:reference_packets.append(bytes(m[(r[5]&0xffffff):(r[5]&0xffffff)+4]))
   events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (0x8c643b9c,struct.unpack_from("<I",binary,0x643d28-0x10000)[0],0x8c65acac,reserved_target,final_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert packets==reference_packets and len(packets)==32
 for slot,data in enumerate(packets):
  level=levels[slot];expected=min(127,max(0,(level//6 if level>0 else level)+106))
  assert data==bytes([expected,7,slot,3]),(slot,data,expected)
 assert calls==events,(calls,events)
 assert len(calls)==103,len(calls)
 assert dispatch_inputs==[(0x8c093be0,levels[i]&0xffffffff,i) for i in range(38)] and calls[-1][0]==final_target
 assert struct.unpack_from('<I',native,mute_global)[0]==0 and r[0]==0
 cases+=1
(out/'analysis/sound_voice_restore_all_native_midi_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='33-instruction native restore-all entry; full retail RAM/CPU/FPU/SR/PR across both banks,SR0/high and service returns0/123. Independent38 volume calls in slot order0..37 with varied signed saved levels,final service target,mute global zero and forced zero return. Volume dispatcher and signed division native; ordinary volume control and MIDI packing native; enqueue, diagnostic logging, reserved volume and final services modeled. Independent 32 MIDI packets checked; actual restored audio not proven.'),indent=2)+'\n')
print(cases,'voice restore-all native MIDI comparisons passed')

