"""Check full-parent coexistence and freeze genuine native motion ownership."""
import argparse,ast,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
from build_native_texture_runtime import function
from package_native_sound_placement import prior
from qualify_native_boot_reset import defined_names
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('parent','audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('upstream','baseline','core','track','check','regression','parent','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_motion_data_package.json';seen=set();prior(r,previous,seen);assert len(seen)==27
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_source_sha256'].items():assert sha(build/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256']
    if a.stage=='parent':
        core=a.core.resolve();cb=read(core/'build_verification.json');assert sha(core/'build_verification.json')==b['baseline_verification_sha256']
        d.mkdir();view=d/'parent_view';view.mkdir();combined=dict(cb,motion_store_DLL=b['DLL'],motion_store_DLL_sha256=b['DLL_sha256'],motion_store_verification_sha256=sha(build/'build_verification.json'))
        combined['authored_source_sha256']=dict(cb['authored_source_sha256'],**b['authored_source_sha256']);(view/'build_verification.json').write_text(json.dumps(combined,indent=2)+'\n')
        original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1
        added=b'''        if a.case=='data':
            from verify_native_motion_store import Info as MotionInfo,View as MotionView
            motions=C.CDLL(b['motion_store_DLL'])
            start_motion=bind(motions,'jp_motions_initialize',I,S,U)
            choose_motion=bind(motions,'jp_motions_select',I,I)
            acquire_motion=bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)
            release_motion=bind(motions,'jp_motions_release',I,Q)
            close_motion=bind(motions,'jp_motions_shutdown',I)
            before=si()
            check(start_motion(8<<20,8)==0 and choose_motion(0)==0,'actual original character motion selection after complete game_init')
            action=MotionView();model=MotionView()
            check(acquire_motion(0,b'sonic_updraft_action',C.byref(action))==0 and acquire_motion(7,b'sonic_object',C.byref(model))==0 and action.data>0xffffffff and model.data>0xffffffff,'real original action and canonical model views acquired on full parent')
            check(choose_motion(2)==7 and close_motion()==7,'full-parent live views prevent bank reload and retirement')
            presented=snapshot(present,'jp_present_snapshot',PresentInfo).presentations
            bind(present,'frame_wait',None)();bind(present,'frame_wait',None)()
            check(snapshot(present,'jp_present_snapshot',PresentInfo).presentations==presented+2,'real frame clock and native presentations progress while views remain live')
            check(release_motion(action.token)==0 and choose_motion(2)==0,'actual same-bank reload after action lease release preserves independent model view')
            check(release_motion(model.token)==0 and close_motion()==0,'caller closes actual motion owner before retiring complete parent')
            actual=MotionInfo();check(bind(motions,'jp_motions_snapshot',I,P)(C.byref(actual))==0 and actual.phase==2 and not actual.allocated_bytes and not actual.live_leases,'actual motion allocations and leases fully retire')
            after=si();current=info()
            check(before.resources==after.resources and before.phase==after.phase and current.audio_stages==15 and current.frame_completed==2,'motion operations preserve actual System audio queue and History ownership')
'''
        caller=d/'private_full_parent_motion_caller.py';caller.write_bytes(raw.replace(marker,added+marker));env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools'))
        cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(view),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','data']
        run=subprocess.run(cmd,capture_output=True,env=env,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr)
        assert run.returncode==0,(run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6500:]);result=read(d/'run/result.json')
        out=dict(status='checked_motion_owner_views_reload_and_explicit_retirement_after_complete_game_init_verified',checks=result['checks'],
            original_python_verifier_sha256=sha(original),derived_python_caller_sha256=sha(caller),parent_view_sha256=sha(view/'build_verification.json'),
            actual_motion_DLL_sha256=b['DLL_sha256'],native_game_or_SDK_functions_replaced=False,actual_additional_presentations=2,
            subsequent_real_JP_file_read_History_write_and_parent_cleanup_verified=True,automatic_System_Exit_motion_handoff_verified=False,
            worker_result_sha256=sha(d/'run/result.json'),build_verification_sha256=sha(build/'build_verification.json'),verifier_sha256=sha(__file__),
            animation_playback_or_full_set_character_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['motion_store_baseline_sha256']
        source=a.upstream/'src/system/loader.c';assert sha(source)==b['motion_store_original_source_sha256']
        original=function(source.read_bytes(),'loader_read_character_motions');native=function((build/'external_motion_selector.c').read_bytes(),'loader_read_character_motions')
        assert hashlib.sha256(original).hexdigest()==b['motion_store_original_body_sha256'] and hashlib.sha256(native).hexdigest()==b['motion_store_native_body_sha256']
        assert native==original.replace(b'        for (;;) {\n        }',b'        jp_motion_bad_character(character);\n        return;')
        kept={}
        for n,h in b['motion_store_kept_objects_sha256'].items():assert sha(base/n)==sha(build/n)==h;kept[n]=h
        assert len(kept)==9 and b['motion_store_new_build_commands']==3 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'][:3])
        used=[x for x in b['commands'][2]['command'] if x.endswith('.obj')]
        assert len(used)==11 and {Path(x).name for x in used}==set(kept)|{'motion_selector_original.obj','motion_store_owner.obj'}
        definitions={}
        for obj in used:
            for n in defined_names(obj):definitions.setdefault(n,[]).append(Path(obj).name)
        for n in ('jp_original_motion_select','jp_motion_source_load','jp_motion_bad_character'):assert len(definitions[n])==1
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        def pe(flag,path):
            run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr
            return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
        original_exports=set(pe('--coff-exports',old['DLL']));current_exports=set(pe('--coff-exports',b['DLL']))
        added={'jp_motions_'+n for n in ('initialize','select','reset_board','acquire','release','shutdown','snapshot')}
        assert current_exports==original_exports|added and len(original_exports)==17
        assert not {'loader_read_character_motions','jp_original_motion_select','jp_motion_source_load','jp_motion_bad_character'}&current_exports
        imports=pe('--coff-imports',b['DLL']);assert not any(n.lower().startswith('jp_native_') for n in imports)
        out=dict(status='original_character_switch_preserved_checked_native_ownership_and_single_data_module_audited',original_valid_selector_body_unchanged=True,
            original_infinite_invalid_character_loop_replaced_with_checked_failure=True,source_area_tag_never_dereferenced=True,
            unchanged_prior_data_objects_sha256=kept,actual_PE_imports=imports,retained_diagnostic_data_exports=17,new_checked_exports=sorted(added),
            unchecked_original_void_loader_not_exported=True,prior_packages_checked=len(seen),automatic_System_Exit_motion_handoff_verified=False,
            build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],'9 unchanged data objects;7 checked APIs',flush=True);return
    reports={};evidence={'build':build/'build_verification.json'}
    for n,tool in (('check','verify_native_motion_store.py'),('regression','verify_native_motion_data.py'),('parent','qualify_native_motion_store.py'),('audit','qualify_native_motion_store.py')):
        path=getattr(a,n)/'verification.json';v=read(path);assert v['build_verification_sha256']==sha(build/'build_verification.json')
        assert v['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[n]=v;evidence[n]=path
    assert reports['check']['checks']==23385 and reports['check']['all_native_record_bindings_checked']==11650 and reports['regression']['checks']==46684
    assert reports['parent']['checks']==47 and reports['parent']['actual_motion_DLL_sha256']==b['DLL_sha256']
    authored=[p/'native'/n for n in ('jp_native_motion_store.h','jp_native_motion_store.c')]+[p/'jp_port_tools'/n for n in ('build_native_motion_store.py','verify_native_motion_store.py','qualify_native_motion_store.py')]+[p/'JP_NATIVE_MOTION_STORE.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    package=p/'analysis/jp_native_motion_store_package.json';targets={n:p/'analysis'/('jp_native_motion_store_'+n+'.json') for n in evidence}
    assert not package.exists() and all(not x.exists() for x in targets.values());d.mkdir();proof={}
    for n,target in targets.items():shutil.copyfile(evidence[n],target);proof[target.relative_to(r).as_posix()]=sha(target)
    new=reports['check']['checks']+reports['parent']['checks'];old_total=read(previous)['combined_qualified_scoped_checks']
    out=dict(status='original_character_motion_selection_native_reload_and_consumer_view_lifetimes_scoped_qualification',new_checks=new,
        selected_regression_checks=46684,combined_qualified_scoped_checks=old_total+new,all_native_record_bindings_checked=11650,
        original_machine_selection_paths=6,original_machine_trace_stops_before_actual_disc_load=True,native_original_valid_switch_executes=True,
        actual_bank_defaults_restored_and_live_views_protected=True,canonical_shared_models_preserved_across_selection=True,
        independent_board_areas_and_fresh_session_tokens_verified=True,complete_parent_with_explicit_motion_cleanup_verified=True,
        prior_qualifications_preserved={'motion_data':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        private_motion_DLL_sha256=b['DLL_sha256'],build_verification_sha256=sha(build/'build_verification.json'),
        automatic_System_Exit_or_renderer_job_motion_ownership_verified=False,full_set_character_or_animation_playback_or_gameplay_verified=False,
        allocator_failure_counter_exhaustion_or_reentrant_callback_forced=False,
        original_game_data_or_combined_DLL_publishable=False,retail_assets_profiles_or_captures_packaged=False,public_research_build_payloads_changed=False,
        playable=False,overall_completion_percent=10,goal_active=True)
    path=d/'qualification.json';path.write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(path,package)
    print(out['status'],new,'new checks;',old_total+new,'combined reported scopes',flush=True)
if __name__=='__main__':main()
