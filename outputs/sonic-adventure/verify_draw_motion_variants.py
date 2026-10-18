"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_overlay_fade as previous
import verify_overlay_draw_overlay_fade as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_motion_variants_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_motion_variants_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_motion_variants.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_motion_variants_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
NODE,DESC,DESC2,PAIR,DUAL,DATA,KEYS,MODEL=0x8cc50000,0x8cc60000,0x8cc60100,0x8cc60200,0x8cc60300,0x8cc70000,0x8cc80000,0x8cc90000
MATRIX=0x8ce00000
PRIMARY_FORMATS=(0x8c0405b2,0x8c0405d2,0x8c040612,0x8c0406a0,0x8c0406e0,0x8c040720,0x8c04073c,0x8c040760)
SECONDARY_FORMATS=(0x8c041516,0x8c04154e,0x8c041600,0x8c041666,0x8c0416cc,0x8c041732,0x8c04175c,0x8c041792)
DEFAULTS=(0x8c04057a,0x8c040588,0x8c040596,0x8c0405a4,0x8c0414a6,0x8c0414cc,0x8c0414f0)
MOTION,DUAL_STATE,FORMATS,TYPES,CHANNEL_ID=0x8c88fe40,0x8c88feb8,0x8c88ff28,0x8c88ff2c,0x8c88ff38
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def literal(m,a):return load(m,a)
def prepare(ram,c):
 entry=c.get('entry',SETUP);_,s,f=SEED.prepare(ram,dict(entry=entry,mode=c.get('mode',0),rm=c.get('rm',0),enable=c.get('enable',0)));m=bytearray(ram);s.pc=entry
 matrix=[bits(int(i%5==0)) for i in range(16)];matrix[14]=bits(4)
 for i,v in enumerate(matrix):store(m,MATRIX+i*4,v)
 store(m,0x8c88f5f8,MATRIX);store(m,0x8c88f698,32);store(m,0x8c88f69c,1);f.banks[(f.fpscr>>21&1)^1][:]=matrix
 graph=[(1,2),(None,None),(None,None)] if c.get('tree') else [(None,None)]
 for i,(child,sibling) in enumerate(graph):
  a=NODE+i*0x100;store(m,a,c.get('flags',0));store(m,a+4,MODEL if c.get('model') else 0)
  for j,v in enumerate((.25,.5,1)):store(m,a+8+j*4,bits(v))
  for j,v in enumerate((0,0,0)):store(m,a+0x14+j*4,v)
  for j,v in enumerate((1,1,1)):store(m,a+0x20+j*4,bits(v))
  store(m,a+0x2c,NODE+child*0x100 if child is not None else 0);store(m,a+0x30,NODE+sibling*0x100 if sibling is not None else 0)
 for i in range(64):store(m,MODEL+i*4,0)
 for i in range(96):store(m,DATA+i*4,0)
 # Valid sparse motion streams; nonzero track proposals stop at real missing
 # interpolation dependencies rather than fabricated sample results.
 for i in range(8):
  store(m,KEYS+i*16,i)
  for j,v in enumerate((i*.25,1,2)):store(m,KEYS+i*16+4+j*4,bits(v))
 if c.get('keys'):
  for i in range(8):store(m,DATA+i*4,KEYS)
 fmt=c.get('format',0);channels=c.get('channels',0);frame=c.get('frame',.5);count=c.get('count',16)
 for a in (DESC,DESC2):
  store(m,a,DATA);store(m,a+4,count);store(m,a+8,channels,2);store(m,a+10,fmt,2)
 store(m,PAIR,NODE);store(m,PAIR+4,DUAL)
 for i,v in enumerate((0 if c.get('null0') else DESC,0 if c.get('null1') else DESC2,bits(frame),bits(c.get('frame1',frame)))):store(m,DUAL+i*4,v)
 for a in (MOTION,DUAL_STATE,DUAL_STATE+28):
  for off,v in ((0,DATA),(4,DATA),(8,DATA+12),(12,bits(frame)),(16,count),(20,0),(24,0)):store(m,a+off,v)
 store(m,0x8c749100,fmt,2);store(m,0x8c749102,channels,2)
 table=load(m,0x8c19ace0+(fmt&0xff3f)*4)
 for a,v in ((0x8c88fe24,table),(0x8c88fe2c,0x8c0400a0 if channels&1 else 0x8c04057a),(0x8c88fe30,(0x8c0401a8 if c.get('flags',0)&32 else 0x8c040150) if channels&2 else (0x8c040596 if c.get('flags',0)&32 else 0x8c040588)),(0x8c88fe34,0x8c0400f8 if channels&4 else 0x8c0405a4)):store(m,a,v)
 for channel in range(2):
  store(m,FORMATS+channel*2,fmt,2);store(m,TYPES+channel*2,channels,2)
  for a,v in ((0x8c88fe80,load(m,0x8c19ad20+(fmt&0xff3f)*4)),(0x8c88fe98,0x8c040df0 if channels&1 else 0x8c0414a6),(0x8c88fea0,0x8c040ef8 if channels&2 else 0x8c0414cc),(0x8c88fea8,0x8c040e74 if channels&4 else 0x8c0414f0)):store(m,a+channel*4,v)
 store(m,CHANNEL_ID,c.get('channel',0),2);store(m,0x8c19ad44,0x8c639ff8 if c.get('flags',0)&32 else 0x8c639cf4)
 store(m,0x8c88ff3c,bits(c.get('blend',.5)));store(m,0x8c890074,0x8c611708 if c.get('optional') else 0)
 for a in (0x8c88ff50,0x8c88ff68,0x8c88ff80):
  for i in range(6):store(m,a+i*4,bits(.25*(i+1)))
 f.banks[f.fpscr>>21&1][4]=bits(frame if entry not in (0x8c041c92,0x8c5fde20) else c.get('blend',.5))
 s.r[4]=NODE;s.r[5]=0
 if entry==SETUP:s.r[4]=0 if c.get('null') else DESC
 if entry==CHANNEL:s.r[4]=DUAL;s.r[5]=c.get('channel',0)
 if entry==CHANNEL_SELECT:s.r[5]=c.get('channel',0)
 if entry==0x8c0409c0:s.r[5]=0 if c.get('null') else DESC
 if entry in (0x8c041c92,0x8c5fde20):s.r[4]=PAIR
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='motion':s.r[5]=0x8cfffffc
 if c.get('special') is not None:f.banks[f.fpscr>>21&1][4]=c['special']
 return m,s,f
def signed32(v):return v-(1<<32) if v&0x80000000 else v
def semantic(m,actual,initial,s,f,c):
 entry=initial.pc;flags=c.get('flags',0);channels=c.get('channels',0);fmt=c.get('format',0);checks=0
 if entry==SETUP:
  accepted=c.get('null') or signed32(c.get('count',16))>c.get('frame',.5)
  assert s.r[0]==int(not accepted),(c,'independent frame acceptance')
  if not accepted:assert actual==m,(c,'independent rejected setup footprint');return 1
  assert load(actual,MOTION+12)==bits(c.get('frame',.5)) and load(actual,MOTION+20)==load(actual,MOTION+24)==0,(c,'independent setup frame/counters')
  assert load(actual,0x8c749100,2)==(0 if c.get('null') else fmt&0xff3f) and load(actual,0x8c749102,2)==(0 if c.get('null') else channels),(c,'independent setup format/type words')
  if not c.get('null'):assert load(actual,MOTION)==DATA and load(actual,MOTION+16)==c.get('count',16),(c,'independent original motion data/count')
  return 3
 if entry in (SELECT,CHANNEL_SELECT):
  if entry==SELECT:
   expected={0x8c88fe24:load(m,0x8c19ace0+(fmt&0xffff)*4),0x8c88fe2c:0x8c0400a0 if channels&1 else 0x8c04057a,0x8c88fe30:(0x8c0401a8 if flags&32 else 0x8c040150) if channels&2 else (0x8c040596 if flags&32 else 0x8c040588),0x8c88fe34:0x8c0400f8 if channels&4 else 0x8c0405a4}
  else:
   offset=c.get('channel',0)*4;expected={0x8c88fe80+offset:load(m,0x8c19ad20+(fmt&0xffff)*4),0x8c19ad44:0x8c639ff8 if flags&32 else 0x8c639cf4,0x8c88fe98+offset:0x8c040df0 if channels&1 else 0x8c0414a6,0x8c88fea0+offset:0x8c040ef8 if channels&2 else 0x8c0414cc,0x8c88fea8+offset:0x8c040e74 if channels&4 else 0x8c0414f0}
  assert all(load(actual,a)==v for a,v in expected.items()),(c,'independent callback selectors')
  return len(expected)
 if entry==CHANNEL:
  channel=c.get('channel',0);desc=0 if c.get('null0' if channel==0 else 'null1') else DESC if channel==0 else DESC2;accepted=not desc or signed32(c.get('count',16))>c.get('frame1',c.get('frame',.5)) if channel else not desc or signed32(c.get('count',16))>c.get('frame',.5)
  assert s.r[0]==int(not accepted),(c,'independent channel frame acceptance')
  if accepted:
   assert load(actual,FORMATS+channel*2,2)==(fmt&0xff3f if desc else 0) and load(actual,TYPES+channel*2,2)==(channels if desc else 0),(c,'independent channel metadata')
  return 1
 if entry in (0x8c0414a6,0x8c0414cc,0x8c0414f0):
  channel=c.get('channel',0);src={0x8c0414a6:8,0x8c0414cc:0x14,0x8c0414f0:0x20}[entry];pool=load(m,{0x8c0414a6:0x8c0415dc,0x8c0414cc:0x8c0415e0,0x8c0414f0:0x8c0415e4}[entry]);assert [load(actual,pool+channel*12+i*4) for i in range(3)]==[load(m,NODE+src+i*4) for i in range(3)],(c,'independent pose buffer copy');return 1
 return checks
def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem)))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original motion-variant state',rc,status,hex(s.pc),hex(other.pc),len(got),len(wanted))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'motion-variant queue state')
 checks=semantic(m,actual,initial,s,f,c) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 and not c.get('bad') else 0
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,queue_match=True,independent_checks=checks,stop_pc=hex(s.pc),steps=len(path)),path,len(got)
def cases():
 for entry in sorted(NEW):yield dict(entry=entry,semantic=True)
 for channels,fmt,flags,bank in itertools.product(range(8),range(9),(0,32),range(2)):
  for entry in (SELECT,CHANNEL_SELECT):yield dict(entry=entry,channels=channels,format=fmt,flags=flags,mode=bank*4,channel=bank,semantic=True)
 for entry,count,frame,null,bank in itertools.product((SETUP,CHANNEL),(0,1,16,0xffffffff),(-1,0,.5,16),(False,True),range(2)):yield dict(entry=entry,count=count,frame=frame,frame1=frame,null=null,null0=null,null1=null,channel=bank,mode=bank*4,channels=7,format=3,semantic=True)
 for entry,bank,flags in itertools.product(DEFAULTS+(0x8c041732,),range(2),(0,8,32,64)):yield dict(entry=entry,mode=bank*4,flags=flags,channel=bank,model=bool(flags&8),semantic=True)
 for entry,tree,flags,model,optional in itertools.product((0x8c040784,0x8c041a2e,0x8c5fdbec),(False,True),(0,32,64),(False,True),(False,True)):yield dict(entry=entry,tree=tree,flags=flags,model=model,optional=optional)
 for entry,mode in itertools.product(sorted(NEW),range(8)):yield dict(entry=entry,mode=mode)
 for entry in PRIMARY_FORMATS+SECONDARY_FORMATS+(0x8c0400a0,0x8c0400f8,0x8c040150,0x8c0401a8,0x8c040df0,0x8c040e74,0x8c040ef8):yield dict(entry=entry,keys=True,channels=7,format=3)
 for entry,bad in itertools.product((SETUP,SELECT,CHANNEL,CHANNEL_SELECT,*DEFAULTS),('stack','source')):yield dict(entry=entry,bad=bad)
def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_motion_variants_connection.json',PROJECT/'analysis/draw_overlay_fade_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,base in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[base:base+len(data)]=data
 ram=caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0];selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=[dict(entry=a,semantic=True) for a in (SETUP,SELECT,CHANNEL,CHANNEL_SELECT,0x8c04057a,0x8c0414a6)]+[dict(entry=0x8c0409c0)]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'original motion-variant comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Motion-variant smoke passed:',len(records));return
 coverage_rows=[];registerable=[]
 for r in manifest['routines']:
  a=int(r['address'],16);wanted={tuple(int(x,16) for x in e) for e in r['conditional_edges']};body=set(PER_BODY[a]);row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]);coverage_rows.append(row)
  associated=[x for x in records if x['input'].get('entry',SETUP)==a]
  if not row['missing_rows'] and not row['missing_edges'] and associated and all(x['status']!=-2 for x in associated):registerable.append(hex(a))
 files=[Path(__file__),PROJECT/'verify_draw_overlay_fade.py',PROJECT/'verify_overlay_draw_overlay_fade.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),newly_registered_entries=0,reused_routine_credit=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='43 original motion/drawing proposals in native selector/traversal/callback chain. Complete reached RAM/machine state, ordered entries and software queues compared with original instructions. Only complete per-body coverage with no reached unconnected direct-entry case qualifies for later registration.',limitations=['Renderer/motion/runtime callbacks are supplied input state drawn from original tables and functions; optional callbacks select the existing real empty return stub. Sparse tracks and explicit unconnected interpolation/geometry boundaries remain, without fake guest service results.','Static pointer-data windows do not prove the entire valid format domain. Existing numerical/vector/queue/MMU-off valid-RAM contracts and legacy entry guards remain. No complete frame/gameplay/GPU pixels/playable port proof.'],cases=records)
 (PROJECT/'analysis/draw_motion_variants_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','registerable_entries','boundary_comparisons','independent_cases','unconnected_targets')},indent=2))
if __name__=='__main__':main()
