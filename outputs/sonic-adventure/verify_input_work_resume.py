import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_work_yield_native
 restore=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_context_restore.dll')).sa_input_context_restore
 restore.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P]
 resume=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_work_yield_resume
 resume.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for delay,tick,bank,savedbank in itertools.product([0,1,0xffffffff,0x80000000],[0,5,0xffffffff],[0,0x200000],[0,0x200000]):
  rng=random.Random(delay^tick);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<2I',ram,0x8e9dd8,0x8ca00000,0x8cab0000);struct.pack_into('<2I',ram,0xa00204,tick,1);struct.pack_into('<I',ram,0xa02690,0x8ca03000)
  words=[rng.getrandbits(32) for i in range(38)];words[0]=0x8cefffe0;words[1]=0x8ca03000;words[2]=0x8c668ef2;words[3]=0x8ca00000;words[4]=0;words[5]=3;words[6]=0x8c1108e0;words[7]=0x8c8e9dd8;words[37]=savedbank|0x40001;struct.pack_into('<38I',ram,0xa00004,*words)
  struct.pack_into('<8I',ram,0xefffe0,bank,0x8c000100,*[0x22340000+i for i in range(9,15)])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank|0x40001;f.fpul=0xabcdef01
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=delay;s.r[15]=0x8cee0000;s.sr=0x70000063;s.pr=0x8c010600;s.pc=0x8c668e1e
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  reference(ram,s,fpu=f,special=special,stop_pc=0x8c000100)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  nf.fpul=fpul.value
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert [mach.value,macl.value,gbr.value]==words[17:20] and pr.value==0x8c000100 and r[15]==0x8cf00000
  assert struct.unpack_from('<I',native,0xa03014)[0]==((tick+delay)&0xffffffff if delay<0x80000000 else 0x7fffffff)
  assert struct.unpack_from('<I',native,0xa03020)[0]==0x8c668e48
  assert struct.unpack_from('<I',native,0xa00204)[0]==((tick+1)&0xffffffff) and struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8cab0000
  # Resume the context produced by native yield above, through the work caller.
  s.r[4]=0x8ca03018;s.r[5]=1;s.pr=0x8c000100;s.pc=0x8c1108e0
  r[4]=0x8ca03018;r[5]=1;pr.value=0x8c000100
  special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value)
  ns=(ctypes.c_uint32*3)(mach.value,macl.value,gbr.value)
  reference(ram,s,fpu=f,special=special,stop_pc=0x8c010600)
  assert restore(view,ctypes.byref(nf),r,ctypes.byref(sr),ctypes.byref(pr),ctypes.byref(fpul),ns)==0
  assert pr.value==0x8c668e48
  assert resume(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert pr.value==0x8c010600 and r[15]==0x8cee0000 and r[14]==0x2234000e
  assert list(ns)==[0x11112222,0x33334444,0x55556666]
  count+=1
 report=dict(cases_passed=count,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_frame_manager','sa_input_context_save','sa_input_context_restore','sa_input_work_yield','input_frame_context_native']},scope='Full retail native work-yield/save/manager-restore/manager-resume comparison with no injected calls. Due-time, saved work continuation, special/FPU/register state, manager caller return and counter wrap checked. Then the natively saved work context is restored and resumed through 0x8c668e48 to its caller, with full retail comparisons and no injected calls. Manager context fixture constructed; general work caller dispatch and complete input graph integration unfinished.')
 (out/'analysis/input_work_resume_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native work suspend/resume comparisons passed')
if __name__=='__main__':main()
