import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_context_save.dll')).sa_input_context_save
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P]+[ctypes.c_uint32]*5;count=0
 for seed,bank,mode in itertools.product(range(12),[0,0x200000],[0,0x40001]):
  rng=random.Random(seed);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  f=Fpu();f.fpscr=bank|mode;f.fpul=rng.getrandbits(32)
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[rng.getrandbits(32) for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=rng.getrandbits(32);s.pr=0x8c000100;s.pc=0x8c110954
  initial=list(s.r);r=(ctypes.c_uint32*16)(*s.r);special={x:rng.getrandbits(32) for x in ['mach','macl','gbr']};initialsr=s.sr
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),r,s.pr,nf.fpul,special['mach'],special['macl'],special['gbr'])==0
  assert native==ram and list(r)==list(s.r) and bytes(f)==bytes(nf) and s.sr==initialsr
  wanted=[initial[15],initial[14],s.pr]+list(reversed(initial[4:14]))+list(reversed(initial[:4]))+[special['mach'],special['macl'],special['gbr']]+list(nf.banks[bank>>21])+[nf.fpul,nf.fpscr]
  assert struct.unpack_from('<38I',native,0xa00000)==tuple(wanted)
  assert r[0]==0 and r[4]==initial[4] and r[14]==initial[14]
  count+=1
 nf.fpscr|=0x100000;before=bytes(nf),bytes(r),bytes(native)
 assert fn(view,ctypes.byref(nf),r,s.pr,nf.fpul,special['mach'],special['macl'],special['gbr'])==-2
 assert (bytes(nf),bytes(r),bytes(native))==before
 rows=(out/'analysis/instructions/input_work_prepare.tsv').read_text().splitlines()[1:];instructions=sum(0x8c110954<=int(x.split('\t')[0],16)<0x8c1109ba for x in rows)
 report=dict(cases_passed=count,instruction_count=instructions,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_context_save.c').read_bytes()).hexdigest(),scope='Full retail RAM/register/FPU comparison with explicit MACH/MACL/GBR; independent saved 38-word context layout, FPSCR bank selection and return-state checks. SZ=0 verified; SZ=1 rejected. Not connected to native scheduler; context restore unfinished.')
 (out/'analysis/input_context_save_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'context save comparisons passed')
if __name__=='__main__':main()
