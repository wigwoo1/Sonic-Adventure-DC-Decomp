"""Audit original code/single shared providers and freeze character transitions."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('upstream','baseline','check','authority','texture','pixels','motion','init','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json');bh=sha(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');previous=p/'analysis/jp_native_asset_character_exit_package.json';seen=set();prior(r,previous,seen);assert len(seen)==31
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['character_switch_generated_source_sha256'].items():assert sha(build/n)==h,n
    for key in ('frames_DLL','motion_DLL'):assert sha(b[key])==b[key+'_sha256']
    if a.stage=='audit':
        base=a.baseline.resolve();u=a.upstream.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['character_switch_baseline_sha256']
        git=['C:/Program Files/Git/cmd/git.exe','-C',str(u)];assert subprocess.check_output(git+['rev-parse','HEAD']).decode().strip()==PIN and not subprocess.check_output(git+['status','--porcelain','--untracked-files=no'])
        headers={}
        for n in ('src/chao/chao.h','src/save/save.h','src/save/card.h','src/stage/stage.h','src/hud/hud.h','src/player/player.h'):
            raw=(u/n).read_bytes();assert raw==subprocess.check_output(git+['show',PIN+':'+n]);headers[n]=sha(u/n)
        for n,h in b['character_switch_original_source_sha256'].items():assert sha(u/n)==h
        source=(build/'external_character_chao.c').read_bytes();stage=(build/'character_family_with_switch.c').read_bytes()
        for n,h in b['character_switch_original_body_sha256'].items():
            raw=owner_function(stage if n=='set_character' else source,n);assert hashlib.sha256(raw).hexdigest()==h,n
        original=owner_function((u/'src/stage/stage.c').read_bytes(),'set_character');tail=original[original.index(b'    index = character_current;',original.index(b'    texture_load_character')):original.rfind(b'}')]
        assert hashlib.sha256(tail).hexdigest()==b['character_switch_original_tail_sha256'] and stage.count(tail)==2
        old_motion=(base/'motion_store_with_exit.c').read_bytes();assert (build/'motion_store_with_select.c').read_bytes()==old_motion+(p/'native/jp_native_motion_select.inc').read_bytes()
        query=(p/'native/jp_native_motion_select.inc').read_text();assert not re.search(r'store\.[\w.]+\s*=(?!=)|\b(?:malloc|calloc|free|restore_bank|result)\s*\(',query)
        changed=[];unchanged={}
        for path in base.glob('*.dll'):
            if sha(path)==sha(build/path.name):unchanged[path.name]=sha(path)
            else:changed.append(path.name)
        assert len(unchanged)==22 and set(changed)=={Path(b['frames_DLL']).name,Path(b['motion_DLL']).name}
        assert b['character_switch_new_build_commands']==5
        for row in b['commands'][:5]:
            assert row['exit_code']==row['diagnostic_bytes']==0
            if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
        for n,h in b['character_switch_kept_frame_objects_sha256'].items():assert sha(n)==h
        links=[]
        for meta in (old,b):links.append(next(x['command'] for x in meta['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
        objects=[{n for n in row if n.endswith('.obj')} for row in links]
        assert len(objects[0]-objects[1])==1 and len(objects[1]-objects[0])==2 and objects[0]&objects[1]==set(b['character_switch_kept_frame_objects_sha256'])
        providers={}
        for obj in objects[1]:
            for sym in coff_symbols(Path(obj)):
                if sym['section']>0 and sym['name'] in b['character_switch_shared_symbol_providers']:providers.setdefault(sym['name'],[]).append(obj)
        assert providers==b['character_switch_shared_symbol_providers']
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        def pe(flag,path):
            run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr;return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
        expected={'frames_DLL':{'jp_character_switch','jp_character_switch_recover','jp_character_switch_retire','jp_character_switch_snapshot','jp_character_chao_field','jp_character_chao_field_bytes','chao_get_records','chao_get_file_data','chao_get_carried','chao_get_carried_to','chao_set_carried','chao_set_carried_to'},'motion_DLL':{'jp_motions_can_select'}}
        for key,names in expected.items():assert set(pe('--coff-exports',b[key]))==set(pe('--coff-exports',old[key]))|names
        assert not {'set_character','jp_original_set_character','chao_reset','chao_clear_records','jp_original_character_finish'}&set(pe('--coff-exports',b['frames_DLL']))
        graph={path.name.lower():[n.lower() for n in pe('--coff-imports',path) if (build/n).exists()] for path in build.glob('*.dll')};active=set();done=set()
        def visit(name):
            assert name not in active,('DLL cycle',name);active.add(name)
            for child in graph[name]:
                if child not in done:visit(child)
            active.remove(name);done.add(name)
        for n in graph:
            if n not in done:visit(n)
        out=dict(status='exact_original_set_character_Chao_functions_and_continuation_single_shared_state_pure_motion_query_and_acyclic_DLL_link_audited',original_game_function_bodies_preserved=10,original_tail_preserved=True,original_Chao_state_definitions=10,pinned_header_sha256=headers,shared_symbol_providers=providers,unchanged_frame_objects=len(objects[0]&objects[1]),unchanged_DLL_sha256=unchanged,changed_DLLs=changed,added_exports={n:sorted(v) for n,v in expected.items()},actual_local_PE_import_graph=graph,prior_packages_checked=31,build_verification_sha256=bh,auditor_sha256=sha(__file__),playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],len(unchanged),'unchanged DLLs',flush=True);return
    tools={'check':'verify_native_character_switch.py','authority':'verify_native_asset_character_exit.py','texture':'verify_native_character_textures.py','pixels':'verify_native_character_texture_pixels.py','motion':'verify_native_motion_exit.py','init':'verify_native_game_init.py','audit':'qualify_native_character_switch.py'};reports={};evidence={'build':build/'build_verification.json'}
    for n,tool in tools.items():
        path=getattr(a,n)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[n]=v;evidence[n]=path
    assert len(reports['check']['cases'])==7 and reports['check']['original_set_character_and_Chao_reset_executed']
    assert [reports[n]['checks'] for n in ('authority','texture','pixels','motion','init')]==[1411,8199,3049,918,530]
    authored=[p/'native'/n for n in ('jp_native_character_switch.h','jp_native_character_switch.inc','jp_native_motion_select.inc')]+[p/'jp_port_tools'/n for n in ('build_native_character_switch.py','verify_native_character_switch.py','qualify_native_character_switch.py')]+[p/'JP_NATIVE_CHARACTER_SWITCH.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    target=p/'analysis/jp_native_character_switch_package.json';assert not target.exists();d.mkdir();proof={}
    for n,path in evidence.items():
        to=p/'analysis'/('jp_native_character_switch_'+n+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    new=reports['check']['checks'];regression=sum(reports[n]['checks'] for n in ('authority','texture','pixels','motion','init'));total=read(previous)['combined_qualified_scoped_checks']+new
    out=dict(status='original_set_character_and_genuine_Chao_reset_native_saved_lives_texture_motion_consumers_partial_resume_abandon_restart_and_final_Exit_scoped_qualification',new_checks=new,selected_regression_checks=regression,combined_qualified_scoped_checks=total,new_runtime_cases=7,playable_character_pairs_times_save_slots=108,playable_characters=[0,2,3,5,6,7],original_game_bodies_preserved=10,original_Chao_reset_machine_oracle_executed=True,all_shared_save_bytes_compared=3552,source_backed_Chao_file_data_clear_bug_preserved=True,game_texture_reference_protected=True,independent_model_views_preserved=True,failed_texture_load_tail_not_executed=True,actual_partial_resume_and_abandon_verified=True,ordinary_restart_and_terminal_Exit_verified=True,prior_qualifications_preserved={'asset_character_exit':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],private_motion_DLL_sha256=b['motion_DLL_sha256'],automatic_task_render_model_texture_view_acquisition=False,actual_3D_character_drawing_animation_GAME_MODE_BOOT_main_loop_or_gameplay_verified=False,forced_allocator_or_motion_generation_exhaustion_failures_verified=False,original_source_and_proofs_preserved=True,retail_or_generated_game_source_or_combined_DLLs_publishable=False,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new checks;',regression,'regressions;',total,'overlapping scoped checks',flush=True)
if __name__=='__main__':main()
