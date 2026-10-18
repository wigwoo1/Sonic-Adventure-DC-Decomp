"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_material_setup as previous
import verify_overlay_draw_material_setup as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_primitive_stream_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_primitive_stream_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_primitive_stream.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_primitive_stream_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
import types
INIT=previous.INIT;ORIGINAL_GBR_LITERAL=previous.ORIGINAL_GBR_LITERAL
UV,SOLID,CLIPPED_UV,CLIPPED_SOLID,ENV_CLIP,SCALE_COPY=0x8c037990,0x8c037c3c,0x8c037ccc,0x8c038668,0x8c0382ac,0x8c60b462
geometry=previous.previous;NODE,MODEL,CACHE=geometry.NODE,geometry.MODEL,geometry.CACHE
DESC,INDICES,UVS,SPEC,OUTPUT,POSITIONS=0x8cd20000,0x8cd21000,0x8cd22000,0x8cd40000,0x8cf28000,0x8cd30000
MATERIAL=0x8cce0000
vector_words=set(namespace['VECTOR_WORDS']);binary=SEED.state.gate.support.morph.MAIN.read_bytes()
for a,t in OWN.items():
 if t.startswith(('fsca ','fipr ','ftrv ','fsrra ')):vector_words.add(int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little'))
namespace['VECTOR_WORDS']=vector_words
exec(compile(code,'primitive_stream_reference','exec'),namespace);reference_run=namespace['reference_run']
runtime=types.FunctionType(previous.runtime.__code__,globals(),'runtime')

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def prepare(ram,c):
 m,s,f=geometry.prepare(ram,c);m,gbr=runtime(m);c['gbr']=gbr;entry=c['entry'];bank=(f.fpscr>>21)&1
 # Unit matrix for environment-coordinate transform, supplied direct inputs.
 for i in range(16):f.banks[1-bank][i]=bits(1 if i in (0,5,10,15) else 0)
 lengths=c.get('lengths',(3,));override=c.get('override',0);ii=uv=0
 for strip,n in enumerate(lengths):
  if not override:store(m,INDICES+ii*2,n|(0x8000 if c.get('negative') else 0),2);ii+=1
  for j in range(n):
   index=(strip*3+j)%16;store(m,INDICES+ii*2,index,2);ii+=1
   for x in (index*8-40,32-index*4):store(m,UVS+uv*2,x&0xffff,2);uv+=1
 for i in range(32):
  for j,v in enumerate((bits(10+i),bits(20-i),bits(c.get('depths',(.25,.5,.75))[i%len(c.get('depths',(.25,.5,.75)))]),0x80a00000+i)):store(m,CACHE+i*16+j*4,v)
  store(m,SPEC+i*4,0x112200+i)
  for j,x in enumerate(((i%3-1)/2,(1-i%3)/2,0)):store(m,POSITIONS+i*12+j*4,bits(x))
 for i in range(512):store(m,OUTPUT+i*4,0xdeaf0000+i)
 store(m,DESC+2,len(lengths),2);store(m,DESC+4,INDICES);store(m,DESC+0x14,UVS)
 store(m,MATERIAL+4,POSITIONS);store(m,gbr+0x38,SPEC);store(m,gbr+0x3c,CACHE);store(m,gbr+0x54,override)
 s.r[2]=DESC;s.r[4]=MATERIAL;s.r[6]=0x400000 if c.get('environment') else 0;s.r[14]=(OUTPUT&0x3ffffff)|0xe0000000
 if entry==SCALE_COPY:
  s.r[4]=NODE;store(m,load(m,0x8c60b4fc),c.get('channel',0),2)
  for i,x in enumerate((1.25,-.5,3.5,7.25)):store(m,NODE+0x20+i*4,bits(x))
 if c.get('bad')=='indices':store(m,DESC+4,0x8cfffffe)
 if c.get('bad')=='cache':store(m,gbr+0x3c,0x8cfffffc)
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='destination':s.r[14]=0xe5000000
 return m,s,f

def semantic(m,actual,initial,initial_f,s,f,gbr,c,native):
 wanted=bytearray(m);entry=initial.pc
 if entry==SCALE_COPY:
  dest=load(m,0x8c60b508)+c.get('channel',0)*12
  for i in range(3):store(wanted,dest+i*4,load(m,NODE+0x20+i*4))
  assert actual==wanted,(c,'independent default scale copy complete footprint');return 1
 if entry not in (UV,SOLID):return 0
 count=0
 for strip,n in enumerate(c.get('lengths',(3,))):
  for j in range(n):
   i=(strip*3+j)%16;words=[0xf0000000 if j==n-1 else 0xe0000000]+[load(m,CACHE+i*16+k*4) for k in range(3)]
   if entry==UV:
    if c.get('environment'):
     x,y=(i%3-1)/2,(1-i%3)/2;u,v=(x+1)/2,(y+1)/2
    else:
     factor=value(load(m,0x8c038f10));u,v=(i*8-40)*factor,(32-i*4)*factor
    words.extend((bits(u),bits(v),load(m,CACHE+i*16+12),load(m,SPEC+i*4)))
   else:
    # Original solid packet retains unused words from its queue bank.
    words.extend((0,0,load(m,CACHE+i*16+12),0))
   for k,w in enumerate(words):store(wanted,OUTPUT+count*32+k*4,w)
   count+=1
 store(wanted,gbr+0x44,0)
 store(wanted,initial.r[15]-4,initial.r[3])
 assert actual==wanted,(c,'independent vertex packets/whole RAM footprint')
 assert s.r[0]==s.r[14]==initial.r[14]+count*32 and s.r[15]==initial.r[15] and s.r[3]==initial.r[3] and native.flushes==count,(c,'independent packet count/cursor/preserved state')
 return count+1

def cases():
 for entry in sorted(NEW):yield dict(entry=entry,semantic=True)
 for entry,lengths,negative,override,environment,bank in itertools.product((UV,SOLID),((2,),(3,),(4,),(6,),(3,4)),(False,True),(0,3),(False,True),range(2)):
  if override and lengths!=(3,):continue
  if entry==SOLID and environment:continue
  yield dict(entry=entry,lengths=lengths,negative=negative,override=override,environment=environment,mode=bank*4,semantic=True)
 for channel,mode in itertools.product((0,1,3),range(8)):yield dict(entry=SCALE_COPY,channel=channel,mode=mode,semantic=True)
 for entry,depths,environment in itertools.product((CLIPPED_UV,CLIPPED_SOLID,ENV_CLIP),((.25,.5,.75),(-.25,.5,.75),(1.5,.5,.75),(-1,-1,-1)),(False,True)):
  yield dict(entry=entry,depths=depths,environment=environment,semantic=True)
 for entry,mode in itertools.product(sorted(NEW),range(8)):yield dict(entry=entry,mode=mode)
 for entry,bad in itertools.product((UV,SOLID,SCALE_COPY),('indices','cache','source','stack','destination')):yield dict(entry=entry,bad=bad)

def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;ref.qacr[:]=[12,12];namespace['queue_reference']=ref
 status,path,wanted,expected_macl,expected_gbr=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));native.qacr[:]=[12,12];ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c.get('gbr',0));got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem),gb.value))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus),ctypes.byref(gb))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl and gb.value==expected_gbr,(c,'geometry stream full state',rc,status,hex(s.pc),hex(other.pc),len(got),len(wanted))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'geometry stream ordered queue/cache state')
 checks=semantic(m,actual,initial,initial_f,s,f,gb.value,c,native) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 and not c.get('bad') else 0
 return dict(input=dict(c),status=rc,full_state_match=True,ordered_boundaries_match=True,gbr_final_and_ordered_entries_match=True,queue_match=True,independent_checks=checks,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_primitive_stream_connection.json',PROJECT/'analysis/draw_material_setup_verification.json']
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
 files=[Path(__file__),PROJECT/'verify_draw_material_setup.py',PROJECT/'verify_overlay_draw_material_setup.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),native_original_renderer_initializer_stages=len(records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Six original primitive/default-scale proposals with real native dependencies and explicit GBR/queue state. Full original-model reached/final RAM/machine/GBR and ordered queue/entry comparisons. Independent per-strip vertex packets, cursor/stack/RAM footprints and default scale buffer checks.',limitations=['Direct fixtures supply indexed strips, projected caches, normals/specular and signed UV records with initial QACR=12 in both software queue windows. Real header setup is checked separately and remains real in connected tests. No natural startup proof.','Three clipping proposals retain explicit missing original callees. Their branch/CPU state dependencies remain incomplete and receive zero registration credit.','Inherited qualified scalar/vector numerical, valid-RAM/MMU-off queue and legacy domains remain; no physical GPU pixels, complete frame/gameplay or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_primitive_stream_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','registerable_entries','independent_cases','queue_flushes','unconnected_targets')},indent=2))
if __name__=='__main__':main()
