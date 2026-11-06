"""Audit original draw controls and freeze the scoped task-animation evidence."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_draw_controls import NAMES
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def link(b):return next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll')
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
 for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
 for n in ('baseline','upstream','controls','task','init','audit'):ap.add_argument('--'+n,type=Path)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(build/'build_verification.json');bh=sha(build/'build_verification.json')
 previous=p/'analysis/jp_native_action_runtime_package.json';seen=set();prior(r,previous,seen);assert len(seen)==34
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
 if a.stage=='audit':
  base=a.baseline.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['draw_controls_baseline_sha256'];u=a.upstream.resolve();raw=(u/'src/system/util.c').read_bytes();assert sha(u/'src/system/util.c')==b['draw_controls_original_source_sha256'] and raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/system/util.c'])
  source=build/'external_draw_controls.c';assert sha(source)==b['draw_controls_generated_source_sha256'];decl=[]
  for name in ('saved_attr_and','saved_attr_or','saved_control_3d'):
   rows=re.findall(rb'^static [^;\r\n]*\b'+name.encode()+rb';\r?\n',raw,re.M);assert len(rows)==1;decl+=rows
  bodies={n:owner_function(raw,n) for n in NAMES}
  assert source.read_bytes()==b'#include <ninja.h>\n'+b''.join(decl)+b'\n'.join(bodies.values())
  assert {n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()}==b['draw_controls_original_function_sha256']
  for row in b['commands'][:2]:
   assert row['exit_code']==row['diagnostic_bytes']==0
   if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
  objects=[{n for n in link(meta) if n.endswith('.obj')} for meta in (old,b)];assert objects[0]<=objects[1] and len(objects[1]-objects[0])==1 and objects[0]==set(b['draw_controls_kept_frame_objects_sha256'])
  for n,h in b['draw_controls_kept_frame_objects_sha256'].items():assert sha(n)==h
  providers={}
  for obj in objects[1]:
   for s in coff_symbols(Path(obj)):
    if s['section']>0:providers.setdefault(s['name'],set()).add(obj)
  for n in NAMES:assert len(providers[n])==1 and next(iter(providers[n])) in objects[1]-objects[0]
  for n in ('_nj_constant_attr_and_','_nj_constant_attr_or_','_nj_control_3d_flag_'):assert len(providers[n])==1 and next(iter(providers[n])) in objects[0]
  unchanged={path.name:sha(path) for path in base.glob('*.dll') if sha(path)==sha(build/path.name)};assert len(unchanged)==23
  reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
  def pe(flag,path):
   run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr;return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
  assert set(pe('--coff-exports',b['frames_DLL']))==set(pe('--coff-exports',old['frames_DLL']))|set(NAMES)
  graph={path.name.lower():[n.lower() for n in pe('--coff-imports',path) if (build/n).exists()] for path in build.glob('*.dll')};active=set();done=set()
  def visit(n):
   assert n not in active;active.add(n)
   for child in graph[n]:
    if child not in done:visit(child)
   active.remove(n);done.add(n)
  for n in graph:
   if n not in done:visit(n)
  out=dict(status='eight_unmodified_original_draw_control_bodies_same_renderer_state_and_acyclic_PE_graph_audited',build_verification_sha256=bh,auditor_sha256=sha(__file__),original_helpers=list(NAMES),prior_Frame_objects=len(objects[0]),added_objects=sorted(objects[1]-objects[0]),unchanged_DLL_sha256=unchanged,actual_local_PE_import_graph=graph,prior_packages_checked=34,original_single_slot_save_state_preserved=True,playable=False,overall_completion_percent=10)
  d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],flush=True);return
 tools={'controls':'verify_native_draw_controls.py','task':'verify_native_action_tasks.py','init':'verify_native_game_init.py','audit':'qualify_native_draw_controls.py'};reports={};evidence={'build':build/'build_verification.json'}
 for name,tool in tools.items():
  path=getattr(a,name)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['auditor_sha256' if name=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[name]=v;evidence[name]=path
 assert [reports[n]['checks'] for n in ('controls','task','init')]==[8208,380,530] and reports['controls']['original_complete_calls']==4104
 t=reports['task'];assert t['task_scheduler_original'] and t['owning_callbacks_are_diagnostic_not_original_player'] and not t['production_automatic_task_render_lease_service_implemented']
 assert len(t['task_callback_events'])==10 and [x[0] for x in t['task_callback_events']]==['draw']*9+['destroy'] and t['actual_window_presents']==10
 old=read(p/'analysis/jp_native_action_runtime_frames.json');assert [x['RGBA_sha256'] for x in t['rendered_original_action_frames']]==[x['RGBA_sha256'] for x in old['rendered_original_action_frames']]
 for row in t['screenshots']+t['rendered_original_action_frames']+[t['animated_preview']]:assert sha(row['path'])==row['sha256']
 authored=[p/'jp_port_tools'/n for n in ('build_native_draw_controls.py','verify_native_draw_controls.py','verify_native_action_tasks.py','qualify_native_draw_controls.py')]+[p/'JP_NATIVE_DRAW_CONTROLS.txt']
 for path in authored:
  if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
 target=p/'analysis/jp_native_draw_controls_package.json';assert not target.exists();d.mkdir();proof={}
 for name,path in evidence.items():
  to=p/'analysis'/('jp_native_draw_controls_'+name+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
 out=dict(status='eight_original_draw_controls_machine_execution_and_original_task_scheduled_animation_diagnostic_scoped_qualification',new_checks=8588,selected_regression_checks=530,combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+8588,original_helper_functions=8,original_complete_machine_calls=4104,diagnostic_task_draw_callbacks=9,diagnostic_task_destructor_calls=1,all_nine_GPU_images_match_prior_direct_action=True,actual_window_presents=10,retained_views_released_by_diagnostic_task_destructor_after_GPU_and_frame_retirement=True,owning_callbacks_are_diagnostic_not_original_player=True,production_automatic_task_render_leases=False,original_Sonic_player_callback_or_gameplay_verified=False,prior_qualifications_preserved={'action_runtime':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],all_prior_Frame_objects_and_23_other_DLLs_preserved=True,retail_external_source_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
 (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],'8588 new; 530 regressions',flush=True)
if __name__=='__main__':main()
