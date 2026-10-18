import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for kind,presence,level,bank,answer in itertools.product(['setup','level'],['valid','missing_context','missing_handle'],[-128,-1,0,127],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c66b28c if kind=='setup' else 0x8c66b304
 fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/sound_control_{kind}_request.dll')),f'sa_sound_control_{kind}_request')
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 global_literal=0x66b2e0 if kind=='setup' else 0x66b354
 packet_literal=0x66b2ec if kind=='setup' else 0x66b360
 id_literal=0x66b2f0 if kind=='setup' else 0x66b364
 commit_literal=0x66b2f8 if kind=='setup' else 0x66b368
 diagnostic_literal=0x66b300 if kind=='setup' else 0x66b370
 null_code_literal=(0x66b2e4 if kind=='setup' else 0x66b358) if presence=='missing_context' else (0x66b2e8 if kind=='setup' else 0x66b35c)
 struct.pack_into('<I',ram,lit(0x8c000000+global_literal),0 if presence=='missing_context' else 0x8ca09000);ram[0xa08001]=0x42
 packet=lit(0x8c000000+packet_literal);initial_packet=bytes(ram[packet:packet+20]);expected=bytearray(initial_packet);struct.pack_into('<I',expected,0,0x29 if kind=='setup' else 0x2a)
 id_offset=lit(0x8c000000+id_literal)-packet;expected[id_offset]=0x42;expected[id_offset+1]=level&255
 auxiliary=127
 bias_literal=0x66b2de if kind=='setup' else 0x66b352;bias=struct.unpack_from('<h',binary,bias_literal-0x10000)[0]
 if kind=='setup':expected[lit(0x8c66b2f4)-packet]=(auxiliary+bias)&255
 else:expected[id_offset+1]=(level+bias)&255
 target=struct.unpack_from('<I',binary,commit_literal-0x10000)[0];error_target=struct.unpack_from('<I',binary,diagnostic_literal-0x10000)[0]
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0 if presence=='missing_handle' else 0x8ca08000;s.r[5]=level&0xffffffff;s.r[6]=auxiliary;s.r[7]=0;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
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
  error_literal=null_code_literal
  error_code=struct.unpack_from('<I',binary,error_literal-0x10000)[0]
  assert len(calls)==1 and calls[0][0]==error_target and calls[0][2]==error_code and r[0]==error_code
 cases+=1
(out/'analysis/sound_control_requests_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native41-instruction setup command29 and39-instruction level command2a builders; valid/missing context/missing handle, signed byte levels,banks,commit returns. Full original state and independent20-byte template/identifier/biased fields/error return/unchanged invalid packet. Commit/diagnostics modeled; audio unverified.'),indent=2)+'\n')
print(cases,'control request comparisons passed')
