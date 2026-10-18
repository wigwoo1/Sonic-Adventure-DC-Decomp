"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_object_effects as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
STATE,HEADER,VIS,SIZE=0x8c654e60,0x8c655620,0x8c64f5bc,0x8c654b60
MANIFEST=PROJECT/'analysis/draw_render_headers_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={STATE,HEADER,VIS,SIZE};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
switch_targets.update({int(r['pc'],16):{int(a,16) for a in r['entries']} for r in manifest['computed_switches']})
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code;exec(compile(code,'draw_render_headers_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_render_headers.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_render_headers_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
PARAM,DESC,POINTS,OBJECT=0x8cc50000,0x8cc60000,0x8cc70000,0x8cc80000
TEMP,OUTPUT,LUT,LAYOUT,KIND,RESULT=0x8c8a3298,0x8c67c014,0x8c67c054,0x8c8a3308,0x8c8a33e8,0x8c8a3374
SOURCE_MODES={0:(5,4),1:(4,5),2:(6,None),3:(2,None),4:(7,None),5:(3,None),6:(5,None),8:(4,None),10:(1,None),11:(0,None)}
DEST_MODES={0:(5,4),1:(4,5),2:(None,6),4:(None,7),6:(None,5),7:(None,3),8:(None,4),9:(None,2),10:(None,1),11:(None,0)}
LAYOUT_BITS=(2,4,5,6,24,29)

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def control(index,variant=0):return 0x80000000|sum((index>>i&1)<<bit for i,bit in enumerate(LAYOUT_BITS))|(variant<<24)

def prepare(ram,c):
 entry=c.get('entry',STATE);m,s,f=SEED.prepare(ram,dict(entry=entry,mode=c.get('mode',0),rm=c.get('rm',0),enable=c.get('enable',0)))
 s.pc=entry;param=c.get('param',PARAM);s.r[4]=param;s.r[5]=0
 for i in range(0,0x240,4):store(m,param+i,c.get('v',0))
 store(m,param,c.get('flags',0));store(m,param+0x20,c.get('texture_mode',0));store(m,param+0x2c,c.get('source',0));store(m,param+0x30,c.get('dest',0))
 store(m,param+0x50,c.get('filter',0));store(m,param+0x54,c.get('filter_dest',(c.get('filter',0)+1)&3));store(m,param+0x6c,DESC if c.get('texture_mode',0) in (2,3) and not c.get('null_texture') else 0)
 for off,v in ((8,c.get('format',0)),(12,c.get('width',32)),(16,c.get('height',64)),(24,c.get('texflags',0)),(28,0x12345678)):store(m,DESC+off,v)
 colours=((0,.25,.5,1,.5,.75,1,0),(.25,.5,.75,1,1,.75,.5,.25),(-.25,1.25,2,.5,1,-.5,.25,0))[c.get('colours',0)]
 for off,v in zip((0x70,0x74,0x78,0x7c,0x80,0x84,0x88,0x8c),colours):store(m,param+off,bits(v))
 cached=c.get('cached',(0x82c4003f,0x45600002,0x954321ff,0x00123456))
 for i,v in enumerate(cached):store(m,param+0x90+i*4,v)
 store(m,param+0xa0,c.get('tail_control',0x89abcdef))
 for i in range(24):store(m,TEMP+i*4,0x12340000+i)
 # Leave input fields intact for explicit alias fixtures after cache setup.
 if entry==HEADER:
  store(m,param+0x90,c.get('control',control(c.get('index',0),c.get('variant',0))))
  if 'lookup_type' in c:
   cw=load(m,param+0x90);index=((cw>>29&1)<<5)|((cw>>24&1)<<4)|((cw>>6&1)<<3)|((cw>>5&1)<<2)|((cw>>4&1)<<1)|(cw>>2&1);store(m,LUT+index*4,c['lookup_type'])
 if entry==VIS:
  store(m,OBJECT,POINTS);s.r[4]=OBJECT;s.r[5]=c.get('count',4)
  limit=c.get('limit',1);store(m,0x8c88f614,bits(limit))
  n=c.get('count',4);n=n if n<0x80000000 else 0
  for i in range(n):
   z=limit if c.get('zcase',0)==0 else limit-1 if c.get('zcase')==1 else limit+1 if c.get('zcase')==2 else limit+1 if i==n-1 else limit-1
   for j,v in enumerate((.25,-.5,z)):store(m,POINTS+i*12+j*4,bits(v))
 if entry==SIZE:s.r[4]=c.get('size',8)
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='descriptor':store(m,param+0x6c,0x8cfffffc)
 if c.get('bad')=='points':store(m,OBJECT,0x8cfffffc)
 if c.get('bad')=='switch':store(m,0x8c654fcc,1,2)
 if c.get('alias'):s.r[4]=(s.r[4]&0xffffff)|c['alias']
 return m,s,f

def pack(values):
 result=0
 for word,shift in zip(values,(24,16,8,0)):
  product=bits(Fraction(value(word))*255);integer=int(Fraction(value(product)));result|=(integer<<shift)&0xffffffff
 return result

def header_oracle(m,param,initial_s):
 result=bytearray(m)
 store(result,initial_s.r[15]-4,initial_s.r[14])
 for src,dst in ((0x90,0),(0x94,0x10),(0x98,0x14)):store(result,TEMP+dst,load(result,param+src))
 cw=load(result,TEMP)
 if cw>>24&7==0:store(result,TEMP+0x14,load(result,TEMP+0x14)&0x03ffffff|0x20000000)
 store(result,TEMP+0x18,load(result,param+0x9c))
 for i,off in enumerate((0x70,0x74,0x78,0x7c,0x80,0x84,0x88,0x8c)):store(result,TEMP+0x20+i*4,load(result,param+off))
 store(result,TEMP+0x40,load(result,param+0xa0))
 index=((cw>>29&1)<<5)|((cw>>24&1)<<4)|((cw>>6&1)<<3)|((cw>>5&1)<<2)|((cw>>4&1)<<1)|(cw>>2&1)
 layout=load(result,LUT+index*4);kind=cw>>24&7;store(result,LAYOUT,layout);store(result,KIND,kind);store(result,OUTPUT,cw)
 cache=[load(result,TEMP+i*4) for i in range(24)]
 if kind&1:
  store(result,OUTPUT+4,cache[16]);length=32
 else:
  for i in range(3):store(result,OUTPUT+4+i*4,cache[4+i])
  if layout==0:length=32
  elif layout==1:
   for i in range(4):store(result,OUTPUT+0x10+i*4,cache[8+i])
   length=32
  elif layout==2:
   for i in range(8):store(result,OUTPUT+0x20+i*4,cache[8+i])
   length=64
  elif layout in (3,4):
   store(result,OUTPUT+0x10,cache[17]);store(result,OUTPUT+0x14,cache[18]);length=32
   if layout==4:
    for i in range(4):store(result,OUTPUT+0x20+i*4,cache[8+i]);store(result,OUTPUT+0x30+i*4,cache[20+i])
    length=64
  elif layout==16:
   store(result,OUTPUT+0x10,pack(cache[8:12]));store(result,OUTPUT+0x14,pack(cache[12:16]));length=32
  else:length=16
 store(result,RESULT,kind);store(result,RESULT+4,length)
 return result,layout,kind,length

def semantic_checks(m,actual,initial_s,s,c):
 entry=initial_s.pc;param=initial_s.r[4]
 if entry==SIZE:
  n=c.get('size',8);want={8<<i:i for i in range(8)}.get(n,n);assert s.r[0]==want and s.r[4]==want,(c,'independent size conversion');return 1
 if entry==VIS:
  count=c.get('count',4);n=count if count<0x80000000 else 0;threshold=value(load(m,0x8c88f614));zs=[value(load(m,POINTS+i*12+8)) for i in range(n)]
  assert s.r[0]==int(not any(z>threshold for z in zs)),(c,'independent signed-count visibility')
  return 1
 if entry==HEADER:
  want,layout,kind,length=header_oracle(m,param,initial_s)
  assert actual==want,(c,'independent complete cache/header/layout/length/colour packing',layout,kind,length)
  assert s.r[0]==0 and s.r[15]==initial_s.r[15],(c,'independent header return/stack');return 1
 if entry==STATE and c.get('flags',0)&~0xc0==0 and c.get('texture_mode',0)==0:
  word=load(m,param+0x98)
  for active,mode,table in ((c.get('flags',0)&0x40,c.get('source',0),SOURCE_MODES),(c.get('flags',0)&0x80,c.get('dest',0),DEST_MODES)):
   if active and mode in table:
    src,dst=table[mode]
    if src is not None:word=word&~(7<<29)|src<<29
    if dst is not None:word=word&~(7<<26)|dst<<26
  assert load(actual,param+0x98)==word and load(actual,param+0x9c)==load(m,param+0x9c) and load(actual,param+0x94)==load(m,param+0x94)&0xffd00000,(c,'independent ordered blend tables/cache words')
  assert s.r[0]==0 and s.r[15]==initial_s.r[15],(c,'independent state return/stack');return 1
 return 0

def compare(m,s,f,c):
 initial_s=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem)))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial_s.pc,None,observe,ctypes.byref(bus))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original renderer-header state',rc,status,hex(s.pc),hex(other.pc))
 assert not native.trace_count and not ref.events,(c,'header/visibility helpers must not fabricate packets')
 checks=semantic_checks(m,actual,initial_s,s,c) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 else 0
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,queue_match=True,independent_checks=checks,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def cases():
 for tex,v,bank in itertools.product((2,3),(0,1),range(2)):yield dict(texture_mode=tex,null_texture=True,flags=0xffffffff,v=v,tail_control=0x60000000,mode=bank*4)
 for source,dest,bank in itertools.product((*range(12),12,0xffffffff),(*range(12),12,0xffffffff),range(2)):yield dict(flags=0xc0,source=source,dest=dest,mode=bank*4,semantic=True)
 for flags,v,tex in itertools.product((0,0xffffffff,*(1<<i for i in range(32))),range(4),(0,2)):yield dict(flags=flags,v=v,texture_mode=tex)
 for fmt,tf,wh,filter in itertools.product((0,0x28000000,0x30000000),(0,1,4,8,16,31),(8,16,32,64,128,256,512,1024),range(4)):yield dict(flags=0xffffffff,texture_mode=2,format=fmt,texflags=tf,width=wh,height=wh,filter=filter,v=filter&1)
 for index,variant,bank in itertools.product(range(64),(0,2,4,6),range(2)):yield dict(entry=HEADER,index=index,variant=variant,mode=bank*4,colours=index%3,semantic=True)
 for lookup_type,bank in itertools.product((7,0xff),range(2)):yield dict(entry=HEADER,index=0,lookup_type=lookup_type,mode=bank*4,semantic=True)
 for count,zcase,bank in itertools.product((0,1,2,4,0x80000000,0xffffffff),range(4),range(2)):yield dict(entry=VIS,count=count,zcase=zcase,mode=bank*4,semantic=True)
 for size,mode in itertools.product((0,1,7,8,9,16,32,64,128,256,512,1024,0x8000,0xffffffff),range(8)):yield dict(entry=SIZE,size=size,mode=mode,semantic=True)
 for entry,mode in itertools.product((STATE,HEADER,VIS),range(8)):yield dict(entry=entry,mode=mode,index=32,flags=0xffffffff,texture_mode=2)
 for entry,bad in itertools.product((STATE,HEADER,VIS,SIZE),('stack','source','descriptor','points')):yield dict(entry=entry,bad=bad,flags=0xffffffff,texture_mode=2)
 yield dict(flags=0x40,bad='switch')
 for alias,entry in itertools.product((0xc000000,0x8c000000,0xac000000),(STATE,HEADER,VIS)):yield dict(entry=entry,alias=alias,index=36,flags=0xc0,semantic=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_render_headers_connection.json',PROJECT/'analysis/draw_object_effects_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,base in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[base:base+len(data)]=data
 selected=list(cases());records=[];coverage=set();edges=set();computed=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=[dict(flags=0xc0,source=0,dest=1,semantic=True),dict(entry=HEADER,index=0,semantic=True),dict(entry=HEADER,index=32,semantic=True),dict(entry=VIS,count=4,zcase=3,semantic=True),dict(entry=SIZE,size=128,semantic=True)]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f) or a in {int(x['pc'],16) for x in manifest['computed_switches']}:
    j=i+(2 if w>>8 in (0x8d,0x8f) or w==0x23 else 1)
    if j<len(path):(computed if w==0x23 else edges).add((a,path[j]))
  if (n+1)%512==0:print(n+1,'original render-header comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Render-header smoke passed:',len(records));return
 # Original TST r8,r8 at 534 and 546, no intervening write to r8.
 # Reaching 538 implies r8 != 0, so the second BT cannot be taken.
 unreachable={(0x8c65554a,0x8c65555a)}
 assert [OWN[a] for a in (0x8c655534,0x8c655536,0x8c655546,0x8c65554a)]==['tst r8,r8','bt 0x8c65555a','tst r8,r8','bt 0x8c65555a']
 assert not any('r8' in OWN[a].split(',')[-1] for a in range(0x8c655538,0x8c655546,2))
 coverage_rows=[]
 for r in manifest['routines']:
  a=int(r['address'],16);wanted={tuple(int(x,16) for x in e) for e in r['conditional_edges']};body=set(PER_BODY[a]);row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),unreachable_edges=[[hex(x),hex(y)] for x,y in sorted(wanted&unreachable)],missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges-unreachable)]);coverage_rows.append(row)
 print(json.dumps(coverage_rows,indent=2))
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in coverage_rows)
 wanted_computed={(int(r['pc'],16),int(a,16)) for r in manifest['computed_switches'] for a in r['entries']};assert computed==wanted_computed
 files=[Path(__file__),PROJECT/'verify_draw_object_effects.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),statically_unreachable_conditional_edges=[[hex(x),hex(y)] for x,y in sorted(unreachable)],unreachable_edge_reason='Reaching 8c655538 requires r8 nonzero at TST/BT 8c655534/536. Straight-line instructions through the second TST/BT 8c655546/54a never write r8; the second BT taken outcome is impossible.',computed_edges_covered=len(computed),boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},newly_registered_entries=4,reused_routine_credit=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Four complete original render-state/header/visibility/size bodies, expanded switch CFG and original-opcode full reached machine/RAM/boundary comparisons. Independent ordered blend-mode, complete header/cache/layout/length/colour-packing, visibility and size checks.',limitations=['Original-table header layouts are tested exhaustively over the six-bit index and variant bits. Explicit lookup-data override cases exercise retained unknown-layout fallback; this is data-fixture coverage, not proof of natural table mutation.','Scalar numerical/reference/MMU-off valid-RAM and finite computed-jump guard domains remain explicit. No hardware fault dispatch, physical GPU pixels, full frame/gameplay/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_render_headers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','computed_edges_covered','boundary_comparisons','independent_cases')},indent=2))
if __name__=='__main__':main()
