"""Original integer-angle interpolation and independent staged arithmetic checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_vector_curve as previous
import verify_overlay_draw_vector_curve as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_angle_curve_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_angle_curve_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_angle_curve.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_angle_curve_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
import types
INIT=previous.INIT;ORIGINAL_GBR_LITERAL=previous.ORIGINAL_GBR_LITERAL
ENTRY=0x8c609d68
import verify_draw_motion_sampling as sampling
KEYS,OUTPUT=sampling.KEYS,sampling.OUTPUT
runtime=types.FunctionType(previous.runtime.__code__,globals(),'runtime')

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def labels(c):return c.get('labels',(0,4,8,12))

def sample(i):return (i*8193-16385,16777217+i*3,32767-i*21845)

def round_fraction(x):
 """Independent rational-to-binary32 round-to-nearest, ties-to-even."""
 x=Fraction(x)
 if not x:return 0
 sign=0x80000000 if x<0 else 0;n,d=abs(x).numerator,abs(x).denominator
 e=n.bit_length()-d.bit_length()
 if (n<d*(1<<e) if e>=0 else n*(1<<-e)<d):e-=1
 q=max(e-23,-149)
 if q>=0:a,b=n,d<<q
 else:a,b=n<<-q,d
 s,r=divmod(a,b)
 if 2*r>b or 2*r==b and s&1:s+=1
 if s>=1<<24:s>>=1;q+=1
 return sign|s if s<1<<23 else sign|((q+150)<<23)|(s&0x7fffff)

def rounded(x):return Fraction(value(round_fraction(x)))

def unsigned_float(x):
 y=rounded(x if x<0x80000000 else x-0x100000000)
 return rounded(y+2**32) if x&0x80000000 else y

def prepare(ram,c):
 m,s,f=sampling.prepare(ram,dict(c,entry=ENTRY,keys=True));m,gbr=runtime(m);c['gbr']=gbr
 for i,k in enumerate(labels(c)):
  store(m,KEYS+i*16,k)
  for j,x in enumerate(sample(i)):store(m,KEYS+i*16+4+j*4,x&0xffffffff)
 channel=c.get('channel',0);store(m,load(m,0x8c609ebc),channel,2)
 store(m,load(m,0x8c609ec0)+channel*28,c.get('special',bits(c.get('frame',5))))
 s.r[4]=KEYS;s.r[5]=c.get('index',0);s.r[6]=len(labels(c));s.r[7]=OUTPUT
 if c.get('alias'):
  for r in (4,7):s.r[r]=(s.r[r]&0xffffff)|c['alias']
 if c.get('bad')=='keys':s.r[4]=0x8cfffffc
 if c.get('bad')=='output':s.r[7]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 return m,s,f

def semantic(m,actual,initial,initial_f,s,f,gbr,c,native):
 index=c.get('index',0);nxt=(index+1)%len(labels(c));start=labels(c)[index];end=labels(c)[nxt]
 span=(end-start)&0xffffffff;signed_span=span if span<0x80000000 else span-0x100000000
 # Formula-level rational oracle, preserving each finite binary32 boundary.
 # A final FMAC rounds its exact product-plus-sum once; the next-key product
 # was already rounded by the preceding FMUL. Large extrapolation therefore
 # cannot be simplified into ideal (1-t)*a+t*b without losing game behavior.
 frame=Fraction(value(bits(c.get('frame',5))))
 denom=unsigned_float(span) if signed_span>0 else Fraction(value(load(m,load(m,0x8c609ec4))))
 t=rounded(rounded(frame-unsigned_float(start))/denom);weight=rounded(1-t)
 floats=[rounded(weight*rounded(a)+rounded(t*rounded(b))) for a,b in zip(sample(index),sample(nxt))]
 # FTRC truncates toward zero and saturates out-of-range finite inputs when
 # invalid exceptions are masked. Integer conversions set no inexact flag.
 expected=[max(-2**31,min(2**31-1,int(x))) for x in floats]
 got=[load(actual,initial.r[7]+j*4) for j in range(3)]
 assert got==[x&0xffffffff for x in expected],(c,'independent signed-angle FLOAT/FMUL/FMAC/FTRC formula',got,expected)
 wanted=bytearray(m)
 for j in range(3):store(wanted,initial.r[7]+j*4,load(actual,initial.r[7]+j*4))
 sp=initial.r[15]
 # The real remainder dependency also retains its saved R3/R4 below locals.
 for off,v in ((-4,initial.r[14]),(-8,initial.r[13]),(-12,initial.pr),(-20,load(m,load(m,0x8c609ec4))),(-16,round_fraction(unsigned_float(span))),(-24,0x8c10cc80),(-28,initial.r[4])):store(wanted,sp+off,v)
 assert actual==wanted,(c,'independent output and retained guest-stack footprint')
 assert s.r[15]==sp and s.r[13]==initial.r[13] and s.r[14]==initial.r[14] and s.pr==initial.pr,(c,'independent preserved registers/stack')
 assert native.trace_count==native.flushes==0,(c,'no queue traffic from angle interpolation')
 return 6

def cases():
 for bank,keys,frame,index,channel in itertools.product(range(2),((0,4,8,12),(0,),(0,4),(0,0,4,12),(12,8,4,0),(0x80000000,0x80000400,0x80000800,0x80000c00)),(-1,0,.5,1,4,5,8,12,16,2147484160.,3221225472.),(0,1,3),(0,1)):
  if index>=len(keys):continue
  yield dict(entry=ENTRY,mode=bank*4,labels=keys,frame=frame,index=index,channel=channel,semantic=True)
 for mode in range(8):yield dict(entry=ENTRY,mode=mode,probe=True)
 for alias,bank in itertools.product((0x0c000000,0xac000000),range(2)):yield dict(entry=ENTRY,alias=alias,mode=bank*4,semantic=True)
 for bad in ('keys','output','stack'):yield dict(entry=ENTRY,bad=bad)
 for special,enable in itertools.product((0,0x80000000,1,0x7f800000,0xff800000,0x7fc12345),(0,0xf80)):yield dict(entry=ENTRY,special=special,enable=enable,probe=True)
 rng=random.Random(0x609d68)
 for i in range(64):
  keys=tuple(rng.getrandbits(32) for _ in range(3));yield dict(entry=ENTRY,labels=keys,index=i%3,frame=rng.randrange(-100,2**32),mode=(i%2)*4,channel=i%2,semantic=True)

def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl,expected_gbr=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c.get('gbr',0));got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem),gb.value))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus),ctypes.byref(gb))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl and gb.value==expected_gbr,(c,'geometry stream full state',rc,status,hex(s.pc),hex(other.pc),len(got),len(wanted))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'geometry stream ordered queue/cache state')
 checks=semantic(m,actual,initial,initial_f,s,f,gb.value,c,native) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 and not c.get('bad') else 0
 return dict(input=dict(c),status=rc,full_state_match=True,ordered_boundaries_match=True,gbr_final_and_ordered_entries_match=True,queue_match=True,independent_checks=checks,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def main():
 for x,raw in ((Fraction(1,3),0x3eaaaaab),(-Fraction(1,3),0xbeaaaaab),(Fraction(2**24+1),0x4b800000),(Fraction(2**24+3),0x4b800002),(Fraction(1,2**150),0)):
  assert round_fraction(x)==raw,('independent rounding oracle anchors',x)
 proofs=[MANIFEST,PROJECT/'analysis/draw_angle_curve_connection.json',PROJECT/'analysis/draw_vector_curve_verification.json']
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
  if (n+1)%128==0:print(n+1,'geometry stream comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Geometry stream smoke passed:',len(records));return
 coverage_rows=[];registerable=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=set(PER_BODY[a]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)])
  coverage_rows.append(row);associated=[r for r in records if r['input']['entry']==a]
  if not row['missing_rows'] and not row['missing_edges'] and all(r['status']!=-2 for r in associated):registerable.append(hex(a))
 files=[Path(__file__),PROJECT/'verify_draw_vector_curve.py',PROJECT/'verify_overlay_draw_vector_curve.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),native_original_renderer_initializer_stages=len(records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Original unsigned-label angle curve connected to real copy/remainder helpers and the preserved native draw pipeline. Full original-model reached/final RAM/machine/GBR and ordered queue/entry comparisons. Independent exact rational/binary32 staged integer-angle formula with signed FLOAT and saturating FTRC, complete output/retained-stack footprint and preserved-register checks.',limitations=['Direct fixtures supply bounded nonempty key arrays and explicit per-channel frame inputs. Sorted, duplicate, descending and unsigned high labels are included; malformed zero counts are not claimed.','Independent formula uses exact rationals and separate nearest-even binary32 quantization at each arithmetic boundary, including fused multiply-add. Original-model final/reached machine and RAM comparisons remain separate.','Inherited numerical/valid-RAM/software queue and legacy domains remain. No natural startup, complete rendering, physical pixels, high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_angle_curve_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','registerable_entries','independent_cases','queue_flushes','unconnected_targets')},indent=2))
if __name__=='__main__':main()
