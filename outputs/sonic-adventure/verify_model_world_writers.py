"""Qualify original USA writers with declared API cases and an unmodified real scene."""
import hashlib,json,math,pickle,struct
from pathlib import Path
from model_world_writers_verified_state import normal,sha,P,W
from model_scene_verified_state import Scene

COUNT=0x8c754ef0;OUT=COUNT+4;NEAR_COUNT=0x8c19e964;LAND_COUNT=NEAR_COUNT+4
NEAR=0x8c757ef4;LAND=0x8c7584f4;ADDED_COUNT=0x8c759704;ADDED=ADDED_COUNT+4
LAND_PTR=0x8c7596f4;ENABLED=0x8c19e978

def put(scene,address,value,width=4):
 start=address&0xffffff;assert start<=len(scene.m)-width
 scene.m[start:start+width]=(value&((1<<(8*width))-1)).to_bytes(width,'little')
def floats(scene,address,values):
 for i,value in enumerate(values):put(scene,address+4*i,int.from_bytes(struct.pack('<f',value),'little'))
def read(scene,address,width=4):return int.from_bytes(scene.m[address&0xffffff:(address&0xffffff)+width],'little')
def clone(base,q):
 scene=Scene.__new__(Scene);scene.m=bytearray(base.m);scene.s=q.v.effects.State.from_buffer_copy(bytes(base.s));scene.f=q.v.effects.Fpu.from_buffer_copy(bytes(base.f))
 scene.c=pickle.loads(pickle.dumps(base.c,protocol=5));scene.records=[];scene.usable=True;scene.prefix=base.prefix
 q.v.bind_vectors(scene.c['vbr']);return scene
def reference_position(scene):
 players=read(scene,0x8c052608);camera=read(scene,read(scene,0x8c05260c))
 work=read(scene,players) or read(scene,players+4) or camera
 result=struct.unpack('<3f',scene.m[(work+0x20)&0xffffff:((work+0x20)&0xffffff)+12])
 assert all(math.isfinite(v) for v in result);return result
def setup(scene,added_count=0,land_flags=(),alias_land=False,near=False,turned=False,far=False,eligible=True,dual=False,threshold=None):
 # Explicit standalone API input data. These buffers never enter the real-scene checkpoint.
 put(scene,ADDED_COUNT,added_count,2);put(scene,LAND_PTR,0);put(scene,ENABLED,1)
 expected_added=[];position=reference_position(scene)
 if dual:
  players=read(scene,0x8c052608)
  for i,pos in enumerate(((0.,0.,0.),(2000.,0.,0.))):
   work=0x8ce80000+0x100*i;put(scene,players+4*i,work);floats(scene,work+0x20,pos)
  position=(2000.,0.,0.)
 for i in range(max(0,added_count)):
  obj=0x8ce00000+i*0x100;model=obj+0x40;owner=obj+0x80;work=obj+0xc0
  flags=1|(0x10000000 if turned else 0)
  for j,value in enumerate((flags,obj,owner)):put(scene,ADDED+12*i+4*j,value)
  put(scene,obj+4,model);put(scene,owner+0x20,work);put(scene,work+4,0x100 if eligible else 0,2)
  pos=list(position);pos[0]+=10000. if far else (threshold or 0.)
  floats(scene,obj+8,pos);floats(scene,obj+0x20,(1.,1.,1.))
  floats(scene,model+0x18,(2.,3.,4.) if turned else (0.,0.,0.));floats(scene,model+0x24,(10. if dual else 50.,))
  if turned:
   for j,angle in enumerate((0x1000,0x2000,0x3000)):put(scene,obj+0x14+4*j,angle)
  expected_added.append((flags,obj,owner))
 if land_flags:
  table=0x8ce90000;entries=LAND+4 if alias_land else 0x8cea0000
  put(scene,table,len(land_flags),2);put(scene,table+12,entries);put(scene,LAND_PTR,table)
  for i,flag in enumerate(land_flags):
   entry=entries+36*i;floats(scene,entry,position);floats(scene,entry+12,(50.,))
   put(scene,entry+24,0x8cec0000+i*0x40);put(scene,entry+28,0x81+i);put(scene,entry+32,flag)
 return expected_added

def main():
 target=P/'analysis/model_world_writers_verification.json';assert not target.exists()
 translation=P/'analysis/model_world_writers_translation.json';manifest=json.loads(translation.read_text())
 assert manifest['instruction_rows']==536 and len(manifest['routines'])==3
 for name,digest in manifest['sha256'].items():assert sha(name)==digest,name
 base,q=normal();dll=q.v.effects.DLL;dll_hash=sha(dll);cases=[];executed=set()
 def call(scene,entry):
  result,path=scene.call(entry,compare=q.fast.compare);executed.update(path)
  assert result['full_state_match'] and result['ordered_boundaries_match']
  return result,path
 # Reset's integer stores retain their original widths. SZ=1 deliberately
 # exercises the odd FMOV pair and its overlap with added_count's padding.
 for bank in (0,1):
  for size in (0,1):
   scene=clone(base,q)
   for low,high in ((0x8c19e960,0x8c19e980),(0x8c754ee0,0x8c754f00),(0x8c7596e0,0x8c759718),(0x8c75a300,0x8c75a310)):
    scene.m[low&0xffffff:high&0xffffff]=b'\xa5'*(high-low)
   scene.f.fpscr=(scene.f.fpscr&~0x380000)|(bank<<21)|(size<<20)
   for b in range(2):
    for reg in range(16):scene.f.banks[b][reg]=0x3f000000+b*0x10000+reg*0x100
   expected_material=scene.f.banks[bank^1][2] if size else 0
   expected_padding=scene.f.banks[bank^1][3]&0xffff0000 if size else 0xa5a50000
   result,path=call(scene,0x8c051a16)
   assert result['status']==0 and scene.usable and len(path)==30
   assert read(scene,COUNT)==0xa5a50000 and read(scene,ADDED_COUNT)==expected_padding
   assert read(scene,0x8c75a308)==0xa5a50000
   assert read(scene,0x8c759700)==expected_material and read(scene,ENABLED)==read(scene,ENABLED-4)==1
   assert read(scene,NEAR_COUNT)==read(scene,LAND_COUNT)==read(scene,LAND_PTR)==0
   assert scene.f.banks[bank][3]==0
   cases.append(dict(kind='reset_FR_SZ_width_case',bank=bank,SZ=size,status=0,steps=len(path)))
  scene=clone(base,q);scene.f.fpscr|=0x80000
  result,path=call(scene,0x8c051a16)
  assert result['status']==-1 and result['stop_pc']=='0x8c051a30' and not scene.usable
  cases.append(dict(kind='reset_reached_PR_guard',bank_input=bank,status=-1,stop_pc=result['stop_pc'],steps=len(path),faulted_state_resumed=False))
 # Integer collection: reverse source order, both capacities, flag filters,
 # signed source counts, exact overlapping memory and a bounded pointer stop.
 all_specs=[('added_three',3,()),('added_capacity',129,()),('land_flags',0,(1,0,2)),
            ('land_capacity',0,(1,)*129),('combined',3,(1,0,2)),('land_overlap',0,(1,2)),
            ('negative_counts',0,()),('integer_PR_mode',3,()),('invalid_land_source',0,(1,))]
 for kind,n,flags in all_specs:
  scene=clone(base,q);expected=setup(scene,n,flags,alias_land=kind=='land_overlap')
  if kind=='negative_counts':
   put(scene,ADDED_COUNT,0xffff,2);table=0x8ce90000;put(scene,table,0xffff,2);put(scene,table+12,0x8cea0000);put(scene,LAND_PTR,table)
  if kind=='integer_PR_mode':scene.f.fpscr|=0x80000
  if kind=='invalid_land_source':put(scene,0x8ce90000+12,0x8cffffe0)
  result,path=call(scene,0x8c052e30)
  if kind=='invalid_land_source':
   assert result['status']==-3 and not scene.usable
  else:
   assert result['status']==0 and scene.usable
   added=min(n,128);land=min(sum(bool(flag&0x400003) for flag in flags),128)
   assert read(scene,NEAR_COUNT)==added and read(scene,LAND_COUNT)==land and read(scene,COUNT,2)==added+land
   for i,record in enumerate(reversed(expected[-added:]) if added else ()):
    assert tuple(read(scene,OUT+12*i+4*j) for j in range(3))==record
    assert tuple(read(scene,NEAR+12*i+4*j) for j in range(3))==record
  cases.append(dict(kind=kind,entry='0x8c052e30',status=result['status'],steps=len(path),stop_pc=result['stop_pc'],declared_API_inputs=True,faulted_state_resumed=False))
 near_specs=[('near_plain',1,(),{}),('near_turned',1,(),dict(turned=True)),
             ('near_far',1,(),dict(far=True)),('near_ineligible',1,(),dict(eligible=False)),
             ('near_boundary_equal',1,(),dict(threshold=450.)),('near_boundary_inside',1,(),dict(threshold=449.)),
             ('near_second_player',1,(),dict(dual=True)),('near_land',0,(1,0,2),{}),
             ('near_capacity',129,(),{})]
 for kind,n,flags,options in near_specs:
  scene=clone(base,q);setup(scene,n,flags,near=True,**options)
  result,path=call(scene,0x8c052518)
  assert result['status']==0 and scene.usable
  added=0 if kind in ('near_far','near_ineligible','near_boundary_equal') else min(n,128)
  land=sum(bool(flag&0x400003) for flag in flags)
  assert read(scene,NEAR_COUNT)==added and read(scene,LAND_COUNT)==land and read(scene,COUNT,2)==added+land,(kind,read(scene,COUNT,2),read(scene,NEAR_COUNT))
  reached={boundary['target'] for boundary in result['boundaries']}
  if options.get('turned'):assert {'0x8c63a8e0','0x8c63a1cc','0x8c639ec8','0x8c639f5c','0x8c638ecc'}<=reached
  if flags:assert '0x8c63a9c4' in reached
  cases.append(dict(kind=kind,entry='0x8c052518',status=0,steps=len(path),near_added=added,near_land=land,declared_API_inputs=True,ordered_targets=[boundary['target'] for boundary in result['boundaries']]))
 # Real prepared scene: no buffer/global/camera/geometry input patching.
 scene=clone(base,q);prior_job=scene.c['pixel_job_seed'];prior_retired=scene.c['ta_list_seed']
 actual=[]
 for entry,args in ((0x8c051a16,()),(0x8c052e30,()),(0x8c052518,()),(0x8c09878c,(0,)),(0x8c09878c,(5,)),(0x8c6050b0,()),(0x8c04fe40,())):
  result,path=scene.call(entry,args,compare=q.fast.compare);executed.update(path)
  assert result['status']==0 and scene.usable and result['full_state_match'] and result['ordered_boundaries_match']
  if len(actual)<3:assert scene.c['pixel_job_seed']==prior_job and scene.c['ta_list_seed']==prior_retired
  actual.append(dict(entry=hex(entry),steps=len(path),status=0,full_state_match=True,ordered_boundaries_match=True))
 assert scene.c['pixel_job_seed'].summary()['processed']==3 and not scene.c['pixel_job_seed'].summary()['ready']
 assert read(scene,COUNT,2)==read(scene,NEAR_COUNT)==read(scene,LAND_COUNT)==0
 assert not scene.c['graphics_in'].packets and scene.c['ta_list_seed']['last_count']==189
 assert scene.c['render_completion_seed']['completed']==3 and scene.c['render_completion_seed']['pending']==7
 folder=W/'model-world-writers-normal';folder.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5);(folder/'normal_public_return.pkl').write_bytes(payload)
 meta=dict(status='matched_original_world_writers_and_following_prepared_frame',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(dll),DLL_sha256=dll_hash,faulted_state_saved=False,completed_native_jobs=3,retired_packets=189,active_packets=0,render_pending=7,world_candidate_count=0,registered_near_count=0,phases=scene.records)
 (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
 from PIL import Image
 Image.frombytes('RGBA',(640,480),scene.c['pixel_job_seed'].pixels,'raw','BGRA').save(folder/'native_third_frame.png')
 files=[Path(__file__),P/'model_world_writers_verified_state.py',translation,dll,P/'build_model_world_writers.ps1']
 report=dict(status='verified_declared_world_writer_API_cases_and_original_prepared_frame',original_native_comparisons=len(cases)+len(actual),normal_returns=sum(case['status']==0 for case in cases)+len(actual),expected_host_guard_stops=sum(case['status']!=0 for case in cases),declared_API_cases=cases,actual_prepared_scene_calls=actual,actual_prepared_scene_input_injection=False,actual_new_native_jobs_completed=1,inherited_completed_jobs=2,completed_jobs=3,world_candidate_count_in_actual_scene=0,populated_level_or_gameplay_qualified=False,external_completion_tokens=0,faulted_state_resumed=False,static_original_rows=536,static_row_coverage=[dict(address=row['address'],rows=row['instruction_rows'],reached=len(executed&{int(a,16) for a in row['reachable_addresses']})) for row in manifest['routines']],new_original_routine_credit=0,upstream_game_C_imported=False,overall_completion_percent=10,goal_paused=False,sha256={str(path):sha(path) for path in files})
 assert sha(dll)==dll_hash
 target.write_text(json.dumps(report,indent=2)+'\n');print('WORLD WRITER CASES AND FOLLOWING FRAME VERIFIED',len(cases)+len(actual),'comparisons',flush=True)
if __name__=='__main__':main()
