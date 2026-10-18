import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_zero_queries.dll'))
@Call
def call(*args):raise AssertionError('Unexpected external call')
cases=0
for entry,name in [(0x8c66869a,'sa_sound_driver_zero_query_a'),(0x8c6686a4,'sa_sound_driver_zero_query_b')]:
 fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 for alias,bank,sr0 in itertools.product([False,True],[0,0x200000],[0,0x70000063]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa20000:0xa20010]=bytes([0xa5])*16
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[5]=0x8ca20004;s.r[6]=s.r[5] if alias else 0x8ca20008;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
  expected=bytearray(bytes([0xa5])*16);expected[4:8]=bytes(4)
  if not alias:expected[8:12]=bytes(4)
  assert native[0xa20000:0xa20010]==expected and r[0]==r[4]==0
  cases+=1
(out/'analysis/sound_driver_zero_queries_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Two native five-instruction zero-output queries66869a/6686a4. Full retail state matched for distinct/aliased valid output pointers,banks/SR. Independent zero output words,neighbor preservation,R0/R4 zero checked. Null/invalid outputs excluded; query purpose unresolved.'),indent=2)+'\n')
print(cases,'zero output query comparisons passed')
