"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_render_headers as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
CLIP,HUV,HCOL,EUV,ECOL,PROJ,PUV=0x8c64f840,0x8c60607a,0x8c605fca,0x8c64fc68,0x8c64fae8,0x8c650144,0x8c64fe0c
MANIFEST=PROJECT/'analysis/draw_polygon_lower_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={CLIP,HUV,HCOL,EUV,ECOL,PROJ,PUV};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_polygon_lower_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_polygon_lower.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_polygon_lower_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
INPUT,OUTPUT,POINTS,COLOURS,UV,CELL,DEST=0x8cc50000,0x8cc50100,0x8cc60000,0x8cc70000,0x8cc80000,0x8cc90000,0x8cca0000
OUT_POINTS,OUT_COLOURS,OUT_UV=0x8ccb0000,0x8ccc0000,0x8ccd0000
HEADER=0x8c890104
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def points(c):
 n=max(0,c.get('count',3));n=n if n<16 else 0
 zs=c.get('zs',[2]*n)
 return [(Fraction(i*2-3,4),Fraction((i*i+1)%7-2,4),Fraction(zs[i%len(zs)])) for i in range(n)]
def prepare(ram,c):
 entry=c.get('entry',CLIP);m,s,f=SEED.prepare(ram,dict(entry=entry,mode=c.get('mode',0),rm=c.get('rm',0),enable=c.get('enable',0)));s.pc=entry
 for a,v in ((0x8c88f610,c.get('near',1)),(0x8c88f5e4,c.get('scale',1)),(0x8c88f618,2),(0x8c88f61c,-2),(0x8c88f5f0,Fraction(1,4)),(0x8c88f5f4,Fraction(1,2))):store(m,a,bits(v))
 for i,p in enumerate(points(c)):
  for j,x in enumerate(p):store(m,POINTS+i*12+j*4,bits(x))
  store(m,COLOURS+i*4,(0x10203040,0xe0c0a080,0x80ff0010,0xff204080,0x80402010,0x12345678,0x87654321,0xa0c0e0ff)[i%8])
  store(m,UV+i*4,(-30000+i*512)&0xffff,2);store(m,UV+i*4+2,(16000-i*768)&0xffff,2)
 for a,v in ((INPUT,POINTS),(INPUT+4,0 if c.get('no_colours') else COLOURS),(INPUT+8,UV),(OUTPUT,OUT_POINTS),(OUTPUT+4,OUT_COLOURS),(OUTPUT+8,OUT_UV)):store(m,a,v)
 for i in range(0,512,4):
  for a in (OUT_POINTS,OUT_COLOURS,OUT_UV):store(m,a+i,0xa5000000+i)
 cursor=c.get('cursor',DEST+c.get('offset',0));store(m,CELL,(0xe0000000|(cursor&0x3ffffff)) if c.get('device',True) else cursor)
 for i,v in enumerate((0x80840012,0x12345678,0x89abcdef,0x13579bdf,cursor,cursor)):store(m,HEADER+i*4,v)
 if entry==CLIP:s.r[4]=INPUT;s.r[5]=c.get('count',3);s.r[6]=OUTPUT;s.r[7]=c.get('flags',0x80000000)
 else:s.r[4]=c.get('count',3);s.r[5]=INPUT;s.r[6]=CELL
 if c.get('alias'):
  for r in ((4,6) if entry==CLIP else (5,6)):s.r[r]=(s.r[r]&0xffffff)|c['alias']
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='record':s.r[4 if entry==CLIP else 5]=0x8cfffffc
 if c.get('bad') in ('points','colours','uv'):store(m,INPUT+{'points':0,'colours':4,'uv':8}[c['bad']],0x8cfffffc)
 if c.get('bad')=='output':store(m,OUTPUT,0x8cfffffc)
 return m,s,f

def signed16(n):return n-0x10000 if n&0x8000 else n
def clip_oracle(m,c):
 p=points(c);near=Fraction(c.get('near',1));out=[]
 for i,b in enumerate(p):
  j=(i-1)%len(p);a=p[j];vertices=[]
  if (a[2]-near)*(b[2]-near)<0:
   t=(near-a[2])/(b[2]-a[2]);vertices.append((tuple(a[k]+t*(b[k]-a[k]) for k in (0,1))+(near,),j,i,t))
  if b[2]>=near:vertices.append((b,i,i,Fraction(0)))
  out.extend(vertices)
 return out

def semantic_checks(m,actual,initial,s,c,native):
 entry=initial.pc;count=c.get('count',3);checks=0
 assert s.r[15]==initial.r[15],(c,'independent restored guest stack')
 if entry==CLIP:
  out=clip_oracle(m,c);assert s.r[0]==len(out),(c,'independent clipped vertex count')
  for k,(xyz,a,b,t) in enumerate(out):
   assert [load(actual,OUT_POINTS+k*12+j*4) for j in range(3)]==[bits(x) for x in xyz],(c,'independent clipped positions',k)
   ca,cb=load(m,COLOURS+a*4),load(m,COLOURS+b*4);colour=sum((int(Fraction(ca>>shift&255)+t*((cb>>shift&255)-(ca>>shift&255)))&255)<<shift for shift in (0,8,16,24))
   assert load(actual,OUT_COLOURS+k*4)==colour,(c,'independent interpolated colour',k)
   if c.get('flags',0x80000000)&0x80000000:
    for j in range(2):
     ua,ub=signed16(load(m,UV+a*4+j*2,2)),signed16(load(m,UV+b*4+j*2,2));want=int(ua+t*(ub-ua))&0xffff
     assert load(actual,OUT_UV+k*4+j*2,2)==want,(c,'independent signed UV interpolation',k,j)
   checks+=1
  assert native.flushes==0 and native.trace_count==0,(c,'clipper has no graphics queue writes');return checks+1
 if entry in (HUV,HCOL):
  cursor=c.get('cursor',DEST+c.get('offset',0));queue_cursor=0xe0000000|(cursor&0x3ffffff);expected=[load(m,HEADER+j*4) for j in range(4)]
  if entry==HCOL:expected[2]=(expected[2]&0x03efffff)|0x20080000
  assert [load(actual,cursor+j*4) for j in range(4)]==expected,(c,'independent acquired header words')
  assert s.r[0]==queue_cursor+32 and native.flushes==1 and list(native.qacr)==[(cursor>>24)&28]*2,(c,'independent queue/header result')
  assert load(actual,0x8c890128)==cursor&0xfc000000 and load(actual,0x8c89012c)==HEADER+(0x14 if entry==HUV else 0x10),(c,'independent cached destination/header slot');return 3
 start=load(m,CELL);final=load(actual,CELL);device=c.get('device',True)
 if entry in (EUV,ECOL):
  order=[];a,b=0,count-1
  while a<b:order.extend((a,b));a+=1;b-=1
  if a==b:order.append(a)
  assert len(order)==count and final==start+32*count and native.prefetches==count and native.flushes==(count if device else 0),(c,'independent alternating strip count/cursor')
  for ordinal,i in enumerate(order):
   address=(c.get('cursor',DEST+c.get('offset',0))+ordinal*32) if device else start+ordinal*32;x,y,z=points(c)[i];scale=Fraction(c.get('scale',1))
   expected=(0xf0000000 if ordinal==count-1 else 0xe0000000,bits(x*scale*2/z+Fraction(1,4)),bits(y*scale*-2/z+Fraction(1,2)),bits(1/z))
   assert tuple(load(actual,address+j*4) for j in range(4))==expected,(c,'independent projected strip packet',ordinal)
   assert load(actual,address+24)==(0xffffffff if c.get('no_colours') else load(m,COLOURS+i*4)),(c,'independent vertex colour',ordinal)
   if entry==EUV:
    assert [load(actual,address+16+j*4) for j in range(2)]==[bits(Fraction(signed16(load(m,UV+i*4+j*2,2)),256)) for j in range(2)],(c,'independent normalized UV',ordinal)
   checks+=1
  return checks+1
 # The wide projected variants emit a four-vertex strip per closed edge,
 # except edges whose two endpoints are exactly on the near plane.
 zs=[p[2] for p in points(c)];near=Fraction(c.get('near',1));edges=sum(not(zs[i]==near and zs[i-1]==near) for i in range(count));packets=edges*4
 assert final==start+packets*32 and native.prefetches==packets and native.flushes==(packets if device else 0),(c,'independent projected edge-strip count/cursor')
 for i in range(packets):
  a=c.get('cursor',DEST+c.get('offset',0))+i*32 if device else start+i*32
  assert load(actual,a)==(0xf0000000 if i%4==3 else 0xe0000000),(c,'independent four-vertex strip termination',i)
 return 1

def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;ref.qacr[:]=[12,12];namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));native.qacr[:]=[12,12];ml=ctypes.c_uint32(0x12345678);got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem)))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original polygon state',rc,status,hex(s.pc),hex(other.pc))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'original polygon queue state/events')
 checks=semantic_checks(m,actual,initial,s,c,native) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 else 0
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,queue_match=True,queue_events=native.trace_count+native.trace_dropped,queue_flushes=native.flushes,independent_checks=checks,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def cases():
 for n in (3,4,5):
  for mask,flags,bank in itertools.product(range(1<<n),(0,0x80000000),range(2)):yield dict(count=n,zs=[2 if mask>>i&1 else 0 for i in range(n)],flags=flags,mode=bank*4,semantic=True)
 for n,bank in itertools.product((0,1,2,0x80000000,0xffffffff),range(2)):yield dict(count=n,mode=bank*4,semantic=True)
 for n,zs,flags in itertools.product((3,4),((1,1,1,1),(0,1,2,1),(-1,3,-1,3)),(0,0x80000000)):yield dict(count=n,zs=zs,flags=flags,semantic=True)
 for entry,offset,bank in itertools.product((HUV,HCOL),(0,32,64,96),range(2)):yield dict(entry=entry,offset=offset,mode=bank*4,semantic=True)
 for entry,n,device,bank,scale in itertools.product((EUV,ECOL,PROJ,PUV),(3,4,5),(False,True),range(2),(1,2)):yield dict(entry=entry,count=n,device=device,mode=bank*4,scale=scale,semantic=True)
 for n,device,bank in itertools.product((3,4,5),(False,True),range(2)):yield dict(entry=ECOL,count=n,device=device,mode=bank*4,no_colours=True,semantic=True)
 for entry,zs in itertools.product((PROJ,PUV),((1,1,1,1),(1,2,1,2),(2,1,2,1),(1,1,2,1),(2,2,2,2))):yield dict(entry=entry,count=4,zs=zs,semantic=True)
 for entry,mode,rm in itertools.product(tuple(NEW),range(8),range(2)):yield dict(entry=entry,mode=mode,rm=rm,zs=(2,4,8))
 for entry,bad in itertools.product(tuple(NEW),('stack','record','points','colours','uv')):yield dict(entry=entry,bad=bad)
 for entry,special,enable in itertools.product((CLIP,EUV,PROJ,PUV),(0,0x80000000,1,0x7f800000,0x7fc12345),(0,0xf80)):yield dict(entry=entry,special=special,enable=enable,probe=True)
 for entry,alias in itertools.product((CLIP,EUV,ECOL),(0xc000000,0x8c000000,0xac000000)):yield dict(entry=entry,alias=alias,semantic=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_polygon_lower_connection.json',PROJECT/'analysis/draw_render_headers_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,base in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[base:base+len(data)]=data
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=[dict(entry=a,semantic=True) for a in (CLIP,HUV,HCOL,EUV,ECOL,PROJ,PUV)]+[dict(zs=(0,2,0),semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c)
  if 'special' in c:store(m,POINTS+8,c['special'])
  record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%256==0:print(n+1,'original polygon comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Polygon primitive smoke passed:',len(records));return
 coverage_rows=[]
 for r in manifest['routines']:
  a=int(r['address'],16);wanted={tuple(int(x,16) for x in e) for e in r['conditional_edges']};body=set(PER_BODY[a]);coverage_rows.append(dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 print(json.dumps(coverage_rows,indent=2));assert all(not r['missing_rows'] and not r['missing_edges'] for r in coverage_rows)
 files=[Path(__file__),PROJECT/'verify_draw_render_headers.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},newly_registered_entries=7,reused_routine_credit=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Seven complete original polygon clipping, header acquisition, projection/strip helper bodies; original-opcode full reached machine/RAM/boundary and software queue comparisons. Independent exact-dyadic clipping/attributes, basic projection/strip packets, header acquisition and projected edge-strip count/termination checks.',limitations=['Runtime view/near-plane, scratch pointers, cached headers and initial QACR12 are explicit supplied input state, not natural renderer initialization proof.','Independent clipping/projection arithmetic covers finite exact-dyadic cases; wider projected variants have independent edge/packet/count/termination checks plus original-code numerical comparisons, not an independent full geometry oracle.','Existing scalar/vector/sqrt numerical/reference/MMU-off valid-RAM/bounded software queue domains remain; no physical GPU pixels, full frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_polygon_lower_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases','independent_checks','queue_flushes','queue_events')},indent=2))
if __name__=='__main__':main()
