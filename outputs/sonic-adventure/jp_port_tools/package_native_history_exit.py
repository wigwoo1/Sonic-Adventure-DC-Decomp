"""Freeze the History/System/Exit scope without changing prior qualifications."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
from package_native_sound_placement import prior

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','history','flash','boot','audit','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(a.build/'build_verification.json');bh=sha(a.build/'build_verification.json')
    evidence={'build':a.build/'build_verification.json'};reports={}
    tools={'check':'verify_native_history_exit.py','history':'verify_native_history.py',
        'flash':'verify_native_flash_manager.py','boot':'verify_native_boot_exit.py','audit':'audit_native_history_exit.py'}
    for n,tool in tools.items():
        path=getattr(a,n)/'verification.json';v=read(path)
        assert v['build_verification_sha256']==bh
        assert v['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool)
        reports[n]=v;evidence[n]=path
    assert reports['check']['checks']==746 and len(reports['check']['cases'])==20
    assert reports['check']['cases']['process_exit']['actual_exit_code']==0
    assert reports['check']['cases']['process_failure']['actual_exit_code']==70
    assert all(reports['check']['cases'][n]['independent_post_process_disk_check'] for n in ('process_exit','process_failure'))
    assert [reports[n]['checks'] for n in ('history','flash','boot')]==[922,739,737]
    assert len(reports['audit']['unchanged_prior_DLLs'])==15 and len(reports['audit']['unchanged_prior_objects'])==17
    assert reports['audit']['prior_qualification_packages_checked']==20
    assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:6])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ('history_DLL','System_DLL','authored_exit_DLL'):assert sha(b[n])==b[n+'_sha256']
    previous=p/'analysis/jp_native_history_package.json';seen=set();prior(r,previous,seen)
    assert read(previous)['combined_qualified_scoped_checks']==942733
    authored=[p/'native'/n for n in ('jp_native_history_exit.h','jp_native_history_exit.inc',
        'jp_native_system_history.h','jp_native_system_history.inc','jp_native_exit_history.h','jp_native_exit_history.inc')]
    authored+=[p/'jp_port_tools'/n for n in ('build_native_history_exit.py','verify_native_history_exit.py','audit_native_history_exit.py','package_native_history_exit.py')]
    authored+=[p/'JP_NATIVE_HISTORY_EXIT.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    targets={n:p/'analysis'/('jp_native_history_exit_'+n+'.json') for n in evidence}
    package=p/'analysis/jp_native_history_exit_package.json'
    assert not package.exists() and all(not x.exists() for x in targets.values())
    d.mkdir();proof={}
    for n,target in targets.items():shutil.copyfile(evidence[n],target);proof[target.relative_to(r).as_posix()]=sha(target)
    out=dict(status='actual_History_same_System_bootstrap_and_ordinary_early_final_Exit_scoped_qualification',
        new_checks=746,selected_regression_checks=2398,combined_qualified_scoped_checks=943479,
        cases=20,actual_original_full_parent_constructor_executed=True,full_game_init_executed=False,
        same_System_registration_and_existing_callback=True,actual_card_completion_before_history_retirement=True,
        independent_complete_VMU_FAT_and_payload_verified=True,final_elapsed_time_after_explicit_save_verified=True,
        failed_profile_commit_retains_UF_work_profile_authority_and_dependent_services=True,
        actual_process_exit_and_denied_commit_exit_codes_verified=True,
        original_source_and_previous_proofs_preserved=True,build_verification_sha256=bh,
        private_DLL_sha256={n:b[n+'_sha256'] for n in ('history_DLL','System_DLL','authored_exit_DLL')},
        prior_qualifications_preserved={'history':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        per_launch_BIOS_history_accounting_implemented=False,exhausted_claim_counter_or_failed_rollback_cancellation_verified=False,
        public_research_build_payloads_changed=False,combined_game_SDK_binaries_publishable=False,
        retail_assets_or_user_profiles_packaged=False,playable=False,overall_completion_percent=10,goal_active=True)
    path=d/'qualification.json';path.write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(path,package)
    print(out['status'],746,'new checks;',2398,'regressions;',943479,'combined reported scopes',flush=True)
if __name__=='__main__':main()
