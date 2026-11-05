"""Bind original SDK/G2 source, runtime evidence and preserved prior artifacts."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path

def read(p):return json.loads(Path(p).read_bytes())
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','g2','g2_check','sdk','sdk_check','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    files=[a.g2/'build_verification.json',a.g2_check/'verification.json',a.sdk/'build_verification.json',a.sdk_check/'verification.json']
    gb,gv,sb,sv=map(read,files);assert gv['runtime_checks']==297 and sv['runtime_checks']==481
    assert sha(files[0])==gv['build_verification_sha256'] and sha(files[2])==sv['build_verification_sha256']
    assert sb['g2_build_sha256']==sha(files[0]) and sv['full_RAM_equal_across_CPU_DMA_and_independent_two_boot_control']
    authored={};private={}
    for folder,meta in ((a.g2,gb),(a.sdk,sb)):
        assert sha(meta['DLL'])==meta['DLL_sha256'] and all(not c['exit_code'] and not c['diagnostic_bytes'] for c in meta['commands'])
        for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h,n;authored[n]=h
        for n,h in meta['dependencies'].items():assert sha(folder/n)==h,n
        private[str(folder.resolve())]={x.name:sha(x) for x in sorted(folder.iterdir()) if x.suffix in ('.dll','.lib','.obj')}
    assert sha(a.sdk/'external_sd_driver.c')==sb['generated_source_sha256']
    for n,h in sb['original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    assert len(sb['original_function_sha256'])==42 and sb['unchanged_function_bodies']==31
    for name in ('verify_native_g2_audio.py','verify_native_sd_driver.py','package_native_sd_driver.py'):
        path=p/'jp_port_tools'/name;authored[path.relative_to(r).as_posix()]=sha(path)
    assert gv['verifier_sha256']==sha(p/'jp_port_tools/verify_native_g2_audio.py') and sv['verifier_sha256']==sha(p/'jp_port_tools/verify_native_sd_driver.py')
    guide=p/'JP_NATIVE_SD_DRIVER.txt';authored[guide.relative_to(r).as_posix()]=sha(guide)
    previous=read(p/'analysis/jp_native_sound_owners_package.json')
    for n,h in previous['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in previous['proof_files_sha256'].items():assert sha(r/n)==h,n
    for folder,hashes in previous['private_artifact_sha256'].items():
        for n,h in hashes.items():assert sha((r/folder).resolve()/n)==h,n
    prior=previous['prior_native_DLLs_preserved']
    for row in prior.values():assert sha(row['path'])==row['sha256']
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selected=qualified_native_builds(r);selection=[dict(path=relative,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for path,relative,raw in selected]
    expected=read(r/'work/sonic-adventure/jp-native-audio-publisher-selection-v1/verification.json')['files']
    assert selection==expected and len(selection)==10
    result=dict(status='native_G2_and_original_SDK_driver_source_and_repeated_boot_evidence_qualified',runtime_checks=778,transport_cases=3,SDK_cases=6,
        original_modules=38,original_function_bodies=42,unchanged_function_bodies=31,complete_native_sdDrvInit_executions=4,
        authored_source_sha256=authored,proof_files_sha256={x.resolve().relative_to(r).as_posix():sha(x) for x in files},private_artifact_sha256=private,prior_native_DLLs_preserved=prior,
        previous_sound_owner_package_sha256=sha(p/'analysis/jp_native_sound_owners_package.json'),original_SDK_work_constructor_executed=True,
        full_CPU_DMA_and_independent_repeated_control_RAM_equal=True,exact_note_and_tail_PCM_match_prior_probe=True,
        public_build_selection=selection,combined_SDK_or_emulator_binaries_publishable=False,retail_assets_published=False,
        original_game_sound_initialized=False,original_bank_and_MIDI_command_producers_integrated=False,global_game_audio_exit_integrated=False,
        physical_console_waveform_or_cycle_accuracy_verified=False,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    for source,name in ((files[0],'jp_native_g2_audio_build.json'),(files[1],'jp_native_g2_audio.json'),(files[2],'jp_native_sd_driver_build.json'),(files[3],'jp_native_sd_driver.json'),(d/'qualification.json','jp_native_sd_driver_package.json')):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],778,'checks; original SDK constructor and four complete driver calls; preserved prior DLLs',len(prior),flush=True)

if __name__=='__main__':main()
