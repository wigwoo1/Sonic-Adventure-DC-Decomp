"""Freeze scoped real history qualification without publishing game/SDK binaries."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','regression','audit','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(a.build/'build_verification.json');bh=sha(a.build/'build_verification.json');reports={}
    evidence={'build':a.build/'build_verification.json'}
    for n,tool in (('check','verify_native_history.py'),('regression','verify_native_flash_manager.py'),('audit','audit_native_history.py')):
        path=getattr(a,n)/'verification.json';value=read(path)
        assert value['build_verification_sha256']==bh
        assert value['auditor_sha256' if n=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool)
        reports[n]=value;evidence[n]=path
    assert reports['check']['checks']==922 and len(reports['check']['cases'])==14
    assert reports['regression']['checks']==739
    assert len(reports['audit']['unchanged_original_functions'])==32
    assert len(reports['audit']['explicitly_adapted_original_functions'])==19
    assert len(reports['audit']['unchanged_prior_DLLs'])==16 and reports['audit']['prior_qualification_packages_checked']==19
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ('flash_DLL','history_DLL'):assert sha(b[n])==b[n+'_sha256']
    old=p/'analysis/jp_native_flash_manager_package.json';seen=set();prior(r,old,seen)
    assert read(old)['combined_qualified_scoped_checks']==941811
    authored=[p/'native'/n for n in ('jp_native_flash_history.h','jp_native_flash_history.inc',
        'jp_native_history.h','jp_native_history.c','jp_native_history_source.inc')]
    authored+=[p/'jp_port_tools'/n for n in ('build_native_history.py','verify_native_history.py',
        'audit_native_history.py','package_native_history.py')]
    authored+=[p/'JP_NATIVE_HISTORY.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    targets={n:p/'analysis'/('jp_native_history_'+n+'.json') for n in evidence}
    package=p/'analysis/jp_native_history_package.json';assert not package.exists() and all(not x.exists() for x in targets.values())
    d.mkdir();proof={}
    for n,target in targets.items():shutil.copyfile(evidence[n],target);proof[target.relative_to(r).as_posix()]=sha(target)
    result=dict(status='original_game_and_UF_history_native_owned_transactional_profile_scoped_qualification',
        new_checks=922,selected_regression_checks=739,combined_qualified_scoped_checks=942733,
        unchanged_original_function_bodies=32,explicitly_adapted_original_functions=19,
        actual_original_history_initialization_save_read_counters_and_safe_refresh_executed=True,
        actual_RTC_peripheral_flash_providers=True,real_OS_denial_rolls_back_whole_history_transaction=True,
        failed_cleanup_retains_live_UF_profile_and_refresh_work=True,high_native_pointer_UF_flags_copy_verified=True,
        dirty_history_is_saved_before_original_read_reinitializes_UF=True,
        Windows_clock_and_controlled_clock_cases_both_verified=True,
        original_source_and_previous_proofs_preserved=True,build_verification_sha256=bh,
        private_DLL_sha256={n:b[n+'_sha256'] for n in ('history_DLL','flash_DLL')},
        prior_qualifications_preserved={'flash_manager':dict(path=old.relative_to(r).as_posix(),sha256=sha(old))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        global_history_exit_handoff_or_full_game_init_verified=False,per_launch_BIOS_history_accounting_implemented=False,
        public_research_build_payloads_changed=False,combined_game_SDK_binaries_publishable=False,
        retail_assets_or_user_profiles_packaged=False,playable=False,overall_completion_percent=10,goal_active=True)
    out=d/'qualification.json';out.write_text(json.dumps(result,indent=2)+'\n');shutil.copyfile(out,package)
    print(result['status'],922,'new checks;',739,'regression checks;',942733,'combined reported scopes',flush=True)
if __name__=='__main__':main()
