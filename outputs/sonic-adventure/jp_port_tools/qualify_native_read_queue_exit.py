"""Audit and freeze read-queue authority in the existing System/Exit graph."""
import argparse,ast,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    for n in ('baseline','check','queue','history','audit'):ap.add_argument('--'+n,type=Path)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve();b=read(build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['read_queue_exit_generated_source_sha256'].items():assert sha(build/n)==h,n
    if a.stage=='audit':
        base=a.baseline.resolve();old=read(base/'build_verification.json');assert sha(base/'build_verification.json')==b['read_queue_exit_baseline_sha256']
        originals={'queue':p/'native/jp_native_read_queue.c','System':base/'system_with_history_adoption_guard.c','Exit':base/'exit_with_history.c'}
        assert {k:sha(v) for k,v in originals.items()}==b['read_queue_exit_prior_owner_sha256']
        system=(build/'system_with_read_queue.c').read_bytes();exit=(build/'exit_with_read_queue.c').read_bytes();queue=(build/'read_queue_with_exit.c').read_bytes()
        assert owner_function(system,'bootstrap_cleanup')==owner_function(originals['System'].read_bytes(),'bootstrap_cleanup')
        assert system.count(b'rc=jp_exit_prepare_bootstrap(')==1
        for raw,name,prefix in ((system,'shutdown_owned',b'CLOSE('),(exit,'jp_exit_cleanup',b'CLOSE_INT(')):
            body=owner_function(raw,name);order=[body.index(prefix+bit) for bit in (b'JP_EXIT_CARD,',b'JP_EXIT_READ_QUEUE,',b'JP_EXIT_HISTORY,',b'JP_EXIT_SOUND,',b'JP_EXIT_GDFS,',b'JP_EXIT_RTC,')]
            assert order==sorted(order)
        close=owner_function(queue,'read_queue_exit_close');assert close.index(b'jp_read_queue_exit_can_close(')<close.index(b'queue.exit_terminal=1')<close.index(b'jp_read_queue_shutdown()')
        assert b'(queue.exit_closing||queue.exit_terminal)&&!queue.exit_call' in owner_function(queue,'enter')
        assert b'(queue.exit_token||queue.exit_terminal)&&!queue.exit_call' in owner_function(queue,'jp_read_queue_shutdown')
        assert b'jp_gdfs_read_queue_enter(queue.info.lease)' in owner_function(queue,'jp_read_queue_exit_activity')
        assert b'jp_boot_lifetime_can_close(JP_BOOT_CARD,card.token)' in owner_function(queue,'jp_read_queue_exit_activity')
        adopt=owner_function(system,'system_read_queue_adopt');assert adopt.index(b'claim.token||claim.terminal')<adopt.index(b'system_owner.info.resources|=')<adopt.index(b'jp_read_queue_exit_claim(')
        start=owner_function(system,'jp_system_start_read_queue');assert start.index(b'old.initialized')<start.index(b'jp_read_queue_initialize()')<start.index(b'system_read_queue_adopt()')
        shutdown=owner_function(system,'jp_system_shutdown');assert shutdown.index(b'system_read_queue_activity()')<shutdown.index(b'jp_system_main_initialization_guard(0)')<shutdown.index(b'system_owner.busy=1')
        attach=owner_function(exit,'jp_exit_attach_bootstrap_read_queue');assert b'bootstrap.cleanup!=cleanup||bootstrap.context!=context' in attach
        assert b'claim.token!=token||claim.terminal||claim.closing' in attach
        bind=owner_function(exit,'jp_exit_bind');assert bind.index(b'!bootstrap.read_queue_token')<bind.index(b'jp_sdk_heap_exit_claim(')<bind.index(b'jp_device_exit_claim(')<bind.index(b'state.read_queue_token=read_queue_token')
        cleanup=owner_function(exit,'jp_exit_cleanup');assert cleanup.index(b'boot_exit_activity()')<cleanup.index(b'state.busy=1')<cleanup.index(b'bootstrap.started=1')
        assert owner_function(exit,'rollback_claims')==owner_function(originals['Exit'].read_bytes(),'rollback_claims')
        changed={'read_queue_DLL','System_DLL','authored_exit_DLL'};preserved={};imports={};reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
        for k,v in old.items():
            if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
                assert sha(b[k])==b[k+'_sha256'];name=Path(v).name.lower()
                if k not in changed:assert sha(v)==sha(b[k]);preserved[name]=sha(v)
                else:assert sha(v)!=sha(b[k])
                if name not in imports:
                    run=subprocess.run([str(reader),'--coff-imports',b[k]],capture_output=True);assert not run.returncode and not run.stderr
                    imports[name]=[n.lower() for n in re.findall(r'^  Name: (.+)$',run.stdout.decode(),re.M)]
        assert len(preserved)==16
        assert {n for n in imports['jp_native_read_queue.dll'] if n.startswith('jp_native_')}=={'jp_native_frames.dll','jp_native_gdfs.dll'}
        assert not {'jp_native_system.dll','jp_native_main_boot.dll'}&set(imports['jp_native_exit.dll'])
        assert 'jp_native_read_queue.dll' in imports['jp_native_exit.dll'] and 'jp_native_read_queue.dll' in imports['jp_native_system.dll']
        done=set()
        def visit(n,stack):
            assert n not in stack,('PE import cycle',stack,n)
            if n in done:return
            for dep in imports[n]:
                if dep in imports:visit(dep,stack+[n])
            done.add(n)
        for n in imports:visit(n,[])
        objects={}
        for path in base.glob('*.obj'):assert sha(path)==sha(build/path.name);objects[path.name]=sha(path)
        assert len(objects)==31 and sha(base/'external_read_queue.c')==sha(build/'external_read_queue.c')
        for key in ('read_queue_DLL','System_DLL','authored_exit_DLL'):
            cmd=next(x['command'] for x in b['commands'][:6] if '-shared' in x['command'] and Path(x['command'][-1]).name==Path(b[key]).name)
            used=[Path(x).name for x in cmd if x.endswith('.obj')]
            assert len(used)==(2 if key=='read_queue_DLL' else 1),used
        assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:6])
        previous=p/'analysis/jp_native_read_queue_package.json';seen=set();prior(r,previous,seen);assert len(seen)==23
        out=dict(status='same_System_bootstrap_read_queue_claim_order_and_acyclic_native_graph_audited',build_verification_sha256=sha(build/'build_verification.json'),auditor_sha256=sha(__file__),
            original_System_bootstrap_callback_unchanged=True,single_existing_bootstrap_registration=True,
            actual_card_queue_history_sound_GDFS_RTC_order_verified=True,original_queue_object_and_GDFS_library_unchanged=True,
            actual_PE_imports=imports,unchanged_prior_DLLs=preserved,unchanged_prior_objects=objects,prior_packages_checked=len(seen),
            exact_queue_token_transfers_after_all_final_claims_succeed=True,exhausted_tokens_or_failed_rollback_cancellation_verified=False,
            actual_OS_cancel_timeout_forced=False,full_game_init_executed=False,playable=False,overall_completion_percent=10)
        d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],'16 unchanged DLLs;31 unchanged objects;23 preserved packages',flush=True);return
    reports={};evidence={'build':build/'build_verification.json'}
    for n,tool in (('check','verify_native_read_queue_exit.py'),('queue','verify_native_read_queue.py'),('history','qualify_native_history_adoption_guard.py'),('audit','qualify_native_read_queue_exit.py')):
        path=getattr(a,n)/'verification.json';v=read(path);assert v['build_verification_sha256']==sha(build/'build_verification.json')
        assert v['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);reports[n]=v;evidence[n]=path
    assert reports['check']['checks']==968 and len(reports['check']['cases'])==21
    assert [reports[n]['checks'] for n in ('queue','history')]==[479,753]
    assert len(reports['audit']['unchanged_prior_DLLs'])==16 and len(reports['audit']['unchanged_prior_objects'])==31
    for n in ('read_queue_DLL','System_DLL','authored_exit_DLL'):assert sha(b[n])==b[n+'_sha256']
    previous=p/'analysis/jp_native_read_queue_package.json';seen=set();prior(r,previous,seen);assert len(seen)==23
    assert read(previous)['combined_qualified_scoped_checks']==943965
    authored=[p/'native'/n for n in ('jp_native_read_queue_exit.h','jp_native_read_queue_exit.inc','jp_native_system_read_queue.h','jp_native_system_read_queue.inc','jp_native_exit_read_queue.h','jp_native_exit_read_queue.inc')]
    authored+=[p/'jp_port_tools'/n for n in ('build_native_read_queue_exit.py','verify_native_read_queue_exit.py','qualify_native_read_queue_exit.py')]+[p/'JP_NATIVE_READ_QUEUE_EXIT.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    targets={n:p/'analysis'/('jp_native_read_queue_exit_'+n+'.json') for n in evidence};package=p/'analysis/jp_native_read_queue_exit_package.json'
    assert not package.exists() and all(not x.exists() for x in targets.values());d.mkdir();proof={}
    for n,target in targets.items():shutil.copyfile(evidence[n],target);proof[target.relative_to(r).as_posix()]=sha(target)
    out=dict(status='actual_read_queue_same_System_ordinary_early_final_Exit_and_failure_retry_scoped_qualification',new_checks=968,selected_regression_checks=1232,
        combined_qualified_scoped_checks=944933,cases=21,complete_original_higher_bootstrap_executed=True,
        pending_actual_reads_cancel_before_GDFS_RTC_or_caller_buffers_retire=True,actual_card_callback_can_queue_before_queue_close=True,
        actual_missing_GDFS_handler_close_failure_retains_slots_handle_lease_and_dependencies=True,
        actual_same_original_handler_restored_and_cleanup_retry_verified=True,real_later_history_commit_failure_after_queue_close_verified=True,
        post_final_bind_actual_reads_and_ordinary_to_global_cleanup_verified=True,
        exact_original_queue_object_GDFS_library_and_earlier_proofs_preserved=True,build_verification_sha256=sha(build/'build_verification.json'),
        private_DLL_sha256={n:b[n+'_sha256'] for n in ('read_queue_DLL','System_DLL','authored_exit_DLL')},
        prior_qualifications_preserved={'read_queue':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        actual_OS_cancel_timeout_forced=False,exhausted_tokens_or_failed_rollback_cancellation_verified=False,full_game_init_executed=False,
        public_research_build_payloads_changed=False,combined_game_SDK_binaries_publishable=False,retail_assets_or_profiles_packaged=False,
        playable=False,overall_completion_percent=10,goal_active=True)
    path=d/'qualification.json';path.write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(path,package)
    print(out['status'],'968 new checks;1232 regressions;944933 combined reported scopes',flush=True)
if __name__=='__main__':main()
