import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
minimum,maximum=struct.unpack_from('<2h',binary,0x66b578-0x10000)
for presence,level,auxiliary,bank,answer in itertools.product(['valid','missing_context','missing_handle'],[minimum-1,minimum,-1,0,maximum,maximum+1,32768,65535],[0,127,0x89abcdef],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c66b504
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_pitch_request.dll')).sa_sound_pitch_request
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c66b57c),0 if presence=='missing_context' else 0x8ca09000);ram[0xa08001]=0x42
 target=0x8c65a464;error_target=0x8c65acac
 packet=lit(0x8c66b590);expected=bytearray(ram[packet:packet+20]);signed_level=ctypes.c_int16(level).value
 valid=presence=='valid' and minimum<=signed_level<=maximum
 if valid:
  struct.pack_into('<I',expected,0,0x28);expected[4]=0x42
  bias=struct.unpack_from('<I',binary,0x66b58c-0x10000)[0]
  biased=signed_level+bias;expected[6:8]=bytes([biased&255,(biased>>8)&255])
  expected[8:10]=bytes([auxiliary&255,(auxiliary>>8)&255])
 error_code=answer if valid else struct.unpack_from('<I',binary,{'missing_context':0x66b580,'missing_handle':0x66b584,'valid':0x66b588}[presence]-0x10000)[0]
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0 if presence=='missing_handle' else 0x8ca08000;s.r[5]=level&0xffffffff;s.r[6]=auxiliary;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 assert native[packet:packet+20]==expected
 assert r[0]==error_code
 if valid:assert calls[0][0:2]==(target,0x8c000000+packet)
 assert len(calls)==int(valid)+int(bool(error_code))
 if error_code:assert calls[-1]==(error_target,entry,error_code)
 cases+=1
(out/'analysis/sound_pitch_request_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native58-instruction canonical pitch command28 builder. Valid/null context/null handle, signed16-bit range boundaries/truncation, three auxiliary words,banks,commit returns. Full retail RAM/CPU/FPU/SR/PR/MACL/service traces match; independent20-byte packet,identifier,biased16-bit pitch,auxiliary low16 bits,untouched bytes,error codes/arguments/return and unchanged invalid packets checked. Commit and diagnostics modeled; real audio unverified.'),indent=2)+'\n')
print(cases,'pitch request comparisons passed')
