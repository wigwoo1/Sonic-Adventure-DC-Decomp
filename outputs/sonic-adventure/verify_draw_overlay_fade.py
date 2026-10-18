"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_polygon_lower as previous
import verify_overlay_draw_polygon_lower as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
VEC=0x8c63a804
LABEL,TEXT,GLYPH,BACKDROP,FILTER=0xc90c384,0xc90c274,0xc90bfc0,0xc90c160,0x8c605480
MANIFEST=PROJECT/'analysis/draw_overlay_fade_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={LABEL,TEXT,GLYPH,BACKDROP,FILTER,VEC};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=(previous.namespace['LEGACY']-NEW)|{VEC},switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_overlay_fade_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_overlay_fade.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_overlay_fade_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
SOURCE,OBJECT,INFO,POSITION,COLOUR=0x8cc50000,0x8cc60000,0x8cc70000,0x8cc80000,0x8cc90000
MATRIX,PROJECTION=0x8ce00000,0x8c67c640
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def runtime(m):
 listing=0xca540d4;names,count=load(m,listing),load(m,listing+4)
 assert names==0xca53fcc and count==22,(hex(names),count)
 for i in range(count):store(m,names+i*12+8,0x8cf32000+i*32)
 return m
def string(c):return bytes(c.get('text',(65,66,67)))
def prepare(ram,c):
 entry=c.get('entry',LABEL);_,s,f=SEED.prepare(ram,dict(entry=entry,mode=c.get('mode',0),rm=c.get('rm',0),enable=c.get('enable',0)));m=bytearray(ram);s.pc=entry
 matrix=[bits(int(i%5==0)) for i in range(16)];matrix[14]=bits(4)
 for a in (MATRIX,PROJECTION):
  for i,v in enumerate(matrix):store(m,a+i*4,v)
 store(m,0x8c88f5f8,MATRIX);store(m,0x8c88f698,32);store(m,0x8c88f69c,1)
 f.banks[(f.fpscr>>21&1)^1][:]=matrix
 for a in (POSITION,SOURCE+0x20):
  for i,v in enumerate((Fraction(1,4),Fraction(1,2),4)):store(m,a+i*4,bits(v))
 store(m,COLOUR,c.get('colour',0xa5402010));store(m,OBJECT+0x60,c.get('flags',2));store(m,OBJECT+0x74,c.get('alpha',80));store(m,OBJECT+0x78,c.get('age',0),2);store(m,OBJECT+0xd8,bits(c.get('scale',1)));store(m,OBJECT+0xec,INFO);store(m,OBJECT+0x8c4,0x00402010)
 for i in range(16):store(m,INFO+i,0,1)
 for i,v in enumerate(string(c)):store(m,INFO+4+i,v,1)
 f.banks[f.fpscr>>21&1][4]=bits(c.get('scale',1))
 if entry==LABEL:s.r[4]=SOURCE;s.r[5]=OBJECT
 elif entry==TEXT:s.r[4]=INFO;s.r[5]=POSITION;s.r[6]=COLOUR
 elif entry==GLYPH:s.r[4]=c.get('code',65);s.r[5]=0 if c.get('null_colour') else COLOUR
 elif entry==BACKDROP:s.r[4]=COLOUR
 elif entry==VEC:
  s.r[4]=MATRIX if c.get('explicit') else 0;s.r[5]=POSITION
 elif entry==FILTER:s.r[4]=c.get('selector',0)
 if c.get('alias_case'):
  # Real RAM alias fixtures exercise the original post-scan text rechecks.
  # No guest call/result is replaced: matrix output or header-cache writes
  # change these bytes while original text/quad routines execute.
  text_address=s.r[15]-76+4 if c['alias_case']=='point' else 0x8c8a3308
  s.r[4]=text_address-4
  for i in range(8):store(m,text_address+i,65,1)
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='owner':s.r[5]=0x8cfffffc
 if c.get('bad')=='info':store(m,OBJECT+0xec,0x8cfffffc)
 if c.get('bad')=='colour':s.r[5 if entry==GLYPH else 6 if entry==TEXT else 4]=0x8cfffffc
 if 'special' in c:store(m,OBJECT+0xd8,c['special']);f.banks[f.fpscr>>21&1][4]=c['special']
 return m,s,f
def signed32(x):return x-(1<<32) if x&0x80000000 else x
def alpha_state(c):
 age=c.get('age',0);a=(c.get('alpha',80)-30)&0xffffffff
 if c.get('flags',2)&2:
  if age<150:a=(a+70)&0xffffffff;age+=1
 else:age=0
 return min(255,max(0,signed32(a))),age
def prefix(c):return string(c).split(b'\0',1)[0][:7]
def packet_snapshot(mem,r):
 desc=r[4];ptrs=[load(mem,desc+i*4) for i in range(3)] if desc&0x1f000000==0xc000000 and (desc&0xffffff)+12<=0x1000000 else []
 if len(ptrs)!=3 or any(a&0x1f000000!=0xc000000 or (a&0xffffff)+n>0x1000000 for a,n in zip(ptrs,(48,16,16))):return dict(valid=False)
 positions,colours,uv=ptrs
 return dict(valid=True,count=r[5],flags=r[6],positions=[load(mem,positions+i*4) for i in range(12)],colours=[load(mem,colours+i*4) for i in range(4)],uv=[load(mem,uv+i*2,2) for i in range(8)])
def semantic_checks(m,actual,initial,s,f,c,path,calls,quads,filter_inputs,text_colours):
 checks=0;entry=initial.pc
 if entry==LABEL and 0xc90c41c in path:
  a,age=alpha_state(c);assert load(actual,OBJECT+0x74)==a and load(actual,OBJECT+0x78,2)==age,(c,'independent signed/wrapped alpha clamp and age');checks+=1
  if text_colours:assert text_colours==[0x00402010|(a<<24)],(c,'independent fading label RGBA');checks+=bool(text_colours)
 if c.get('semantic') and not c.get('alias_case') and not c.get('bad'):
  if entry==VEC:
   desired=[bits(x) for x in (Fraction(1,4),Fraction(1,2),8,1)]
   assert ([load(actual,MATRIX+48+i*4) for i in range(4)] if c.get('explicit') else list(f.banks[(f.fpscr>>21&1)^1][12:16]))==desired,(c,'independent vector translation')
   checks+=1
  if entry in (LABEL,TEXT):
   visible=entry==TEXT or alpha_state(c)[0]>10;chars=list(prefix(c)) if visible else []
   assert calls==chars,(c,'independent seven-byte text cap/order',calls,chars)
   assert len(quads)==(len(chars)+1 if chars else 0),(c,'independent glyph/backdrop count')
   if visible and chars:assert load(actual,INFO+11,1)==0,(c,'independent forced terminator')
   checks+=1
  if entry in (GLYPH,BACKDROP):assert len(quads)==1,(c,'independent direct quad count')
  if entry in (LABEL,TEXT,GLYPH,BACKDROP):
   codes=([c.get('code',65)] if entry==GLYPH else list(prefix(c)))
   for i,q in enumerate(quads):
    backdrop=entry==BACKDROP or entry in (LABEL,TEXT) and i==len(quads)-1
    assert q['valid'] and q['count']==4 and q['flags']==0x80000060,(c,'independent quad descriptor')
    template=load(m,0xc90c3c0 if backdrop else 0xc90c140)
    assert q['positions']==[load(m,template+j*4) for j in range(12)],(c,'independent original quad template')
    colour=(0x00402010|(alpha_state(c)[0]<<24)) if entry==LABEL else c.get('colour',0xa5402010)
    if backdrop:colour=(colour&0xff000000)|0xffffff;uv=[0,255,0,0,255,0,255,255]
    else:
     if c.get('null_colour'):colour=0xffffffff
     code=codes[i]&255;index=code-32 if 32<=code<=191 else 0;u=(index&15)*16;v=(index>>4)*16;uv=[u,v+15,u,v,u+15,v,u+15,v+15]
    assert q['colours']==[colour]*4 and q['uv']==uv,(c,'independent atlas UV and quad RGBA',i);checks+=1
  if entry==FILTER:
   selector=c.get('selector',0);param=load(m,load(m,0x8c6054ec));expected={0:0,1:2,2:1}.get(selector,load(m,param+0x54));flag=signed32(load(m,0x8c6054e8,2)|((0xffff0000) if load(m,0x8c6054e8,2)&0x8000 else 0))&0xffffffff
   assert filter_inputs and filter_inputs[0]==(flag,expected),(c,'independent filter selector/state arguments');checks+=1
 return checks

def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);got=[];calls=[];quads=[];filter_inputs=[];text_colours=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem)))
  if target==GLYPH:calls.append(r[4]&255)
  if target==0x8c63d414:quads.append(packet_snapshot(mem,r))
  if target==0x8c654e60:filter_inputs.append((load(mem,r[4]),load(mem,r[4]+0x54)))
  if target==TEXT and initial.pc==LABEL:text_colours.append(load(mem,r[6]))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original overlay fade state',rc,status,hex(s.pc),hex(other.pc),len(got),len(wanted))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'original fade queue state/events')
 checks=semantic_checks(m,actual,initial,s,f,c,path,calls,quads,filter_inputs,text_colours) if not initial_f.fpscr&0x180000 and not c.get('bad') else 0
 if rc==0 and not initial_f.fpscr&0x180000:assert s.r[15]==initial.r[15],(c,'restored guest stack')
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,queue_match=True,queue_events=native.trace_count+native.trace_dropped,queue_flushes=native.flushes,independent_checks=checks,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def cases():
 for explicit,bank in itertools.product((False,True),range(2)):yield dict(entry=VEC,explicit=explicit,mode=bank*4,semantic=True)
 for alpha,flags,age in itertools.product((0,10,29,30,39,40,41,80,215,255,256,0x7fffffff,0x80000000,0xffffffff),(0,2),(0,149,150,65535)):yield dict(alpha=alpha,flags=flags,age=age,text=(),semantic=True)
 for code in range(256):yield dict(entry=GLYPH,code=code,semantic=True)
 for code,null_colour,bank in itertools.product((0,31,32,191,192,255,0x12340041,0xffffffff),(False,True),range(2)):yield dict(entry=GLYPH,code=code,null_colour=null_colour,mode=bank*4,semantic=True)
 for text,bank,scale in itertools.product(((),(65,),(65,66),(65,66,67,68,69,70,71),(65,66,67,68,69,70,71,72),(128,191,192),(65,0,66)),range(2),(1,.5)):yield dict(entry=TEXT,text=text,mode=bank*4,scale=scale,semantic=True)
 for alpha,flags,bank in itertools.product((40,41,80,255),(0,2),range(2)):yield dict(alpha=alpha,flags=flags,mode=bank*4,semantic=True)
 for colour,bank in itertools.product((0,0xff000000,0x80402010,0xffffffff),range(2)):yield dict(entry=BACKDROP,colour=colour,mode=bank*4,semantic=True)
 for selector,bank in itertools.product((0,1,2,3,0xffffffff),range(2)):yield dict(entry=FILTER,selector=selector,mode=bank*4,semantic=True)
 for alias in ('point','cache'):yield dict(entry=TEXT,alias_case=alias)
 for entry,mode in itertools.product(tuple(NEW),range(8)):yield dict(entry=entry,mode=mode,text=())
 for entry,bad in itertools.product(tuple(NEW),('stack','source','owner','info','colour')):yield dict(entry=entry,bad=bad)
 for special,enable in itertools.product((0,0x80000000,1,0x7f800000,0x7fc12345),(0,0xf80)):yield dict(special=special,enable=enable,probe=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_overlay_fade_connection.json',PROJECT/'analysis/draw_polygon_lower_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,base in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[base:base+len(data)]=data
 ram=runtime(caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]);selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=[dict(entry=a,semantic=True) for a in (LABEL,TEXT,GLYPH,BACKDROP,FILTER)]+[dict(entry=VEC,explicit=e,semantic=True) for e in (False,True)]+[dict(entry=TEXT,bad='owner')]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'original overlay fade comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Overlay fade smoke passed:',len(records));return
 # Original EXTU.B at c0a8 makes r0 at c0ac nonnegative. Retain its signed
 # remainder fallback, but do not fabricate five unreachable instructions.
 impossible_rows=set(range(0xc90c0b4,0xc90c0be,2));impossible_edges={(0xc90c0ae,0xc90c0b4),(0xc90c2be,0xc90c2ca)}
 assert [OWN[a] for a in (0xc90c0a8,0xc90c0aa,0xc90c0ac,0xc90c0ae)]==['extu.b r5,r6','mov r6,r0','cmp/pz r0','bf 0x0c90c0b4']
 assert all(x>=0 for x in range(256))
 assert [OWN[a] for a in (0xc90c2ae,0xc90c2b0,0xc90c2b2,0xc90c2b6,0xc90c2b8,0xc90c2ba,0xc90c2bc,0xc90c2be)]==['mov.b @r5,r2','tst r2,r2','bt/s 0x0c90c368','mov r6,r0','mov.b r0,@(0x7,r5)','mov.b @r5,r3','tst r3,r3','bt 0x0c90c2ca']
 assert OWN[0xc90c2b4]=='mov r6,r13' and 7%(1<<24)!=0
 coverage_rows=[]
 for r in manifest['routines']:
  a=int(r['address'],16);wanted={tuple(int(x,16) for x in e) for e in r['conditional_edges']};body=set(PER_BODY[a]);coverage_rows.append(dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),unreachable_rows=[hex(x) for x in sorted(body&impossible_rows)],missing_rows=[hex(x) for x in sorted(body-coverage-impossible_rows)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),unreachable_edges=[[hex(x),hex(y)] for x,y in sorted(wanted&impossible_edges)],missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges-impossible_edges)]))
 print(json.dumps(coverage_rows,indent=2));assert all(not r['missing_rows'] and not r['missing_edges'] for r in coverage_rows)
 files=[Path(__file__),PROJECT/'verify_draw_polygon_lower.py',PROJECT/'verify_overlay_draw_polygon_lower.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),unreachable_rows=[hex(x) for x in sorted(impossible_rows)],unreachable_edges=[[hex(x),hex(y)] for x,y in sorted(impossible_edges)],boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),newly_registered_entries=5,reused_routine_credit=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Five original fade-label/text/quad/filter-state bodies plus the reused guarded 56-row vector translation body connected to real native matrix/render/header/polygon services. Full reached machine/RAM/ordered entries and bounded software queue comparisons; independent alpha/age, seven-byte text order/count, atlas UV, original quad templates, RGBA and filter state checks.',limitations=['Five retained signed-remainder fallback rows and two outcomes are impossible under checked unsigned-byte / coherent valid-RAM arguments. Explicit stack-point and header-cache alias fixtures exercise real text post-scan rechecks, not injected callbacks.','Runtime view/matrix/scratch and texture metadata remain supplied. The original 22-name list 0ca540d4 has only runtime descriptor fields populated; names/attributes/model/motion/templates remain intact. Natural initialization/loading unproved.','Scalar/vector/sqrt numerical/reference and MMU-off valid-RAM/queue limits remain. First 4096 queue events per case compared individually; later counts, final queue/counters and full reached/final RAM compared. No GPU pixels, complete frame/gameplay/high-FPS or playable port proof.'],cases=records)
 (PROJECT/'analysis/draw_overlay_fade_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases','independent_checks','queue_flushes','queue_events','unconnected_targets')},indent=2))
if __name__=='__main__':main()
