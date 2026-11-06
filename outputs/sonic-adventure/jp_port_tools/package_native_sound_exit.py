"""Qualify exact sound/System/global exit graph and preserved original evidence."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','owner','system','frame','startup','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    files=[a.build/'build_verification.json',a.check/'verification.json',a.owner/'verification.json',a.system/'verification.json',a.frame/'verification.json',a.build/'save-verification.json',a.startup/'verification.json']
    b,v,owner,system,frame,save,startup=map(read,files);h=sha(files[0])
    assert all(h==x['build_verification_sha256'] for x in (v,owner,system,frame))
    assert h==save['build_report_sha256']==startup['build_report_sha256']
    assert [v['runtime_checks'],owner['runtime_checks'],system['checks'],frame['runtime_checks'],save['checks'],startup['checks']]==[652,1471,152,98,591,7281]
    assert v['fresh_process_cases']==20 and v['actual_process_exit_codes']==[0,70]
    assert save['original_calls']==29 and startup['actual_GPU_images']==startup['actual_Presents']==570
    assert all(not row['exit_code'] and not row['diagnostic_bytes'] for row in b['commands'][:7])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['sound_exit_generated_sha256'].items():assert sha(a.build/n)==h,n
    assert sha(a.build/'external_game_sound.c')==b['game_sound_generated_source_sha256']
    changed={'game_sound_DLL','sound_SDK_DLL','System_DLL','authored_exit_DLL'};preserved={}
    for key,row in b['sound_exit_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in changed:assert b[key+'_sha256']==row['sha256'];preserved[key]=row
    prior=read(p/'analysis/jp_native_game_sound_package.json')
    for group in ('authored_source_sha256','proof_files_sha256'):
        for n,h in prior[group].items():assert sha(r/n)==h,n
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selected=[dict(path=rel,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for path,rel,raw in qualified_native_builds(r)]
    assert selected==prior['public_build_selection'] and len(selected)==10
    authored=[p/'native'/('jp_native_'+n) for n in ('sound_exit.h','sound_exit.inc','exit_sound.h','exit_sound.inc','system_sound.h','system_sound.inc')]
    authored+=[p/'jp_port_tools'/n for n in ('build_native_sound_exit.py','verify_native_sound_exit.py','package_native_sound_exit.py')];authored.append(p/'JP_NATIVE_SOUND_EXIT.txt')
    hashes={x.relative_to(r).as_posix():sha(x) for x in authored}
    assert sha(p/'jp_port_tools/verify_native_sound_exit.py')==v['verifier_sha256']
    capture=a.startup/'movie0/native-reset-boot-logo.png';assert sha(capture)=='9b28f7b43d8786cfee116fc34b39ed33ff44fb8ed93a0f62b21477b4902d2071'
    result=dict(status='actual_original_game_sound_System_early_final_exit_lifetime_and_preserved_native_graphics_saves_audio_qualified',
        runtime_checks=10245,new_exit_checks=652,fresh_exit_process_cases=20,owner_checks=1471,system_checks=152,frame_checks=98,save_checks=591,startup_checks=7281,
        actual_process_exit_codes=[0,70],actual_GPU_images=570,actual_Presents=570,original_unhooked_save_calls=29,
        authored_source_sha256=hashes,proof_files_sha256={x.resolve().relative_to(r).as_posix():sha(x) for x in files},
        unchanged_prior_DLLs=preserved,all_prior_owner_source_and_proofs_preserved=True,public_build_selection=selected,
        actual_original_sound_startup_under_compiled_System=True,global_audio_exit_qualified=True,ordinary_restart_and_early_final_retry_verified=True,
        original_masks_and_single_bootstrap_preserved=True,sustained_real_time_audio_scheduler_verified=False,
        full_game_queue_spatial_streams_higher_main_or_gameplay_verified=False,combined_full_audiovisual_gameplay_verified=False,
        combined_game_SDK_core_binaries_or_retail_captures_published=False,publication_pending_next_daily_review=True,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    names=('jp_native_sound_exit_build.json','jp_native_sound_exit.json','jp_native_sound_exit_owner.json','jp_native_sound_exit_system.json','jp_native_sound_exit_frame.json','jp_native_sound_exit_saves.json','jp_native_sound_exit_startup.json','jp_native_sound_exit_package.json')
    for source,name in zip(files+[d/'qualification.json'],names):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],10245,'checks;20 new lifecycle process cases;570 GPU frames',flush=True)
if __name__=='__main__':main()
