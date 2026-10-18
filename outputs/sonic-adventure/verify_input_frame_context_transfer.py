import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_frame_context_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for seed,bank,savedbank in itertools.product(range(8),[0,0x200000],[0,0x200000]):
  rng=random.Random(seed);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<2I',ram,0x8e9dd8,0x8cab0000,0xdeadbeef);struct.pack_into('<3I',ram,0xa00204,5,1,1)
  struct.pack_into('<I',ram,0xa0021c,1);struct.pack_into('<I',ram,0xa00224,5)
  words=[rng.getrandbits(32) for i in range(38)];words[0]=0x8cee0000;words[2]=0x8c010600+seed*2;words[37]=savedbank|0x40001;struct.pack_into('<38I',ram,0xa00228,*words)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank;f.fpul=0xabcdef01
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c668e60
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  reference(ram,s,fpu=f,special=special,stop_pc=words[2])
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==1
  nf.fpul=fpul.value
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert [mach.value,macl.value,gbr.value]==words[17:20] and pr.value==words[2] and r[15]==words[0] and r[0]==1
  assert struct.unpack_from('<I',native,0xa00204)[0]==5 and struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8ca00000
  count+=1
 report=dict(cases_passed=count,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_frame_manager','sa_input_context_save','sa_input_context_restore','sa_input_work_dispatch','input_frame_context_native']},scope='Full retail comparison through state-1 due work and native restore to distinct continuation. No injected calls. Restored special/FPU/register state and abandonment of old frame epilogue checked. Result 1 exposes transfer; continuation execution and complete interrupt graph integration unfinished.')
 (out/'analysis/input_frame_context_transfer_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native frame transfer comparisons passed')
if __name__=='__main__':main()
