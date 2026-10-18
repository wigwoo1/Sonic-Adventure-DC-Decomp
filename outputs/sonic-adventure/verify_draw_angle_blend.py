"""Original integer-angle blend and independent sequential arithmetic/overlap checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_vector_blend as previous
import verify_overlay_draw_vector_blend as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_angle_blend_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_angle_blend_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_angle_blend.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_angle_blend_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
import types
INIT=previous.INIT;ORIGINAL_GBR_LITERAL=previous.ORIGINAL_GBR_LITERAL
ENTRY=0x8c60a250
import verify_draw_motion_sampling as sampling
KEYS,OUTPUT=sampling.KEYS,sampling.OUTPUT
runtime=types.FunctionType(previous.runtime.__code__,globals(),'runtime')

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

round_fraction=previous.round_fraction
rounded=previous.rounded
PAIRS=(
 ((65535,32767,-32768),(0,-32768,32767)),
 ((-16385,16777217,-1),(8194,16777223,1)),
 ((2147483647,-2147483648,16777217),(-2147483648,2147483647,-16777217)),
 ((0,0,0),(1,2147483520,-2147483648)),
)

def prepare(ram,c):
 m,s,f=sampling.prepare(ram,dict(c,entry=ENTRY,keys=True));m,gbr=runtime(m);c['gbr']=gbr
 for i,v in enumerate(sum((tuple(v) for v in c.get('pair',PAIRS[0])),())):store(m,KEYS+i*4,int(v)&0xffffffff)
 s.r[4]=KEYS;s.r[5]=KEYS+c['offset'] if c.get('offset') is not None else OUTPUT
 if c.get('alias'):
  for r in (4,5):s.r[r]=(s.r[r]&0xffffff)|c['alias']
 if c.get('bad')=='input':s.r[4]=0x8cfffffc
 if c.get('bad')=='output':s.r[5]=0x8cfffffc
 f.banks[(f.fpscr>>21)&1][4]=c.get('special',bits(c.get('weight',.25)))
 return m,s,f

def semantic(m,actual,initial,initial_f,s,f,gbr,c,native):
 bank=(initial_f.fpscr>>21)&1;t=Fraction(value(initial_f.banks[bank][4]));w=rounded(1-t)
 wanted=bytearray(m)
 for j in range(3):
  raw_a=load(wanted,initial.r[4]+j*4);raw_b=load(wanted,initial.r[4]+12+j*4)
  a=rounded(raw_a if raw_a<0x80000000 else raw_a-2**32)
  b=rounded(raw_b if raw_b<0x80000000 else raw_b-2**32)
  x=rounded(rounded(w*a)+t*b) if j==0 else rounded(w*a+rounded(t*b))
  expected=max(-2**31,min(2**31-1,int(x)))&0xffffffff
  got=load(actual,initial.r[5]+j*4)
  assert got==expected,(c,'independent signed angle blend and FTRC',j,got,expected)
  store(wanted,initial.r[5]+j*4,expected)
 assert actual==wanted,(c,'independent sequential integer overlap and whole RAM')
 regs=list(initial.r);regs[2]=raw_a;regs[3]=expected
 assert list(s.r)==regs and s.sr==initial.sr and s.pr==initial.pr and f.fpul==expected,(c,'leaf registers/stack/FPUL')
 assert native.trace_count==native.flushes==0,(c,'no queue traffic')
 return 6

def cases():
 for bank,pair,weight,offset in itertools.product(range(2),PAIRS,(-2,0,.25,.5,.75,1,2,100,16777216),(None,-4,0,4,8,12,16,20)):
  yield dict(entry=ENTRY,mode=bank*4,pair=pair,weight=weight,offset=offset,semantic=True)
 for mode in range(8):yield dict(entry=ENTRY,mode=mode,probe=True)
 for alias,bank in itertools.product((0x0c000000,0xac000000),range(2)):yield dict(entry=ENTRY,alias=alias,mode=bank*4,semantic=True)
 for bad in ('input','output'):yield dict(entry=ENTRY,bad=bad)
 for special,enable in itertools.product((0,0x80000000,1,0x7f800000,0xff800000,0x7fc12345),(0,0xf80)):yield dict(entry=ENTRY,special=special,enable=enable,probe=True)
 rng=random.Random(0x60a250)
 for i in range(64):
  pair=tuple(tuple(rng.randrange(-100000,100000) for _ in range(3)) for _ in range(2))
  yield dict(entry=ENTRY,pair=pair,weight=rng.randrange(-100,100)/8,mode=(i%2)*4,offset=(None,-4,0,4,8,12,16,20)[i%8],semantic=True)

def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl,expected_gbr=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c.get('gbr',0));got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem),gb.value))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus),ctypes.byref(gb))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl and gb.value==expected_gbr,(c,'angle blend full state',rc,status,hex(s.pc),hex(other.pc),len(got),len(wanted))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'angle blend ordered queue/cache state')
 checks=semantic(m,actual,initial,initial_f,s,f,gb.value,c,native) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 and not c.get('bad') else 0
 return dict(input=dict(c),status=rc,full_state_match=True,ordered_boundaries_match=True,gbr_final_and_ordered_entries_match=True,queue_match=True,independent_checks=checks,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def main():
 for x,raw in ((Fraction(1,3),0x3eaaaaab),(-Fraction(1,3),0xbeaaaaab),(Fraction(2**24+1),0x4b800000),(Fraction(2**24+3),0x4b800002),(Fraction(1,2**150),0)):
  assert round_fraction(x)==raw,('independent rounding oracle anchors',x)
 proofs=[MANIFEST,PROJECT/'analysis/draw_angle_blend_connection.json',PROJECT/'analysis/draw_vector_blend_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,offset in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[offset:offset+len(data)]=data
 ram=caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=selected[:9]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%128==0:print(n+1,'angle blend comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Vector blend smoke passed:',len(records));return
 coverage_rows=[];registerable=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=set(PER_BODY[a]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)])
  coverage_rows.append(row);associated=[r for r in records if r['input']['entry']==a]
  if not row['missing_rows'] and not row['missing_edges'] and all(r['status']!=-2 for r in associated):registerable.append(hex(a))
 files=[Path(__file__),PROJECT/'verify_draw_vector_blend.py',PROJECT/'verify_overlay_draw_vector_blend.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),native_original_renderer_initializer_stages=len(records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Original integer-angle blend connected to the preserved native draw pipeline. Full original-model reached/final RAM/machine/GBR and ordered entry/queue comparisons. Independent exact rational/binary32 arithmetic preserves the signed FLOAT/FTRC conversion and different X versus Y/Z FMUL/FMAC ordering, sequential reloads under overlap, whole RAM and leaf register/stack state.',limitations=['Direct fixtures provide six signed 32-bit integer values and a floating-register weight. The original component order and overlapping source/output accesses are preserved; no additional angle clamp or shortest-arc behavior is introduced; signed FTRC saturation is preserved.','Independent formula uses exact rationals and separate nearest-even binary32 quantization at each arithmetic boundary, including fused multiply-add. Original-model final/reached machine and RAM comparisons remain separate.','Inherited numerical/valid-RAM/software queue and legacy domains remain. No natural startup, complete rendering, physical pixels, high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_angle_blend_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','registerable_entries','independent_cases','queue_flushes','unconnected_targets')},indent=2))
if __name__=='__main__':main()
