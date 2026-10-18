"""Explicit bank/control state versus independent canonical physical banks."""
import collections,ctypes,hashlib,itertools,json,random
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
DLL=ROOT/'work/sonic-adventure/build/sh4_integer_state.dll'
U=ctypes.c_uint32;P=ctypes.POINTER(U)
class Extra(ctypes.Structure):_fields_=[('mach',U),('other',U*8)]
class Visible(ctypes.Structure):_fields_=[('before',U),('r',U*16),('after',U)]
class Hidden(ctypes.Structure):_fields_=[('before',U),('extra',Extra),('after',U)]
BITS=(0,1,4,5,6,7,8,9,15,28,29,30)
def valid_sr(x):return sum(((x>>b)&1)<<b for b in BITS)
def bank(sr):return 1 if sr&(1<<30) and sr&(1<<29) else 0
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
class Machine:
 def __init__(self,sr,seed):
  rng=random.Random(seed);self.banks=[[rng.getrandbits(32) for _ in range(8)] for _ in range(2)];self.common=[rng.getrandbits(32) for _ in range(8)];self.status=valid_sr(sr);self.lo=rng.getrandbits(32);self.hi=rng.getrandbits(32)
  self.visible=Visible();self.hidden=Hidden();self.visible.before=self.hidden.before=0x1234a55a;self.visible.after=self.hidden.after=0xcafe5678
  self.visible.r[:]=self.registers();self.hidden.extra.other[:]=self.banks[1-bank(self.status)];self.hidden.extra.mach=self.hi;self.sr=U(self.status);self.macl=U(self.lo)
 def registers(self):return self.banks[bank(self.status)]+self.common
 def write(self,n,value):
  if n<8:self.banks[bank(self.status)][n]=value
  else:self.common[n-8]=value
 def step(self,op,n=0,m=1,value=0,delay=0):
  expected=0;priv=bool(self.status&(1<<30));regs=self.registers();extra=ctypes.byref(self.hidden.extra)
  if op=='ldcsr':
   operand=regs[n] if value is None else value
   expected=-5 if delay else -4 if not priv else 0
   if not expected:self.status=valid_sr(operand)
   got=LIB.sa_sh4_ldc_sr(operand,ctypes.byref(self.sr),self.visible.r,extra,delay)
  elif op=='stcsr':
   expected=0 if priv else -5 if delay else -4
   if not expected:self.write(n,self.status)
   out=ctypes.cast(ctypes.byref(self.visible.r,n*4),P);got=LIB.sa_sh4_stc_sr(self.sr.value,out,delay)
  elif op=='ldcbank':
   expected=-6 if n>=8 else 0 if priv else -5 if delay else -4
   if not expected:self.banks[1-bank(self.status)][n]=value
   got=LIB.sa_sh4_ldc_bank(value,n,self.sr.value,extra,delay)
  elif op=='stcbank':
   expected=-6 if n>=8 else 0 if priv else -5 if delay else -4
   if not expected:self.write(m,self.banks[1-bank(self.status)][n])
   out=ctypes.cast(ctypes.byref(self.visible.r,m*4),P);got=LIB.sa_sh4_stc_bank(n,self.sr.value,extra,out,delay)
  else:
   assert op=='dmuls';a,b=regs[n],regs[m];a=a if a<2**31 else a-2**32;b=b if b<2**31 else b-2**32
   result=(a*b)%(2**64);self.lo=result%(2**32);self.hi=result//(2**32)
   LIB.sa_sh4_dmuls_state(regs[n],regs[m],ctypes.byref(self.macl),extra);got=0
  assert got==expected,(op,n,m,hex(self.status),got,expected)
  assert list(self.visible.r)==self.registers() and list(self.hidden.extra.other)==self.banks[1-bank(self.status)] and self.sr.value==self.status and self.macl.value==self.lo and self.hidden.extra.mach==self.hi,(op,n,m,'complete canonical-bank/register/MACH/MACL state')
  assert self.visible.before==self.hidden.before==0x1234a55a and self.visible.after==self.hidden.after==0xcafe5678,'host canaries'
  COUNTS[op]+=1;STATUS[got]+=1
  return got
def main():
 global LIB,COUNTS,STATUS
 LIB=ctypes.CDLL(str(DLL));COUNTS=collections.Counter();STATUS=collections.Counter();ep=ctypes.POINTER(Extra)
 LIB.sa_sh4_integer_bank.argtypes=[U];LIB.sa_sh4_integer_bank.restype=ctypes.c_uint
 LIB.sa_sh4_ldc_sr.argtypes=[U,P,P,ep,ctypes.c_uint];LIB.sa_sh4_ldc_sr.restype=ctypes.c_int
 LIB.sa_sh4_stc_sr.argtypes=[U,P,ctypes.c_uint];LIB.sa_sh4_stc_sr.restype=ctypes.c_int
 LIB.sa_sh4_ldc_bank.argtypes=[U,ctypes.c_uint,U,ep,ctypes.c_uint];LIB.sa_sh4_ldc_bank.restype=ctypes.c_int
 LIB.sa_sh4_stc_bank.argtypes=[ctypes.c_uint,U,ep,P,ctypes.c_uint];LIB.sa_sh4_stc_bank.restype=ctypes.c_int
 LIB.sa_sh4_dmuls_state.argtypes=[U,U,P,ep];LIB.sa_sh4_dmuls_state.restype=None
 states=(0,0x20000000,0x400083f3,0x600083f3)
 targets=(0,0xffffffff,0x40000000,0x60000000,0x20000000,0x80000000,0x700083f3,0xdeadbeef)
 for sr,n,value,delay in itertools.product(states,range(16),targets,(0,1)):Machine(sr,0x400e+n).step('ldcsr',n,value=value,delay=delay)
 for sr,n,delay in itertools.product(states,range(16),(0,1)):Machine(sr,0x0002+n).step('stcsr',n,delay=delay)
 for sr,n,m,delay in itertools.product(states,(*range(8),8,0xffffffff),range(16),(0,1)):
  Machine(sr,0x408e+n).step('ldcbank',n,value=0xdeadbeef,delay=delay)
  Machine(sr,0x0082+n).step('stcbank',n,m,delay=delay)
 edges=(0,1,2,255,256,65535,65536,0x7ffffffe,0x7fffffff,0x80000000,0x80000001,0xfffffffe,0xffffffff)
 for sr,(a,b) in itertools.product(states,itertools.product(edges,repeat=2)):
  x=Machine(sr,a^b);x.write(0,a);x.write(15,b);x.visible.r[:]=x.registers();x.step('dmuls',0,15)
 rng=random.Random(0x400e2000);bank_checks=0
 for seq in range(64):
  x=Machine(0x400083f3|((seq%2)<<29),seq)
  for i in range(128):
   assert LIB.sa_sh4_integer_bank(x.sr.value)==bank(x.status);bank_checks+=1
   op=rng.choice(('ldcsr','stcsr','ldcbank','stcbank','dmuls'))
   x.step(op,n=rng.randrange(8 if 'bank' in op else 16),m=rng.randrange(16),value=(rng.getrandbits(32)|0x40000000),delay=0)
  x.step('ldcsr',value=(rng.getrandbits(32)&~0x40000000));x.step('stcsr',n=seq%16);x.step('ldcsr',value=0xffffffff);x.step('dmuls',seq%16,(seq+1)%16)
 # LDC must capture an operand in the old visible bank before switching.
 for n,sr in itertools.product(range(16),(0x400083f3,0x600083f3)):
  x=Machine(sr,n);x.write(n,sr^0x20000000);x.visible.r[:]=x.registers();x.step('ldcsr',n,value=None)
 files=[Path(__file__),PROJECT/'source/sh4_integer_state.c',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_signed_product.c',PROJECT/'source/sh4_signed_product.h',PROJECT/'source/fpu_cleanup_transfer.h',PROJECT/'build_sh4_integer_state.ps1',DLL]
 report=dict(cases_passed=sum(COUNTS.values()),operation_counts=dict(COUNTS),status_counts=dict(STATUS),stateful_sequences=64,steps_per_sequence=132,bank_selector_checks=bank_checks,canonical_physical_banks=True,complete_state_checked_after_each_operation=True,host_canaries_checked=True,source_sha256=digest(PROJECT/'source/sh4_integer_state.c'),dll_sha256=digest(DLL),newly_recovered_game_entries=0,connected_to_game=False,manual='https://www.renesas.com/en/document/mas/sh-4-software-manual',scope='Native integer extra-state/control kernels versus independent canonical physical banks (no swap algorithm in the oracle), explicit SR fields and unbounded signed multiplication. SR masking, MD/RB combinations, inactive bank reads/writes, source-before-switch, repeated stateful transitions, common-register/MACH/MACL preservation and mutation-free typed privilege/slot faults. Fault delivery/vectoring, interrupts, timing, memory LDC forms, full clippers and hardware execution are not proved. No game recovery credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/sh4_integer_state_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:v for k,v in report.items() if k not in ('sha256','scope')},indent=2))
if __name__=='__main__':main()
