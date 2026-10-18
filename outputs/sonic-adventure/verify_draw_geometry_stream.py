"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_gbr_geometry as previous
import verify_overlay_draw_gbr_geometry as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_geometry_stream_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_geometry_stream_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_geometry_stream.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_geometry_stream_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
import types
INIT=previous.INIT;ORIGINAL_GBR_LITERAL=previous.ORIGINAL_GBR_LITERAL
PROJECT_VERTICES,ROTATION,NORMALS,DISPATCH,FLAG,VIEW_BUILD,VIEW_RESET,FRAME,BOOT=0x8c037294,0x8c60a61a,0x8c037350,0x8c0376d0,0x8c036ffc,0x8c604ba0,0x8c604d68,0x8c0544e2,0x8c10b840
NODE,MODEL=previous.NODE,previous.MODEL
VERTICES,NORMAL_DATA,CACHE,LUT,MATERIAL=0x8ccf0000,0x8ccf8000,0x8cf10000,0x8ce20000,0x8cce0000
sampling=previous.previous
vector_words=set(namespace['VECTOR_WORDS']);binary=SEED.state.gate.support.morph.MAIN.read_bytes()
for a,t in OWN.items():
 if t.startswith(('fsca ','fipr ','ftrv ','fsrra ')):vector_words.add(int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little'))
namespace['VECTOR_WORDS']=vector_words
exec(compile(code,'geometry_stream_reference','exec'),namespace);reference_run=namespace['reference_run']
runtime=types.FunctionType(previous.runtime.__code__,globals(),'runtime')

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def position(i,c):return (Fraction(i%5-2,4),Fraction(2-i%5,4),c.get('zs',(0,4,-3.5))[i%len(c.get('zs',(0,4,-3.5)))])

def normal(i,c):return (0,0,c.get('normals',(-4,4,0,-1,1,-.5,.5))[i%len(c.get('normals',(-4,4,0,-1,1,-.5,.5)))])

def palette_word(address):return 0x60000000|((address-LUT)&0xffffff)

def prepare(ram,c):
 entry=c.get('entry',PROJECT_VERTICES);m,s,f=sampling.prepare(ram,dict(c,entry=entry));m,gbr=runtime(m);c['gbr']=gbr
 count=c.get('count',4);cache=CACHE+c.get('cache_offset',0)
 for i in range(128):
  for j,x in enumerate(position(i,c)):store(m,VERTICES+i*12+j*4,bits(x))
  for j,x in enumerate(normal(i,c)):store(m,NORMAL_DATA+i*12+j*4,bits(x))
 for i in range(512):store(m,CACHE-64+i*4,0xa5000000+i)
 for a in range(LUT-0x4000,LUT+0x4000,4):store(m,a,palette_word(a))
 for off,v in ((0,VERTICES),(4,NORMAL_DATA),(8,count),(12,MATERIAL),(16,MATERIAL)):store(m,MODEL+off,v)
 store(m,0x8c88f64c,cache);store(m,gbr+0x3c,cache);store(m,gbr+0x34,MATERIAL);store(m,gbr+0x2c,c.get('flags',0));store(m,gbr+0x58,LUT)
 store(m,MATERIAL+4,VERTICES if c.get('alternate') else 0)
 light=load(m,0x8c0373b4)
 for i,x in enumerate((0,0,1)):store(m,light+i*4,bits(x))
 if entry in (PROJECT_VERTICES,NORMALS,DISPATCH):s.r[4]=MODEL;s.r[13]=c.get('near_count',7)
 if entry==ROTATION:
  slot=c.get('slot',0);state=sampling.DUAL_STATE+c.get('channel',0)*28
  store(m,sampling.POINTERS+slot*4,0 if c.get('absent') else sampling.KEYS+0x100)
  store(m,state+20,slot);store(m,0x8c890020,0x8c60c19c if c.get('cubic') else 0x8c60bf5e)
  s.r[4]=NODE
 if entry in (VIEW_BUILD,VIEW_RESET):
  screen=load(m,0x8c604e44)
  for i,x in enumerate((c.get('distance',320),c.get('width',640),c.get('height',480),17,19)):store(m,screen+i*4,bits(x))
  for p in (0x8c604e38,0x8c604e3c,0x8c604e40):store(m,load(m,p),0xa5a5a5a5)
  s.r[4]=screen
 if entry==FLAG:
  root=load(m,0x8c037008);store(m,root+8,c.get('word',0x12345678))
 if c.get('alias') is not None:s.r[4]=(s.r[4]&0xffffff)|c['alias']
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='vertices':store(m,MODEL,0x8cfffffc)
 if c.get('bad')=='normals':store(m,MODEL+4,0x8cfffffc)
 if c.get('bad')=='palette':store(m,gbr+0x58,0x8cfffffc)
 if c.get('special') is not None:
  target=VERTICES+8 if entry==PROJECT_VERTICES else NORMAL_DATA+8 if entry==NORMALS else sampling.DUAL_STATE+c.get('channel',0)*28+12 if entry==ROTATION else load(m,0x8c604e44)+4
  store(m,target,c['special'])
 return m,s,f

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

def semantic(m,actual,initial,initial_f,s,f,gbr,c,native):
 entry=initial.pc;wanted=bytearray(m)
 if entry==PROJECT_VERTICES and not c.get('cache_offset'):
  pairs=max(1,(c.get('count',4)+1)//2);processed=pairs*2
  scale=value(load(m,0x8c8ffe08));yscale=scale*value(load(m,0x8c8ffe0c));cx=value(load(m,0x8c8ffe00));cy=value(load(m,0x8c8ffe04));near=value(load(m,0x8c8ffe10))
  for i in range(processed):
   x,y,z=position(i,c);z+=4
   vals=(float(x)*scale/z+cx,float(y)*yscale/z+cy,1/z)
   for j,v in enumerate(vals):store(wanted,CACHE+i*16+j*4,bits(v))
   store(wanted,CACHE+i*16+12,0)
  store(wanted,gbr+0x3c,CACHE)
  assert actual==wanted,(c,'independent paired projected records/complete RAM footprint')
  assert s.r[13]==c.get('near_count',7)+sum(position(i,c)[2]+4<=near for i in range(processed)),(c,'independent near-plane vertex counter')
  assert native.flushes==native.invalidates==native.prefetches==pairs and list(native.qacr)==[(CACHE>>24)&28]*2,(c,'independent queue/cache counts and QACR');return processed+3
 if entry==NORMALS:
  n=max(2,c.get('count',4));limit=load(m,0x8c0373ac,2);offset=load(m,0x8c0373ae,2);offset=offset-0x10000 if offset&0x8000 else offset
  if c.get('flags',0)&load(m,0x8c0373bc) or not c.get('alternate'):offset=0
  factor=value(load(m,load(m,0x8c0373b8)));dest=load(m,0x8c0373b0)
  for i in range(n):
   index=max(0,min(limit,int(float(normal(i,c)[2])*factor+factor)))
   store(wanted,CACHE+i*16+12,palette_word(LUT+index*8));store(wanted,dest+i*4,palette_word(LUT+index*8+4+offset))
  store(wanted,gbr+0x40,dest)
  assert actual==wanted,(c,'independent clamped lighting palette/cache footprint')
  assert native.flushes==native.trace_count==0,(c,'normal shading has no queue traffic');return n+2
 if entry==FLAG:
  root=load(m,0x8c037008);store(wanted,root+8,c.get('word',0x12345678)|0xc0)
  assert actual==wanted,(c,'independent exact flag update footprint');return 1
 if entry==VIEW_RESET:
  screen=load(m,0x8c604e44);w=c.get('width',640);h=c.get('height',480)
  store(wanted,screen+12,bits(w/2));store(wanted,screen+16,bits(h/2))
  clip=load(m,0x8c604e48)
  for i,v in enumerate((0,0,w-1,h-1)):store(wanted,clip+i*4,bits(v))
  for p in (0x8c604e38,0x8c604e3c,0x8c604e40):store(wanted,load(m,p),0)
  # The original also retains its temporary rectangle on the guest stack.
  for i,v in enumerate((0,0,w-1,h-1)):store(wanted,initial.r[15]-16+i*4,bits(v))
  assert actual==wanted and s.r[15]==initial.r[15],(c,'independent viewport reset/stack footprint');return 4
 if entry==ROTATION:
  state=sampling.DUAL_STATE+c.get('channel',0)*28
  assert load(actual,state+20)==c.get('slot',0)+1,(c,'independent consumed angle track')
  if c.get('absent'):
   pool=load(m,0x8c60a880)+c.get('channel',0)*12
   assert [load(actual,pool+j*4) for j in range(3)]==[load(m,NODE+0x14+j*4) for j in range(3)],(c,'independent default angle buffer')
  return 2
 return 0

def cases():
 for entry in (PROJECT_VERTICES,ROTATION,NORMALS,DISPATCH,FLAG,VIEW_BUILD,VIEW_RESET,FRAME,BOOT):yield dict(entry=entry,keys=True,semantic=True)
 for count,bank,offset,zs,near in itertools.product((0,1,2,3,4,5,8,15,16),range(2),(0,16),((0,4,-3.5),(0,),(4,),(-3.5,)),(0,7)):
  yield dict(entry=PROJECT_VERTICES,count=count,mode=bank*4,cache_offset=offset,zs=zs,near_count=near,semantic=True)
 for count,bank,flags,alternate,normals in itertools.product((0,1,2,3,4,5,8,9),range(2),(0,0x80000),(False,True),((-4,4,0),(4,-4,0),(0,0,-4),(0,0,4))):
  yield dict(entry=NORMALS,count=count,mode=bank*4,flags=flags,alternate=alternate,normals=normals,semantic=True)
 for bank,absent,cubic,frame,slot in itertools.product(range(2),(False,True),(False,True),(-1,0,.5,1,3.5,5,12,16,2147484160.,3221225472.),(0,2)):
  yield dict(entry=ROTATION,mode=bank*4,channel=bank,absent=absent,cubic=cubic,frame=frame,slot=slot,keys=True,probe=True,semantic=True)
 for bank,size,distance in itertools.product(range(2),((0,0),(1,1),(2,2),(640,480),(1920,1080),(3840,2160),(7680,4320),(.5,1.5)),(.5,1,320)):
  yield dict(entry=VIEW_RESET,mode=bank*4,width=size[0],height=size[1],distance=distance,semantic=True)
 for bank,word in itertools.product(range(2),(0,1,0x40,0x80,0xc0,0x12345678,0xffffffff)):yield dict(entry=FLAG,mode=bank*4,word=word,semantic=True)
 for entry,mode in itertools.product(sorted(NEW),range(8)):yield dict(entry=entry,mode=mode,keys=True,probe=True)
 for entry,bad in itertools.product((PROJECT_VERTICES,ROTATION,NORMALS,FLAG,VIEW_RESET),('source','stack','vertices','normals','palette')):
  yield dict(entry=entry,bad=bad,keys=True,absent=entry==ROTATION and bad=='source')
 for entry,special,enable in itertools.product((PROJECT_VERTICES,ROTATION,NORMALS,VIEW_RESET),(0,0x80000000,1,0x7f800000,0x7fc12345),(0,0xf80)):
  yield dict(entry=entry,special=special,enable=enable,keys=True,probe=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_geometry_stream_connection.json',PROJECT/'analysis/draw_gbr_geometry_verification.json']
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
 files=[Path(__file__),PROJECT/'verify_draw_gbr_geometry.py',PROJECT/'verify_overlay_draw_gbr_geometry.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),native_original_renderer_initializer_stages=len(records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Nine original geometry/view/initialization-caller proposals connected through real native GBR and RAM/store-queue cache operations. Full original-model reached/final RAM/machine/GBR and ordered queue/entry comparisons. Independent paired projection, exact lighting palette/record footprint, default angle/track counters, flag update and viewport reset/guest-stack checks.',limitations=['Direct tests supply synthetic vertices/normals/materials, deterministic relocated lighting tables and screen dimensions. Original initializer output is checked natively and in the instruction model; natural whole-engine initialization is unproved.','Projection preserves original unconditional paired processing, including zero/one counts and lookahead reads. Intended aligned-cache output has an independent analytic oracle; offset-cache cases retain only full original-state/queue comparisons. These are software queues, not physical GPU output.','Inherited qualified scalar/vector/FSCA/FSRRA/sqrt, MMU-off valid-RAM/bus and legacy-mode limits remain. Partial dispatch/view/frame/boot proposals retain missing-service boundaries and zero registration credit. No complete rendering, gameplay, high-FPS/8K or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_geometry_stream_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','registerable_entries','independent_cases','queue_flushes','unconnected_targets')},indent=2))
if __name__=='__main__':main()
