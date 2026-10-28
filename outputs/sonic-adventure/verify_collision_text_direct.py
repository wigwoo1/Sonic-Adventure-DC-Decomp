"""Original/native glyph/string execution and independent exact quad packet oracle."""
import collections,hashlib,itertools,json,sys
from fractions import Fraction
from pathlib import Path
import collision_text_fixtures as fixture
from collision_text_fixtures import GLYPH,TEXT,STRLEN,CURSOR,STRING,STATE_B,FONT_COLOUR,FONT_SIZE,CURSOR_SLOT,bits,get,signed
fast=fixture.fast;v=fixture.v;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
 for character,bank,size,device in itertools.product((0,1,15,16,31,65,127,128,239,255,0x12340041,0xffffffff),range(2),(8,12.5),(False,True)):
  yield dict(kind='glyph',entry=GLYPH,character=character,mode=bank*4,size=size,device=device)
 for data,bank,size,coords in itertools.product(((),(65,),(65,66,67),(1,15,16,127,128,255),(65,0,66)),range(2),(8,12.5),((2,3),(-2,-3))):
  yield dict(kind='text',entry=TEXT,string=data,mode=bank*4,size=size,x=coords[0],y=coords[1])
 for entry,mode,rm in itertools.product((GLYPH,TEXT),range(8),range(2)):yield dict(kind='mode',entry=entry,mode=mode,rm=rm)
 for entry,alias,offset,colour in itertools.product((GLYPH,TEXT),(0x0c000000,0x8c000000,0xac000000),(0,32),(0,0xffffffff)):
  yield dict(kind='view',entry=entry,alias=alias,offset=offset,colour=colour)
 for entry,coord in itertools.product((GLYPH,TEXT),(-32768,-1,0,32767,0x12348000,0xffffffff)):
  yield dict(kind='coordinates',entry=entry,x=coord,y=coord,size=8)
 for entry,word,bank in itertools.product((GLYPH,TEXT),(0x7f800000,0xff800000,0x7fbfffff,0x7fc00001,1,0x7f7fffff),range(2)):
  yield dict(kind='edge',entry=entry,size_word=word,mode=bank*4)
 for entry,bad in ((GLYPH,'cursor'),(GLYPH,'edge_cursor'),(TEXT,'stack'),(TEXT,'string'),(TEXT,'edge_string'),(TEXT,'state')):
  for bank in range(2):yield dict(kind='guard',entry=entry,bad=bad,mode=bank*4)
 for length in (32768,65536,65537):yield dict(kind='length',entry=TEXT,length=length)
def packets(c):
 size=Fraction(c.get('size',8));x=signed(c.get('x',10 if c['entry']==GLYPH else 2));y=signed(c.get('y',20 if c['entry']==GLYPH else 3))
 chars=[c.get('character',65)&255]
 if c['entry']==TEXT:
  chars=list(fixture.input_bytes(c).split(b'\0',1)[0]);count=signed(len(chars));chars=chars[:count] if count>0 else [];x=int(x*size);y=int(y*size)
 out=[]
 for ch in chars:
  xx,yy=Fraction(signed(x)),Fraction(signed(y));u=Fraction(ch&15,16)+Fraction(1,1024);w=Fraction(ch>>4,16)+Fraction(1,1024);du=Fraction(63,1024)
  for i,(px,py,uv,vv) in enumerate(((xx,yy,u,w),(xx,yy+size,u,w+du),(xx+size,yy,u+du,w),(xx+size,yy+size,u+du,w+du))):
   out.append((0xf0000000 if i==3 else 0xe0000000,bits(px),bits(py),bits(1),bits(uv),bits(vv),c.get('colour',0x80a0c0e0)))
  x=int(Fraction(signed(x))+size)
 return chars,out
def main():
 configs=[dict(c,case_id=i) for i,c in enumerate(configurations())];shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
 if shard is not None:configs=[c for c in configs if c['case_id']%3==shard]
 smoke='--smoke' in sys.argv
 if smoke:configs=[dict(kind='glyph' if e==GLYPH else 'text',entry=e,case_id=i,string=data,device=device) for i,(e,data,device) in enumerate(((GLYPH,(65,),False),(GLYPH,(65,),True),(TEXT,(),True),(TEXT,(65,66),True)))]
 case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
 if case_id is not None:configs=[c for c in configs if c['case_id']==case_id];assert len(configs)==1
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0;counts=collections.Counter();oracles=0
 for c in configs:
  cc=0
  def require(ok,label):
   nonlocal checks,cc
   assert ok,(c,label);checks+=1;cc+=1
  m,s,f,cfg=fixture.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s)
  r,path,a,s,f=fast.compare(m,s,f,cfg)
  require(r['status'] in (0,1,-1,-3),'complete/existing numeric stop/mapped guard;no missing game dependency')
  require(r['full_state_match'] and r['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/devices/queue/ordered boundaries')
  targets=[int(b['target'],16) for b in r['boundaries']]
  if r['status']==0:
   require(s.r[15]==initial.r[15] and s.pr==initial.pr,'completed stack/PR restored')
   require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved GPRs preserved')
  if c['kind'] in ('glyph','text','view','coordinates','length'):
   require(r['status']==0,'finite authored font geometry completes');chars,words=packets(c)
   require(targets==([GLYPH] if c['entry']==GLYPH else [TEXT,STRLEN]+[GLYPH]*len(chars)),'actual original string-length and glyph calls;no provided game-callee results')
   cursor=CURSOR+c.get('offset',0);head=32 if c['entry']==TEXT else 0
   for j,wanted in enumerate(words):require(tuple(get(a,cursor+head+j*32+4*k) for k in range(7))==wanted,'independent complete control/position/depth/UV/colour vertex words')
   delta=head+len(words)*32
   require(get(a,CURSOR_SLOT)==((0xe0000000|((cursor+delta)&0x3ffffff)) if c['entry']==TEXT or c.get('device',True) else (c.get('alias',0x8c000000)|((cursor+delta)&0xffffff))),'independent final glyph cursor')
   if c['entry']==TEXT:
    require(tuple(get(a,cursor+4*k) for k in range(4))==(get(before,0x8c890594),get(before,0x8c890598),get(before,0x8c890590),get(before,0x8c89058c)),'independent original header word order')
    require(get(a,STATE_B+8)==(c.get('alias',0x8c000000)|((cursor+delta)&0xffffff)),'independent committed original renderer cursor')
    require(get(a,0x8c67348c)==0xfffffffe,'original text drawing state marker')
    require(a[STRING&0xffffff:(STRING&0xffffff)+len(fixture.input_bytes(c))+1]==before[STRING&0xffffff:(STRING&0xffffff)+len(fixture.input_bytes(c))+1],'input text bytes unchanged')
   oracles+=1
  if c.get('bad'):require(r['status']==-3,'reached mapped-memory guard')
  seen.update(path)
  for i,pc in enumerate(path):
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((pc,path[j]))
  counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True,independent_checks=cc))
  if len(records)%24==0:print('TEXT ROUTE',shard,len(records),dict(counts),flush=True)
 coverage=[]
 for body in v.ANALYSIS_ROUTINES:
  wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(x),hex(y)] for x,y in sorted(outcomes-edges)]))
 files=[Path(__file__),P/'collision_text_fixtures.py',P/'collision_text_reference.py',P/'write_watch_collision_text.py',v.MP,P/'analysis/collision_text_connection.json',v.effects.DLL,P/'source/sa_collision_text.c']
 report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_quad_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual original glyph/string helpers and string-length callee execute;fullCPU/FPU/RAM/device/queue/orderedboundaries throughunchangedobserver. Independent exactquadcontrol/positions/depth/atlasUV/colour/header/cursor/stringoracles forfiniteordinarycases. Explicitfont/renderer/textinputstate only;no suppliedgameoutputs/frames. Existingnumeric/MMUoff validRAM/fixedcode/softwarequeue domains;physicalGPU/exceptiondelivery/gameplayunqualified.')
 suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('collision_text_direct'+suffix+'_verification.json')
 if smoke:dest=P.parents[1]/'work/sonic-adventure/collision_text_smoke.json'
 if case_id is not None:dest=P.parents[1]/'work/sonic-adventure'/('collision_text_case_'+str(case_id)+'.json')
 dest.write_text(json.dumps(report,indent=2)+'\n');print('TEXT DIRECT PASS',len(records),checks,dict(counts),coverage,flush=True)
if __name__=='__main__':main()
