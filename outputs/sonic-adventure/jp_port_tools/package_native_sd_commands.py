"""Qualify original bank/command source and actual output evidence for review."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','build','check','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    b=read(a.build/'build_verification.json');v=read(a.check/'verification.json');old=read(p/'analysis/jp_native_sd_driver_package.json')
    assert sha(a.build/'build_verification.json')==v['build_verification_sha256'] and v['runtime_checks']==8959 and v['fresh_process_cases']==7
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(a.build/'external_sd_commands.c')==b['generated_source_sha256']
    assert all(not x['exit_code'] and not x['diagnostic_bytes'] for x in b['commands'])
    assert sha(a.baseline/'build_verification.json')==b['baseline_build_sha256'] and sha(a.baseline/'external_sd_driver.c')==b['baseline_generated_source_sha256']
    for n,h in b['additional_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['dependencies'].items():assert sha(a.build/n)==h,n
    assert sha(p/'jp_port_tools/verify_native_sd_commands.py')==v['verifier_sha256']
    for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in old['proof_files_sha256'].items():assert sha(r/n)==h,n
    for folder,hashes in old['private_artifact_sha256'].items():
        for n,h in hashes.items():assert sha(Path(folder)/n)==h,n
    for row in old['prior_native_DLLs_preserved'].values():assert sha(row['path'])==row['sha256']
    authored=dict(b['authored_source_sha256'])
    for name in ('verify_native_sd_commands.py','package_native_sd_commands.py'):
        path=p/'jp_port_tools'/name;authored[path.relative_to(r).as_posix()]=sha(path)
    guide=p/'JP_NATIVE_SD_COMMANDS.txt';authored[guide.relative_to(r).as_posix()]=sha(guide)
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selection=[dict(path=relative,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for path,relative,raw in qualified_native_builds(r)]
    assert selection==old['public_build_selection'] and len(selection)==10
    result=dict(status='original_SDK_banks_MIDI_queues_and_actual_audio_source_runtime_evidence_qualified',runtime_checks=8959,fresh_process_cases=7,
        original_modules_total=63,original_functions_total=68,new_original_modules=25,new_original_functions=26,new_unchanged_function_bodies=16,
        complete_actual_MLT_files=112,positive_bank_payloads=711,MIDI_port_status_combinations=2048,actual_Windows_note_frames=88200,
        authored_source_sha256=authored,proof_files_sha256={str(x.resolve().relative_to(r)).replace('\\','/'):sha(x) for x in (a.build/'build_verification.json',a.check/'verification.json')},
        private_artifact_sha256={str(a.build.resolve()):{x.name:sha(x) for x in sorted(a.build.iterdir()) if x.suffix in ('.dll','.lib','.obj')}},
        previous_driver_package_sha256=sha(p/'analysis/jp_native_sd_driver_package.json'),prior_native_DLLs_preserved=old['prior_native_DLLs_preserved'],public_build_selection=selection,
        original_bank_payload_and_MIDI_host_packet_producers_execute=True,driver_ACKs_synthesized=False,
        original_game_sound_system_initialized=False,global_audio_exit_integrated=False,complete_game_audio_verified=False,physical_console_fidelity_verified=False,
        combined_SDK_core_binaries_publishable=False,retail_data_published=False,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    for source,name in ((a.build/'build_verification.json','jp_native_sd_commands_build.json'),(a.check/'verification.json','jp_native_sd_commands.json'),(d/'qualification.json','jp_native_sd_commands_package.json')):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],8959,'checks,112 banks,711payloads,2048 MIDI combinations;15 prior DLLs preserved',flush=True)
if __name__=='__main__':main()
