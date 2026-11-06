"""Freeze the complete-parent scope without publishing game/SDK binaries."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
from package_native_sound_placement import prior
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','ownership','audit','regression','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(a.build/'build_verification.json');bh=sha(a.build/'build_verification.json');reports={};evidence={'build':a.build/'build_verification.json'}
    for name,tool in (('check','verify_native_main_boot.py'),('ownership','verify_native_main_boot_ownership.py'),('audit','audit_native_main_boot_parent.py'),('regression','verify_native_sound_placement.py')):
        path=getattr(a,name)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh
        assert v['auditor_sha256' if name=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool)
        reports[name]=v;evidence[name]=path
    assert reports['check']['checks']==458 and len(reports['check']['cases'])==12
    assert reports['check']['cases']['fatal_driver']['actual_exit_code']==72
    assert reports['check']['cases']['failure_retry']['observations']['initialize_result']==5
    assert reports['check']['cases']['late_texture_failure']['observations']['initialize_result']==6
    assert reports['ownership']['checks']==293 and set(reports['ownership']['cases'])=={'preexisting_card','preexisting_system','preexisting_texture'}
    assert reports['regression']['checks']==35
    assert len(reports['audit']['unchanged_prior_binaries_and_objects'])==59 and reports['audit']['prior_qualification_packages_checked']==16
    assert reports['audit']['nonlocal_stack_unwinding_used'] is False and b['main_boot_original_body_unchanged'] is False
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['main_boot_DLL'])==b['main_boot_DLL_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:3])
    old=p/'analysis/jp_native_sound_placement_package.json';seen=set();prior(r,old,seen);assert read(old)['combined_qualified_scoped_checks']==939584
    authored=[p/'native/jp_native_main_boot.c',p/'native/jp_native_main_boot.h']+[p/'jp_port_tools'/n for n in ('build_native_main_boot.py','verify_native_main_boot.py','verify_native_main_boot_ownership.py','audit_native_main_boot_parent.py','package_native_main_boot.py')]+[p/'JP_NATIVE_MAIN_BOOT.txt',p/'JP_NATIVE_MAIN_BOOT_DRAFT.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    d.mkdir();proof={}
    for name,source in evidence.items():
        target=p/'analysis'/('jp_native_main_boot_'+name+'.json');assert not target.exists();shutil.copyfile(source,target);proof[target.relative_to(r).as_posix()]=sha(target)
    result=dict(status='complete_native_higher_bootstrap_checked_sound_recovery_owned_local_cleanup_and_foreign_owner_preservation_scoped_qualification',new_checks=751,constructor_runtime_checks=458,ownership_conflict_checks=293,selected_regression_checks=35,combined_qualified_scoped_checks=940335,complete_original_successful_constructor_call_order_retained=True,one_checked_return_native_parent_adaptation=True,nonlocal_stack_unwinding_used=False,real_firmware_clock_GPU_font_VMU_card_and_peripheral_paths_verified=True,preexisting_card_System_and_texture_owners_preserved=True,late_font_cleanup_retains_failure_authority_and_retries=True,terminal_global_card_exit_handoff_verified=False,full_game_init_or_stage_gameplay_verified=False,build_verification_sha256=bh,private_parent_DLL_sha256=b['main_boot_DLL_sha256'],prior_qualifications_preserved={'sound_placement':dict(path=old.relative_to(r).as_posix(),sha256=sha(old))},authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,original_source_and_previous_proofs_preserved=True,combined_game_SDK_binaries_publishable=False,retail_assets_or_captures_packaged=False,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    q=d/'qualification.json';q.write_text(json.dumps(result,indent=2)+'\n');target=p/'analysis/jp_native_main_boot_package.json';assert not target.exists();shutil.copyfile(q,target)
    print(result['status'],751,'new checks;',35,'regression checks;',940335,'combined qualified scopes',flush=True)
if __name__=='__main__':main()
