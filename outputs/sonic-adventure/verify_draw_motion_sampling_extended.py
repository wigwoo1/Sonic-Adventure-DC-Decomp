"""Independent sampler outputs and interleaved format-record movement."""
import collections,itertools,json,types
from pathlib import Path
import verify_draw_motion_sampling as base
from verify_overlay_selector_initializer import load
PROJECT=base.PROJECT
COMPLETE_NEW=base.INDEX+base.CURVES+base.DISPATCH[:2]
FORMATS=tuple(base.PRIMARY_KIND)+tuple(base.SECONDARY_KIND)+base.DISPATCH[:2]

def semantic(m,actual,initial,s,f,c):
 entry=initial.pc;checks=base.semantic(m,actual,initial,s,f,c)
 if entry in FORMATS:
  state=base.MOTION if entry in base.PRIMARY_KIND else base.DUAL_STATE+c.get('channel',0)*28
  kind=base.PRIMARY_KIND.get(entry,base.SECONDARY_KIND.get(entry,2 if entry==base.DISPATCH[0] else 0))
  if kind:
   assert [load(actual,state+off) for off in (0,4,8,20)]==[base.DATA+kind*8,base.DATA,base.DATA+kind*4,c.get('channels',0).bit_count()],(c,'independent format stream/cursors/consumption')
  else:assert load(actual,state+20)==c.get('slot',0)+c.get('channels',0).bit_count(),(c,'independent paired callback consumption')
  if entry not in base.PRIMARY_KIND:
   other=base.DUAL_STATE+(c.get('channel',0)^1)*28
   assert bytes(actual[(other&0xffffff):(other&0xffffff)+28])==bytes(m[(other&0xffffff):(other&0xffffff)+28]),(c,'independent other-channel record preserved')
  checks+=2
 if entry in base.SAMPLE[4:]:
  comp=base.component_for(entry,c);pool=load(m,{0:0x8c041010,1:0x8c041020,2:0x8c041018}[comp]);dest=pool+c.get('channel',0)*12
  got=[load(actual,dest+j*4) for j in range(3)]
  if c.get('absent'):
   off={0:8,1:0x14,2:0x20}[comp]
   assert got==[load(m,base.NODE+off+j*4) for j in range(3)],(c,'independent default channel pose')
  else:
   frame=int(c.get('frame',5))&0xffffffff;index=max([i for i,label in enumerate(base.labels(c)) if label<=frame],default=0)
   want=base.curve_oracle(c,comp,index)
   if comp==1:
    got=[v-0x100000000 if v&0x80000000 else v for v in got]
    assert all(abs(v-int(w))<=1 for v,w in zip(got,want)),(c,'independent sampled angle buffer',got,want)
   else:assert all(abs(base.value(v)-float(w))<=2e-5 for v,w in zip(got,want)),(c,'independent sampled vector buffer',got,want)
  checks+=3
 return checks

namespace=dict(base.__dict__,semantic=semantic)
compare=types.FunctionType(base.compare.__code__,namespace,base.compare.__name__)
assert compare.__globals__['semantic'] is semantic

def cases():
 for entry,bank,typ,absent in itertools.product(FORMATS,range(2),range(8),(False,True)):
  kind=base.PRIMARY_KIND.get(entry,base.SECONDARY_KIND.get(entry,2 if entry==base.DISPATCH[0] else 3))
  if typ.bit_count()<=kind:yield dict(entry=entry,mode=bank*4,channel=bank,channels=typ,absent=absent,keys=True,semantic=True)
 for entry,bank,absent,slot,frame in itertools.product(base.SAMPLE,range(2),(False,True),(0,2),(1,5,12)):
  yield dict(entry=entry,mode=bank*4,channel=bank,absent=absent,slot=slot,frame=frame,keys=True,semantic=True)
 for entry,alias,bank in itertools.product(COMPLETE_NEW,(0x0c000000,0x8c000000,0xac000000),range(2)):
  yield dict(entry=entry,alias=alias,mode=bank*4,channel=bank,semantic=True)
 for entry,bad in itertools.product(COMPLETE_NEW,('stack','source','key','output')):yield dict(entry=entry,bad=bad)

def main():
 proof=PROJECT/'analysis/draw_motion_sampling_verification.json';body=json.loads(proof.read_text())
 for p,h in body['sha256'].items():assert base.digest(p)==h,p
 ram=bytearray(16<<20)
 for path,offset in ((base.SEED.state.gate.support.morph.MAIN,0x10000),(base.SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[offset:offset+len(data)]=data
 ram=base.caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0];records=[];boundaries=0
 for n,c in enumerate(cases()):
  record,_,b=compare(*base.prepare(ram,c),c);records.append(record);boundaries+=b
  if (n+1)%128==0:print(n+1,'independent sampling/format comparisons passed',flush=True)
 files=[Path(__file__),PROJECT/'verify_draw_motion_sampling.py',proof]
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),boundary_comparisons=boundaries,unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),external_game_service_fixtures=0,dll_sha256=base.digest(base.DLL),sha256={str(p):base.digest(p) for p in files},scope='Independent interleaved record cursor/consumption, unchanged other channel, default and approximate interpolated sampled buffers, valid format/type combinations, both banks/channels, RAM aliases and invalid pointers. Native/original full reached/final state and ordered entries/queue state compared. Prior qualified numerical and valid-RAM limits retained.',cases=records)
 assert not report['unconnected_targets']
 (PROJECT/'analysis/draw_motion_sampling_extended_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','independent_cases','independent_checks','boundary_comparisons')},indent=2))
if __name__=='__main__':main()
