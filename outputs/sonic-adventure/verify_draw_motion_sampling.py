"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_motion_variants as previous
import verify_overlay_draw_motion_variants as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_motion_sampling_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_motion_sampling_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_motion_sampling.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_motion_sampling_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
NODE,DESC,DESC2,PAIR,DUAL,DATA,KEYS,MODEL=previous.NODE,previous.DESC,previous.DESC2,previous.PAIR,previous.DUAL,previous.DATA,previous.KEYS,previous.MODEL
MOTION,DUAL_STATE,CHANNEL_ID=previous.MOTION,previous.DUAL_STATE,previous.CHANNEL_ID
POINTERS,COUNTS,OUTPUT=0x8cca0000,0x8ccb0000,0x8ccc0000
INDEX=(0x8c03feb8,0x8c040a60)
CURVES=(0x8c03ff2c,0x8c03ff90,0x8c040c5c,0x8c040d60)
DISPATCH=(0x8c60b596,0x8c60b4a8,0x8c60b81c)
SAMPLE=(0x8c0400a0,0x8c0400f8,0x8c040150,0x8c0401a8,0x8c040df0,0x8c040e74,0x8c040ef8)
PRIMARY_KIND={a:i+1 for i,a in enumerate(previous.PRIMARY_FORMATS[1:5])}
SECONDARY_KIND={a:i+1 for i,a in enumerate(previous.SECONDARY_FORMATS[1:5])}
ALL_BODIES=dict(previous.PER_BODY)
ALL_BODIES.update(PER_BODY)
TRACKED_ROWS=set().union(*ALL_BODIES.values())

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def labels(c):return c.get('labels',(0,4,8,12))

def key_values(component,i):
 if component==1:return ((i-1)*1024,(i*i-2)*256,(3-i)*128)
 if component==2:return (Fraction(i+2,4),Fraction(3-i,4),Fraction(i*i+1,4))
 return (Fraction(i-2,4),Fraction(i*i-3,8),Fraction(3-i,2))

def component_for(entry,c):
 if entry in (SAMPLE[1],SAMPLE[5]):return 2
 if entry in (SAMPLE[2],SAMPLE[3],SAMPLE[6],CURVES[1],CURVES[3]):return 1
 return c.get('component',0)

def prepare(ram,c):
 entry=c.get('entry',INDEX[0]);m,s,f=previous.prepare(ram,dict(c,entry=entry))
 frame=c.get('frame',5);channel=c.get('channel',0);component=component_for(entry,c);slot=c.get('slot',0)
 for comp in range(3):
  for i,label in enumerate(labels(c)):
   store(m,KEYS+comp*0x100+i*16,label)
   for j,v in enumerate(key_values(comp,i)):store(m,KEYS+comp*0x100+i*16+4+j*4,int(v)&0xffffffff if comp==1 else bits(v))
 for i in range(16):store(m,POINTERS+i*4,0);store(m,COUNTS+i*4,len(labels(c)))
 active=bool(c.get('keys')) and not c.get('absent')
 store(m,POINTERS+slot*4,KEYS+component*0x100 if active else 0)
 for state in (MOTION,DUAL_STATE,DUAL_STATE+28):
  for off,v in ((0,DATA),(4,POINTERS),(8,COUNTS),(12,bits(frame)),(16,c.get('count',16)),(20,slot),(24,0)):store(m,state+off,v)
 # Real interleaved format records: K track pointers followed by K counts.
 kind=PRIMARY_KIND.get(entry,SECONDARY_KIND.get(entry,2 if entry==DISPATCH[0] else 3))
 channels=c.get('channels',0);components=[i for i in range(3) if channels>>i&1]
 for node in range(3):
  a=DATA+node*kind*8
  for i in range(kind):
   comp=components[i] if i<len(components) else 0
   store(m,a+i*4,KEYS+comp*0x100 if active and i<len(components) else 0)
   store(m,a+kind*4+i*4,len(labels(c)))
 for i in range(12):store(m,OUTPUT+i*4,0xa5a50000+i)
 if entry in INDEX:s.r[4]=KEYS+component*0x100;s.r[5]=len(labels(c));s.r[6]=OUTPUT
 elif entry in CURVES:s.r[4]=KEYS+component*0x100;s.r[5]=c.get('index',1);s.r[6]=len(labels(c));s.r[7]=OUTPUT
 if c.get('alias'):
  for r in ((4,6) if entry in INDEX else (4,7) if entry in CURVES else (4,)):s.r[r]=(s.r[r]&0xffffff)|c['alias']
 if c.get('bad')=='key' and entry in INDEX+CURVES:s.r[4]=0x8cfffffc
 if c.get('bad')=='output' and entry in INDEX+CURVES:s.r[6 if entry in INDEX else 7]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('special') is not None:
  for state in (MOTION,DUAL_STATE,DUAL_STATE+28):store(m,state+12,c['special'])
 return m,s,f

def curve_oracle(c,component,index):
 keys=labels(c);nxt=(index+1)%len(keys);span=(keys[nxt]-keys[index])&0xffffffff
 span=span if span<0x80000000 else span-0x100000000
 # Original FSRRA is squared; mathematical denominator is a positive span,
 # otherwise normalization is skipped. Backend error is tested with tolerance.
 t=Fraction(c.get('frame',5))-keys[index]
 if span>0:t/=span
 return [(1-t)*a+t*b for a,b in zip(key_values(component,index),key_values(component,nxt))]

def semantic(m,actual,initial,s,f,c):
 entry=initial.pc
 if entry in INDEX:
  frame=int(c.get('frame',5))&0xffffffff
  want=max([i for i,label in enumerate(labels(c)) if label<=frame],default=0)
  assert load(actual,initial.r[6])==want,(c,'independent unsigned key interval')
  assert s.r[15]==initial.r[15],(c,'independent search stack restore');return 2
 if entry in CURVES:
  vals=curve_oracle(c,component_for(entry,c),c.get('index',1))
  got=[load(actual,initial.r[7]+j*4) for j in range(3)]
  if entry in (CURVES[1],CURVES[3]):
   got=[v-0x100000000 if v&0x80000000 else v for v in got]
   assert all(abs(v-int(w))<=1 for v,w in zip(got,vals)),(c,'independent angle interpolation',got,vals)
  else:assert all(abs(value(v)-float(w))<=2e-5 for v,w in zip(got,vals)),(c,'independent vector interpolation',got,vals)
  return 3
 if entry in SAMPLE:
  state=MOTION if entry in SAMPLE[:4] else DUAL_STATE+c.get('channel',0)*28
  assert load(actual,state+20)==c.get('slot',0)+1,(c,'independent consumed track count')
  return 1
 return previous.semantic(m,actual,initial,s,f,c) if entry in (previous.SETUP,previous.SELECT,previous.CHANNEL,previous.CHANNEL_SELECT,*previous.DEFAULTS) else 0

import inspect
exec(compile(inspect.getsource(previous.compare),'motion_sampling_compare','exec'),globals())

def cases():
 yield from previous.cases()
 for entry,bank,keylabels,frame in itertools.product(INDEX,range(2),((0,),(0,4),(0,4,8,12),(0,0,4,12)),(-1,0,1,4,5,8,12,16)):
  yield dict(entry=entry,mode=bank*4,channel=bank,labels=keylabels,frame=frame,semantic=True)
 for entry,bank,index,frame in itertools.product(CURVES,range(2),range(4),(0,1,5,9,12)):
  yield dict(entry=entry,mode=bank*4,channel=bank,index=index,frame=frame,semantic=True)
 for entry,bank,absent,slot in itertools.product(SAMPLE,range(2),(False,True),(0,2)):
  yield dict(entry=entry,mode=bank*4,channel=bank,absent=absent,slot=slot,keys=True,semantic=True)
 for entry,bank,typ in itertools.product(tuple(PRIMARY_KIND)+tuple(SECONDARY_KIND),range(2),range(8)):
  kind=PRIMARY_KIND.get(entry,SECONDARY_KIND.get(entry))
  if typ.bit_count()<=kind:yield dict(entry=entry,mode=bank*4,channel=bank,channels=typ,keys=True,semantic=True)
 for entry,bank,typ in itertools.product(DISPATCH,range(2),(0,1,2,3,4,5,6)):
  yield dict(entry=entry,mode=bank*4,channel=bank,channels=typ,keys=True,semantic=True)
 for entry,mode in itertools.product(sorted(NEW),range(8)):yield dict(entry=entry,mode=mode,keys=True,probe=True)
 for entry,special,enable in itertools.product(INDEX+CURVES,(0,0x80000000,1,0x7f800000,0x7fc12345),(0,0xf80)):
  yield dict(entry=entry,special=special,enable=enable,probe=True)
 for entry,bad in itertools.product(INDEX+CURVES,('key','output','stack')):yield dict(entry=entry,bad=bad)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_motion_sampling_connection.json',PROJECT/'analysis/draw_motion_variants_verification.json',PROJECT/'analysis/draw_motion_variants_extended_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,offset in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[offset:offset+len(data)]=data
 ram=caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=[dict(entry=a,keys=True,semantic=True) for a in INDEX+CURVES+SAMPLE+DISPATCH]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in TRACKED_ROWS:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'sampling comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Sampling smoke passed:',len(records));return
 all_routines=previous.manifest['routines']+manifest['routines'];coverage_rows=[];registerable=[]
 for r in all_routines:
  a=int(r['address'],16);body=set(ALL_BODIES[a]);wanted={tuple(int(x,16) for x in e) for e in r['conditional_edges']}
  row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)])
  coverage_rows.append(row);associated=[x for x in records if x['input']['entry']==a]
  if not row['missing_rows'] and not row['missing_edges'] and associated and all(x['status']!=-2 for x in associated):registerable.append(hex(a))
 files=[Path(__file__),PROJECT/'verify_draw_motion_variants.py',PROJECT/'verify_overlay_draw_motion_variants.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],reused_routines=previous.manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),newly_registered_entries=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Nine original key search/linear interpolation/paired-motion proposals connected to 43 prior native motion/drawing bodies. Original full reached/final machine/RAM, ordered entries and software queues compared; independent sorted-key interval, approximate vector/angle curves and consumed-track checks.',limitations=['FSRRA uses the previously pinned native numerical backend and its qualified domain/error limits; analytic vector checks tolerate 2e-5 and angle checks one integer unit, not arbitrary hardware bit equality. Other numerical/valid-RAM and bounded queue domains remain.','Valid sorted nonempty bounded key arrays are supplied; zero key counts can cause the original search to stall and are not claimed as supported input. Existing real unknown callbacks remain explicit. No complete renderer/GPU pixels/gameplay or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_motion_sampling_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','registerable_entries','boundary_comparisons','independent_cases','unconnected_targets')},indent=2))
if __name__=='__main__':main()
