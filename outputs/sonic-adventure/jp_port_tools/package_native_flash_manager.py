"""Freeze verified native flash evidence; combined original SDK DLL stays private."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
from package_native_sound_placement import prior
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','audit','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(a.build/'build_verification.json');bh=sha(a.build/'build_verification.json')
    check=read(a.check/'verification.json');audit=read(a.audit/'verification.json')
    assert check['build_verification_sha256']==audit['build_verification_sha256']==bh
    assert check['verifier_sha256']==sha(p/'jp_port_tools/verify_native_flash_manager.py')
    assert audit['auditor_sha256']==sha(p/'jp_port_tools/audit_native_flash_manager.py')
    assert check['checks']==739 and len(check['cases'])==11
    assert check['cases']['normal']['observations']['independent_reopen_checks']==21
    assert check['cases']['normal']['observations']['contender_checks']==2
    assert len(audit['unchanged_prior_DLLs'])==16 and audit['prior_qualification_packages_checked']==18
    assert audit['original_FM_exported_algorithms']==9 and audit['original_FM_state_copies']==1
    assert sha(b['flash_DLL'])==b['flash_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    old=p/'analysis/jp_native_boot_lifetimes_package.json';seen=set();prior(r,old,seen)
    assert read(old)['combined_qualified_scoped_checks']==941072
    authored=[p/'native'/n for n in ('jp_native_flash.h','jp_native_flash.c','jp_native_flash_fm.inc')]
    authored+=[p/'jp_port_tools'/n for n in ('build_native_flash_manager.py','verify_native_flash_manager.py',
        'audit_native_flash_manager.py','package_native_flash_manager.py')]
    authored+=[p/'JP_NATIVE_FLASH_MANAGER.txt']
    for path in authored:
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    evidence={'build':a.build/'build_verification.json','check':a.check/'verification.json','audit':a.audit/'verification.json'}
    targets={n:p/'analysis'/('jp_native_flash_manager_'+n+'.json') for n in evidence}
    package=p/'analysis/jp_native_flash_manager_package.json'
    assert not package.exists() and all(not x.exists() for x in targets.values())
    d.mkdir();proof={}
    for n,path in targets.items():shutil.copyfile(evidence[n],path);proof[path.relative_to(r).as_posix()]=sha(path)
    report=dict(status='original_FM_CRC_and_native_durable_flash_scoped_qualification',new_checks=739,
        additional_independent_child_checks=23,combined_qualified_scoped_checks=941811,
        original_FM_and_CRC_translation_unit_bytes_preserved=True,original_FM_owner_copies=1,
        real_OS_denied_write_erase_and_lock_tests=True,independent_image_and_CRC_oracles=True,
        original_async_compaction_and_explicit_drop_verified=True,
        primitive_atomicity_only_not_whole_format_or_UF_transaction=True,
        original_source_and_previous_proofs_preserved=True,
        build_verification_sha256=bh,private_flash_DLL_sha256=b['flash_DLL_sha256'],
        prior_qualifications_preserved={'boot_lifetimes':dict(path=old.relative_to(r).as_posix(),sha256=sha(old))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        public_research_build_payloads_changed=False,combined_game_SDK_binaries_publishable=False,
        retail_flash_or_user_saves_used=False,full_UF_history_or_game_init_executed=False,
        playable=False,overall_completion_percent=10,goal_active=True)
    result=d/'qualification.json';result.write_text(json.dumps(report,indent=2)+'\n');shutil.copyfile(result,package)
    print(report['status'],739,'new checks;',941811,'combined reported scopes',flush=True)
if __name__=='__main__':main()
