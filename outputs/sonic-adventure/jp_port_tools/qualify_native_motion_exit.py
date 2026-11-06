"""Audit/freeze motion authority integrated into the existing native parents."""
import argparse,ast,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('core','motion','check','owner','data','regression','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json');bh=sha(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_motion_store_package.json';seen=set();prior(r,previous,seen);assert len(seen)==28
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['motion_exit_generated_source_sha256'].items():assert sha(build/n)==h,n
    for n,v in b.items():
        if (n=='DLL' or n.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):assert sha(v)==b[n+'_sha256'],n
    assert sha(Path(b['motion_data_view'])/'build_verification.json')==b['motion_data_view_sha256']
    if a.stage=='audit':
        core,motion=a.core.resolve(),a.motion.resolve();cb=read(core/'build_verification.json');mb=read(motion/'build_verification.json')
        assert sha(core/'build_verification.json')==b['motion_exit_core_baseline_sha256'] and sha(motion/'build_verification.json')==b['motion_exit_data_baseline_sha256']
        assert b['motion_exit_new_build_commands']==8 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'][:8])
        changed={'jp_native_system.dll','jp_native_exit.dll','jp_native_main_boot.dll'};kept={}
        for path in core.glob('*.dll'):
            current=build/path.name
            if path.name in changed:assert sha(path)!=sha(current)
            else:assert sha(path)==sha(current);kept[path.name]=sha(path)
        assert len(kept)==20
        data_objects={}
        for name in read(p/'analysis/jp_native_motion_store_audit.json')['unchanged_prior_data_objects_sha256']:
            assert sha(motion/name)==sha(build/name);data_objects[name]=sha(build/name)
        assert len(data_objects)==9 and sha(motion/'motion_selector_original.obj')==sha(build/'motion_selector_original.obj')
        shared=['bootstrap_cleanup'];preserved={}
        for fn in shared:
            left=owner_function((core/'system_with_game_init.c').read_bytes(),fn);right=owner_function((build/'system_with_motions.c').read_bytes(),fn)
            assert left==right;preserved[fn]=hashlib.sha256(right).hexdigest()
        for fn in ('rollback_claims','jp_exit_prepare_bootstrap','jp_exit_cancel_bootstrap','syBtExit'):
            left=owner_function((core/'exit_with_read_queue.c').read_bytes(),fn);right=owner_function((build/'exit_with_motions.c').read_bytes(),fn)
            assert left==right;preserved[fn]=hashlib.sha256(right).hexdigest()
        # Exactly the prior registration sites remain. Attachment extends their
        # resource mask instead of adding a second cleanup callback.
        left=(core/'system_with_game_init.c').read_bytes();right=(build/'system_with_motions.c').read_bytes()
        assert left.count(b'jp_exit_prepare_bootstrap(')==right.count(b'jp_exit_prepare_bootstrap(')==1
        assert left.count(b'jp_exit_cancel_bootstrap(')==right.count(b'jp_exit_cancel_bootstrap(')
        assert right.index(b'CLOSE(JP_EXIT_MOTIONS,')<right.index(b'CLOSE(JP_EXIT_CARD,')
        raw=(build/'exit_with_motions.c').read_bytes();assert raw.index(b'CLOSE_INT(JP_EXIT_MOTIONS,')<raw.index(b'CLOSE_INT(JP_EXIT_CARD,')
        reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        def pe(flag,path):
            run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr
            return re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)
        graph={}
        for path in build.glob('*.dll'):graph[path.name.lower()]=[n.lower() for n in pe('--coff-imports',path) if (build/n).exists()]
        active=set();done=set()
        def visit(name):
            assert name not in active,('DLL cycle',name);active.add(name)
            for child in graph[name]:
                if child not in done:visit(child)
            active.remove(name);done.add(name)
        for name in graph:
            if name not in done:visit(name)
        assert not graph['jp_native_motion_data.dll']
        assert 'jp_native_motion_data.dll' in graph['jp_native_exit.dll'] and 'jp_native_system.dll' not in graph['jp_native_exit.dll']
        additions={
            'motion_DLL':{'jp_motion_exit_'+n for n in ('activity','snapshot','claim','cancel','can_close','can_retire','close','finish')},
            'System_DLL':{'jp_system_start_motions','jp_system_adopt_motions','jp_system_motion_cleanup_activity','jp_system_prepare_boot_shutdown'},
            'authored_exit_DLL':{'jp_exit_attach_bootstrap_motions'},'main_boot_DLL':set()}
        for key,names in additions.items():
            old=mb['DLL'] if key=='motion_DLL' else cb[key]
            assert set(pe('--coff-exports',b[key]))==set(pe('--coff-exports',old))|names,key
        assert not {'loader_read_character_motions','jp_original_motion_select','jp_motion_source_load','jp_motion_bad_character'}&set(pe('--coff-exports',b['motion_DLL']))
        out=dict(status='same_owners_single_bootstrap_motion_first_shutdown_unchanged_original_selector_and_acyclic_DLL_imports_audited',
            unchanged_core_DLL_sha256=kept,unchanged_data_object_sha256=data_objects,unchanged_motion_selector_object_sha256=sha(build/'motion_selector_original.obj'),
            unchanged_parent_function_sha256=preserved,actual_local_DLL_import_graph=graph,additional_exports={k:sorted(v) for k,v in additions.items()},
            single_existing_System_bootstrap_registration=True,original_valid_selector_and_data_objects_unchanged=True,prior_packages_checked=len(seen),
            build_verification_sha256=bh,auditor_sha256=sha(__file__),playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],'20 unchanged core DLLs;9 data objects',flush=True);return
    reports={};evidence={'build':build/'build_verification.json','motion_view':Path(b['motion_data_view'])/'build_verification.json'}
    tools={'check':'verify_native_motion_exit.py','owner':'verify_native_motion_store.py','data':'verify_native_motion_data.py','regression':'verify_native_game_init.py','audit':'qualify_native_motion_exit.py'}
    for name,tool in tools.items():
        path=getattr(a,name)/'verification.json';v=read(path);expected=b['motion_data_view_sha256'] if name in ('owner','data') else bh
        assert v['build_verification_sha256']==expected and v['auditor_sha256' if name=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool)
        reports[name]=v;evidence[name]=path
    assert [reports[n]['checks'] for n in ('check','owner','data','regression')]==[918,23385,46684,530]
    assert len(reports['check']['cases'])==14 and reports['check']['actual_process_exit_codes']==[0,70]
    assert reports['owner']['all_native_record_bindings_checked']==reports['data']['native_definitions']==11650
    authored=[p/'native'/n for n in ('jp_native_motion_exit.h','jp_native_motion_exit.inc','jp_native_system_motion.h','jp_native_system_motion.inc','jp_native_exit_motion.h','jp_native_exit_motion.inc')]
    authored += [p/'jp_port_tools'/n for n in ('build_native_motion_exit.py','verify_native_motion_exit.py','qualify_native_motion_exit.py')]+[p/'JP_NATIVE_MOTION_EXIT.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    target=p/'analysis/jp_native_motion_exit_package.json';assert not target.exists();d.mkdir();proof={}
    for name,source in evidence.items():
        path=p/'analysis'/('jp_native_motion_exit_'+name+'.json');assert not path.exists();shutil.copyfile(source,path);proof[path.relative_to(r).as_posix()]=sha(path)
    new=reports['check']['checks'];regression=sum(reports[n]['checks'] for n in ('owner','data','regression'));total=read(previous)['combined_qualified_scoped_checks']+new
    out=dict(status='actual_motion_owner_same_System_MainBoot_bootstrap_final_Exit_and_live_consumer_retirement_scoped_qualification',new_checks=new,selected_regression_checks=regression,
        combined_qualified_scoped_checks=total,actual_process_cases=14,actual_process_exit_codes=[0,70],live_views_block_all_parent_cleanup_before_resource_mutation=True,
        handoff_with_live_views_verified=True,actual_card_completion_after_motion_retirement_verified=True,actual_History_failure_recovery_and_restart_verified=True,
        unchanged_core_DLLs=20,unchanged_motion_data_objects=9,unchanged_original_selector_object=True,single_existing_System_bootstrap=True,acyclic_actual_PE_imports=True,
        prior_qualifications_preserved={'motion_store':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},
        proof_files_sha256=proof,build_verification_sha256=bh,private_motion_DLL_sha256=b['motion_DLL_sha256'],
        task_renderer_or_texture_view_acquisition_automatic=False,full_set_character_animation_GAME_MODE_BOOT_or_gameplay_verified=False,
        forced_allocator_failure_token_exhaustion_or_rollback_failure_verified=False,original_data_C_or_combined_game_SDK_DLL_publishable=False,
        retail_assets_profiles_or_private_fixtures_packaged=False,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new checks;',regression,'regressions;',total,'overlapping scoped checks',flush=True)
if __name__=='__main__':main()
