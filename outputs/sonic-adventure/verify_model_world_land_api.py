"""Original/native land API cases; synthetic buffers never enter the scene checkpoint."""
import csv,json,pickle,struct
from pathlib import Path
from model_world_land_verified_state import normal,sha,P,W
from model_scene_verified_state import Scene

ADDED_COUNT=0x8c759704;ADDED=ADDED_COUNT+4;LAND_PTR=0x8c7596f4
def put(scene,a,value,n=4):
 assert a&0x1f000000==0x0c000000 and (a&0xffffff)<=len(scene.m)-n
 scene.m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')
def read(scene,a,n=4):
 assert a&0x1f000000==0x0c000000 and (a&0xffffff)<=len(scene.m)-n
 return int.from_bytes(scene.m[a&0xffffff:(a&0xffffff)+n],'little')
def floats(scene,a,values):
 for i,v in enumerate(values):put(scene,a+4*i,int.from_bytes(struct.pack('<f',v),'little'))
def clone(base,q):
 s=Scene.__new__(Scene);s.m=bytearray(base.m);s.s=q.v.effects.State.from_buffer_copy(bytes(base.s));s.f=q.v.effects.Fpu.from_buffer_copy(bytes(base.f))
 s.c=pickle.loads(pickle.dumps(base.c,protocol=5));s.records=[];s.usable=True;s.prefix=base.prefix;q.v.bind_vectors(s.c['vbr']);return s
def object_input(scene,center=(0.,0.,0.),angle=0,count=0):
 obj=0x8ce00000;model=obj+0x40;owner=obj+0x80;work=obj+0xc0
 put(scene,ADDED_COUNT,count,2);put(scene,obj+4,model);put(scene,owner+0x20,work);put(scene,work+4,0x2481,2)
 floats(scene,model+0x18,center);floats(scene,model+0x24,(50.,));floats(scene,obj+0x20,(1.,1.,1.));put(scene,obj+0x14,angle)
 return obj,model,owner,work
def land_input(scene,flags=0,state=0,count=0,motions=0):
 table=0x8cee0000;entries=table+0x100;motion=table+0x400
 catalog=read(scene,0x8c051b48);put(scene,catalog,table);put(scene,LAND_PTR,table)
 put(scene,table,count,2);put(scene,table+2,motions,2);put(scene,table+4,flags,2);put(scene,table+6,state,2)
 put(scene,table+12,entries);put(scene,table+16,motion);put(scene,0x8c75a308,0,2)
 return table,entries,motion

def main():
 target=P/'analysis/model_world_land_api_verification.json';assert not target.exists()
 for stem in ('model_world_land_translation','model_world_land_dependencies_v2_translation'):
  manifest=json.loads((P/('analysis/'+stem+'.json')).read_text())
  for name,h in manifest['sha256'].items():assert sha(name)==h,name
 base,q=normal();cases=[];executed=set()
 def call(scene,name,entry,args=(),status=0,fixture=True):
  result,path=scene.call(entry,args,compare=q.fast.compare);executed.update(path)
  assert result['status']==status,(name,result['status'],result['stop_pc'])
  assert scene.usable==(status==0) and result['full_state_match'] and result['ordered_boundaries_match']
  assert scene.c['pixel_job_seed']==base.c['pixel_job_seed'] and scene.c['ta_list_seed']==base.c['ta_list_seed']
  row=dict(case=name,entry=hex(entry),arguments=list(args),status=status,stop_pc=result['stop_pc'],steps=len(path),declared_input_fixture=fixture,full_state_match=True,ordered_boundaries_match=True,faulted_state_resumed=False)
  cases.append(row);print(name,status,len(path),flush=True);return row
 # Register widths, exact threshold, low-angle mask and original capacity.
 for bank in (0,1):
  for size in (0,1):
   scene=clone(base,q);obj,model,owner,work=object_input(scene)
   scene.f.fpscr=(scene.f.fpscr&~0x380000)|(bank<<21)|(size<<20)
   for b in range(2):
    for i in range(16):scene.f.banks[b][i]=0
   call(scene,f'register_FR{bank}_SZ{size}',0x8c0528ae,(5,owner,obj))
   assert read(scene,ADDED_COUNT,2)==1 and [read(scene,ADDED+4*i) for i in range(3)]==[5,obj,owner]
   assert read(scene,work+4,2)==0x2581
 for label,center,angle,turned in [('below',(14.,0.,0.),0x1000,False),('equal',(15.,0.,0.),0x1000,True),('above',(16.,0.,0.),0x1000,True),('negative',(-15.,0.,0.),0x1000,True),('masked_low_angle',(16.,0.,0.),15,False),('no_angle',(16.,0.,0.),0,False)]:
  scene=clone(base,q);obj,model,owner,work=object_input(scene,center,angle)
  call(scene,'register_'+label,0x8c0528ae,(1,owner,obj))
  assert read(scene,ADDED)==1|(0x10000000 if turned else 0)
 scene=clone(base,q);obj,model,owner,work=object_input(scene,count=255)
 call(scene,'register_last_slot',0x8c0528ae,(0x80000001,owner,obj));assert read(scene,ADDED_COUNT,2)==256
 assert [read(scene,ADDED+255*12+4*i) for i in range(3)]==[0x80000001,obj,owner]
 scene=clone(base,q);put(scene,ADDED_COUNT,256,2);scene.f.fpscr|=0x80000
 call(scene,'register_full_skips_invalid_arguments_and_PR_math',0x8c0528ae,(0xffffffff,0xffffffff,0xffffffff));assert read(scene,ADDED_COUNT,2)==256
 for bank in (0,1):
  scene=clone(base,q);obj,model,owner,work=object_input(scene)
  scene.f.fpscr=(scene.f.fpscr&~0x380000)|(bank<<21)|0x80000
  row=call(scene,f'register_FR{bank}_reached_PR_guard',0x8c0528ae,(1,owner,obj),-1)
  assert row['stop_pc']=='0x8c0528ce' and read(scene,ADDED_COUNT,2)==0
 for name in ('object','model','owner','work'):
  scene=clone(base,q);obj,model,owner,work=object_input(scene)
  if name=='object':obj=0x8cfffffe
  if name=='model':put(scene,obj+4,0x8cfffff0)
  if name=='owner':owner=0x8cfffff0
  if name=='work':put(scene,owner+0x20,0x8cfffffe)
  call(scene,'register_invalid_'+name,0x8c0528ae,(1,owner,obj),-3)
  if name in ('owner','work'):assert read(scene,ADDED_COUNT,2)==1
 # Real registration output becomes an eligible collector/query candidate.
 scene=clone(base,q);obj,model,owner,work=object_input(scene)
 players=read(scene,0x8c052608);camera=read(scene,read(scene,0x8c05260c));source=read(scene,players) or read(scene,players+4) or camera
 position=struct.unpack('<3f',scene.m[((source+0x20)&0xffffff):((source+0x20)&0xffffff)+12])
 floats(scene,obj+8,position);put(scene,LAND_PTR,0);put(scene,0x8c19e978,1)
 call(scene,'registration_to_collection_register',0x8c0528ae,(1,owner,obj))
 call(scene,'registration_to_collection_near',0x8c052518);assert read(scene,0x8c754ef0,2)==read(scene,0x8c19e964)==1
 bank=(scene.f.fpscr>>21)&1
 for i,v in enumerate((*position,1.)):scene.f.banks[bank][4+i]=int.from_bytes(struct.pack('<f',v),'little')
 call(scene,'registration_to_collection_query',0x8c0522c0)
 # Original 256-byte flag pool clear, both banks and unreached PR arithmetic.
 for bank in (0,1):
  for pr in (0,1):
   scene=clone(base,q);pool=read(scene,0x8c052b60);start=pool&0xffffff
   scene.m[start-4:start+260]=b'\xa5'*264;scene.f.fpscr=(scene.f.fpscr&~0x280000)|(bank<<21)|(pr<<19)
   call(scene,f'pool_clear_FR{bank}_PR{pr}',0x8c052a00)
   assert scene.m[start:start+256]==bytes(256) and scene.m[start-4:start]==scene.m[start+256:start+260]==b'\xa5'*4
 scene=clone(base,q);scene.f.fpscr|=0x80000;call(scene,'original_zero_hook',0x8c094dc4);assert scene.s.r[0]==0
 # Land setup API fixtures exercise source/header decisions, not resource I/O.
 for label,flags,state,motions in [('plain',0,0,0),('motion_empty',1,0,0),('motion_negative',1,0,-1),('motion_null_records',1,0,2),('texture_null_name',8,0,0),('texture_already_loaded',8,1,0),('direct_already_loaded',2,1,0),('PRS_already_loaded',0x10,4,0),('PRS_already_failed',0x10,8,0)]:
  scene=clone(base,q);table,entries,motion=land_input(scene,flags,state,motions=motions)
  if label=='texture_already_loaded':put(scene,table+20,0xffffffff)
  call(scene,'load_act_'+label,0x8c051a52,(0,0))
  expected=state+(4 if flags&0x10 else 0)+(1 if flags&(2|8) else 0)
  assert read(scene,table+6,2)==expected
 scene=clone(base,q);catalog=read(scene,0x8c051b48);put(scene,catalog,0);call(scene,'load_act_null_catalog_entry',0x8c051a52,(0,0))
 # Scalar motion clocks: one advancing and one wrapping, both original branches.
 for prs in (0,0x10):
  scene=clone(base,q);table,entries,motion=land_input(scene,1|prs,motions=2)
  floats(scene,motion,(1.,1.,10.));floats(scene,motion+24,(9.,1.,10.))
  call(scene,f'motion_clock_mode_{prs:x}',0x8c052048)
  assert struct.unpack('<f',scene.m[(motion&0xffffff):(motion&0xffffff)+4])[0]==2.
  assert read(scene,motion+24)==0
 scene=clone(base,q);land_input(scene,0,motions=2);scene.f.fpscr|=0x80000;call(scene,'motion_clock_disabled_skips_PR_math',0x8c052048)
 # Container scanner: header-kind rejection and unknown chunks, no mocked callees.
 for kind in (0,1,2,3,4):
  scene=clone(base,q);data=0x8cef0000;put(scene,data+3,kind,1);put(scene,data+8,0,2)
  call(scene,f'container_kind_{kind}',0x8c09f3e0,(data,));assert scene.s.r[0]==int(kind==4)
 scene=clone(base,q);data=0x8cef0000;put(scene,data+3,4,1);put(scene,data+8,2,2);put(scene,data+12,0);put(scene,data+16,0,2)
 call(scene,'container_unknown_chunk_skipped',0x8c09f3e0,(data,));assert scene.s.r[0]==1
 scene=clone(base,q);call(scene,'container_invalid_header',0x8c09f3e0,(0x8cfffffe,),-3)
 # Unpatched ordinary API call on the preserved scene. It does not load a level.
 scene=clone(base,q);call(scene,'unpatched_prepared_land_start',0x8c051cc0,fixture=False)
 coverage=[]
 for stem in ('model_world_land_translation','model_world_land_dependencies_v2_translation'):
  for row in json.loads((P/('analysis/'+stem+'.json')).read_text())['routines']:
   addresses={int(a,16) for a in row['reachable_addresses']}
   coverage.append(dict(address=row['address'],rows=row['instruction_rows'],reached=len(executed&addresses),unreached=[hex(a) for a in sorted(addresses-executed)]))
 files=[Path(__file__),P/'model_world_land_verified_state.py',P/'model_world_land_reference.py',q.v.effects.DLL]
 report=dict(status='verified_declared_land_API_cases_and_unpatched_land_start',original_native_comparisons=len(cases),normal_returns=sum(c['status']==0 for c in cases),expected_host_guard_stops=sum(c['status']!=0 for c in cases),cases=cases,static_instruction_coverage=coverage,full_state_and_ordered_boundaries_match=True,new_native_jobs=0,prior_jobs_and_retired_generation_preserved=True,fixture_states_saved_as_canonical=False,faulted_states_resumed=False,real_populated_level_qualified=False,positive_resource_load_or_all_container_chunk_handlers_qualified=False,all_paths_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('WORLD LAND API VERIFIED:',len(cases),'comparisons',flush=True)
if __name__=='__main__':main()
