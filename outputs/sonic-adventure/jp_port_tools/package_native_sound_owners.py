"""Qualify source/evidence for owned AICA and original sound-pool components."""
import argparse,hashlib,json,re,shutil
from pathlib import Path

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','aica','aica_check','pool','pool_check','upstream','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(w);d.mkdir()
    proof_files=[a.aica/'build_verification.json',a.aica_check/'verification.json',a.pool/'build_verification.json',a.pool_check/'verification.json'];core,cv,pool,pv=map(read,proof_files)
    assert sha(proof_files[0])==cv['build_verification_sha256'] and sha(proof_files[2])==pv['build_verification_sha256']
    assert cv['runtime_checks']==362 and cv['fresh_process_cases']==6 and pv['checks']==292 and pv['original_complete_calls']==74 and not pv['original_instruction_function_hooks_used']
    authored={};private={}
    for folder,meta in ((a.aica,core),(a.pool,pool)):
        assert sha(meta['DLL'])==meta['DLL_sha256']
        for name,h in meta['authored_source_sha256'].items():assert sha(r/name)==h,name;authored[name]=h
        assert all(not x['exit_code'] for x in meta['commands'])
        private[str(folder)]={x.name:sha(x) for x in sorted(folder.iterdir()) if x.suffix in ('.dll','.lib','.obj')}
    assert all(not x['diagnostic_bytes'] for x in pool['commands']) and all(not x['diagnostic_bytes'] for x in core['commands'][2:])
    assert sum(len(re.findall(rb'warning:',(a.aica/f'build_{j}.txt').read_bytes())) for j in (0,1))==10
    assert sha(a.aica/'external_dcsound_owned.c')==core['generated_DCSOUND_sha256'] and sha(a.aica/'external_arm_reset.c')==core['generated_ARM_sha256']
    for name,h in pool['original_source_sha256'].items():assert sha(a.upstream/name)==h,name
    assert sha(p/'jp_port_tools/verify_native_aica.py')==cv['verifier_sha256'] and sha(p/'jp_port_tools/verify_native_sd_memory.py')==pv['verifier_sha256']
    for name in ('verify_native_aica.py','verify_native_sd_memory.py','package_native_sound_owners.py'):
        path=p/'jp_port_tools'/name;authored[path.relative_to(r).as_posix()]=sha(path)
    for name in ('JP_NATIVE_AICA_OWNER.txt','JP_NATIVE_SD_MEMORY.txt'):
        path=p/name;authored[path.relative_to(r).as_posix()]=sha(path)
    prior=read(p/'analysis/jp_aica_probe_package.json')['prior_native_DLLs_preserved']
    for row in prior.values():assert sha(row['path'])==row['sha256']
    result=dict(status='owned_AICA_and_original_sound_pool_source_and_runtime_evidence_qualified',runtime_checks=654,complete_original_Japanese_calls=74,
        authored_source_sha256=authored,proof_files_sha256={x.resolve().relative_to(r).as_posix():sha(x) for x in proof_files},private_artifact_sha256=private,prior_native_DLLs_preserved=prior,
        original_SDK_or_game_bootstrap_initialized=False,original_repeated_sdDrvInit_executed=False,pool_release_and_reuse_alias_proved=True,ARM_reset_masks_IRQ_and_FIQ=True,
        physical_console_waveform_or_cycle_accuracy_verified=False,public_artifacts_selected=False,artifact_publishable=False,retail_assets_published=False,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    for source,name in ((proof_files[0],'jp_native_aica_build.json'),(proof_files[1],'jp_native_aica_owner.json'),(proof_files[2],'jp_native_sd_memory_build.json'),(proof_files[3],'jp_native_sd_memory.json'),(d/'qualification.json','jp_native_sound_owners_package.json')):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],654,'runtime checks,74 original calls,preserved DLLs',len(prior),flush=True)

if __name__=='__main__':main()
