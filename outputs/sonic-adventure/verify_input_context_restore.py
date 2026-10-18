import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_context_restore.dll')).sa_input_context_restore
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P];count=0
 for seed,bank,savedbank,value in itertools.product(range(4),[0,0x200000],[0,0x200000],[0,1,0xffffffff]):
  rng=random.Random(seed);ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  words=[rng.getrandbits(32) for i in range(38)];words[0]=0x8cef0000;words[2]=0x8c000100;words[37]=savedbank|0x40001;struct.pack_into('<38I',ram,0xa00000,*words)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank
  for b in range(2):
   for i in range(16):f.banks[b][i]=rng.getrandbits(32)
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[rng.getrandbits(32) for i in range(16)];s.r[4]=0x8ca00000;s.r[5]=value;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c1108e0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();special={x:0 for x in ['mach','macl','gbr']};ns=(ctypes.c_uint32*3)()
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),r,ctypes.byref(sr),ctypes.byref(pr),ctypes.byref(fpul),ns)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and list(ns)==[special[x] for x in ['mach','macl','gbr']]
  assert r[0]==(value or 1) and r[15]==words[0] and pr.value==words[2] and nf.fpscr==words[37] and fpul.value==words[36]
  assert list(nf.banks[bank>>21])==words[20:36] and list(ns)==words[17:20]
  count+=1
 rows=(out/'analysis/instructions/input_context_restore.tsv').read_text().splitlines()[1:];instructions=sum(int(x.split('\t')[0],16)<0x8c110950 for x in rows)
 report=dict(cases_passed=count,instruction_count=instructions,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_context_restore.c').read_bytes()).hexdigest(),scope='Full retail state comparison; independent saved context/FPU bank and return-value checks. SZ=0 tested; continuation equals harness exit. Connected nonlocal continuation transfer still unfinished.')
 (out/'analysis/input_context_restore_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'context restore comparisons passed')
if __name__=='__main__':main()
