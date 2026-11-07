"""Audit/freeze sampled original blend behavior and actual GPU evidence."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from PIL import Image
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','baseline','build','poses','frames','init','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();u=a.upstream.resolve();base=a.baseline.resolve();build=a.build.resolve()
 assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(build/'build_verification.json');old=read(base/'build_verification.json');recipe=b['action_link_recipe'];bh=sha(build/'build_verification.json')
 previous=p/'analysis/jp_native_sonic_toes_package.json';seen=set();prior(r,previous,seen);assert len(seen)==36
 for meta in (old,b):
  for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h
  for key,value in meta.items():
   if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==meta[key+'_sha256']
 assert b['action_link_baseline_sha256']==sha(base/'build_verification.json') and recipe['upstream_commit']==PIN
 raw=(u/'src/draw/action_link.c').read_bytes();globals_raw=(u/'sdk/ninja/njGlobal.c').read_bytes()
 for n in recipe['original_source_sha256']:
  assert sha(u/n)==recipe['original_source_sha256'][n] and (u/n).read_bytes()==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])
 for name,h in recipe['generated_source_sha256'].items():assert sha(build/name)==h
 for name,h in recipe['new_object_sha256'].items():assert sha(build/name)==h
 # Check the complete external body, including only the documented context
 # and the three zero-valued matching-spelling casts, before RZ adaptation.
 anchor=b'#include "draw/model.h"\n';context=b'#include "jp_native_action_numeric.h"\n#include "jp_native_sprite_numeric.h"\n#define GCC 1\n#define ALWAYS_INLINE static inline __attribute__((always_inline))\n#define zam_fipr_self jp_action_fipr_self\n#define _builtin_fsrra jp_action_fsrra\n';expected=raw.replace(anchor,anchor+context);body=owner_function(expected,'action_link_start_motion');assert body.count(b'(Uint32)link')==2 and body.count(b'(Sint32)link')==1 and b'link = 0;' in body;expected=expected.replace(body,body.replace(b'(Uint32)link',b'0').replace(b'(Sint32)link',b'0'));assert expected==(build/'external_action_link.c').read_bytes()
 commands=b['commands'][:5];assert len(commands)==5 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in commands)
 for row in commands:
  if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
 run=subprocess.run(commands[0]['command'],capture_output=True);assert not run.returncode and not run.stderr;adapted,stats=rewrite(expected,json.loads(run.stdout),recipe['source_functions']);assert adapted==(build/'external_action_link_rz.c').read_bytes() and stats==recipe['scalar_arithmetic_adaptation'] and stats['total_operators_rewritten']==88
 assert recipe['source_function_count']==39 and len(recipe['source_functions'])==39 and not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(build/'action_link.ll').read_text())
 def linked(meta):return next(row['command'] for row in meta['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll')
 earlier={n for n in linked(old) if n.endswith('.obj')};now={n for n in linked(b) if n.endswith('.obj')};assert now-earlier=={str(build/'action_link_original.obj'),str(build/'action_link_state.obj')} and not earlier-now
 assert {n:sha(n) for n in earlier}==b['action_link_kept_frame_objects_sha256']
 providers={s['name'] for n in earlier for s in coff_symbols(Path(n)) if s['section']>0};undefined={s['name'] for s in coff_symbols(build/'action_link_original.obj') if s['section']==0};missing=sorted(undefined-providers-{'_fltused'});assert missing==recipe['original_globals_added'] and len(missing)==25
 rows=[]
 for name in missing:
  found=re.findall(rb'^[^\r\n]*\b'+name.encode()+rb'\b[^\r\n]*;\r?\n',globals_raw,re.M);assert len(found)==1 and hashlib.sha256(found[0]).hexdigest()==recipe['original_global_definition_sha256'][name];rows+=found
 assert b'#include <ninja.h>\n'+b''.join(rows)==(build/'external_action_link_state.c').read_bytes()
 defined={s['name'] for s in coff_symbols(build/'action_link_state.obj') if s['section']>0}
 assert set(defined)==set(missing) and not set(missing)&providers
 reader=Path(commands[0]['command'][0]).with_name('llvm-readobj.exe')
 def pe(flag,path):
  x=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not x.returncode and not x.stderr;return x.stdout.decode()
 def symbols(flag,path):return set(re.findall(r'^  Name: (.+)$',pe(flag,path),re.M))
 assert symbols('--coff-exports',b['frames_DLL'])==symbols('--coff-exports',old['frames_DLL'])|{'draw_action_link','draw_motion_link','draw_shape_motion_link'}
 assert symbols('--coff-imports',b['frames_DLL'])==symbols('--coff-imports',old['frames_DLL'])
 unchanged={x.name:sha(x) for x in base.glob('*.dll') if x.name!='jp_native_frames.dll'};assert unchanged==recipe['unchanged_DLL_sha256'] and all(sha(build/n)==h for n,h in unchanged.items())
 evidence={'build':build/'build_verification.json'};reports={}
 for name,tool in (('poses','verify_native_action_link_poses.py'),('frames','verify_native_action_link_frames.py'),('init','verify_native_game_init.py')):
  path=getattr(a,name)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['verifier_sha256']==sha(p/'jp_port_tools'/tool);evidence[name]=path;reports[name]=v
 pose=reports['poses'];frame=reports['frames'];assert pose['checks']==519 and pose['original_complete_action_calls']==102 and frame['checks']==382 and reports['init']['checks']==530
 samples=(0.,4.,8.5,12.,16.,20.,24.,28.,32.)
 for bank in (0,1):
  for sample in samples:assert any(x['mode']=='cross' and x['frame']==8.5 and x['second']==16. and x['ratio']==sample/32 and x['bank']==bank and x['visible_models']==28 for x in pose['records'])
 assert frame['actual_window_presents']==10 and frame['texture_Off_backend_bindings_and_uploads_zero'] and frame['both_genuine_action_views_retained_until_task_destruction'] and frame['all_original_model_and_motion_records_preserved'] and frame['owning_callbacks_are_diagnostic_not_original_player']
 for row in frame['screenshots']+frame['rendered_blend_samples']+[frame['animated_preview']]:assert sha(row['path'])==row['sha256']
 preview=Image.open(frame['animated_preview']['path']);assert preview.n_frames==9
 for index,row in enumerate(frame['rendered_blend_samples']):
  preview.seek(index);image=Image.open(row['path']).convert('RGBA');assert image.size==(512,512) and preview.convert('RGBA').tobytes()==image.tobytes() and hashlib.sha256(image.convert('RGB').tobytes()).hexdigest()==row['displayed_RGB_sha256'] and row['blend_ratio']==samples[index]/32.
 assert frame['opaque_window_RGB_preserved'] and frame['readback_alpha_not_stored_in_opaque_window_PNG']
 d.mkdir();audit=dict(status='original_action_link_source_globals_object_providers_PE_preservation_and_exact_captured_pose_coverage_audited',build_verification_sha256=bh,auditor_sha256=sha(__file__),prior_packages_verified=36,source_functions=39,scalar_RZ_adaptations=88,original_globals=25,all_previous_Frame_objects_preserved=True,existing_private_symbols_not_duplicated=True,unchanged_DLLs=len(unchanged),new_exports=recipe['new_exports'],imports_unchanged=True,all_nine_GPU_sample_inputs_machine_compared_in_both_banks=True,captures_and_APNG_rehashed=True,original_player_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
 (d/'audit.json').write_text(json.dumps(audit,indent=2)+'\n');evidence['audit']=d/'audit.json'
 names=['jp_port_tools/'+n for n in ('build_native_action_link.py','verify_native_action_link_poses.py','verify_native_action_link_frames.py','qualify_native_action_link.py')]+['JP_NATIVE_ACTION_LINK.txt']
 for name in names:
  if name.endswith('.py'):ast.parse((p/name).read_text())
 target=p/'analysis/jp_native_action_link_package.json';assert not target.exists();proof={}
 for name,path in evidence.items():
  to=p/'analysis'/('jp_native_action_link_'+name+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
 new=pose['checks']+frame['checks'];assert new==901
 out=dict(status='sampled_genuine_Sonic_POS_ANG_action_link_original_machine_poses_actual_parent_task_GPU_and_texture_Off_qualified',new_checks=new,selected_regression_checks=530,combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+new,original_complete_action_link_calls=102,compiled_source_functions=39,genuine_original_globals_added=25,actual_window_presents=10,visible_models=28,all_nine_render_sample_inputs_machine_verified_in_both_FPU_banks=True,scope=pose['scope'],shape_scale_other_characters_all_link_entrypoints_or_negative_frames_verified=False,original_player_callback_FSM_mainloop_or_gameplay_verified=False,automatic_production_task_and_texture_ownership=False,preview_is_not_measured_game_FPS=True,prior_qualifications_preserved={'sonic_toes':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={(p/name).relative_to(r).as_posix():sha(p/name) for name in names},proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],retail_external_source_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
 (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new scoped assertions;530 regressions',flush=True)
if __name__=='__main__':main()
