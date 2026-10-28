"""Execute the complete original collision caller and real dependencies, retaining gaps."""
import collections,hashlib,itertools,json,sys
from fractions import Fraction
from pathlib import Path
import collision_contact_fixtures as fixture
from collision_contact_fixtures import ENTRY,QUERY,COUNT,CONTACTS,get
fast=fixture.fast;v=fixture.v;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
 for radius in (-1,0):yield dict(kind='early',radius=radius)
 for config in (dict(no_world=True),dict(face_count=0),dict(face_count=-1),dict(node_count=3,match_node=2),dict(node_count=3,match_node=99)):yield dict(kind='world',**config)
 for point,bank,flags in itertools.product(((.5,.25,.5),(.5,-.25,.5),(.5,2,.5),(4,.25,4),(1,.5,.5),(.5,0,.5),(-.5,.25,.5),(.5,.25,-.5)),range(2),(0,0x800000,0x80000000)):
  yield dict(kind='plane',point=point,mode=bank*4,face_flags=flags)
 for vertices,point in itertools.product((((0,0,0),(1,0,0),(2,0,0)),((0,0,0),(2,0,0),(0,0,2)),((0,0,0),(0,2,0),(0,0,2)),((0,0,0),(2,0,0),(0,2,0)),((0,0,0),(2,1,0),(0,1,2))),((.5,.25,.5),(-.5,.25,.5))):
  yield dict(kind='geometry',vertices=vertices,point=point)
 for motion,point,flags in itertools.product(((0,0,2),(0,0,-2),(0,-2,0),(1,1,1)),((.5,.25,2.5),(.5,2.5,.5),(.5,.25,.5),(.5,-.25,.5)),(0,0x800000,0x80000000)):
  yield dict(kind='motion',motion=motion,point=point,face_flags=flags)
 for count,offsets,debug in itertools.product((2,3,16,17),(((0,0,0),),((0,0,0),(0,.5,0),(0,-.5,0))),(0x80,0)):
  yield dict(kind='multiple',face_count=count,face_offsets=offsets,debug=debug)
 for entrycase,mode,rm in itertools.product((dict(radius=0),dict(no_world=True),dict(point=(.5,.25,.5))),range(8),range(2)):
  yield dict(kind='mode',**entrycase,mode=mode,rm=rm)
 for alias,bank,debug in itertools.product((0x0c000000,0x8c000000,0xac000000),range(2),(0x80,0)):
  yield dict(kind='view',alias=alias,mode=bank*4,debug=debug)
 for word in (0x7f800000,0xff800000,0x7fbfffff,0x7fc00001,1,0x7f7fffff):yield dict(kind='edge',radius_word=word)
 for bad in ('query','stack','face','node'):yield dict(kind='guard',bad=bad)
def main():
 configs=[dict(c,case_id=i) for i,c in enumerate(configurations())];shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
 if shard is not None:configs=[c for c in configs if c['case_id']%3==shard]
 case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
 if case_id is not None:configs=[c for c in configs if c['case_id']==case_id];assert len(configs)==1
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0;counts=collections.Counter();oracles=0
 for c in configs:
  cc=0
  def require(ok,label):
   nonlocal checks,cc
   assert ok,(c,label);checks+=1;cc+=1
  m,s,f,cfg=fixture.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
  r,path,a,s,f=fast.compare(m,s,f,cfg)
  require(r['status'] in (0,1,-1,-3),'complete/existing numeric stop/mapped guard;no missing game dependency')
  require(r['full_state_match'] and r['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/device/queue/ordered boundaries')
  if r['status']==0:
   require(s.r[15]==initial.r[15] and s.pr==initial.pr,'completed stack/PR restored')
   require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
   if not initial_f.fpscr&0x180000:
    bank=(initial_f.fpscr>>21)&1;require(list(f.banks[bank][12:])==list(initial_f.banks[bank][12:]),'callee-saved physical FPU registers')
  if c['kind']=='early':
   require(r['status']==0 and s.r[0]==0,'nonpositive radius returns no contact');require(a[QUERY&0xffffff:(QUERY&0xffffff)+0x100]==before[QUERY&0xffffff:(QUERY&0xffffff)+0x100] and get(a,COUNT)==get(before,COUNT),'nonpositive radius preserves query/contact state');oracles+=1
  if c['kind']=='world' and (c.get('no_world') or c.get('face_count',1)<=0 or c.get('match_node')==99):
   require(r['status']==0 and s.r[0]==0 and get(a,COUNT)==0,'empty/unmatched linked world has no contacts');oracles+=1
  if c['kind']=='plane':
   p=c['point'];inside=0<=p[0] and 0<=p[2] and p[0]+p[2]<=2;hit=inside and abs(p[1])<=1
   require(r['status']==0 and s.r[0]==int(hit) and get(a,COUNT)==int(hit),'independent stationary finite horizontal-triangle contact decision');oracles+=1
  if c['kind']=='multiple' and len(c['face_offsets'])==1:
   count=min(16,c['face_count']);require(r['status']==0 and s.r[0]==1 and get(a,COUNT)==count,'independent repeated-face count and16contact cap');oracles+=1
  if c.get('bad'):require(r['status']==-3,'reached mapped-memory guard')
  seen.update(path)
  for i,pc in enumerate(path):
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((pc,path[j]))
  counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],original_rows=len(path),result=s.r[0],contact_count=get(a,COUNT),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True,independent_checks=cc))
  if len(records)%12==0:print('CONTACT ROUTE',shard,len(records),dict(counts),flush=True)
 coverage=[]
 for body in v.ANALYSIS_ROUTINES:
  wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(x),hex(y)] for x,y in sorted(outcomes-edges)]))
 files=[Path(__file__),P/'collision_contact_fixtures.py',P/'collision_contact_reference.py',P/'write_watch_collision_contact.py',v.MP,P/'analysis/collision_contact_connection.json',v.effects.DLL,P/'source/sa_collision_contact.c']
 report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_contact_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Completeoriginalcollisioncaller plusactualmath/matrix/renderer/text callees. FullCPU/FPU/RAM/device/softwarequeue/orderedboundaries. Independentnonpositive/emptyworld/stationaryhorizontalcontact/capcases only;othergeometry/movementoutputusesoriginalinstructioncomparison. Authoredquery/world/mesh/matrix/renderinputs,notcalleeoutputs/frames. No routinecreditpendingcoverage/qualification;allgapsretained. Existingnumeric/MMUoff/validRAM/softwarequeueABI;physicaldelivery/gameplayunqualified.')
 suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('collision_contact_direct'+suffix+'_verification.json')
 if case_id is not None:dest=P.parents[1]/'work/sonic-adventure'/('collision_contact_case_'+str(case_id)+'.json')
 dest.write_text(json.dumps(report,indent=2)+'\n');print('CONTACT DIRECT PASS',len(records),checks,dict(counts),coverage,flush=True)
if __name__=='__main__':main()
