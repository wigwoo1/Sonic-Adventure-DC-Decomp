"""Authored architecture support versus independent physical-register banks."""
import ctypes,hashlib,itertools,json,random
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];U=ctypes.c_uint32;UP=ctypes.POINTER(U)
class Extra(ctypes.Structure):_fields_=[('mach',U),('other',U*8)]
class ExceptionState(ctypes.Structure):_fields_=[(n,U) for n in ('ssr','spc','sgr','expevt','intevt','tra','returned')]
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,ctypes.c_uint,UP)
Write=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,U,ctypes.c_uint)
Cache=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,U,ctypes.c_uint)
class Bus(ctypes.Structure):_fields_=[('context',ctypes.c_void_p),('read',Read),('write',Write),('cache',Cache)]
class Memory(ctypes.Structure):_fields_=[('parent',Bus),('state',ctypes.POINTER(ExceptionState)),('expevt_reads',U),('intevt_reads',U),('tra_reads',U)]
def bank(sr):return 1 if sr&0x40000000 and sr&0x20000000 else 0
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 dll=ROOT/'work/sonic-adventure/build/sh4_exception_state.dll';lib=ctypes.CDLL(str(dll));ep=ctypes.POINTER(ExceptionState);xp=ctypes.POINTER(Extra)
 lib.sa_sh4_interrupt_enter.argtypes=[ep,UP,UP,xp,UP,U,U,ctypes.c_uint,ctypes.c_uint]
 lib.sa_sh4_rte_begin.argtypes=[ep,UP,UP,xp,UP,ctypes.c_uint]
 lib.sa_sh4_exception_stc.argtypes=[ep,ctypes.c_uint,U,UP,ctypes.c_uint]
 lib.sa_sh4_exception_ldc.argtypes=[ep,ctypes.c_uint,U,U,ctypes.c_uint]
 lib.sa_exception_bus_init.argtypes=[ctypes.POINTER(Memory),ctypes.POINTER(Bus),ep,ctypes.POINTER(Bus)]
 rng=random.Random(0x65e80c);accepted=pending=register_cases=rte_cases=bus_checks=0
 for high,mask,blocked,priority,delay in itertools.product((0,0x20000000,0x40000000,0x60000000),range(16),(0,1),(1,6,15),(0,1)):
  initial=high|0x8303|(mask<<4)|(blocked<<28);physical=[[rng.getrandbits(32) for _ in range(8)] for _ in range(2)];common=[rng.getrandbits(32) for _ in range(7)]+[0x8cf00000]
  registers=(U*16)(*(physical[bank(initial)]+common));extra=Extra(0x12345678,(U*8)(*physical[1-bank(initial)]));sr=U(initial);pc=U(0x8cfed000);e=ExceptionState(1,2,3,4,5,6,7)
  before=(bytes(registers),bytes(extra),sr.value,pc.value,bytes(e))
  result=lib.sa_sh4_interrupt_enter(ctypes.byref(e),ctypes.byref(sr),registers,ctypes.byref(extra),ctypes.byref(pc),0x8ce50000,0x360,priority,delay)
  if delay or blocked or mask>=priority:
   assert result==1 and before==(bytes(registers),bytes(extra),sr.value,pc.value,bytes(e));pending+=1;continue
  assert result==0 and sr.value==initial|0x70000000 and pc.value==0x8ce50600
  assert list(registers)==physical[1]+common and list(extra.other)==physical[0] and extra.mach==0x12345678
  assert (e.ssr,e.spc,e.sgr,e.expevt,e.intevt,e.tra,e.returned)==(initial,0x8cfed000,0x8cf00000,4,0x360,6,0)
  target=U(0xaaaaaaaa);assert lib.sa_sh4_rte_begin(ctypes.byref(e),ctypes.byref(sr),registers,ctypes.byref(extra),ctypes.byref(target),0)==0
  assert target.value==0x8cfed000 and sr.value==initial and bytes(registers)==before[0] and bytes(extra)==before[1] and not e.returned
  accepted+=1
 for current,saved,delay in itertools.product((0,0x20000000,0x40000000,0x60000000),(0,0x20008303,0x400083f3,0x60008363,0xffffffff,0xdeadbeef),(0,1)):
  physical=[[rng.getrandbits(32) for _ in range(8)] for _ in range(2)];common=[rng.getrandbits(32) for _ in range(8)]
  registers=(U*16)(*(physical[bank(current)]+common));extra=Extra(0x11223344,(U*8)(*physical[1-bank(current)]));sr=U(current);e=ExceptionState(saved,0x8c0546c0,3,4,5,6,0);target=U(0xaaaaaaaa)
  before=(bytes(registers),bytes(extra),sr.value,bytes(e),target.value);status=-5 if delay else -4 if not current&0x40000000 else 0
  assert lib.sa_sh4_rte_begin(ctypes.byref(e),ctypes.byref(sr),registers,ctypes.byref(extra),ctypes.byref(target),delay)==status
  if status:assert before==(bytes(registers),bytes(extra),sr.value,bytes(e),target.value)
  else:assert list(registers)==physical[bank(saved)]+common and list(extra.other)==physical[1-bank(saved)] and sr.value==saved and target.value==e.spc
  rte_cases+=1
 for sr,which,value,delay in itertools.product((0,0x20000000,0x40000000,0x60000000),range(4),(0,0xffffffff,0x8cfed000,0x600083f3),(0,1)):
  e=ExceptionState(0x40008303,0x8c0546c0,0x8cf00000,4,5,6,7);before=bytes(e);out=U(0xa5a5a5a5)
  status=-6 if which>2 else 0 if sr&0x40000000 else -5 if delay else -4
  assert lib.sa_sh4_exception_stc(ctypes.byref(e),which,sr,ctypes.byref(out),delay)==status
  assert bytes(e)==before and out.value==((e.ssr,e.spc,e.sgr)[which] if not status else 0xa5a5a5a5)
  status=-6 if which>1 else 0 if sr&0x40000000 else -5 if delay else -4
  assert lib.sa_sh4_exception_ldc(ctypes.byref(e),which,value,sr,delay)==status
  if status:assert bytes(e)==before
  elif which==0:assert e.ssr==value and e.spc==0x8c0546c0
  else:assert e.spc==value and e.ssr==0x40008303
  register_cases+=2
 forwarded=[]
 @Read
 def read(ctx,a,n,out):forwarded.append(('read',ctx,a,n));out[0]=0x12345678;return -7
 @Write
 def write(ctx,a,value,n):forwarded.append(('write',ctx,a,value,n));return -8
 @Cache
 def cache(ctx,a,op):forwarded.append(('cache',ctx,a,op));return -9
 parent=Bus(0x1234,read,write,cache);memory=Memory();bus=Bus();e=ExceptionState(1,2,3,0x180,0x360,0x7fc,0)
 assert lib.sa_exception_bus_init(ctypes.byref(memory),ctypes.byref(parent),ctypes.byref(e),ctypes.byref(bus))==0
 for address,value in ((0xff000020,0x7fc),(0xff000024,0x180),(0xff000028,0x360)):
  out=U(0xaaaaaaaa);assert bus.read(bus.context,address,4,ctypes.byref(out))==0 and out.value==value;bus_checks+=1
  for width in (1,2,8):
   out.value=0xaaaaaaaa;assert bus.read(bus.context,address,width,ctypes.byref(out))==-3 and out.value==0xaaaaaaaa;bus_checks+=1
  assert bus.write(bus.context,address,0,4)==-3;bus_checks+=1
 assert (memory.expevt_reads,memory.intevt_reads,memory.tra_reads)==(1,1,1);bus_checks+=1
 out=U();assert bus.read(bus.context,0x8cd00000,4,ctypes.byref(out))==-7 and out.value==0x12345678;bus_checks+=1
 assert bus.write(bus.context,0xa05f7800,7,4)==-8 and bus.cache(bus.context,0x8cd00000,2)==-9;bus_checks+=2
 assert forwarded==[('read',0x1234,0x8cd00000,4),('write',0x1234,0xa05f7800,7,4),('cache',0x1234,0x8cd00000,2)];bus_checks+=1
 files=[Path(__file__),P/'build_sh4_exception_state.ps1',P/'source/sh4_exception_state.c',P/'source/sh4_exception_state.h',P/'source/sh4_integer_state.c',P/'source/sh4_integer_state.h']
 report=dict(accepted_interrupt_roundtrips=accepted,pending_unchanged_controls=pending,rte_begin_cases=rte_cases,register_access_cases=register_cases,bus_checks=bus_checks,total_cases=accepted+pending+rte_cases+register_cases+bus_checks,newly_registered_entries=0,dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files},sources=['https://www.renesas.com/en/document/mas/sh-4-software-manual','https://www.renesas.com/en/document/mah/sh7750-sh7750s-sh7750r-group-users-manual-hardware'],scope='Authored CPU/platform support checked against independent canonical physical banks and documented ordinary interrupt/RTE/register semantics. Pending BL/IMASK/delay controls preserve complete state. RTE begin restores SR/banks and captures SPC; the original delay slot remains the caller responsibility. Software interrupt acceptance is explicit and untimed, not physical execution, game-code recovery or automatic runtime delivery.')
 (P/'analysis/sh4_exception_state_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k not in ('sha256','sources','scope')})
if __name__=='__main__':main()
