import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 save=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_context_save.dll')).sa_input_context_save
 restore=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_context_restore.dll')).sa_input_context_restore
 save.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P]+[ctypes.c_uint32]*5
 restore.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P];count=0
 for seed,savedbank,activebank,value in itertools.product(range(8),[0,0x200000],[0,0x200000],[0,1,0x80000000,0xffffffff]):
  rng=random.Random(seed);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=savedbank|0x40001;f.fpul=rng.getrandbits(32)
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  nf=Fpu.from_buffer_copy(f);originalf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[rng.getrandbits(32) for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c010600+seed*2;s.pc=0x8c110954
  original=list(s.r);continuation=s.pr;special={x:rng.getrandbits(32) for x in ['mach','macl','gbr']};originalspecial=dict(special)
  r=(ctypes.c_uint32*16)(*s.r)
  reference(ram,s,fpu=f,special=special)
  assert save(view,ctypes.byref(nf),r,continuation,nf.fpul,special['mach'],special['macl'],special['gbr'])==0
  assert list(r)==list(s.r) and native==ram and bytes(f)==bytes(nf)
  # Simulate execution of different work between save and resume.
  s.r[:]=[rng.getrandbits(32) for i in range(16)];s.r[4]=0x8ca00000;s.r[5]=value;s.r[15]=0x8cee0000;s.pr=0x8c000100;s.pc=0x8c1108e0;s.sr=0x70000062
  f.fpscr=activebank;f.fpul=rng.getrandbits(32)
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  beforebank=list(f.banks[1-(activebank>>21)]);nf=Fpu.from_buffer_copy(f);r=(ctypes.c_uint32*16)(*s.r)
  special={x:rng.getrandbits(32) for x in originalspecial};ns=(ctypes.c_uint32*3)(*[special[x] for x in ['mach','macl','gbr']]);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(nf.fpul)
  reference(ram,s,fpu=f,special=special,stop_pc=continuation)
  assert restore(view,ctypes.byref(nf),r,ctypes.byref(sr),ctypes.byref(pr),ctypes.byref(fpul),ns)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
  assert list(r)[1:]==original[1:] and r[0]==(value or 1) and pr.value==continuation
  assert list(ns)==[originalspecial[x] for x in ['mach','macl','gbr']] and fpul.value==originalf.fpul and nf.fpscr==originalf.fpscr
  assert list(nf.banks[activebank>>21])==list(originalf.banks[savedbank>>21]) and list(nf.banks[1-(activebank>>21)])==beforebank
  count+=1
 report=dict(cases_passed=count,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_context_save','sa_input_context_restore']},scope='Native save and restore compared against retail instructions in sequence, with intervening register/FPU/special-state changes and distinct incoming/restored continuations. Independent general register restoration, special state and active-bank FMOV checks. Stops at restored continuation; execution beyond it and connected scheduler transfer remain unfinished.')
 (out/'analysis/input_context_roundtrip_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'context roundtrip comparisons passed')
if __name__=='__main__':main()
