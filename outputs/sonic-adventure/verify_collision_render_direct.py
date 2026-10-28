"""Full original/native triangle wrapper calls plus independent geometry/packet checks."""
import collections,hashlib,itertools,json,sys
from fractions import Fraction
from pathlib import Path
import collision_render_fixtures as fixture
from collision_render_fixtures import ENTRY,OBJECT,POINTS,COLOURS,UV,STATE_A,STATE_B,CURSOR,HEADER,bits,get
fast=fixture.fast;v=fixture.v;P=v.P
VIS,CLIP=0x8c64f5bc,0x8c64f840
EMITTERS={0x8c64fc68,0x8c64fae8,0x8c650144,0x8c64fe0c}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
 for flags,bank,matrix,zs in itertools.product((0,1,2,3,0x20,0x21,0x22,0x40,0x60,0x80000000,0x80000020,0x80000040,0x80000060),range(2),range(2),((2,2,2),(0,2,0),(0,0,0),(1,1,1),(101,2,2))):
  yield dict(kind='finite',flags=flags,mode=bank*4,matrix=matrix,zs=zs)
 for flags,count in itertools.product((0,1,2,3,0x20,0x80000020),(-1,0,2,3)):yield dict(kind='count',flags=flags,count=count,zs=(2,4,2,4))
 for mode,rm,flags in itertools.product(range(8),range(2),(0x20,0x80000060)):yield dict(kind='mode',mode=mode,rm=rm,flags=flags)
 for alias,offset,flags,swap,env in itertools.product((0x0c000000,0x8c000000,0xac000000),(0,32),(0x20,0x80000060),range(2),(0,0x1400)):
  yield dict(kind='view',alias=alias,offset=offset,flags=flags,swap=swap,env=env)
 for bad,bank in itertools.product(('object','points','stack'),range(2)):yield dict(kind='guard',bad=bad,mode=bank*4)
 for word,bank in itertools.product((0x7f800000,0xff800000,0x7fbfffff,0x7fc00001,1,0x7f7fffff),range(2)):yield dict(kind='edge',special=word,mode=bank*4)
def transformed(c):
 mat=fixture.matrix(c.get('matrix',0));return [tuple(sum(mat[k*4+j]*x for k,x in enumerate(p+(Fraction(1),))) for j in range(3)) for p in fixture.geometry(c)]
def clip_triangle(p):
 out=[]
 for i,b in enumerate(p):
  a=p[i-1]
  if (a[2]-1)*(b[2]-1)<0:
   t=(1-a[2])/(b[2]-a[2]);out.append(tuple(a[k]+t*(b[k]-a[k]) for k in (0,1))+(Fraction(1),))
  if b[2]>=1:out.append(b)
 return out
def main():
 configs=[dict(c,case_id=i) for i,c in enumerate(configurations())];shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
 if shard is not None:configs=[c for c in configs if c['case_id']%3==shard]
 smoke='--smoke' in sys.argv
 if smoke:configs=[dict(flags=x,kind='finite',case_id=i) for i,x in enumerate((0x20,0x80000020,0,0x80000000,0x60,0x80000060))]
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
  require(r['status'] in (0,1,-1,-3),'complete/existing numeric trap/unsupported/mapped guard;no missing game dependency')
  require(r['full_state_match'] and r['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/devices and ordered boundaries')
  targets=[int(b['target'],16) for b in r['boundaries']]
  if r['status']==0:
   require(s.r[15]==initial.r[15] and s.pr==initial.pr,'completed stack/PR restored')
   require(list(s.r[8:15])==list(initial.r[8:15]),'all callee-saved GPRs restored')
   require(targets[0]==ENTRY and targets[1]==0x8c605dac and targets[-2:]==[0x8c6069a4,0x8c605e0a],'actual saved-state/shader/header/finish/restore dependencies execute')
  finite=c['kind'] in ('finite','count','view')
  if finite:
   require(r['status']==0,'finite authored geometry completes through actual dependencies');flags=c.get('flags',0x20);count=max(0,c.get('count',1));stride=1 if flags&2 else 2 if flags&1 else 3
   points=transformed(c);triangles=[(0 if flags&1 else i*stride,1+i*stride,2+i*stride) for i in range(count)]
   visible=[tuple(points[j] for j in t) for t in triangles if all(points[j][2]<=100 for j in t)]
   clipped=[clip_triangle(p) for p in visible];emitted=[p for p in clipped if len(p)>=3]
   expected=0x8c64fc68 if flags&0x20 and flags&0x80000000 else 0x8c64fae8 if flags&0x20 else 0x8c650144 if flags&0x80000000 else 0x8c64fe0c
   require(targets.count(0x8c638ecc)==count*3 and targets.count(VIS)==count,'independent fan/strip/list transform and visibility counts')
   require(targets.count(CLIP)==len(visible),'independent far-depth rejection')
   require([x for x in targets if x in EMITTERS]==[expected]*len(emitted),'independent near clipping and selected actual emitter')
   calls=[b for b in r['boundaries'] if int(b['target'],16)==0x8c638ecc]
   wanted=[(POINTS+12*j)&0xffffff for t in triangles for j in t]
   require([b['arguments'][1]&0xffffff for b in calls]==wanted,'independent fan/strip/list source indices, no supplied game outputs')
   packets=sum(len(p) if flags&0x20 else 4*sum(not(p[i][2]==1 and p[i-1][2]==1) for i in range(len(p))) for p in emitted)
   # Header is one packet; completed command stores the cursor in the
   # original header-selected destination. Queue mapping is MMU-off.
   cursor=CURSOR+c.get('offset',0);final=cursor+32*(1+packets)
   selected=STATE_B+(16 if c.get('swap') else 8) if flags&0x40 else STATE_B
   require(get(a,selected)&0x3ffffff==final&0x3ffffff,'independent header plus clipped vertex/edge packet count and final cursor')
   if flags&0x20 and c.get('zs',(2,2,2))==(2,2,2):
    # No clipping intersections: independently check every projected packet,
    # its alternating strip order, per-vertex colours and signed normalized UV.
    ordinal=0
    for tri in triangles:
     for k,j in enumerate((tri[0],tri[2],tri[1])):
      x,y,z=points[j];address=cursor+32*(1+ordinal);ordinal+=1
      expected_words=(0xf0000000 if k==2 else 0xe0000000,bits(x*2/z+Fraction(1,4)),bits(y*-2/z+Fraction(1,2)),bits(1/z))
      require(tuple(get(a,address+4*l) for l in range(4))==expected_words,'independent projected position/depth/strip termination')
      require(get(a,address+24)==get(before,COLOURS+4*j),'independent vertex colour')
      if flags&0x80000000:
       signed=lambda n:n-65536 if n&32768 else n
       require([get(a,address+16+4*l) for l in range(2)]==[bits(Fraction(signed(get(before,UV+4*j+2*l,2)),256)) for l in range(2)],'independent signed UV values')
   oracles+=1
  if c.get('bad'):require(r['status']==-3,'reached mapped-memory guard')
  seen.update(path)
  for i,pc in enumerate(path):
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((pc,path[j]))
  counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True,independent_checks=cc))
  if len(records)%24==0:print('RENDER ROUTE',shard,len(records),dict(counts),flush=True)
 coverage=[]
 for body in v.ANALYSIS_ROUTINES:
  wanted=set(int(x,16) for x in body['reachable_addresses']);outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(x),hex(y)] for x,y in sorted(outcomes-edges)]))
 files=[Path(__file__),P/'collision_render_fixtures.py',P/'collision_render_reference.py',P/'write_watch_collision_render.py',v.MP,P/'analysis/collision_render_connection.json',v.effects.DLL,P/'source/sa_collision_render.c']
 report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_geometry_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual original triangle wrapper and all reached game dependencies execute. FullCPU/FPU/RAM/device/queue/ordered boundaries via unchanged observer. Independent triangle indexing/transform counts,farvisibility,nearclipped count,emitter choice,packet count/cursor,andfiniteunclippedposition/colour/UVoracles. Explicit geometry/render inputstate only. SRFDclear/MMUoff validRAM softwarequeue ABI;physicalhardware/exceptiondelivery/pixels/gameplay unqualified. No routinecredit from comparisons alone.')
 suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('collision_render_direct'+suffix+'_verification.json')
 if smoke:dest=P.parents[1]/'work/sonic-adventure/collision_render_smoke.json'
 if case_id is not None:dest=P.parents[1]/'work/sonic-adventure'/('collision_render_case_'+str(case_id)+'.json')
 dest.write_text(json.dumps(report,indent=2)+'\n');print('RENDER DIRECT PASS',len(records),checks,dict(counts),coverage,flush=True)
if __name__=='__main__':main()
