"""Bind real frame sound, source audit and graphics/save regression evidence."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def read(p):return json.loads(Path(p).read_bytes())
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','startup','audit','upstream','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    files=[a.build/'build_verification.json',a.check/'verification.json',a.build/'save-verification.json',a.startup/'verification.json',a.audit/'dependency_inventory.json']
    b,v,save,startup,audit=map(read,files);assert v['runtime_checks']==98 and save['checks']==591 and startup['checks']==7281
    assert startup['actual_GPU_images']==startup['actual_Presents']==570 and save['original_calls']==29
    assert sha(files[0])==v['build_verification_sha256']==save['build_report_sha256']==startup['build_report_sha256']
    assert sha(b['present_DLL'])==b['present_DLL_sha256'] and sha(b['sound_SDK_DLL'])==b['sound_SDK_DLL_sha256']
    assert all(not c['exit_code'] and not c['diagnostic_bytes'] for c in b['commands'][:3])
    assert audit['remaining_provider_count']==23 and len(audit['actual_unresolved_link_symbols'])==27 and not audit['whole_game_sound_object_linked_or_executed']
    for n,h in audit['original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    assert sha(a.audit/'external_sound.c')==audit['generated_source_sha256'] and sha(a.audit/'sound.obj')==audit['object_sha256']
    assert sha(a.build/'present_with_card_lcd.c')==b['dispatch_generated_source_sha256']
    for row in b['preserved_prior_DLLs'].values():assert sha(row['path'])==row['sha256']
    prior=read(p/'analysis/jp_native_sd_commands_package.json')
    for n,h in prior['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in prior['proof_files_sha256'].items():assert sha(r/n)==h,n
    for row in prior['prior_native_DLLs_preserved'].values():assert sha(row['path'])==row['sha256']
    authored={}
    for relative,key in (('native/jp_native_sound_dispatch.c','dispatch_source_sha256'),('native/jp_native_sound_dispatch.h','dispatch_header_sha256'),('jp_port_tools/build_native_sound_dispatch.py','builder_sha256')):
        path=p/relative;assert sha(path)==b[key];authored[path.relative_to(r).as_posix()]=sha(path)
    assert sha(p/'jp_port_tools/verify_native_sound_dispatch.py')==v['verifier_sha256']
    for n,h in audit['authored_source_sha256'].items():assert sha(r/n)==h,n;authored[n]=h
    for name in ('verify_native_sound_dispatch.py','package_native_sound_dispatch.py'):
        path=p/'jp_port_tools'/name;authored[path.relative_to(r).as_posix()]=sha(path)
    guide=p/'JP_NATIVE_SOUND_DISPATCH.txt';authored[guide.relative_to(r).as_posix()]=sha(guide)
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selection=[dict(path=relative,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for path,relative,raw in qualified_native_builds(r)];assert selection==prior['public_build_selection'] and len(selection)==10
    result=dict(status='native_frame_sound_dispatch_whole_module_audit_and_preserved_startup_qualified',runtime_checks=7970,
        dispatch_checks=98,save_checks=591,startup_checks=7281,actual_GPU_images=570,actual_Presents=570,original_unhooked_save_calls=29,
        authored_source_sha256=authored,proof_files_sha256={x.resolve().relative_to(r).as_posix():sha(x) for x in files},
        prior_native_DLLs_preserved=prior['prior_native_DLLs_preserved'],public_build_selection=selection,
        original_frame_to_actual_SDK_and_ARM_note_verified=True,whole_game_sound_native_compiles=True,whole_game_sound_linked_or_executed=False,
        whole_module_absent_provider_names=23,whole_module_actual_unresolved_link_symbols=27,global_game_audio_owner_integrated=False,
        combined_SDK_or_core_binaries_publishable=False,retail_assets_or_captures_published=False,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    for source,name in ((files[0],'jp_native_sound_dispatch_build.json'),(files[1],'jp_native_sound_dispatch.json'),(files[2],'jp_native_sound_dispatch_saves.json'),(files[3],'jp_native_sound_dispatch_startup.json'),(files[4],'jp_native_game_sound_module_audit.json'),(d/'qualification.json','jp_native_sound_dispatch_package.json')):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],7970,'checks;570 GPU images;27 real unresolved whole-module link symbols',flush=True)
if __name__=='__main__':main()
