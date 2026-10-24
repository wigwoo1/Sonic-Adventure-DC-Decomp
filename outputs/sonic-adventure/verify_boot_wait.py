"""Original timer/arithmetic/callback bodies and native wait prefixes."""
import collections,csv,ctypes,itertools,json,random,sys,types
from pathlib import Path
import verify_boot_setup as boot
import verify_draw_view_registers as registers
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/boot_wait_translation.json';manifest=json.loads(MANIFEST.read_text())
WAIT,CLOCK,DIFFERENCE,CONVERT,CALLBACK=0x8c6050b0,0x8c60260e,0x8c602618,0x8c602652,0x8c60509a
TIMER,NOP,UNKNOWN,DEVICE=0xffd8000c,0x8c63eeb0,0x8c00f100,0x8c642440
LIMIT=0x28870;TICK_LIMIT=(LIMIT*128+99)//100
ROWS=dict(boot.ROWS);OWN={};PER_BODY={};NEW={int(r['address'],16) for r in manifest['routines']}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
binary=boot.effects.SEED.state.gate.support.morph.MAIN.read_bytes()
assert binary[NOP-0x8c010000:NOP-0x8c010000+4]==bytes.fromhex('0b000900')
ROWS.update({NOP:'rts',NOP+2:'nop'})
code=registers.code;marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if a==TIMER:\n    if c.get('null_bus'):stop(pc,r,sr,-2)\n    if c.get('clock_status'):stop(pc,r,sr,c['clock_status'])\n    if queue_reference.clock_cursor>=len(queue_reference.ticks):stop(pc,r,sr,-4)")
marker='tuple(queue_reference.registers.get(0x005f8000+i*4,0) for i in range(2048))';assert code.count(marker)==1
code=code.replace(marker,marker+',queue_reference.clock_cursor')
namespace=dict(boot.namespace,ROWS=ROWS,TIMER=TIMER,RETURNS=boot.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=boot.namespace['LEGACY']-NEW)
exec(compile(code,'boot_wait_original_model','exec'),namespace)
effects=types.SimpleNamespace(**boot.effects.__dict__);effects.CONNECTED=boot.effects.CONNECTED|NEW|{NOP};effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_wait.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_wait_connected;effects.fn.argtypes=boot.native_fn.argtypes;effects.fn.restype=ctypes.c_int
effects.LIB.sa_render_register_bus_init.argtypes=boot.effects.LIB.sa_render_register_bus_init.argtypes

class ClockReference(registers.RegisterReference):
 def __init__(self,ticks):super().__init__();self.ticks=list(ticks);self.clock_cursor=0;self.clock_reads=[]
 def __contains__(self,a):return a==TIMER or super().__contains__(a)
 def __getitem__(self,a):
  if a!=TIMER:return super().__getitem__(a)
  tick=self.ticks[self.clock_cursor];self.clock_cursor+=1;raw=(0xffffffff-tick)&0xffffffff;self.clock_reads.append((a,raw,4));return raw

def compare(m,s,f,c):
 other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=ClockReference(c.get('ticks',[0x12345678]));ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,ml_expected,gb_expected,mh_expected,hidden,live=effects.reference_run(expected,other,ef,c,tracked=tuple(effects.CONNECTED))
 memory=registers.Memory();bus=effects.Bus();view=effects.RAM_TYPE.from_buffer(actual);effects.LIB.sa_render_register_bus_init(ctypes.byref(memory),view,ctypes.byref(bus))
 native_clock=[];original_read=bus.read
 @dict(effects.Bus._fields_)['read']
 def read(ctx,a,width,out):
  if a!=TIMER:return original_read(ctx,a,width,out)
  if width!=4:return -3
  if c.get('clock_status'):return c['clock_status']
  ticks=c.get('ticks',[0x12345678])
  if len(native_clock)>=len(ticks):return -4
  raw=(0xffffffff-ticks[len(native_clock)])&0xffffffff;out[0]=raw;native_clock.append((a,raw,width));return 0
 bus.read=read
 ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c['gbr']);extra=effects.Extra();extra.mach=c['mach'];extra.other[:]=c['other'];events=[];boundaries=[]
 @effects.OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  events.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,effects.ram_digest(mem),gb.value,extra.mach,tuple(extra.other),tuple(memory.registers),len(native_clock)))
  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15])))
 rc=effects.fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,effects.Fpu.fpul.offset),effects.core.P),ctypes.byref(ml),s.pc,None,observe,None if c.get('null_bus') else ctypes.byref(bus),ctypes.byref(gb),ctypes.byref(extra))
 assert rc==status and events==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==ml_expected and gb.value==gb_expected and extra.mach==mh_expected and list(extra.other)==hidden,(c,'full wait machine/RAM/boundaries',rc,status,hex(s.pc),hex(other.pc),len(events),len(wanted))
 assert native_clock==ref.clock_reads,(c,'exact timer read order',native_clock,ref.clock_reads)
 v=memory.vertex;trace=[(e.kind,e.address,e.value,e.width) for e in v.trace[:v.trace_count]]
 assert trace==ref.events[:4096] and v.trace_dropped==max(0,len(ref.events)-4096) and list(v.queue)==ref.queue and list(v.qacr)==ref.qacr and (v.flushes,v.prefetches,v.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'ordered render/queue effects')
 assert list(memory.registers)==[ref.registers.get(0x005f8000+i*4,0) for i in range(2048)]
 return dict(input=dict(c),status=rc,stop_pc=hex(s.pc),steps=len(path),full_state_match=True,ordered_boundaries_match=True,clock_reads=[list(x) for x in native_clock],boundary_comparisons=len(events),macl=ml.value,final_r14=s.r[14],register_writes=[list(x) for x in ref.events if x[0]==7],independent_checks=0,boundaries=boundaries),path,actual,s,f

def prepare(ram,c):
 m,s,f=boot.prepare(ram,c)
 s.r[4]=c.get('value',c.get('left',0x12345678));s.r[5]=c.get('right',0x87654321)
 callback=0 if c.get('callback','none')=='none' else NOP if c['callback']=='native' else UNKNOWN
 store(m,load(m,0x8c6051d8),callback)
 store(m,load(m,0x8c6051e8),c.get('gate',0));store(m,load(m,0x8c6051f4),c.get('pending',0))
 for literal in (0x8c605200,0x8c605204,0x8c605208,0x8c60520c):store(m,load(m,literal),literal^0x13572468)
 return m,s,f

def semantic(m,actual,initial,initial_f,final,f,record,c):
 if record['status']!=0:return 0
 regs=list(initial.r);entry=initial.pc;wanted=bytearray(m)
 if entry==CLOCK:
  tick=c['ticks'][0];regs[0]=tick;regs[2]=TIMER;regs[3]=(0xffffffff-tick)&0xffffffff
  assert record['clock_reads']==[[TIMER,regs[3],4]]
 elif entry==DIFFERENCE:regs[0]=(initial.r[5]-initial.r[4])&0xffffffff
 elif entry==CONVERT:
  value=initial.r[4];regs[0]=(value*100)//128;regs[2]=(value>>7)*100;regs[3]=0xfffffff9;regs[5]=100
  assert record['macl']==regs[2]
 elif entry==CALLBACK:
  regs[3]=load(m,0x8c6051d8);sp=initial.r[15];store(wanted,sp-4,initial.r[14]);store(wanted,sp-8,initial.pr)
  target=load(m,regs[3]);calls=[b for b in record['boundaries'] if b['target']==hex(NOP)]
  assert len(calls)==int(target!=0)
  if calls:assert calls[0]['pr']=='0x8c6050aa'
 else:return 0
 assert actual==wanted and list(final.r)==regs,(c,'independent full helper RAM/GPR footprint')
 expected_sr=(initial.sr&~1)|int(c.get('callback','none')=='none') if entry==CALLBACK else initial.sr
 assert final.sr==expected_sr and final.pr==initial.pr and bytes(f)==bytes(initial_f)
 return 4 if entry==CONVERT else 3

def wait_checks(m,actual,record,c):
 if record['stop_pc']!=hex(DEVICE):return 0
 assert record['status']==-2
 assert load(actual,load(m,0x8c6051e0))==c['ticks'][0]
 assert load(actual,load(m,0x8c6051f4))==1
 assert all(load(actual,load(m,literal))==load(m,load(m,literal)) for literal in (0x8c605200,0x8c605204,0x8c605208,0x8c60520c))
 callbacks=[b for b in record['boundaries'] if b['target']==hex(NOP)]
 assert len(callbacks)==int(bool(c.get('gate')) and not c.get('pending') and c.get('callback')=='native')
 expected_reads=1;elapsed=0
 if c.get('gate') and c.get('pending'):
  for i,tick in enumerate(c['ticks'][1:],start=2):
   elapsed=(((tick-c['ticks'][0])&0xffffffff)*100)//128
   if elapsed>=LIMIT:expected_reads=i;break
  else:raise AssertionError('A wait prefix reached the device boundary without enough elapsed time')
 assert len(record['clock_reads'])==expected_reads and record['final_r14']==elapsed
 return 5

def cases():
 values=(0,1,127,128,129,0x7fffffff,0x80000000,0xfffffffe,0xffffffff)
 for value,mode in itertools.product(values,range(8)):
  yield dict(entry=CLOCK,ticks=[value],mode=mode)
  yield dict(entry=CONVERT,value=value,mode=mode)
 for left,right,bank in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff),(0,1,0x80000000,0xffffffff),range(2)):yield dict(entry=DIFFERENCE,left=left,right=right,mode=bank*4)
 rng=random.Random(CONVERT)
 for i in range(32):yield dict(entry=CONVERT,value=rng.getrandbits(32),mode=(i%2)*4)
 for callback,mode in itertools.product(('none','native'),range(8)):yield dict(entry=CALLBACK,callback=callback,mode=mode)
 for entry,alias,bank in itertools.product((CLOCK,DIFFERENCE,CONVERT,CALLBACK),(0x0c000000,0xac000000),range(2)):yield dict(entry=entry,value=0xffffffff,left=0xffffffff,right=0,ticks=[0xffffffff],callback='native',alias=alias,mode=bank*4)
 for callback,bank in itertools.product(('unresolved',),range(2)):yield dict(entry=CALLBACK,callback=callback,mode=bank*4)
 for bank,kind in itertools.product(range(2),('missing','rejected','null','exhausted')):
  yield dict(entry=CLOCK,mode=bank*4,ticks=[] if kind=='exhausted' else [7],**({'clock_status':-2 if kind=='missing' else -3} if kind in ('missing','rejected') else {'null_bus':True} if kind=='null' else {}))
 for gate,pending,callback,bank in itertools.product((0,1,0xffffffff),(0,1),('none','native'),range(2)):
  ticks=[100,100,100+TICK_LIMIT] if gate and pending else [100]
  yield dict(entry=WAIT,gate=gate,pending=pending,callback=callback,ticks=ticks,mode=bank*4)
 for start,deltas,bank in itertools.product((0,0xfffffff0),((TICK_LIMIT,),(TICK_LIMIT+1,),(1,127,128,TICK_LIMIT),(0xffffffff,)),range(2)):
  yield dict(entry=WAIT,gate=1,pending=1,callback='native',ticks=[start]+[(start+d)&0xffffffff for d in deltas],mode=bank*4)
 for bank in range(2):yield dict(entry=WAIT,gate=1,pending=1,ticks=[0,0,0],mode=bank*4)
 for entry,stack in itertools.product((WAIT,CALLBACK),(0,0x8d000004)):yield dict(entry=entry,bad_stack=stack,mode=0)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_wait_connection.json',PROJECT/'analysis/boot_setup_verification.json']
 for p in proofs:
  for file,h in json.loads(p.read_text())['sha256'].items():assert boot.digest(file)==h,file
 ram=bytearray(16<<20)
 for p,offset in ((effects.SEED.state.gate.support.morph.MAIN,0x10000),(effects.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 assert load(ram,0x8c6051f8)==LIMIT and load(ram,0x8c60271c)==TIMER
 selected=list(cases())
 if '--failure-check' in sys.argv:selected=[c for c in selected if c.get('clock_status') or c.get('null_bus') or c.get('ticks')==[] or c['entry']==WAIT and c.get('ticks')==[0,0,0]]
 if '--smoke' in sys.argv:selected=[dict(entry=CLOCK,ticks=[0xffffffff],mode=0),dict(entry=CONVERT,value=0xffffffff,mode=0),dict(entry=DIFFERENCE,left=0xffffffff,right=0,mode=0),dict(entry=CALLBACK,callback='native',mode=0),dict(entry=WAIT,gate=1,pending=1,callback='native',ticks=[0,0,TICK_LIMIT],mode=0)]
 records=[];coverage=set();edges=set()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);initial_f=effects.Fpu.from_buffer_copy(f);record,path,actual,final,out=compare(m,s,f,c)
  record['independent_checks']=semantic(m,actual,initial,initial_f,final,out,record,c)+wait_checks(m,actual,record,c)
  records.append(record);coverage.update(path)
  for j,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b):edges.add((a,path[j+1]))
  if (i+1)%64==0:print(i+1,'clock/wait comparisons passed',flush=True)
 prefix=[];boot_coverage=set()
 for bank,active,alias in itertools.product(range(2),(False,True),(0x8c000000,0x0c000000,0xac000000)):
  c=dict(entry=boot.BOOT,mode=bank*4,active=active,alias=alias,gate=1,pending=1,ticks=[0xfffffff0,(0xfffffff0+TICK_LIMIT)&0xffffffff])
  m,s,f=prepare(ram,c);record,path,actual,*_=compare(m,s,f,c);assert record['status']==-2 and record['stop_pc']==hex(DEVICE)
  record['independent_checks']=wait_checks(m,actual,record,c);prefix.append(record);boot_coverage.update(set(path)&set(boot.geometry.PER_BODY[boot.BOOT]))
 if '--smoke' in sys.argv:print('Clock/wait smoke passed:',len(records),'direct +',len(prefix),'boot prefixes');return
 if '--failure-check' in sys.argv:print('Clock failure checks passed:',len(records),'direct +',len(prefix),'boot prefixes');return
 coverage_rows=[]
 for a,body in PER_BODY.items():
  routine=next(r for r in manifest['routines'] if int(r['address'],16)==a);wanted={tuple(int(x,16) for x in edge) for edge in routine['conditional_edges']}
  coverage_rows.append(dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 assert all(not row['missing_rows'] and not row['missing_edges'] for row in coverage_rows if int(row['address'],16)!=WAIT)
 wait_row=next(r for r in coverage_rows if int(r['address'],16)==WAIT);assert wait_row['missing_rows'] and not wait_row['missing_edges']
 assert len(records)==304 and collections.Counter(r['status'] for r in records)=={0:248,-2:46,-3:6,-4:4}
 assert wait_row['covered_rows']==46 and len(boot_coverage)==16
 files=[Path(__file__),PROJECT/'verify_boot_setup.py',PROJECT/'verify_draw_view_registers.py',PROJECT/'verify_startup.py']+proofs
 for p in proofs:files.extend(Path(file) for file in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=boot.digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,independent_checks=sum(r['independent_checks'] for r in records),boundary_comparisons=sum(r['boundary_comparisons'] for r in records),timer_read_events=sum(len(r['clock_reads']) for r in records),boot_parent_rows_covered=len(boot_coverage),wait_rows_covered=wait_row['covered_rows'],helper_rows_covered=sum(row['covered_rows'] for row in coverage_rows if int(row['address'],16)!=WAIT),boot_prefix_independent_checks=sum(r['independent_checks'] for r in prefix),boot_prefix_cases=len(prefix),boot_prefix_stops=dict(collections.Counter(r['stop_pc'] for r in prefix)),wait_recovery_credit=0,external_game_service_fixtures=0,dll_sha256=boot.digest(effects.DLL),sha256={str(p):boot.digest(p) for p in files},scope='Original clock, wrapped difference, elapsed conversion and optional callback bodies, plus actual wait/setup and outer boot prefixes. Full reached/final RAM/machine/FPU/banks/MACH/MACL/GBR/register map, ordered native boundaries and every timer read match the instruction model. Independent arithmetic/footprint/callback-return/counter checks.',limitations=['Hardware timer samples are finite explicit peripheral fixtures. No physical timer frequency, interrupt timing or frame-rate behavior is proved. Exhausted samples stop explicitly with -4; they do not fabricate elapsed time or a successful wait.','The wait/setup entry remains partial at its first unresolved frame/device call 8c642440, with zero parent recovery credit. The later 8c641f00/8c601e2a calls and final snapshot have not executed.','The known callback is the actual byte-proved RTS/NOP function; arbitrary callback bodies remain explicit unresolved targets.','Qualified privileged bank-1/FD=0, MMU-off RAM and prior numerical/backend domains apply. Typed memory/peripheral failures are not complete hardware exception delivery. Natural boot order, visible rendering, gameplay/settings and ports remain incomplete.'],cases=records,boot_prefix=prefix)
 (PROJECT/'analysis/boot_wait_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','coverage_by_routine','independent_checks','timer_read_events','boot_prefix_cases','boot_prefix_stops')},indent=2))

if __name__=='__main__':main()
