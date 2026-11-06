"""Bind owned original game sound initialization to exact preserved evidence."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','check','frame-check','startup','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    files=[a.build/'build_verification.json',a.check/'verification.json',a.frame_check/'verification.json',a.build/'save-verification.json',a.startup/'verification.json']
    b,v,f,s,g=map(read,files);h=sha(files[0])
    assert h==v['build_verification_sha256']==f['build_verification_sha256']==s['build_report_sha256']==g['build_report_sha256']
    assert v['runtime_checks']==1471 and v['fresh_process_cases']==11 and f['runtime_checks']==98 and s['checks']==591 and g['checks']==7281
    assert s['original_calls']==29 and g['actual_GPU_images']==g['actual_Presents']==570
    assert all(not x['exit_code'] and not x['diagnostic_bytes'] for x in b['commands'][:3])
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'] and b['sound_SDK_DLL']==b['game_sound_DLL']
    assert sha(a.build/'external_game_sound.c')==b['game_sound_generated_source_sha256']
    for n,h in b['game_sound_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    for row in b['preserved_prior_sound_owner_DLLs'].values():assert sha(row['path'])==row['sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    prior=read(p/'analysis/jp_native_sound_dispatch_package.json')
    for n,h in prior['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in prior['proof_files_sha256'].items():assert sha(r/n)==h,n
    for row in prior['prior_native_DLLs_preserved'].values():assert sha(row['path'])==row['sha256']
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selected=[dict(path=relative,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for path,relative,raw in qualified_native_builds(r)]
    assert selected==prior['public_build_selection'] and len(selected)==10
    authored={}
    for rel in ('native/jp_native_game_sound.c','native/jp_native_game_sound.h','jp_port_tools/build_native_game_sound.py','jp_port_tools/game_sound_owner_bridge.inc','jp_port_tools/verify_native_game_sound.py','jp_port_tools/package_native_game_sound.py','JP_NATIVE_GAME_SOUND.txt'):
        path=p/rel;authored[path.relative_to(r).as_posix()]=sha(path)
    assert sha(p/'jp_port_tools/verify_native_game_sound.py')==v['verifier_sha256']
    assert authored[(p/'native/jp_native_game_sound.c').relative_to(r).as_posix()]==b['sound_owner_source_sha256']
    capture=a.startup/'movie0/native-reset-boot-logo.png'
    assert sha(capture)=='9b28f7b43d8786cfee116fc34b39ed33ff44fb8ed93a0f62b21477b4902d2071'
    proofs={x.resolve().relative_to(r).as_posix():sha(x) for x in files}
    result=dict(status='owned_original_game_sound_startup_local_cleanup_and_preserved_native_frame_audio_graphics_saves_qualified',
        runtime_checks=9441,owner_checks=1471,fresh_owner_process_cases=11,frame_checks=98,save_checks=591,startup_checks=7281,
        actual_original_initialization_functions=10,actual_initialization_commands=39,actual_GPU_images=570,actual_Presents=570,original_unhooked_save_calls=29,
        authored_source_sha256=authored,proof_files_sha256=proofs,prior_native_DLLs_preserved=prior['prior_native_DLLs_preserved'],public_build_selection=selected,
        original_game_sound_constructor_and_local_ownership_verified=True,original_full_game_request_queue_linked=False,
        global_audio_exit_and_sustained_real_time_scheduler_verified=False,combined_full_audiovisual_gameplay_verified=False,
        original_source_and_proofs_preserved=True,combined_game_SDK_or_core_binaries_publishable=False,retail_assets_or_capture_published=False,
        publication_pending_next_daily_review=True,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    names=('jp_native_game_sound_build.json','jp_native_game_sound.json','jp_native_game_sound_frame.json','jp_native_game_sound_saves.json','jp_native_game_sound_startup.json','jp_native_game_sound_package.json')
    for source,name in zip(files+[d/'qualification.json'],names):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],9441,'checks; ten original game functions;570 GPU frames',flush=True)
if __name__=='__main__':main()
