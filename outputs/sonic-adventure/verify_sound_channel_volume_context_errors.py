import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for presence,selector,level,bank,answer in itertools.product(['valid','missing_context','missing_handle'],[-1,0,1,2,3],[-127,0,127,255],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c66b66c
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_channel_volume.dll')).sa_sound_channel_volume
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c66b700),0x8ca09000);ram[0xa08001]=0x42
 target=struct.unpack_from('<I',binary,0x66b72c-0x10000)[0];error_target=struct.unpack_from('<I',binary,0x66b734-0x10000)[0]
 packet=lit(0x8c66b70c);expected=bytearray(ram[packet:packet+20]);struct.pack_into('<I',expected,0,0x25)
 expected[4]=ram[lit(0x8c66b710)+(selector&3)]|0x42
 signed_level=ctypes.c_int8(level).value;bias=struct.unpack_from('<h',binary,0x66b6fc-0x10000)[0]
 auxiliary=0x89abcdef
 for active,literals in [(selector in (0,-1),(0x66b714,0x66b718,0x66b71c)),(selector in (1,-1),(0x66b720,0x66b724,0x66b728))]:
  if active:
   for addr,value in zip(literals,((signed_level+bias)&255,auxiliary&255,(ctypes.c_int32(auxiliary).value>>8)&255)):expected[lit(0x8c000000+addr)-packet]=value
 if presence=='missing_context':struct.pack_into('<I',ram,lit(0x8c66b700),0)
 initial_packet=bytes(ram[packet:packet+20])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0 if presence=='missing_handle' else 0x8ca08000;s.r[5]=selector&0xffffffff;s.r[6]=level&0xffffffff;s.r[7]=auxiliary;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 if presence=='valid':
  assert native[packet:packet+20]==expected
  assert calls[0][0:2]==(target,0x8c000000+packet) and len(calls)==(2 if answer else 1)
 else:
  assert native[packet:packet+20]==initial_packet
  error_literal=0x66b704 if presence=='missing_context' else 0x66b708
  error_code=struct.unpack_from('<I',binary,error_literal-0x10000)[0]
  assert len(calls)==1 and calls[0][0]==error_target and calls[0][2]==error_code and r[0]==error_code
 cases+=1
(out/'analysis/sound_channel_volume_context_errors_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native72-instruction per-channel command25 builder; selectors-1/0/1/2/3, signed byte levels,both banks and commit returns. Full retail state; independent20-byte command template/identifier/channel fields/auxiliary arithmetic shift/untouched bytes. Valid, missing context and missing handle branches verified; independently checked unchanged packet,exact error value,diagnostic call and return. Commit/error services modeled; audio unverified.'),indent=2)+'\n')
print(cases,'channel volume context/error comparisons passed')
