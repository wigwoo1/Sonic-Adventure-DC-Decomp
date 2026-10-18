"""Original motion mesh preprocessing through the native material/queue chain."""
import collections,csv,ctypes,hashlib,inspect,itertools,json,re,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_motion_formats as previous
import verify_main_dispatch_mesh_families as renderer
import verify_startup as startup
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/draw_motion_mesh_translation.json';manifest=json.loads(MANIFEST.read_text())
PREP,WRAP,LOWER=0x8c617c34,0x8c621574,0x8c6215ba
NEW={PREP,WRAP};ROWS=dict(previous.ROWS);OWN={};LISTINGS=[]
for r in manifest['routines']:
 for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t'):
  a=int(row['address'],16);OWN[a]=row['instruction'].lstrip('_');ROWS[a]=OWN[a]
connection=json.loads((PROJECT/'analysis/draw_motion_mesh_connection.json').read_text())
compiled={Path(p).name for p in connection['sha256'] if p.endswith('.c')}
registry=json.loads((PROJECT/'analysis/recovered_routines.json').read_text(encoding='utf-8-sig'))
for r in registry['routines']:
 if r.get('module','1ST_READ.BIN')!='1ST_READ.BIN' or Path(r.get('source','')).name not in compiled or 'instruction_inventory' not in r:continue
 listing=PROJECT/r['instruction_inventory'];LISTINGS.append(listing)
 for row in csv.DictReader(listing.open(),delimiter='\t'):ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
lower=(PROJECT/'source/sa_draw_motion_mesh_lower_connected.c').read_text()
LOWER_TARGETS={int(a,16) for a in re.findall(r'(?:target==|ROUTE\()(0x[0-9a-f]+)u',lower)}
CONNECTED=previous.CONNECTED|NEW|LOWER_TARGETS
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
# Extend the established original-opcode executor only for device transfers/PREF
# and pre-trace MACL publication. No guest function or result is injected.
cpu_code=inspect.getsource(startup.reference)
cpu_code=cpu_code.replace('cache_purge=None, trace=None):','cache_purge=None, trace=None, cache_prefetch=None):')
marker='        if trace is not None:';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,"        if special is not None:special['macl']=macl\n"+marker)
marker="            struct.pack_into('<I', ram, index(regs[n]), regs[m])";assert cpu_code.count(marker)==1;cpu_code=cpu_code.replace(marker,'            write32(regs[n],regs[m])')
marker='        elif word == 9: pass';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,'        elif word & 0xF0FF == 0x0083:\n            assert cache_prefetch is not None\n            cache_prefetch(regs[n])\n'+marker)
cpu_ns=dict(startup.reference.__globals__);exec(compile(cpu_code,'motion_mesh_original_device_executor','exec'),cpu_ns)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,reference=cpu_ns['reference'],queue_reference=None)
code=previous.code.replace('if a&0x1f000000!=0xc000000 or (a&0xffffff)+n>0x1000000:','if a not in queue_reference and (a&0x1f000000!=0xc000000 or (a&0xffffff)+n>0x1000000):')
marker='instruction_budget=400000)';assert code.count(marker)==1
code=code.replace(marker,'instruction_budget=2000000,mmio=queue_reference,cache_invalidate=queue_reference.invalidate,cache_writeback=queue_reference.writeback,cache_prefetch=queue_reference.prefetch)')
exec(compile(code,'motion_mesh_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=previous.previous.ROOT/'work/sonic-adventure/build/draw_motion_mesh.dll';LIB=ctypes.CDLL(str(DLL));Bus,Memory,QueueReference=renderer.Bus,renderer.Memory,renderer.QueueReference
fn=LIB.sa_draw_motion_mesh_connected;fn.argtypes=previous.fn.argtypes+[ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=[ctypes.POINTER(Memory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Bus)]
State,Fpu,OBSERVE=previous.State,previous.Fpu,previous.OBSERVE
VIEW,VIEW_SLOT,BASIS,LIGHT=0x8cf30000,0x8c8ea3e0,0x8c8e9f20,0x8c89023c
bits,value=previous.bits,previous.value

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def runtime(m,c):
 store(m,VIEW_SLOT,VIEW)
 identity=[Fraction(1 if i%5==0 else 0) for i in range(16)]
 a=list(identity);b=list(identity)
 if c.get('matrix',0):
  a[0]=Fraction(1,2);a[1]=Fraction(1,4);a[4]=Fraction(-1,2);a[5]=2;a[8]=Fraction(1,4);a[10]=Fraction(3,2)
  b[0]=2;b[2]=Fraction(1,2);b[4]=Fraction(1,4);b[5]=Fraction(3,4);b[8]=Fraction(-1,4);b[9]=Fraction(1,2)
 for i,v in enumerate(a):store(m,VIEW+i*4,bits(v))
 for i,v in enumerate(b):store(m,BASIS+i*4,bits(v))
 for i,v in enumerate(c.get('light',(Fraction(1,4),Fraction(1,2),Fraction(-1,4)))):store(m,VIEW+0x4c+i*4,bits(v))
 store(m,VIEW+0x128,bits(Fraction(1,2)));store(m,VIEW+0x12c,bits(Fraction(3,4)))
 if c.get('bad')=='view':store(m,VIEW_SLOT,0x8cfffffc)

def prepare(c,n):
 fixture=dict(entry='mesh',families=c.get('families',('flat',)),topology=c.get('topology',0),near_mask=0,reverse=False,mode=c.get('bank',0)<<21,secondary=c.get('secondary',0),cursor_offset=0,kind='combined')
 _,m,s,f=renderer.prepare(fixture,n);s.pc=c.get('entry',WRAP);s.pr=0x8cfed000;s.r[15]=0x8cf00000
 # The render-state wrapper uses its original renderer records.
 state_a=0x8cf40000;state_b=state_a+0x100
 store(m,0x8c88fcb4,state_a);store(m,0x8c88fcd4,state_b)
 for i in range(4):store(m,state_a+0x90+i*4,0x12345000+i)
 for i in range(5):store(m,state_b+i*4,renderer.PACKETS if i==0 else 0x8cde0000 if i in (2,4) else 0)
 runtime(m,c);f.fpscr|=c.get('mode',0)<<19|c.get('rm',0)|c.get('enable',0)
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('mode',0)&2:store(m,VIEW_SLOT,0x8cfffffc)
 return m,s,f

def exact_preprocessing(m):
 a=[Fraction(value(load(m,VIEW+i*4))) for i in range(16)];b=[Fraction(value(load(m,BASIS+i*4))) for i in range(16)]
 out=[sum(a[i*4+k]*b[k*4+j] for k in range(3)) for i in range(3) for j in range(3)]
 direction=[Fraction(value(load(m,VIEW+0x4c+k*4))) for k in range(3)]
 light=[sum(out[k*3+j]*direction[k] for k in range(3)) for j in range(3)]
 return [bits(v) for v in out],[bits(v) for v in light]

def compare(m,s,f,c):
 entry=s.pc;other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 got=[];errors=[];checks=[];ml=ctypes.c_uint32(0x12345678);native=Memory();bus=Bus();view=(ctypes.c_uint8*len(actual)).from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus))
 matrices,light=exact_preprocessing(m)
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  try:
   got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
   if target==LOWER and not fp.contents.fpscr&0x180000:
    have=[load(mem,r[15]+0x30+i*16+j*4) for i in range(3) for j in range(3)]
    assert have==matrices and [load(mem,LIGHT+j*4) for j in range(3)]==light,(c,'independent matrix/light handoff',have,matrices)
    assert load(mem,0x8c890254)==load(m,VIEW+0x12c) and load(mem,0x8c890258)==load(m,VIEW+0x128),(c,'independent projection fields');checks.append(True)
  except Exception as exc:errors.append(repr(exc))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),previous.previous.state.gate.support.morph.P),ctypes.byref(ml),entry,None,observe,ctypes.byref(bus))
 assert not errors and rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'original motion mesh full state',rc,status,hex(s.pc),hex(other.pc),errors,len(got),len(wanted))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'original queue state/events')
 if not any(k in c for k in ('bad','mode','rm','enable')):assert rc==0 and checks and native.flushes>0,(c,'intended native complete packet path')
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,queue_match=True,independent_matrix_light_checks=len(checks),queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def cases():
 for entry,bank,matrix,family in itertools.product((PREP,WRAP),range(2),range(2),renderer.FAMILIES):yield dict(entry=entry,bank=bank,matrix=matrix,families=(family,))
 for topology,bank,secondary in itertools.product((0,0x4000,0x8000,0xc000),range(2),range(2)):yield dict(bank=bank,topology=topology,secondary=secondary,families=tuple(renderer.FAMILIES))
 for entry,bad in itertools.product((PREP,WRAP),('view','stack')):yield dict(entry=entry,bad=bad)
 for mode,entry in itertools.product(range(1,8),(PREP,WRAP)):yield dict(entry=entry,mode=mode)
 for entry,rm,enable in itertools.product((PREP,WRAP),range(4),(0,0x80,0x800)):yield dict(entry=entry,rm=rm,enable=enable,probe=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_motion_mesh_connection.json',PROJECT/'analysis/draw_motion_formats_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 binary=previous.previous.state.gate.support.morph.MAIN.read_bytes();records=[];coverage=set();targets=collections.Counter();boundaries=0;selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=PREP),dict(entry=WRAP,matrix=1,bank=1),dict(secondary=1,families=tuple(renderer.FAMILIES)),dict(rm=0,enable=0x80,probe=True),dict(entry=PREP,rm=0,enable=0x800,probe=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);targets.update(a for a in path if a in CONNECTED);boundaries+=b
  if (n+1)%32==0:print(n+1,'motion mesh original/native packet comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Motion mesh full native packet smoke passed:',len(records));return
 assert set(OWN)<=coverage,list(map(hex,set(OWN)-coverage))
 files=[Path(__file__),PROJECT/'verify_draw_motion_formats.py',PROJECT/'verify_startup.py',PROJECT/'verify_main_dispatch_mesh_families.py',PROJECT/'verify_main_dispatch_vertex_workers.py',PROJECT/'verify_main_dispatch_environment_constant_alternate_strips.py']+proofs+LISTINGS
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),conditional_edges_covered=0,boundary_comparisons=boundaries,independent_matrix_light_checks=sum(r['independent_matrix_light_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},external_game_service_fixtures=0,newly_recovered_entries=2,reused_routine_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Two complete guarded motion mesh preprocessing/wrapper bodies, all 278 rows, through existing native mesh/material/primitive pipeline and coherent-RAM queue model. Full reached original state, ordered entries, queue events/state, exact dyadic matrix/light/projection handoffs.',limitations=['Supplied valid runtime view/basis/light records and constructed bounded mesh streams. Old lower pipeline retains narrow-FPU/valid-RAM/saved-pointer and partial-error-PC limits; no universal exception or hardware-fault claim.','Matrix/vector numerical reference remains qualified separately; exact dyadic preprocessing oracle is independent. Software queue model is MMU-off/coherent RAM, not GPU output or cache timing.','No natural scene/player initialization, full frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_motion_mesh_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','boundary_comparisons','independent_matrix_light_checks','queue_flushes','queue_events')},indent=2))
if __name__=='__main__':main()
