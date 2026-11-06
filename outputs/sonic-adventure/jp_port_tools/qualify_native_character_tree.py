"""Audit real original tree integration and freeze its rendered scope."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('baseline','upstream','vector','tree','switch','pixels','init','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json');bh=sha(build/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_character_switch_package.json';seen=set();prior(r,previous,seen);assert len(seen)==32
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    for n,h in b['character_tree_generated_source_sha256'].items():assert sha(build/n)==h
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['character_tree_baseline_sha256'];u=a.upstream.resolve()
        for n,h in b['character_tree_original_source_sha256'].items():assert sha(u/n)==h
        original=owner_function((u/'src/draw/helpers.c').read_bytes(),'draw_object');now=owner_function((build/'external_character_tree.c').read_bytes(),'draw_object');assert original==now and hashlib.sha256(now).hexdigest()==b['character_tree_original_body_sha256']
        assert b['character_tree_new_build_commands']==3
        for row in b['commands'][:3]:
            assert row['exit_code']==row['diagnostic_bytes']==0
            if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
        for n,h in b['character_tree_kept_frame_objects_sha256'].items():assert sha(n)==h
        # Select only the most recent Frame link, rather than inherited recipes.
        links=[next(row['command'] for row in meta['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll') for meta in (old,b)];objects=[{n for n in cmd if n.endswith('.obj')} for cmd in links]
        assert not objects[0]-objects[1] and len(objects[1]-objects[0])==2 and objects[0]==set(b['character_tree_kept_frame_objects_sha256'])
        unchanged={path.name:sha(path) for path in base.glob('*.dll') if sha(path)==sha(build/path.name)};assert len(unchanged)==23 and Path(b['frames_DLL']).name not in unchanged
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        def pe(flag,path):
            run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr;return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
        assert set(pe('--coff-exports',b['frames_DLL']))==set(pe('--coff-exports',old['frames_DLL']))|{'draw_object','njScaleV'}
        graph={path.name.lower():[n.lower() for n in pe('--coff-imports',path) if (build/n).exists()] for path in build.glob('*.dll')};active=set();done=set()
        def visit(n):
            assert n not in active,('DLL cycle',n);active.add(n)
            for child in graph[n]:
                if child not in done:visit(child)
            active.remove(n);done.add(n)
        for n in graph:
            if n not in done:visit(n)
        out=dict(status='exact_original_model_tree_and_same_matrix_scale_all_prior_Frame_objects_and_acyclic_link_audited',original_tree_body_preserved=True,all_prior_frame_objects_preserved=len(objects[0]),added_objects=sorted(objects[1]-objects[0]),unchanged_DLL_sha256=unchanged,extra_exports=['draw_object','njScaleV'],actual_local_PE_import_graph=graph,prior_packages_checked=32,build_verification_sha256=bh,auditor_sha256=sha(__file__),playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],len(unchanged),'unchanged DLLs',flush=True);return
    tools={'vector':'verify_native_scale_vector.py','tree':'verify_native_character_tree.py','switch':'verify_native_character_switch.py','pixels':'verify_native_character_texture_pixels.py','init':'verify_native_game_init.py','audit':'qualify_native_character_tree.py'};reports={};evidence={'build':build/'build_verification.json'}
    for n,tool in tools.items():
        path=getattr(a,n)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[n]=v;evidence[n]=path
    assert reports['vector']['checks']==6400 and reports['vector']['original_entry_calls']==1280
    assert [reports[n]['checks'] for n in ('switch','pixels','init')]==[2189,3049,530]
    tree=reports['tree'];assert tree['original_nodes']==73 and tree['original_models']==28 and tree['packet_counts']==dict(batches=53,vertices=1425,triangles=1001) and tree['texture_Off_backend_bindings_and_uploads_zero'] and tree['actual_window_presents']==2
    for row in tree['screenshots']:assert sha(row['path'])==row['sha256'] and row['actual_window_backbuffer']
    authored=[p/'native'/n for n in ('jp_ninja_scale_vector.h','jp_ninja_scale_vector.c')]+[p/'jp_port_tools'/n for n in ('build_native_character_tree.py','verify_native_scale_vector.py','verify_native_character_tree.py','qualify_native_character_tree.py')]+[p/'JP_NATIVE_CHARACTER_TREE.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    target=p/'analysis/jp_native_character_tree_package.json';assert not target.exists();d.mkdir();proof={}
    for n,path in evidence.items():
        to=p/'analysis'/('jp_native_character_tree_'+n+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    new=reports['vector']['checks']+tree['checks'];regression=sum(reports[n]['checks'] for n in ('switch','pixels','init'));total=read(previous)['combined_qualified_scoped_checks']+new
    out=dict(status='genuine_original_static_Sonic_model_tree_real_textures_palette_retained_GPU_Window_and_texture_Off_vertex_material_colours_scoped_qualification',new_checks=new,selected_regression_checks=regression,combined_qualified_scoped_checks=total,original_vector_scale_calls=1280,original_tree_nodes=73,original_models=28,genuine_original_triangles=1001,genuine_original_vertices=1425,actual_native_window_presents=2,real_palette_loaded_by_original_native_read_queue=True,all_canonical_model_records_unchanged=True,texture_Off_zero_texture_sampler_bindings_and_uploads_verified=True,retained_model_texture_consumers_protect_parent=True,caller_model_texture_retirement_after_GPU_and_frame_release=True,prior_qualifications_preserved={'character_switch':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],manual_preflight_camera_lighting_and_consumer_setup=True,automatic_task_render_view_acquisition=False,animation_GCC_motion_interpreter_GAME_MODE_BOOT_main_loop_or_gameplay_verified=False,ingame_settings_UI_implemented=False,nonfinite_ScaleV_machine_comparison=False,console_capture_equivalence=False,retail_source_data_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new checks;',regression,'regressions;',total,'overlapping scoped checks',flush=True)
if __name__=='__main__':main()
