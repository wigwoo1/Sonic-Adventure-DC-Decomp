import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_group_pitch_command.dll')).sa_sound_group_pitch_command;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
for mode,count,action,flags,pitch,answer,bank in itertools.product(range(3),[1,2,3],[0,1,2,3],[0,15,16],[90,0xffffffff],[0,123],[0,0x200000]):
 sr0=0x70000063
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 context=0 if mode==0 else 0x8ca08000;inputptr=0 if mode==1 else 0x8ca10000
 struct.pack_into('<I',ram,0x67c1d0,context);ram[0xa10001]=1
 record=0xa080a8+44;ram[record:record+44]=bytes([0x55])*44;ram[record+5]=count
 packet=0x8e9e24;template=bytes(ram[packet:packet+20])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c65a464,a
  actual.append(r[4]);r[0]=answer;return 0
 def hook(mem,r,sr):expected.append(r[4]);r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=inputptr;s.r[5]=action;s.r[6]=pitch;s.r[7]=flags;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66b030
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,{0x8c65a464:hook},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 valid=mode==2 and flags<16 and count in (1,2) and action in (0,1,2)
 code=0x0d000001 if mode==0 else 0x07000103 if mode==1 else answer if valid else 0x03000001
 assert r[0]==code and actual==expected and macl.value==special['macl']
 assert actual==([0x8c8e9e24] if valid else [])
 expected_packet=bytearray(template)
 if valid:
  struct.pack_into('<I',expected_packet,0,0x21)
  expected_packet[4]=1|(128 if count==2 else 0)|{0:16,1:0,2:48}[action]
  expected_packet[5]=0x55;expected_packet[6]=pitch&255;expected_packet[7]=(pitch>>8)&255;expected_packet[8]=(flags*8)&255
  expected_packet[9:11]=b'\x55\x55'
 assert native[packet:packet+20]==expected_packet
 expected_record=bytearray([0x55])*44;expected_record[5]=count
 if mode==2 and flags<16:struct.pack_into('<I',expected_record,0,action)
 if valid:struct.pack_into('<2I',expected_record,12,0,0)
 assert native[record:record+44]==expected_record
 if code:assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66b030,code)
 cases+=1
(out/'analysis/sound_group_pitch_command_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native109-instruction group pitch command66b030.864 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Missing context/input,channel counts1/2/3,action0/1/2/3,flags0/15/16,pitch90/FFFFFFFF,commit0/123,banks. Native error recorder; packet commit modeled. Independent complete command21 packet, flag/pitch/control bytes, record action/reset mutations and error propagation. Audio/full game unverified.'),indent=2)+'\n')
print(cases,'group pitch comparisons passed')
