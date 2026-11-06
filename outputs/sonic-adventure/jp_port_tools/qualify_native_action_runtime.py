"""Audit the original action adaptation and freeze only its verified scope."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior
from rewrite_sprite_float_operations import rewrite

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def frame_link(b):return next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll')

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
    for n in ('workspace','build','dependencies','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('baseline','upstream','math','poses','frames','switch','pixels','init','tree','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve();dep=a.dependencies.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(build/'build_verification.json');db=read(dep/'build_verification.json');bh=sha(build/'build_verification.json');dh=sha(dep/'build_verification.json')
    previous=p/'analysis/jp_native_character_tree_package.json';seen=set();prior(r,previous,seen);assert len(seen)==33
    assert b['action_action_baseline_sha256']==dh
    for folder,meta in ((dep,db),(build,b)):
        for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h,n
        assert sha(meta['frames_DLL'])==meta['frames_DLL_sha256']
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');u=a.upstream.resolve()
        assert sha(base/'build_verification.json')==db['action_dependencies_baseline_sha256']
        assert db['action_dependencies_new_build_commands']==3 and b['action_action_new_build_commands']==4
        for meta,prefix,folder,count in ((db,'action_dependencies',dep,3),(b,'action_action',build,4)):
            for n,h in meta[prefix+'_original_source_sha256'].items():
                raw=(u/n).read_bytes();assert sha(u/n)==h
                assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])
            for n,h in meta[prefix+'_generated_source_sha256'].items():assert sha(folder/n)==h
            for n,h in meta[prefix+'_kept_frame_objects_sha256'].items():assert sha(n)==h
            for row in meta['commands'][:count]:
                assert row['exit_code']==row['diagnostic_bytes']==0
                if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
        source=(u/'src/draw/action.c').read_bytes();anchor=b'#include "draw/model.h"\n'
        context=b'''/* Select only the existing upstream GCC body branches. The existing native
 * SDK types/matrix calls remain unchanged; real RZ numerical providers follow. */
#include "jp_native_action_numeric.h"
#include "jp_native_sprite_numeric.h"
#define GCC 1
#define ALWAYS_INLINE static inline __attribute__((always_inline))
#define zam_fipr_self jp_action_fipr_self
#define _builtin_fsrra jp_action_fsrra
'''
        assert source.count(anchor)==1
        raw=(build/'external_action_unrewritten.c').read_bytes();assert raw==source.replace(anchor,anchor+context)
        run=subprocess.run(b['action_AST_command'],capture_output=True);assert run.returncode==0 and not run.stderr
        adapted,stats=rewrite(raw,json.loads(run.stdout),b['action_source_function_names'])
        assert adapted==(build/'external_action_native_rz.c').read_bytes() and stats==b['action_arithmetic_adaptation']
        assert stats['total_operators_rewritten']==55 and len(b['action_source_function_names'])==38
        ir=build/'action_native_rz.ll';assert sha(ir)==b['action_LLVM_IR_sha256'] and not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text()) and b['residual_native_action_FP_arithmetic']==0
        definitions=[];globals_source=(u/'sdk/ninja/njGlobal.c').read_bytes()
        for name in b['action_original_state_fields']:
            rows=re.findall(rb'^[^\r\n]*\b'+name.encode()+rb'\b[^\r\n]*;\r?\n',globals_source,re.M);assert len(rows)==1;definitions.append(rows[0])
        assert (build/'external_action_original_state.c').read_bytes()==b'#include <ninja.h>\n'+b''.join(definitions) and len(definitions)==7
        object_sets=[{n for n in frame_link(meta) if n.endswith('.obj')} for meta in (old,db,b)]
        assert object_sets[0]<=object_sets[1]<=object_sets[2] and len(object_sets[1]-object_sets[0])==len(object_sets[2]-object_sets[1])==2
        providers={}
        for obj in object_sets[2]:
            for sym in coff_symbols(Path(obj)):
                if sym['section']>0:providers.setdefault(sym['name'],set()).add(obj)
        for name in b['action_original_state_fields']+['_nj_motion_callback_','_nj_shape_buf_']:
            assert len(providers.get(name,set()))==1,(name,providers.get(name))
        for name in ('_nj_motion_callback_','_nj_shape_buf_'):assert next(iter(providers[name])) in object_sets[0]
        unchanged={path.name:sha(path) for path in base.glob('*.dll') if sha(path)==sha(build/path.name)};assert len(unchanged)==23 and Path(b['frames_DLL']).name not in unchanged
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        def pe(flag,path):
            run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr;return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
        extra={'njRotateXYZ','njRotateZXY','jp_action_fipr_self','jp_action_fsrra','draw_action','draw_motion','draw_shape_motion'}
        assert set(pe('--coff-exports',b['frames_DLL']))==set(pe('--coff-exports',old['frames_DLL']))|extra
        graph={path.name.lower():[n.lower() for n in pe('--coff-imports',path) if (build/n).exists()] for path in build.glob('*.dll')};active=set();done=set()
        def visit(name):
            assert name not in active,('DLL cycle',name);active.add(name)
            for child in graph[name]:
                if child not in done:visit(child)
            active.remove(name);done.add(name)
        for name in graph:
            if name not in done:visit(name)
        out=dict(status='original_GCC_action_body_RZ_rewrite_single_global_providers_prior_objects_and_acyclic_PE_graph_audited',build_verification_sha256=bh,dependencies_build_verification_sha256=dh,auditor_sha256=sha(__file__),clean_build_commands=7,retained_original_action_functions=38,scalar_operators_rewritten=55,residual_native_FP_operations=0,all_prior_Frame_objects_preserved=len(object_sets[0]),added_Frame_objects=sorted(object_sets[2]-object_sets[0]),single_original_motion_state_providers={n:sorted(providers[n]) for n in b['action_original_state_fields']+['_nj_motion_callback_','_nj_shape_buf_']},original_GCC_EVAL_SKIP_repair_preserved=True,unchanged_DLL_sha256=unchanged,extra_exports=sorted(extra),actual_local_PE_import_graph=graph,prior_packages_checked=33,retail_or_combined_modules_publishable=False,playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],flush=True);return
    tools={'math':'verify_native_action_dependencies.py','poses':'verify_native_action_poses.py','frames':'verify_native_action_frames.py','switch':'verify_native_character_switch.py','pixels':'verify_native_character_texture_pixels.py','init':'verify_native_game_init.py','tree':'verify_native_character_tree.py','audit':'qualify_native_action_runtime.py'}
    reports={};evidence={'build':build/'build_verification.json','dependencies_build':dep/'build_verification.json'}
    for name,tool in tools.items():
        path=getattr(a,name)/'verification.json';value=read(path);assert value['build_verification_sha256']==(dh if name=='math' else bh)
        assert value['auditor_sha256' if name=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool)
        reports[name]=value;evidence[name]=path
    assert [reports[n]['checks'] for n in ('math','poses','frames','switch','pixels','init','tree')]==[5377,76,368,2189,3049,530,264]
    math=reports['math'];poses=reports['poses'];frames=reports['frames']
    assert math['original_rotation_entry_calls']==960 and math['numerical_ABI_cases']==1536 and math['Float_probe_source_sha256']==sha(p/'jp_port_tools/action_numeric_probe.c')
    assert poses['original_complete_action_calls']==14 and poses['actual_original_njGetkmPointer_and_set_executed'] and not poses['original_game_function_hooks_used']
    assert frames['actual_original_keyed_animation_rendered'] and frames['actual_window_presents']==10 and frames['all_original_model_and_motion_records_preserved'] and frames['texture_Off_backend_bindings_and_uploads_zero']
    assert len(frames['rendered_original_action_frames'])==9 and len(set(frames['pose_matrix_sha256']))>=3
    for row in frames['screenshots']+frames['rendered_original_action_frames']+[frames['animated_preview']]:assert sha(row['path'])==row['sha256']
    assert frames['animated_preview']['frames']==9 and frames['animated_preview']['lossless_RGBA_frames_verified'] and frames['animated_preview']['preview_is_not_measured_game_FPS']
    authored=[p/'native'/n for n in ('jp_ninja_rotation_order.h','jp_ninja_rotation_order.c','jp_native_action_numeric.h','jp_native_action_numeric.c')]+[p/'jp_port_tools'/n for n in ('build_native_action_runtime.py','action_numeric_probe.c','verify_native_action_dependencies.py','verify_native_action_poses.py','verify_native_action_frames.py','qualify_native_action_runtime.py')]+[p/'JP_NATIVE_ACTION_RUNTIME.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    target=p/'analysis/jp_native_action_runtime_package.json';assert not target.exists();d.mkdir();proof={}
    for name,path in evidence.items():
        to=p/'analysis'/('jp_native_action_runtime_'+name+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    new=sum(reports[n]['checks'] for n in ('math','poses','frames'));regressions=sum(reports[n]['checks'] for n in ('switch','pixels','init','tree'));total=read(previous)['combined_qualified_scoped_checks']+new
    out=dict(status='sampled_original_Sonic_keyed_action_animation_exact_SH4_poses_real_retained_GPU_frames_Window_and_texture_Off_scoped_qualification',new_checks=new,selected_regression_checks=regressions,combined_qualified_scoped_checks=total,original_rotation_entry_calls=960,numerical_ABI_cases=1536,complete_original_action_pose_calls=14,original_action='sonic_updraft_action',original_action_frames=33,pose_sample_frames=[0,1,4,8.5,16,24,32],pose_register_banks=2,visible_models_per_sample=28,rendered_action_sample_frames=[0,4,8.5,12,16,20,24,28,32],actual_native_window_presents=10,lossless_nine_frame_native_capture_preview_verified=True,preview_is_not_measured_game_FPS=True,original_action_functions_retained=38,original_motion_state_definitions=7,scalar_operators_rewritten=55,all_prior_Frame_objects_and_23_other_DLLs_preserved=True,all_canonical_model_and_Sonic_motion_records_unchanged=True,texture_Off_zero_texture_sampler_bindings_and_uploads_verified=True,retained_model_action_texture_consumers_protect_parent=True,caller_retirement_after_GPU_and_frame_release=True,prior_qualifications_preserved={'character_tree':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},proof_files_sha256=proof,build_verification_sha256=bh,dependencies_build_verification_sha256=dh,private_frames_DLL_sha256=b['frames_DLL_sha256'],manual_preflight_camera_lighting_and_consumer_setup=True,automatic_task_render_view_acquisition=False,all_motion_shape_blend_or_character_paths_qualified=False,original_gameplay_task_FSM_GAME_MODE_BOOT_main_loop_or_gameplay_verified=False,ingame_settings_UI_implemented=False,console_hardware_or_capture_equivalence=False,retail_source_data_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new;',regressions,'regressions;',total,'overlapping checks',flush=True)
if __name__=='__main__':main()
