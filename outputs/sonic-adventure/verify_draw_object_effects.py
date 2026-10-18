"""Original effect/profile states, exact initialization and independent phase checks."""
import collections,csv,ctypes,hashlib,itertools,json,math,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_point as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.previous.seed
MANIFEST=PROJECT/'analysis/draw_object_effects_translation.json';manifest=json.loads(MANIFEST.read_text())
DISPATCH,REGULAR,SPECIAL,PHASE,RNG=0xc909d80,0xc908b00,0xc908d80,0x8c63a9b8,0x8c10d334
PROFILES=tuple(int(r['address'],16) for r in manifest['routines'] if r['leaf_profile']);NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW|{RNG}
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
for row in csv.DictReader((PROJECT/'analysis/draw-effect-rng/8c10d334_instructions.tsv').open(),delimiter='\t'):ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW)
switch_targets={int(r['pc'],16):{int(a,16) for a in r['entries']} for r in manifest['computed_switches']}
code=previous.code;marker='  if pc in tracked:events.append(event(pc,r,sr,pr))';assert code.count(marker)==1
code=code.replace(marker,'''  if pc-2 in switch_targets and (pc+2+r[0])&0xffffffff not in switch_targets[pc-2]:stop(pc-2,r,sr,-3)
'''+marker);namespace['switch_targets']=switch_targets
exec(compile(code,'draw_object_effects_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_object_effects.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_object_effects_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
OBJECT,INFO=0x8cc80000,0x8cc90000;DETAIL=OBJECT+0x810

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def prepare(ram,c):
 entry=c.get('entry',PROFILES[1]);m,s,f=SEED.prepare(ram,dict(entry=entry,mode=c.get('mode',0),rm=c.get('rm',0),enable=c.get('enable',0)))
 s.pc=entry;s.r[4]=OBJECT;s.r[5]=OBJECT;s.r[6]=4;s.r[7]=0
 for i in range(0,0x1000,4):store(m,OBJECT+i,0)
 for off in (0xc,0x10,0x14,0xd8):store(m,OBJECT+off,bits(1))
 store(m,OBJECT+0xec,INFO);store(m,INFO,c.get('kind',0),1)
 for off in (0x44,0x48,0x4c,0x5c,0x60,0x68,0x6c,0x84,0x88,0x8c,0x9c,0xa0,0xa8,0xb4,0xb8):store(m,DETAIL+off,bits(c.get('value',Fraction(1,4))))
 for off,key,default in ((2,'selector',0),(0x34,'age',0),(0x38,'state',0),(0x74,'age_second',0),(0x78,'second',0)):store(m,DETAIL+off,c.get(key,default),2)
 store(m,DETAIL+0x36,0xaaaa,2);store(m,DETAIL+0x76,0xbbbb,2);store(m,DETAIL+0x30,c.get('phase',0x1234))
 store(m,DETAIL+4,c.get('countdown',0),2)
 for i in range(16):f.banks[f.fpscr>>21&1][i]=bits(Fraction(i-4,8))
 if entry==PHASE:s.r[4]=c.get('phase',0x1234)
 if 'seed' in c:store(m,0x8c5fbb08,c['seed'])
 if entry in (REGULAR,SPECIAL):s.r[5]=DETAIL
 if entry==0x8c605628:s.r[4]=c.get('slot',0);s.r[5]=c.get('selector',0)
 if entry==0x8c608d14:s.r[4]=c.get('texture',0)
 if entry==0x8c63d414:s.r[4]=OBJECT;s.r[5]=c.get('count',0);s.r[6]=0;store(m,OBJECT,DETAIL)
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='object':s.r[5]=0x8cfffffc
 if c.get('bad')=='kind':store(m,OBJECT+0xec,0x8cfffffc)
 if c.get('bad')=='switch':store(m,0xc909da8,1,2)
 if 'dn' in c:f.fpscr=(f.fpscr&~0x40000)|c['dn']*0x40000
 return m,s,f

def semantic_checks(m,actual,initial_s,initial_f,s,f,c):
 entry=initial_s.pc;checks=0
 if entry==RNG:
  want=(load(m,0x8c5fbb08)*0x41c64e6d+0x3039)&0xffffffff
  assert load(actual,0x8c5fbb08)==want and s.r[0]==(want>>16)&0x7fff,(c,'independent LCG seed/result')
  assert load(actual,initial_s.r[15]-4)==0x12345678 and s.r[15]==initial_s.r[15],(c,'independent MACL stack save')
  return 1
 if entry==PHASE:
  phase=c.get('phase',0x1234)&0xffff;fr=f.banks[f.fpscr>>21&1]
  assert s.r[4]==phase and f.fpul==phase
  for got,want in zip(fr[:2],(math.sin(phase*math.tau/65536),math.cos(phase*math.tau/65536))):assert abs(value(got)-want)<=2**-21,(c,'independent FSCA manual error bound')
  return 1
 if entry not in PROFILES:return 0
 assert load(actual,DETAIL+0x34,2)==c.get('age',0) and load(actual,DETAIL+0x74,2)==c.get('age_second',0),(c,'profile timer preservation')
 if entry==PROFILES[0]:
  phase=0 if c.get('second',0)==0 else c.get('phase',0x1234)
  assert load(actual,DETAIL+0x30)==(phase+0xc00)&0xffffffff,(c,'independent phase reset/advance')
  if c.get('second',0)==0:
   assert load(actual,DETAIL+0x36,2)==0 and load(actual,DETAIL+0x76,2)==1 and load(actual,DETAIL+0x78,2)==1
   for off in (0x44,0x4c,0x84,0x88,0x8c):assert load(actual,DETAIL+off)==0
   for off in (0x9c,0xa0):assert load(actual,DETAIL+off)==bits(1)
  checks+=1
 else:
  if c.get('state',0)==2:
   assert load(actual,DETAIL+4,2)==(c.get('countdown',0)-1)&0xffff,(c,'independent unsigned countdown')
   checks+=1
  if c.get('state',0)==0:
   assert load(actual,DETAIL+0x36,2)==PROFILES.index(entry)+1 and load(actual,DETAIL+0x38,2)==1,(c,'independent primary initialization')
   for off in (0x68,0x6c):assert load(actual,DETAIL+off)==0
   checks+=1
  if c.get('second',0)==0 and c.get('state',0)==0:
   assert load(actual,DETAIL+0x78,2)==1,(c,'independent secondary initialization')
   checks+=1
  # Primary state 3 can reset the secondary state before its dispatch, so its
  # one-step scale formula is qualified to primary states that do not do that.
  if entry==PROFILES[1] and c.get('second')==1 and c.get('state',0)!=3:
   delta=Fraction(value(load(m,0xc909360)));x=bits(Fraction(value(load(m,DETAIL+0x9c)))-delta);y=bits(Fraction(2)-Fraction(value(x)))
   assert load(actual,DETAIL+0x9c)==x and load(actual,DETAIL+0xa0)==y,(c,'independent secondary paired scale')
   checks+=1
 return checks

def compare(m,s,f,c):
 initial_s=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem)))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial_s.pc,None,observe,ctypes.byref(bus))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original object-effect state',rc,status,hex(s.pc),hex(other.pc))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'queue state/events')
 checks=semantic_checks(m,actual,initial_s,initial_f,s,f,c) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 else 0
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,queue_match=True,independent_checks=checks,stop_pc=hex(s.pc),steps=len(path),queue_events=native.trace_count+native.trace_dropped),path,len(got)

def cases():
 for entry,state,second,value,bank in itertools.product(PROFILES,range(5),range(4),(0,.25,1,-1,4),range(2)):
  yield dict(entry=entry,state=state,second=second,value=value,mode=bank*4,semantic=True)
 for entry,age,age_second in itertools.product(PROFILES[1:],(0,10,11,99,100,101,0xffff),(0,10,101)):
  yield dict(entry=entry,state=1,second=1,age=age,age_second=age_second,value=.25,semantic=True)
 for entry,countdown,bank in itertools.product(PROFILES[1:],(0,1,0x8000,0xffff),range(2)):yield dict(entry=entry,state=2,second=3,countdown=countdown,mode=bank*4,semantic=True)
 for selector,kind,bank in itertools.product((0,1,2,3,4,5,6,7,8,9,10,0xffff),(0,6),range(2)):yield dict(entry=DISPATCH,selector=selector,kind=kind,mode=bank*4)
 for entry,mode,state in itertools.product((*PROFILES,DISPATCH,PHASE),range(8),(0,2)):yield dict(entry=entry,mode=mode,state=state,second=state)
 for phase,mode,enable in itertools.product((0,1,0x3fff,0x4000,0x7fff,0x8000,0xbfff,0xc000,0xffff,0x12345678),range(8),(0,0x80)):yield dict(entry=PHASE,phase=phase,mode=mode,enable=enable,semantic=True)
 for entry,bad in itertools.product((*PROFILES,DISPATCH),('stack','object','kind')):yield dict(entry=entry,bad=bad)
 yield dict(entry=DISPATCH,bad='switch')
 for entry in (REGULAR,SPECIAL,0x8c605628,0x8c608d14,0x8c63d414):yield dict(entry=entry)
 for seed in (0,1,0x12345678,0x7fffffff,0x80000000,0xffffffff):
  yield dict(entry=RNG,seed=seed,semantic=True)
  yield dict(entry=SPECIAL,seed=seed,kind=6)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_object_effects_connection.json',PROJECT/'analysis/draw_point_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,base in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[base:base+len(data)]=data
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=[dict(entry=PROFILES[0],semantic=True),dict(entry=PROFILES[1],semantic=True),dict(entry=PROFILES[1],state=2,second=1,value=.25,semantic=True),dict(entry=PHASE,semantic=True),dict(entry=DISPATCH)]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'original object-effect comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Object-effect smoke passed:',len(records));return
 profiles=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=set(PER_BODY[a]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)])
  profiles.append(row)
 print(json.dumps([{k:v for k,v in r.items() if k not in ('missing_rows','missing_edges')} for r in profiles],indent=2))
 for row in profiles:
  if int(row['address'],16) in PROFILES:assert not row['missing_rows'] and not row['missing_edges'],row
 files=[Path(__file__),PROJECT/'verify_draw_point.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=profiles,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},registerable_entries=[hex(a) for a in PROFILES],newly_registered_entries=6,reused_routine_credit=0,external_game_service_fixtures=0,unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Six complete original effect profiles with all rows/conditional outcomes checked; thirteen-body native connection retains explicit lower boundaries. All reached CPU/FPU/FPUL/MACL/RAM, ordered entries, stop state and bounded queue traces compared. Independent initialization/phase/paired-scale checks.',limitations=['Dispatcher, two emitters and three main renderer services remain compiled partial bodies pending complete lower dependencies and path coverage; they receive zero registration credit. Existing four-row phase adapter gets extended evidence and no duplicate credit.','Scalar opcode reference and pinned vector/numerical support retain their documented domains. FSCA reference shares the separately qualified real core; independent mathematical error-bound checks are not measured bit-exact SH7091 outputs.','Valid-RAM/MMU-off and finite computed-switch proposal guards are explicit. No natural renderer/texture initialization, physical GPU pixels, full accepted drawing/frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_object_effects_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','boundary_comparisons','independent_cases','independent_checks','unconnected_targets')},indent=2))
if __name__=='__main__':main()
