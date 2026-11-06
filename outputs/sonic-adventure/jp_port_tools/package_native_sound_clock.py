"""Bind timed firmware/PCM, real wait failures and unchanged original evidence."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','exit','owner','system','frame','startup','chain-view','chain','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    files=[a.build/'build_verification.json',a.check/'verification.json',a.exit/'verification.json',a.owner/'verification.json',a.system/'verification.json',a.frame/'verification.json',a.build/'save-verification.json',a.startup/'verification.json']
    b,v,exit,owner,system,frame,save,startup=map(read,files);root_hash=sha(files[0])
    assert all(x['build_verification_sha256']==root_hash for x in (v,exit,owner,system,frame))
    assert save['build_report_sha256']==startup['build_report_sha256']==root_hash
    assert v['runtime_checks']==3289 and v['fresh_process_cases']==19
    assert [exit['runtime_checks'],owner['runtime_checks'],system['checks'],frame['runtime_checks'],save['checks'],startup['checks']]==[652,1471,152,98,591,7281]
    assert save['original_calls']==29 and startup['actual_GPU_images']==startup['actual_Presents']==570
    assert all(not x['exit_code'] and not x['diagnostic_bytes'] for x in b['commands'][:11])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['sound_clock_generated_sha256'].items():assert sha(a.build/n)==h,n
    for n,h in b['present_link_inputs_sha256'].items():assert sha(n)==h,n
    assert sha(a.build/'external_game_sound.c')==b['game_sound_generated_source_sha256']
    changed={'present_DLL','game_sound_DLL','sound_SDK_DLL','System_DLL','authored_exit_DLL'};unchanged={}
    for key,row in b['sound_clock_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in changed:assert b[key+'_sha256']==row['sha256'];unchanged[key]=row
    old=read(p/'analysis/jp_native_sound_exit_package.json')
    for group in ('authored_source_sha256','proof_files_sha256'):
        for n,h in old[group].items():assert sha(r/n)==h,n
    chain_view=read(a.chain_view/'build_verification.json');chain=read(a.chain/'verification.json')
    assert chain_view['parent_graph_build_sha256']==root_hash and chain_view['present_DLL_sha256']==b['present_DLL_sha256']
    assert chain['build_report_sha256']==sha(a.chain_view/'build_verification.json') and chain['checks']==79103 and chain['original_entry_calls']==679
    assert read(a.chain_view/'present/build_verification.json')['DLL_sha256']==b['present_DLL_sha256']
    files.extend([a.chain_view/'build_verification.json',a.chain_view/'present/build_verification.json',a.chain/'verification.json'])
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selected=[dict(path=rel,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for path,rel,raw in qualified_native_builds(r)]
    assert selected==old['public_build_selection'] and len(selected)==10
    authored=[p/'native'/('jp_native_'+n) for n in ('wait_service.c','wait_service.h','sound_clock.h','sound_clock.inc','system_sound_clock.h','system_sound_clock.inc','exit_sound_clock.h','exit_sound_clock.inc','vblank_retire.c')]
    authored+=[p/'jp_port_tools'/n for n in ('build_native_sound_clock.py','verify_native_sound_clock.py','package_native_sound_clock.py')];authored.append(p/'JP_NATIVE_SOUND_CLOCK.txt')
    hashes={x.relative_to(r).as_posix():sha(x) for x in authored}
    assert sha(p/'jp_port_tools/verify_native_sound_clock.py')==v['verifier_sha256']
    reference=v['cases']['reference']['observations']
    for case in ('rate60','rate120','rate240','fraction','backpressure_recovery'):
        assert v['cases'][case]['observations']['actual_PCM_FNV']==reference['float_FNV']
    capture=a.startup/'movie0/native-reset-boot-logo.png';assert sha(capture)=='9b28f7b43d8786cfee116fc34b39ed33ff44fb8ed93a0f62b21477b4902d2071'
    result=dict(status='actual_elapsed_time_original_firmware_PCM_native_wait_and_fatal_cleanup_qualified',
        runtime_checks=92637,new_clock_checks=3289,fresh_clock_process_cases=19,existing_graph_checks=10245,original_chain_regression_checks=79103,
        actual_original_chain_entry_calls=679,original_unhooked_save_calls=29,actual_GPU_images=570,actual_Presents=570,
        authored_source_sha256=hashes,proof_files_sha256={x.resolve().relative_to(r).as_posix():sha(x) for x in files},
        unchanged_prior_DLLs=unchanged,all_prior_qualified_source_and_proofs_preserved=True,public_build_selection=selected,
        timed_original_firmware_and_actual_Windows_PCM_verified=True,original_sample_frames_per_counted_second=44100,
        complete_PCM_and_RAM_match_independent_reference_across_cadence=True,real_queue_backpressure_and_lossless_recovery_verified=True,
        actual_QPC_native_waits_and_nonzero_audio_verified=True,terminal_wait_failure_preserved_during_safe_callback_retirement=True,
        actual_pending_file_read_cancellation_after_clock_fault_verified=True,
        hardware_cycle_fidelity_or_full_game_queue_streams_main_gameplay_verified=False,
        full_game_60_120_240FPS_or_combined_audiovisual_gameplay_verified=False,
        combined_game_SDK_core_binaries_or_retail_media_published=False,publication_pending_next_daily_review=True,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    sources=[*files[:8],files[10],d/'qualification.json'];names=('build','verification','exit','owner','system','frame','saves','startup','chain','package')
    for source,name in zip(sources,names):
        name='jp_native_sound_clock.json' if name=='verification' else 'jp_native_sound_clock_'+name+'.json';target=p/'analysis'/name
        assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],92637,'scoped checks;3289 new clock checks;19 fresh clock cases',flush=True)
if __name__=='__main__':main()
