import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_destroy.dll')).sa_sound_work_destroy;fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=abi
cases=0
for kind,pattern,bank,stack in itertools.product(range(3),[0,1,2,127,128,255,0xffffffff],[0,1],[0,0x8ca40000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa20000:0xa20100]=bytes([pattern&255])*256
 stream=0x8ca30000 if kind==2 else 0;struct.pack_into('<I',ram,0xa20004,stream);struct.pack_into('<I',ram,0xf00000,stack)
 before=bytes(ram[0xa20000:0xa20100]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 def service(address,mem,r,trace):
  assert address in [0x8c667440,0x8c65ad68,0x8c65ad8e];trace.append(address)
  if address==0x8c667440:assert kind==2 and r[4]==stream
 def hook(address):
  def run(mem,r,sr):service(address,mem,r,expected);return sr
  return run
 @Call
 def call(ctx,address,m,f,srp,r,prp):
  if address==0x8c10d31c:return fill(m,f,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  service(address,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0 if kind==0 else 0x8ca20000;s.r[15]=0x8cf00000;s.pc=0x8c65b72c;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{a:hook(a) for a in [0x8c667440,0x8c65ad68,0x8c65ad8e]},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 wanted=bytearray(before)
 if kind:wanted[:4]=bytes(4)
 assert native[0xa20000:0xa20100]==wanted and r[15]==0x8cf00000
 assert actual==([] if kind==0 else ([0x8c667440] if kind==2 else [])+[0x8c65ad68,0x8c65ad8e])
 cases+=1
(out/'analysis/sound_work_destroy_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='84 full retail RAM/CPU/FPU/SR/PR comparisons of work destructor65b72c..65b760:null work,work with null/non-null stream,seven byte patterns,both banks,two stack values. Independent complete256-byte work object:first4 bytes cleared for non-null work,all other bytes preserved including stream pointer; null work unchanged. Native byte fill10d31c. Stream destruction and critical enter/exit65ad68/65ad8e modeled; exact ordered calls,stream argument and restored stack checked. Native complete release/reuse and fullgame unverified.'),indent=2)+'\n')
print(cases,'sound work destruction comparisons passed')
