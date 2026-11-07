"""Audit/freeze floor values, sampled pose trees and angle differences.

Linked player_weld/event_motions_draw are explicitly not runtime-qualified.
"""
import argparse,ast,ctypes as C,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from build_native_event_pose import derive,comdat_any
from package_native_sound_placement import prior
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def linked(b):return next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll')
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','baseline','floor-build','build','floor','poses','angles','init','blend','frames','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');base=a.baseline.resolve();fb=a.floor_build.resolve();build=a.build.resolve();old=read(base/'build_verification.json');f=read(fb/'build_verification.json');b=read(build/'build_verification.json');bh=sha(build/'build_verification.json');previous=p/'analysis/jp_native_action_link_package.json';seen=set();prior(r,previous,seen);assert len(seen)==37
 for meta in (old,f,b):
  for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h
  for key,value in meta.items():
   if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==meta[key+'_sha256']
 assert f['runtime_floor_baseline_sha256']==sha(base/'build_verification.json') and b['event_pose_baseline_sha256']==sha(fb/'build_verification.json')
 originals={}
 for recipe in (f['runtime_floor_recipe'],b['event_pose_recipe']):
  assert recipe['upstream_commit']==PIN
  for name,h in recipe['original_source_sha256'].items():
   raw=(u/name).read_bytes();assert hashlib.sha256(raw).hexdigest()==h and raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+name]);originals[name]=raw
 raw=originals['sdk/runtime/_i_flors.c'];assert raw.count(b'(float)(int)x')==1;expected=b'#define _errno jp_sdk_errno\n#include "jp_native_runtime_floor.h"\n#include "jp_native_sprite_numeric.h"\n'+raw.replace(b'(float)(int)x',b'jp_runtime_trunc_scalar(x)')+b'\n'+originals['sdk/runtime/floorf.c'];assert expected==(fb/'external_runtime_floor.c').read_bytes()
 fr=f['runtime_floor_recipe'];assert fr['original_globals_added']==['_NIFONES'] and fr['existing_SDK_print_errno_owner_reused'] and fr['SDK_errno_native_symbol']=='jp_sdk_errno';rows=re.findall(rb'^[^\r\n]*\b_NIFONES\b[^\r\n]*;\r?\n',originals['sdk/runtime/consts.c'],re.M);assert len(rows)==1 and hashlib.sha256(rows[0]).hexdigest()==fr['original_global_definition_sha256']['_NIFONES'];assert (fb/'external_runtime_floor_state.c').read_bytes()==b'#define _errno jp_sdk_errno\n_Static_assert(sizeof(unsigned long)==4,"SDK word width");\n'+rows[0]
 def adapted_source(folder,raw,names,command,target,stats):
  x=subprocess.run(command,capture_output=True);assert not x.returncode and not x.stderr;out,current=rewrite(raw,json.loads(x.stdout),names);assert out==(folder/target).read_bytes() and current==stats
 adapted_source(fb,expected,fr['source_functions'],f['commands'][0]['command'],'external_runtime_floor_rz.c',fr['scalar_arithmetic_adaptation'])
 assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp|fptosi|sitofp)\b',(fb/'runtime_floor.ll').read_text())
 command=f['commands'][3]['command'];sdk=command[:command.index('-c')];x=subprocess.run(sdk+['-S','-emit-llvm',str(p/'native/jp_native_runtime_floor.c'),'-o','-'],capture_output=True);assert not x.returncode and not x.stderr and not re.findall(rb'\b(?:fadd|fsub|fmul|fdiv|fcmp|fptosi|sitofp)\b',x.stdout);bridge_ir_sha256=hashlib.sha256(x.stdout).hexdigest()
 units=derive(originals);er=b['event_pose_recipe'];assert set(units)==set(er['units']) and er['original_source_function_count']==8
 for index,(tag,(raw,names)) in enumerate(units.items()):
  info=er['units'][tag];assert raw==(build/('external_'+tag+'.c')).read_bytes();adapted_source(build,raw,names,b['commands'][3*index]['command'],'external_'+tag+'_rz.c',info['scalar_arithmetic_adaptation']);assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(build/(tag+'.ll')).read_text())
 # Check every original function body independently of the unit constructor.
 owners={'pose':'src/draw/pose.c','weld':'src/player/weld.c','player_get':'src/player/player.c','event_draw':'src/event/event.c','bindings':'src/system/util.c'}
 for tag,(raw,names) in units.items():
  for name in names:
   expected=owner_function(originals[owners[tag]],name)
   if name=='pose_node':expected=expected.replace(b'(NJS_MKEY_A *)y',b'(NJS_MKEY_A *)(uintptr_t)(Uint32)y')
   assert owner_function(raw,name)==expected
 for folder,meta,count,key in ((fb,f,6,'runtime_floor'),(build,b,16,'event_pose')):
  recipe=meta[key+'_recipe']
  groups=[recipe] if key=='runtime_floor' else list(recipe['units'].values())
  for group in groups:
   for name,h in group['generated_source_sha256'].items():assert sha(folder/name)==h
  if key=='runtime_floor':
   for name,h in recipe['new_object_sha256'].items():assert sha(folder/name)==h
  else:
   for tag,info in recipe['units'].items():assert sha(folder/(tag+'.obj'))==info['object_sha256']
  for row in meta['commands'][:count]:
   assert row['exit_code']==row['diagnostic_bytes']==0
   if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
  previous_meta=old if key=='runtime_floor' else f;before={n for n in linked(previous_meta) if n.endswith('.obj')};after={n for n in linked(meta) if n.endswith('.obj')};assert not before-after and len(after-before)==(3 if key=='runtime_floor' else 5);assert {n:sha(n) for n in before}==meta[key+'_kept_frame_objects_sha256']
  providers={s['name'] for n in before for s in coff_symbols(Path(n)) if s['section']>0};all_defined=set(providers);all_undefined=set()
  for path in sorted(after-before):
   symbols=coff_symbols(Path(path));defined={s['name'] for s in symbols if s['section']>0};duplicate=defined&all_defined
   for sym in symbols:
    if sym['name'] in duplicate:assert re.fullmatch('__real@[0-9a-f]+',sym['name']) and comdat_any(Path(path),sym['section'])
   all_defined|=defined;all_undefined|={s['name'] for s in symbols if s['section']==0}
  assert not all_undefined-all_defined-{'_fltused'}
  assert recipe['unchanged_DLL_sha256']=={x.name:sha(x) for x in (base if key=='runtime_floor' else fb).glob('*.dll') if x.name!='jp_native_frames.dll'} and all(sha(folder/n)==h for n,h in recipe['unchanged_DLL_sha256'].items())
 reader=Path(f['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
 def pe(flag,path):
  x=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not x.returncode and not x.stderr;return set(re.findall(r'^  Name: (.+)$',x.stdout.decode(),re.M))
 for before,after,key in ((old,f,'runtime_floor'),(f,b,'event_pose')):
  assert pe('--coff-exports',after['frames_DLL'])==pe('--coff-exports',before['frames_DLL'])|set(after[key+'_recipe']['new_exports']);assert pe('--coff-imports',after['frames_DLL'])==pe('--coff-imports',before['frames_DLL'])
 evidence={'floor_build':fb/'build_verification.json','build':build/'build_verification.json'};reports={}
 for name,tool in (('floor','verify_native_runtime_floor.py'),('poses','verify_native_event_poses.py'),('angles','verify_native_pose_angles.py'),('init','verify_native_game_init.py'),('blend','verify_native_action_link_poses.py'),('frames','verify_native_action_link_frames.py')):
  path=getattr(a,name)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['verifier_sha256']==sha(p/'jp_port_tools'/tool);evidence[name]=path;reports[name]=v
 assert reports['floor']['checks']==39447 and reports['floor']['original_complete_floor_calls']==13148 and reports['angles']['checks']==131072 and reports['angles']['original_complete_calls']==65536;pose=reports['poses'];assert pose['checks']==23816 and pose['original_calls']=={'pose_action':256,'pose_get_count':256,'pose_get_matrix':1024} and pose['optional_output_pointer_and_NULL_both_verified']
 assert len(pose['records'])==256 and {x['out_enabled'] for x in pose['records']}=={False,True} and {x['retained_nodes'] for x in pose['records']}=={1,9,73}
 assert reports['init']['checks']==530 and reports['blend']['checks']==519 and reports['frames']['checks']==382
 frames=reports['frames'];assert frames['actual_window_presents']==10 and frames['texture_Off_backend_bindings_and_uploads_zero'];
 for row in frames['screenshots']+frames['rendered_blend_samples']+[frames['animated_preview']]:assert sha(row['path'])==row['sha256']
 d.mkdir();audit=dict(status='original_floor_pose_angle_sources_RZ_value_bridge_native_globals_PE_and_prior_owner_preservation_audited',build_verification_sha256=bh,auditor_sha256=sha(__file__),prior_packages_verified=37,bridge_LLVM_sha256=bridge_ir_sha256,no_host_FP_integer_conversion_or_arithmetic_in_bridge=True,shared_existing_SDK_print_errno_owner_reused=True,host_CRT_errno_function_not_redefined=True,genuine_player_work_BSS_added=True,existing_player_records_not_duplicated=True,all_previous_Frame_objects_preserved=True,imports_unchanged=True,all_compiler_constant_collisions_are_COFF_COMDAT_Any=True,player_weld_and_event_draw_linked_but_runtime_unverified=True,playable=False,overall_completion_percent=10)
 (d/'audit.json').write_text(json.dumps(audit,indent=2)+'\n');evidence['audit']=d/'audit.json';target=p/'analysis/jp_native_event_pose_package.json';assert not target.exists();proof={}
 for name,path in evidence.items():
  to=p/'analysis'/('jp_native_event_pose_'+name+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
 files=['native/jp_native_runtime_floor.c','native/jp_native_runtime_floor.h']+['jp_port_tools/'+n for n in ('build_native_runtime_floor.py','build_native_event_pose.py','verify_native_runtime_floor.py','verify_native_event_poses.py','verify_native_pose_angles.py','qualify_native_event_pose.py')]+['JP_NATIVE_EVENT_POSE.txt']
 for name in files:
  if name.endswith('.py'):ast.parse((p/name).read_text())
 new=sum(reports[n]['checks'] for n in ('floor','poses','angles'));assert new==194335
 out=dict(status='genuine_floor_values_shared_SDK_errno_sampled_POS_ANG_pose_trees_and_angle_difference_scoped_qualification',new_checks=new,selected_regression_checks=1431,combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+new,original_complete_floor_calls=13148,original_angle_difference_calls=65536,original_pose_and_getter_calls=pose['original_calls'],compiled_original_source_functions=11,all_previous_Frame_objects_preserved=True,same_existing_SDK_print_errno_and_player_records_owners_reused=True,original_player_work_native_BSS_added=True,FPSCR_flags_traps_and_arbitrary_host_FP_modes_verified=False,three_factor_scale_pose_path_verified=False,player_weld_and_event_motions_draw_linked_but_not_runtime_qualified=True,production_player_record_task_texture_lifetimes_verified=False,original_player_callback_FSM_mainloop_or_gameplay_verified=False,actual_regression_window_Presents=10,texture_Off_zero_bindings_and_uploads=True,prior_qualifications_preserved={'action_link':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={(p/name).relative_to(r).as_posix():sha(p/name) for name in files},proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],retail_external_source_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
 (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new checks;1431 regressions',flush=True)
if __name__=='__main__':main()
