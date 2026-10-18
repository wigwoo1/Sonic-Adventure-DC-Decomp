"""Original renderer register access with an independent bounded host register map."""
import collections,csv,ctypes,itertools,json,sys,types
from pathlib import Path
import verify_draw_view_setup as previous
import verify_overlay_draw_clipping as machine
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/draw_view_registers_translation.json';manifest=json.loads(MANIFEST.read_text())
WRAPPER,WRITE,COPY=0x8c652d3a,0x8c653444,0x8c10cddc
ROWS=dict(previous.ROWS);OWN={}
for r in manifest['routines']:
 body={int(x['address'],16):x['instruction'].lstrip('_') for x in csv.DictReader(Path(r['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body)
COPY_LISTING=PROJECT/'analysis/overlay-model-blend-copy/8c10cddc_instructions.tsv'
for x in csv.DictReader(COPY_LISTING.open(),delimiter='\t'):ROWS[int(x['address'],16)]=x['instruction'].lstrip('_')
code=previous.previous.code
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if a in queue_reference and register_address(a) and c.get('null_bus'):stop(pc,r,sr,-2)\n   if a in queue_reference and register_address(a) and c.get('reject_register'):stop(pc,r,sr,-3)")
marker="special['mach'],tuple(special['other'])";assert code.count(marker)==1
code=code.replace(marker,marker+",tuple(queue_reference.registers.get(0x005f8000+i*4,0) for i in range(2048))")
marker='  access=dynamic_access(text,r,f.fpscr,special)';assert code.count(marker)==1
code=code.replace(marker,"  if text.startswith(('fadd ','fsub ','fmul ','fdiv ','fmac ')) and f.fpscr&3>1:stop(pc,r,sr,-1)\n"+marker)
def register_address(a):return not a&3 and 0x005f8000<=a&0x1fffffff<0x005fa000
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-{WRAPPER,WRITE},register_address=register_address)
exec(compile(code,'view_register_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|{WRAPPER,WRITE,COPY};effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=previous.previous.SEED.ROOT/'work/sonic-adventure/build/draw_view_registers.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_draw_view_registers_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
Bus,VertexMemory=effects.Bus,effects.Memory
class Memory(ctypes.Structure):_fields_=[('vertex',VertexMemory),('vertex_bus',Bus),('registers',ctypes.c_uint32*2048)]
effects.LIB.sa_render_register_bus_init.argtypes=[ctypes.POINTER(Memory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Bus)]
class RegisterReference(effects.QueueReference):
 def __init__(self):super().__init__();self.registers={}
 def __contains__(self,a):return register_address(a) or super().__contains__(a)
 def __getitem__(self,a):
  if not register_address(a):return super().__getitem__(a)
  value=self.registers.get(a&0x1fffffff,0);self.events.append((8,a,value,4));return value
 def __setitem__(self,a,value):
  if not register_address(a):return super().__setitem__(a,value)
  value&=0xffffffff;self.events.append((7,a,value,4));self.registers[a&0x1fffffff]=value

def compare(m,s,f,c):
 other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=RegisterReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,ml_expected,gb_expected,mh_expected,hidden,live=effects.reference_run(expected,other,ef,c,tracked=tuple(effects.CONNECTED))
 memory=Memory();bus=Bus();view=effects.RAM_TYPE.from_buffer(actual);effects.LIB.sa_render_register_bus_init(ctypes.byref(memory),view,ctypes.byref(bus))
 original_write=bus.write
 @dict(Bus._fields_)['write']
 def reject(ctx,address,value,width):return -3 if register_address(address) else original_write(ctx,address,value,width)
 if c.get('reject_register'):bus.write=reject
 ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c['gbr']);extra=effects.Extra();extra.mach=c['mach'];extra.other[:]=c['other'];events=[];pending=[];copy_checks=[];copy_errors=[]
 def check_copy():
  if not pending:return
  address,expected_bytes=pending.pop()
  got=bytes(actual[address&0xffffff:(address&0xffffff)+len(expected_bytes)])
  if got!=expected_bytes:copy_errors.append((hex(address),'independent native copy bytes'))
  copy_checks.append(len(expected_bytes))
 @effects.OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  events.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,effects.ram_digest(mem),gb.value,extra.mach,tuple(extra.other),tuple(memory.registers)))
  try:
   check_copy()
   if target==COPY and c.get('check_view_copies'):
    count=r[0];source=r[2]&0xffffff;assert count==192 and source+count<=len(actual)
    pending.append((r[1],bytes(actual[source:source+count])))
  except Exception as exc:copy_errors.append(repr(exc))
 rc=effects.fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,effects.Fpu.fpul.offset),effects.core.P),ctypes.byref(ml),s.pc,None,observe,None if c.get('null_bus') else ctypes.byref(bus),ctypes.byref(gb),ctypes.byref(extra))
 assert rc==status and events==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==ml_expected and gb.value==gb_expected and extra.mach==mh_expected and list(extra.other)==hidden,(c,'full register-service machine/RAM/boundaries',rc,status,hex(s.pc),hex(other.pc),len(events),len(wanted))
 if rc==0:check_copy()
 assert not copy_errors,(c,copy_errors)
 v=memory.vertex;emitted=[(e.kind,e.address,e.value,e.width) for e in v.trace[:v.trace_count]]
 assert emitted==ref.events[:4096] and v.trace_dropped==max(0,len(ref.events)-4096) and list(v.queue)==ref.queue and list(v.qacr)==ref.qacr and (v.flushes,v.prefetches,v.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'ordered register/queue state')
 assert list(memory.registers)==[ref.registers.get(0x005f8000+i*4,0) for i in range(2048)],(c,'full independent register map')
 return dict(input=dict(c),status=rc,stop_pc=hex(s.pc),steps=len(path),full_state_match=True,ordered_boundaries_match=True,queue_match=True,register_map_match=True,bank0_live_in=[[hex(a),i] for a,i in live],independent_copy_lengths=copy_checks,boundary_comparisons=len(events),queue_events=len(ref.events),register_writes=[list(e) for e in ref.events if e[0]==7]),path,actual,s,f,memory

def prepare(ram,c):
 _,s,f=previous.previous.SEED.prepare(ram,dict(entry=c['entry'],mode=c.get('mode',0)));m=bytearray(ram);s.pc=c['entry'];s.sr=0x600003f3;c['gbr']=0;machine.extra(c,0x653444)
 s.r[4]=c.get('offset',0x78);s.r[5]=c.get('value',0x12345678)
 f.banks[(f.fpscr>>21)&1][4]=c.get('value',0x12345678);f.banks[(f.fpscr>>21)&1][5]=s.pr
 if c.get('alias'):s.r[15]=(s.r[15]&0xffffff)|c['alias']
 if c.get('bad')=='stack':s.r[15]=0
 return m,s,f

def semantic(m,actual,initial,fp,final,c,mem):
 wanted=bytearray(m)
 if initial.pc==WRAPPER:
  raw=fp.banks[(fp.fpscr>>21)&1][4];store(wanted,initial.r[15]-4,initial.pr);store(wanted,initial.r[15]-8,raw)
  expected=(7,0xa05f8078,raw&0x7fffffff,4)
  assert final.r[0]==0 and final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent wrapper result/stack')
 else:
  address=(initial.r[4]+0xa05f8000)&0xffffffff;expected=(7,address,initial.r[5],4)
  assert final.r[0]==1 and final.r[3]==0xa05f8000 and final.r[4]==address and final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent register writer state')
 assert actual==wanted and mem.vertex.trace_count==1 and (mem.vertex.trace[0].kind,mem.vertex.trace[0].address,mem.vertex.trace[0].value,mem.vertex.trace[0].width)==expected,(c,'independent RAM footprint/exact register write')
 assert mem.registers[((expected[1]&0x1fffffff)-0x005f8000)//4]==expected[2],(c,'independent stored register')
 return 3

def bus_checks():
 ram=bytearray(16<<20);view=effects.RAM_TYPE.from_buffer(ram);mem=Memory();ctypes.memset(ctypes.byref(mem),0xa5,ctypes.sizeof(mem));bus=Bus();effects.LIB.sa_render_register_bus_init(ctypes.byref(mem),view,ctypes.byref(bus));ref=RegisterReference();ref.ram=bytearray(ram);word=ctypes.c_uint32();operations=0
 assert list(mem.registers)==[0]*2048 and list(mem.vertex.qacr)==[0,0] and list(mem.vertex.queue)==[0]*16 and (mem.vertex.trace_count,mem.vertex.trace_dropped,mem.vertex.flushes,mem.vertex.prefetches,mem.vertex.invalidates)==(0,0,0,0,0)
 for alias in (0,0x80000000,0xa0000000):
  for i in range(2048):
   a=alias|0x005f8000|i*4;assert bus.read(bus.context,a,4,ctypes.byref(word))==0 and word.value==ref[a];operations+=1
   value=(i*0x1234567)^alias;assert bus.write(bus.context,a,value,4)==0;ref[a]=value;operations+=1
   assert bus.read(bus.context,a,4,ctypes.byref(word))==0 and word.value==ref[a];operations+=1
 for a,value in ((0xff000038,12),(0xff00003c,12),*((0xe0001000+i*4,0x10203040+i) for i in range(8))):assert bus.write(bus.context,a,value,4)==0;ref[a]=value;operations+=1
 assert bus.cache(bus.context,0xe0001000,1)==0;ref.prefetch(0xe0001000);operations+=1
 for a,width in ((0xa05f8001,4),(0xa05fa000,4),(0xa05f8000,8),(0xa05f8000,2)):
  before=bytes(mem);assert bus.write(bus.context,a,0x12345678,width)==-3 and bytes(mem)==before;operations+=1
 v=mem.vertex;assert list(mem.registers)==[ref.registers[0x005f8000+i*4] for i in range(2048)]
 assert [(e.kind,e.address,e.value,e.width) for e in v.trace[:v.trace_count]]==ref.events[:4096] and v.trace_dropped==len(ref.events)-4096 and list(v.queue)==ref.queue and list(v.qacr)==ref.qacr and ram==ref.ram
 return dict(operations_passed=operations,register_words=2048,aliases=3,events=len(ref.events),stored_event_prefix=4096,later_event_count=v.trace_dropped,invalid_writes_mutation_free=4,queue_flushes=v.flushes)

def cases():
 values=(0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0xffc54321,1,0xffffffff)
 for value,mode in itertools.product(values,range(8)):yield dict(entry=WRAPPER,value=value,mode=mode)
 for offset,value in itertools.product((0,0x78,0x1ffc,0xe0000078,0x60000078),values):yield dict(entry=WRITE,offset=offset,value=value,mode=0)
 for entry,alias in itertools.product((WRAPPER,WRITE),(0x0c000000,0xac000000)):yield dict(entry=entry,alias=alias,value=0xffc54321,mode=0)
 for entry,failure in itertools.product((WRAPPER,WRITE),('null_bus','reject_register')):yield dict(entry=entry,value=0xdeadbeef,mode=0,**{failure:True})
 for offset in (1,0x2000,0xffffffff):yield dict(entry=WRITE,offset=offset,value=0xdeadbeef,mode=0)
 yield dict(entry=WRAPPER,bad='stack',value=0x12345678,mode=0)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_view_registers_connection.json',PROJECT/'analysis/draw_view_setup_verification.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((previous.previous.SEED.state.gate.support.morph.MAIN,0x10000),(previous.previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:8]
 records=[];coverage=set();targets=collections.Counter()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);r,path,actual,final,_,mem=compare(m,s,f,c)
  r['independent_checks']=semantic(m,actual,initial,fp,final,c,mem) if r['status']==0 else 0;records.append(r);coverage.update(path);targets.update(a for a in path if a in effects.CONNECTED)
  if (i+1)%32==0:print(i+1,'original register-service comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Register-service smoke passed:',len(records));return
 backend=bus_checks();files=[Path(__file__),PROJECT/'verify_draw_view_setup.py',COPY_LISTING]+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=machine.digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),rows_static=len(OWN),missing_rows=[hex(a) for a in sorted(set(OWN)-coverage)],independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),bus_verification=backend,target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),register_write_events=sum(len(r['register_writes']) for r in records),newly_registered_entries=0,reused_routine_credit=0,dll_sha256=machine.digest(effects.DLL),sha256={str(p):machine.digest(p) for p in files},scope='Two existing original register services adapted to explicit guarded bus/state ABI, with bit-exact reached/final machine/RAM/banks/MACH/GBR, complete register map at entries and ordered register/queue events. Independent raw sign-mask, exact bus address/value, registers, stack and whole RAM footprints; bounded backend alias/read/write/queue/overflow/failure checks.',limitations=['Zero new original routine credit: prior original sources/APIs/proofs remain preserved; guarded adapters are separately qualified.','Register model is explicit bounded host storage, not physical GPU execution, status-register semantics, command consumption, timing or pixels.','Normal CPU state privileged bank 1 with FD=0. Raw moves preserve bit patterns; paired-move direct tests deliberately place the original PR in the adjacent floating word so their original stack overwrite remains returnable. The real view tests separately exercise its actual paired-move failure.','Rejected/missing bus and invalid-access outcomes are explicit typed host stops with reached partial state; no full hardware exception delivery claim.'],cases=records)
 assert len(records)==142 and report['rows_covered']==report['rows_static']==19 and not report['missing_rows']
 assert report['status_counts']=={0:134,-2:2,-3:6} and report['independent_cases']==134 and report['independent_checks']==402
 assert backend==dict(operations_passed=18447,register_words=2048,aliases=3,events=18444,stored_event_prefix=4096,later_event_count=14348,invalid_writes_mutation_free=4,queue_flushes=1)
 (PROJECT/'analysis/draw_view_registers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','missing_rows','independent_cases','independent_checks','bus_verification')},indent=2))
if __name__=='__main__':main()
