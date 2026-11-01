"""Differential clock-edge simulation, independent bounds and bus contracts."""
import ctypes,hashlib,json,random
from pathlib import Path
import tmu0_clock_native as n
from tmu0_clock_reference import Clock,ADDRESSES,DIVISORS,MAX64
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/tmu0_clock_verification.json';assert not target.exists()
 rng=random.Random(0x6025e8);checks=operations=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def write(s,a,value,width):return n.LIB.sa_tmu0_write(ctypes.byref(s),a,value,width)
 def read(s,a,width):
  out=n.U(0x13579bdf);rc=n.LIB.sa_tmu0_read(ctypes.byref(s),a,width,ctypes.byref(out));return rc,out.value
 def advance(s,cycles):return n.LIB.sa_tmu0_advance(ctypes.byref(s),cycles)
 for trial in range(600):
  s=n.initialized();r=Clock();require(n.snapshot(s)==r.snapshot(),'reset values')
  for _ in range(45):
   before=bytes(s);old=r.snapshot();kind=rng.randrange(4)
   if kind==0:
    a=rng.choice(tuple(ADDRESSES));width=ADDRESSES[a][1] if rng.random()<.8 else rng.choice((1,2,4,8))
    value=rng.choice((0,1,2,3,4,5,6,7,0x20,0x22,0x100,0x120,0x13f,0x140,0xffffffff,rng.getrandbits(32)))
    rc=write(s,a,value,width);wanted=r.write(a,value,width);require(rc==wanted,'write status')
   elif kind==1:
    a=rng.choice(tuple(ADDRESSES));width=rng.choice((ADDRESSES[a][1],1,2,4,8))
    rc,value=read(s,a,width);wanted,expected=r.read(a,width)
    require(rc==wanted and value==(expected if not wanted else 0x13579bdf),'read result and rejected output preservation')
   else:
    cycles=rng.randrange(5000);rc=advance(s,cycles);wanted=r.advance(cycles);require(rc==wanted,'elapsed-cycle advance status')
   require(n.snapshot(s)==r.snapshot(),'independent edge-by-edge register/counter equality')
   require(n.LIB.sa_tmu0_irq_pending(ctypes.byref(s))==r.pending(),'IRQ level equality')
   if rc:require(bytes(s)==before and r.snapshot()==old,'rejected operation is atomic')
   operations+=1
 for select,divisor in enumerate(DIVISORS):
  for counter in (0,1,4,0xffffffff):
   for reload in (0,1,7,0xffffffff):
    s=n.initialized();r=Clock()
    for a,value,width in ((0xffd80010,select|0x20,2),(0xffd80008,reload,4),(0xffd8000c,counter,4),(0xffd80004,1,1)):
     require(write(s,a,value,width)==r.write(a,value,width)==0,'boundary setup')
    for cycles in (divisor-1,1,divisor,divisor*19+3):
     require(advance(s,cycles)==r.advance(cycles)==0 and n.snapshot(s)==r.snapshot(),'zero/underflow/reload and fractional phase boundaries')
    old=bytes(s);require(read(s,0xffd8000c,4)==(0,s.tcnt) and bytes(s)==old,'reads do not advance time')
    require(write(s,0xffd80010,select|0x20,2)==0 and not n.LIB.sa_tmu0_irq_pending(ctypes.byref(s)),'write-zero clears UNF and pending')
 for _ in range(1000):
  s=n.initialized();select=rng.randrange(5)
  s.tcr=select;s.tstr=1;s.tcor=rng.getrandbits(32);s.tcnt=rng.getrandbits(32);s.peripheral_cycles=rng.randrange(2048)
  other=n.State.from_buffer_copy(s);cycles=rng.randrange(1<<50);split=rng.randrange(cycles+1)
  require(advance(s,cycles)==advance(other,split)==advance(other,cycles-split)==0 and bytes(s)==bytes(other),'large elapsed advance is independent of host chunking')
 for reload,counter in ((0,0),(0xffffffff,0xffffffff)):
  s=n.initialized();s.tcor=reload;s.tcnt=counter;s.tstr=1;s.tcr=0x20
  require(advance(s,MAX64)==0,'largest elapsed argument has no intermediate overflow')
  edges=MAX64//4
  require(s.tcnt==(0 if reload==0 else 0xffffffff-(edges&0xffffffff)) and s.underflows==(edges if reload==0 else edges>>32),'independent huge-interval integer oracle')
  old=bytes(s);require(advance(s,1)==-6 and bytes(s)==old,'elapsed overflow is atomic')
 s=n.initialized();s.tstr=1;s.tcnt=0;s.tcor=0;s.underflows=MAX64
 old=bytes(s);require(advance(s,4)==-6 and bytes(s)==old,'diagnostic underflow-count overflow is atomic')
 for a,width in ADDRESSES.items():
  for nbytes in (1,2,4,8):
   if nbytes==width[1]:continue
   s=n.initialized();old=bytes(s);require(read(s,a,nbytes)==(-3,0x13579bdf) and write(s,a,0,nbytes)==-3 and bytes(s)==old,'all wrong register widths preserve state and output')
 for value in (5,6,7):
  s=n.initialized();require(write(s,0xffd80010,value,2)==0,'store stopped unsupported clock selection')
  old=bytes(s);require(write(s,0xffd80004,1,1)==-2 and bytes(s)==old,'unsupported clock cannot silently start')
 s=n.initialized();require(write(s,0xffd80004,0xffffff01,1)==0 and s.tstr==1,'byte bus ignores unwritten high bits')
 require(n.LIB.sa_tmu0_init(None)==-6 and n.LIB.sa_tmu0_advance(None,0)==-6,'null state contracts')
 require(n.LIB.sa_tmu0_read(ctypes.byref(s),0xffd8000c,4,None)==-6,'null read output contract')
 calls=[]
 @n.READ
 def parent_read(ctx,a,width,out):calls.append(('read',a,width));out[0]=0xdeadbeef;return -7
 @n.WRITE
 def parent_write(ctx,a,value,width):calls.append(('write',a,value,width));return -8
 @n.CACHE
 def parent_cache(ctx,a,op):calls.append(('cache',a,op));return -9
 parent=n.Bus(None,parent_read,parent_write,parent_cache);bound=n.Bus();binding=n.Binding()
 require(n.LIB.sa_tmu0_bind(ctypes.byref(binding),ctypes.byref(s),ctypes.byref(parent),ctypes.byref(bound))==0,'per-entry binding')
 out=n.U();require(bound.read(bound.context,0xa05f810c,4,ctypes.byref(out))==-7 and out.value==0xdeadbeef,'unrelated read preserves parent status/output')
 require(bound.write(bound.context,0xa05f8040,3,4)==-8 and bound.cache(bound.context,0x8c100000,2)==-9,'unrelated writes/cache preserve parent status')
 count=len(calls);require(bound.read(bound.context,0xffd8000c,4,ctypes.byref(out))==0 and out.value==s.tcnt and len(calls)==count,'TMU read owned by native model')
 files=[Path(__file__),P/'tmu0_clock_reference.py',P/'tmu0_clock_native.py',P/'source/tmu0_clock.c',P/'source/tmu0_clock.h',P/'build_tmu0_clock.ps1',n.DLL]
 report=dict(status='native_TMU0_internal_clock_contract_verified',random_operations=operations,random_trials=600,reload_boundary_profiles=80,large_interval_partition_cases=1000,checks=checks,internal_divisors=list(DIVISORS),reads_advance_time=False,elapsed_unit='peripheral PCLK cycles',source='authored platform support, not recovered game code',physical_clock_rate_qualified=False,automatic_VBlank=False,full_game_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,reference='Renesas R01UH0456EJ0702 sections 12.2-12.5; explicit supported-domain policy',limitations=['Only channel 0 and divided internal clocks. RTC/external clocks, other channels, active configuration writes, reset/standby analog phase and SH7750 UNF write-one erratum behavior remain explicit unsupported domains.','Peripheral time is supplied to advance; this core does not assign cycles to guest instructions or generate VBlank/CPU interrupt entry. No read-count-based or wall-clock game timing.'],sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('VERIFIED TMU0',operations,'random operations;',checks,'checks',flush=True)

if __name__=='__main__':main()
