"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_motion_sampling as previous
import verify_overlay_draw_motion_sampling as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_gbr_geometry_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_gbr_geometry_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_gbr_geometry.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_gbr_geometry_connected;fn.argtypes=previous.fn.argtypes+[ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
import math,re
import verify_draw_motion_mesh as cpu_seed
INIT,CULL,SAMPLE,POINT,ROTATE,SCALE,COPY=0x8c036f60,0x8c03718c,0x8c60a3f8,0x8c64f31c,0x8c64f244,0x8c64f2dc,0x8c60b3e2
NODE,MODEL,VECTOR,KEYS=previous.NODE,previous.MODEL,previous.OUTPUT,previous.KEYS
INDEX_WORD,BLOCKS=0x8ccd0000,0x8cce0000
ORIGINAL_GBR_LITERAL=0x8c036fb4

cpu_code=cpu_seed.cpu_code
marker='        elif word == 9: pass';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,"""        elif word == 0x0008:sr &= ~1
        elif word == 0x0018:sr |= 1
        elif word & 0xFF00 == 0xC600:
            assert special is not None,'GBR-relative load requires explicit special-register state'
            regs[0]=read32(special['gbr']+((word&255)*4))
"""+marker)
cpu_ns=dict(cpu_seed.cpu_ns);exec(compile(cpu_code,'gbr_original_device_executor','exec'),cpu_ns)
old_dynamic=previous.namespace['dynamic_access']
def dynamic_access(text,r,fpscr,special):
 if 'GBR' in text and (match:=re.search(r'@\(0x([0-9a-f]+),GBR\)',text)):
  return (special['gbr']+int(match[1],16))&0xffffffff,4
 return old_dynamic(text,r,fpscr)
namespace=dict(namespace,reference=cpu_ns['reference'],dynamic_access=dynamic_access)
vector_words=set(namespace['VECTOR_WORDS'])
for a,t in OWN.items():
 if t.startswith(('fsca ','fipr ','ftrv ','fsrra ')):
  raw=SEED.state.gate.support.morph.MAIN.read_bytes();vector_words.add(int.from_bytes(raw[a-0x8c010000:a-0x8c010000+2],'little'))
namespace['VECTOR_WORDS']=vector_words
code=previous.code
marker="special={'macl':c.get('macl',0x12345678)}";assert code.count(marker)==1
code=code.replace(marker,"special={'macl':c.get('macl',0x12345678),'gbr':c.get('gbr',0)}")
marker="special['macl'],hashlib.sha256(memory).digest()";assert code.count(marker)==1
code=code.replace(marker,marker+",special['gbr']")
code=code.replace('dynamic_access(text,r,f.fpscr)','dynamic_access(text,r,f.fpscr,special)')
code=code.replace("return status,path,events,special['macl']","return status,path,events,special['macl'],special['gbr']")
exec(compile(code,'draw_gbr_geometry_reference','exec'),namespace);reference_run=namespace['reference_run']

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def runtime(ram):
 """Compute initializer output with both original instructions and native code."""
 _,s,f=SEED.prepare(ram,dict(entry=INIT,mode=0));s.pc=INIT
 expected=bytearray(ram);actual=bytearray(ram);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 rc,path,_,macl,gbr=reference_run(expected,other,ef,dict(gbr=0),tracked=())
 native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(0)
 got=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),INIT,None,OBSERVE(),ctypes.byref(bus),ctypes.byref(gb))
 assert got==rc==0 and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==macl and gb.value==gbr
 assert gbr==load(actual,ORIGINAL_GBR_LITERAL) and len(path)==41
 return actual,gbr

def prepare(ram,c):
 entry=c.get('entry',INIT);m,s,f=previous.prepare(ram,dict(c,entry=entry))
 if entry not in (INIT,CULL):m,gbr=runtime(m)
 else:gbr=c.get('gbr',0x8ce10000)
 c['gbr']=gbr
 if entry==CULL:
  # Explicit renderer inputs for direct predicate tests; caller tests retain
  # their existing globals and compute initialization from the original routine.
  for a,v in ((0x8c88f5f0,1),(0x8c88f5f4,0),(0x8c88f5e4,1),(0x8c88f61c,1),(0x8c88f610,1),(0x8c88f614,8)):
   store(m,a,bits(c.get('globals',{}).get(hex(a),v)))
  m,gbr=runtime(m);c['gbr']=gbr
  clip=load(m,0x8c037210)
  for i,v in enumerate((-1,-1,1,1)):store(m,clip+i*4,bits(v))
  for i,v in enumerate(c.get('center',(0,0,0))):store(m,MODEL+0x18+i*4,bits(v))
  store(m,MODEL+0x24,bits(c.get('radius',1)))
  store(m,MODEL+0xc,INDEX_WORD);store(m,MODEL+0x10,BLOCKS);store(m,INDEX_WORD,c.get('material',2),2)
  masks=[load(m,a) for a in (0x8c03727c,0x8c037288,0x8c03728c)]
  flagcode=c.get('flag_code',0);flags=0
  for i,mask in enumerate(masks):
   if flagcode>>i&1:flags|=mask
  store(m,load(m,0x8c037278),flags)
  profile=c.get('attribute_profile',0)
  store(m,load(m,0x8c037280),0xf0f0f0f0 if profile else 0x12345678)
  store(m,load(m,0x8c037284),0x10203040 if profile else 0xa1b2c3d4)
  matmask=load(m,0x8c037276,2);store(m,BLOCKS+(c.get('material',2)&matmask)*20+16,0x13579bdf)
  for off in (0x1c,0x20,0x24,0x28,0x2c,0x34):store(m,gbr+off,0xa5a5a5a5)
  s.r[4]=MODEL
 elif entry in (POINT,SCALE,ROTATE):
  vals=c.get('angles',(0,0,0)) if entry==ROTATE else c.get('scale',(1,2,.5)) if entry==SCALE else c.get('point',(.25,.5,1))
  for i,v in enumerate(vals):store(m,VECTOR+i*4,int(v)&0xffffffff if entry==ROTATE else bits(v))
  s.r[4]=VECTOR;s.r[5]=c.get('order',0)
 elif entry==SAMPLE:
  store(m,0x8c89001c,0x8c60c026 if c.get('cubic') else 0x8c60beb0)
 if entry==INIT:
  for a,v in c.get('globals',{}).items():store(m,int(a,16),bits(v))
 if c.get('special') is not None:
  target=0x8c88f5e4 if entry==INIT else MODEL+0x24 if entry==CULL else previous.DUAL_STATE+c.get('channel',0)*28+12 if entry==SAMPLE else VECTOR+4
  store(m,target,c['special'])
 if c.get('gbr_alias') is not None:c['gbr']=(c['gbr']&0xffffff)|c['gbr_alias']
 if c.get('bad')=='gbr':c['gbr']=0x8cfffffc
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='index' and entry==CULL:store(m,MODEL+0xc,0x8cffffff)
 if c.get('bad')=='block' and entry==CULL:store(m,MODEL+0x10,0x8cfffffc)
 return m,s,f

def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl,expected_gbr=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c.get('gbr',0));got=[];curve_checks=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem),gb.value))
  if initial.pc==SAMPLE and c.get('semantic') and not c.get('bad') and target==load(m,0x8c89001c):
   channel=c.get('channel',0);frame=value(load(m,previous.DUAL_STATE+channel*28+12));threshold=value(load(m,0x8c60a57c));extra=value(load(m,0x8c60a580))
   converted=int(value(bits(frame+extra)) if frame>threshold else frame)&0xffffffff
   slot=c.get('slot',0);key=load(m,previous.POINTERS+slot*4);n=load(m,previous.COUNTS+slot*4)
   want=max([i for i in range(n) if load(m,key+i*16)<=converted],default=0)
   assert r[4]==key and r[5]==want and r[6]==n,(c,'independent unsigned-frame search/curve arguments')
   curve_checks.append(True)
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus),ctypes.byref(gb))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl and gb.value==expected_gbr,(c,'original GBR geometry full state',rc,status,hex(s.pc),hex(other.pc),len(got),len(wanted),hex(gb.value),hex(expected_gbr))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'GBR geometry queue state')
 checks=semantic(m,actual,initial,initial_f,s,f,gb.value,c)+len(curve_checks) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 and not c.get('bad') else 0
 return dict(input=dict(c),status=rc,full_state_match=True,ordered_boundaries_match=True,gbr_final_and_ordered_entries_match=True,queue_match=True,independent_checks=checks,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def semantic(m,actual,initial,initial_f,s,f,gbr,c):
 if initial.pc==INIT:
  assert gbr==load(m,ORIGINAL_GBR_LITERAL),(c,'independent original GBR literal')
  fields=[(0,0x8c036fb8),(4,0x8c036fbc),(8,0x8c036fc0),(12,0x8c036fc4),(16,0x8c036fc8)]
  assert all(load(actual,gbr+off)==load(m,load(m,lit)) for off,lit in fields),(c,'independent renderer field copies')
  assert load(actual,gbr+0x58)==load(m,0x8c036fd0) and load(actual,gbr+0x38)==load(m,0x8c036fd4),(c,'independent renderer handlers');return 3
 if initial.pc==CULL:
  x,y,z=c.get('center',(0,0,0));z+=4;radius=c.get('radius',1)
  rejected=z<=1-radius or z>8+radius
  if not rejected:
   inverse=1/(z+radius);right=(x+radius)*inverse+1;left=(x-radius)*inverse+1
   top=(y+radius)*inverse;bottom=(y-radius)*inverse
   rejected=right<=-1 or left>1 or top<=-1 or bottom>1
  assert s.sr&1==int(rejected),(c,'independent original sphere predicate')
  wanted=bytearray(m)
  if not rejected:
   flags=load(m,load(m,0x8c037278));a,b,d=[load(m,l) for l in (0x8c03727c,0x8c037288,0x8c03728c)]
   selection=1 if flags&d else 0xffffffff if flags&b else 0
   block=load(m,MODEL+0x10)+(load(m,load(m,MODEL+0xc),2)&load(m,0x8c037276,2))*20
   attrs=load(m,block+16)
   # The configuration pointer is GBR+1c, the record just written above.
   # The original therefore gates on flag A and uses the newly copied globals.
   assert load(m,0x8c037290)&0xffffff==(gbr+0x1c)&0xffffff
   if flags&a:attrs=(attrs&load(m,load(m,0x8c037280)))|load(m,load(m,0x8c037284))
   for off,v in ((0x1c,int(bool(flags&a))),(0x20,load(m,load(m,0x8c037280))),(0x24,load(m,load(m,0x8c037284))),(0x28,selection),(0x34,block),(0x2c,attrs)):store(wanted,gbr+off,v)
  assert actual==wanted,(c,'independent predicate RAM footprint and attributes');return 2
 if initial.pc==POINT:
  bank=(initial_f.fpscr>>21&1)^1;point=c.get('point',(.25,.5,1))
  assert list(f.banks[bank][12:16])==[bits(point[0]),bits(point[1]),bits(point[2]+4),bits(1)],(c,'independent translated XMTRX column')
  assert s.r[4]==initial.r[4]+12,(c,'independent point consumption');return 2
 if initial.pc==SCALE:
  bank=(initial_f.fpscr>>21&1)^1;scale=c.get('scale',(1,2,.5));wanted=list(initial_f.banks[bank])
  for col in range(3):
   for row in range(4):wanted[col*4+row]=bits(value(wanted[col*4+row])*scale[col])
  assert list(f.banks[bank])==wanted,(c,'independent matrix scale and preserved translation');return 2
 if initial.pc==ROTATE:
  bank=(initial_f.fpscr>>21&1)^1;matrix=[value(x) for x in f.banks[bank]];columns=[matrix[i*4:i*4+3] for i in range(3)]
  assert all(abs(sum(x*x for x in col)-1)<2e-6 for col in columns),(c,'independent unit rotation columns')
  assert all(abs(sum(a*b for a,b in zip(columns[i],columns[j])))<2e-6 for i,j in ((0,1),(0,2),(1,2))),(c,'independent perpendicular rotation columns')
  assert list(f.banks[bank][12:16])==list(initial_f.banks[bank][12:16]),(c,'independent rotation translation preserved');return 3
 if initial.pc==SAMPLE:
  state=previous.DUAL_STATE+c.get('channel',0)*28
  assert load(actual,state+20)==c.get('slot',0)+1,(c,'independent consumed sampled track')
  if c.get('absent'):
   pool=load(m,0x8c60a578)+c.get('channel',0)*12
   assert [load(actual,pool+j*4) for j in range(3)]==[load(m,NODE+8+j*4) for j in range(3)],(c,'independent sampled default pose')
  return 2
 if initial.pc==COPY:
  pool=load(m,0x8c60b500);channel=load(m,load(m,0x8c60b4fc),2)
  assert [load(actual,pool+channel*12+j*4) for j in range(3)]==[load(m,NODE+8+j*4) for j in range(3)],(c,'independent default buffer copy');return 1
 return 0

def cases():
 for entry in (INIT,CULL,SAMPLE,POINT,ROTATE,SCALE,COPY):yield dict(entry=entry,keys=True,semantic=True)
 for bank,offset,scale in itertools.product(range(2),(0,.5,1),(.5,1,2,4)):
  yield dict(entry=INIT,mode=bank*4,globals={'0x8c88f5f0':offset,'0x8c88f5f4':-offset,'0x8c88f5e4':scale},semantic=True)
 centers=((0,0,-4),(0,0,32),(-32,0,0),(32,0,0),(0,-32,0),(0,32,0),(0,0,0))
 for bank,center,radius,flags,profile in itertools.product(range(2),centers,(.5,1),(0,1,2,4,7),(0,1)):
  yield dict(entry=CULL,mode=bank*4,center=center,radius=radius,flag_code=flags,attribute_profile=profile,semantic=True)
 for bank,absent,cubic,frame,slot in itertools.product(range(2),(False,True),(False,True),(-1,-.5,0,.5,1,3.5,4,5,12,16),(0,2)):
  yield dict(entry=SAMPLE,mode=bank*4,channel=bank,absent=absent,cubic=cubic,frame=frame,slot=slot,keys=True,semantic=True)
 for bank,frame in itertools.product(range(2),(2147483648.,2147484160.,3221225472.,4294967040.)):
  yield dict(entry=SAMPLE,mode=bank*4,channel=bank,frame=frame,keys=True,probe=True,semantic=True)
 angles=((0,0,0),(0x4000,0,0),(0,0x4000,0),(0,0,0x4000),(0x4000,0x4000,0x4000),(0x8000,0xc000,0),(0x1234,0xabcd,0x4321),(0xffffffff,1,0x10000))
 for bank,order,angle in itertools.product(range(2),(-1,0,1,2),angles):yield dict(entry=ROTATE,mode=bank*4,order=order&0xffffffff,angles=angle,semantic=True)
 for bank,point in itertools.product(range(2),((0,0,0),(.25,.5,1),(-1,2,-4),(1,-.5,2))):yield dict(entry=POINT,mode=bank*4,point=point,semantic=True)
 for bank,scale in itertools.product(range(2),((1,1,1),(.5,2,4),(-1,0,2),(0,0,0))):yield dict(entry=SCALE,mode=bank*4,scale=scale,semantic=True)
 for bank,channel,alias in itertools.product(range(2),range(2),(0x0c000000,0x8c000000,0xac000000)):yield dict(entry=COPY,mode=bank*4,channel=channel,gbr_alias=alias,semantic=True)
 for entry,mode in itertools.product(sorted(NEW),range(8)):yield dict(entry=entry,mode=mode,keys=True,probe=True)
 for entry,bad in itertools.product(sorted(NEW),('stack','source','gbr')):yield dict(entry=entry,bad=bad,keys=True)
 for bad in ('index','block'):yield dict(entry=CULL,bad=bad)
 for bank,alias in itertools.product(range(2),(0x0c000000,0x8c000000,0xac000000)):yield dict(entry=CULL,mode=bank*4,gbr_alias=alias,semantic=True)
 for entry,special,enable in itertools.product((INIT,CULL,SAMPLE,POINT,SCALE,ROTATE),(0,0x80000000,1,0x7f800000,0x7fc12345),(0,0xf80)):
  yield dict(entry=entry,special=special,enable=enable,probe=True,keys=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_gbr_geometry_connection.json',PROJECT/'analysis/draw_motion_sampling_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,offset in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[offset:offset+len(data)]=data
 ram=caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=selected[:7]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%128==0:print(n+1,'explicit-GBR comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Explicit-GBR smoke passed:',len(records));return
 coverage_rows=[];registerable=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=set(PER_BODY[a]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]);coverage_rows.append(row)
  tests=[r for r in records if r['input']['entry']==a]
  if not row['missing_rows'] and not row['missing_edges'] and all(r['status']!=-2 for r in tests):registerable.append(hex(a))
 files=[Path(__file__),PROJECT/'verify_draw_motion_sampling.py',PROJECT/'verify_draw_motion_mesh.py',PROJECT/'verify_startup.py',PROJECT/'verify_overlay_draw_motion_sampling.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),original_gbr_literal=hex(load(ram,ORIGINAL_GBR_LITERAL)),native_original_initializer_stages_per_noninitializer_case=1,external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Seven original renderer/GBR/geometry/position sampling bodies, explicit original LDC/GBR-relative loads/stores and SETT/CLRT. Full original-model machine/RAM/GBR, ordered entries including GBR and software queues compared. Real initializer executes natively and in original instruction model for supplied runtime renderer inputs.',limitations=['Runtime renderer/view/texture globals and direct synthetic model/key records are explicit inputs; initializer execution is not proof of natural engine startup scheduling. Original caller records are retained by the integration suite.','Qualified scalar/vector/FSCA/FSRRA/sqrt numeric model, legacy mode guards, MMU-off valid RAM and bounded queue policies retained. No arbitrary hardware bit/timing/MMU/fault or complete GPU/frame/gameplay/playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_gbr_geometry_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','registerable_entries','independent_cases','unconnected_targets')},indent=2))
if __name__=='__main__':main()
