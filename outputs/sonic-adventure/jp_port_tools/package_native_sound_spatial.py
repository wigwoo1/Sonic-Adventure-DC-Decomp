"""Bind current spatial/audio/startup proofs and the honest full-module frontier."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','build','check','qsound','clock','owner','exit','startup','audit','output'):ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files=dict(build=build/'build_verification.json',verification=a.check/'verification.json',qsound=a.qsound/'verification.json',clock=a.clock/'verification.json',owner=a.owner/'verification.json',exit=a.exit/'verification.json',saves=build/'save-verification.json',startup=a.startup/'verification.json',audit=a.audit/'dependency_inventory.json')
    reports={n:read(path) for n,path in files.items()};b=reports['build'];v=reports['verification'];bh=sha(files['build'])
    for n in ('verification','qsound','clock','owner','exit','audit'):assert reports[n]['build_verification_sha256']==bh,n
    for n in ('saves','startup'):assert reports[n]['build_report_sha256']==bh,n
    assert v['runtime_checks']==55295 and v['complete_original_entry_calls']==4600
    assert v['original_entry_calls']==dict(njUnitVector=1632,matrix_calc_point=1024,qsound_place=1944)
    assert not v['original_function_hooks'] and v['actual_single_existing_frame_matrix_and_scalar_imports']
    assert v['guard_page_exact_vector_bytes']==12 and v['vector_approximation_model_is_shared_with_native']
    assert reports['qsound']['runtime_checks']>18000 and reports['qsound']['original_complete_instruction_calls']==3292
    assert reports['clock']['fresh_process_cases']==19 and reports['clock']['runtime_checks']>3000
    assert reports['owner']['runtime_checks']==1471 and reports['exit']['runtime_checks']==652
    assert reports['saves']['checks']==591 and reports['saves']['original_calls']==29
    assert reports['startup']['checks']==7281 and reports['startup']['actual_GPU_images']==reports['startup']['actual_Presents']==570
    assert all(not row['exit_code'] and not row['diagnostic_bytes'] for row in b['commands'][:4])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(build/n)==h,n
    assert sha(build/'external_game_sound.c')==b['spatial_generated_source_sha256']==b['game_sound_generated_source_sha256']==b['qsound_generated_source_sha256']
    assert sha(build/'sound_owner_qsound.c')==b['qsound_owner_generated_sha256']
    old_path=p/'analysis/jp_native_qsound_package.json';old=read(old_path)
    clock_old=p/'analysis/jp_native_sound_clock_package.json';assert sha(clock_old)==old['preserved_previous_qualification']['sha256']
    for previous in (old,read(clock_old)):
        for group in ('authored_source_sha256','proof_files_sha256'):
            for n,h in previous[group].items():assert sha(r/n)==h,n
    base_file=next(r/n for n in old['proof_files_sha256'] if n.endswith('/build_verification.json') and sha(r/n)==b['spatial_baseline_sha256'])
    base=read(base_file);unchanged={}
    for key,row in b['spatial_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert b[key+'_sha256']==row['sha256'];unchanged[key]=row
    for name in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/name)==sha(base_file.parent/name)
        unchanged[name]=dict(path=str(base_file.parent/name),sha256=sha(build/name))
    assert b['frames_DLL_sha256']==b['spatial_shared_frame_DLL_sha256']==base['frames_DLL_sha256']
    header=p/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h'
    assert sha(header)=='cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
    assert v['pinned_vector_instruction_model_DLL_sha256']==sha(r/'work/sonic-adventure/build/sh4_vector_numeric.dll')=='2e56428fadb1903c319da319cf6fa3731839eab1b16163e4e820821d6e4c2db2'
    for name,script in (('verification','verify_native_sound_spatial.py'),('qsound','verify_native_qsound.py'),('clock','verify_native_sound_clock.py'),('owner','verify_native_game_sound.py'),('exit','verify_native_sound_exit.py'),('saves','verify_native_boot_saves.py'),('startup','verify_native_system_startup.py')):
        assert reports[name]['verifier_sha256']==sha(p/'jp_port_tools'/script),name
    assert v['probe_source_sha256']==sha(p/'jp_port_tools/sound_spatial_value_probe.c')
    assert v['probe_DLL_sha256']==sha(a.check/'spatial_value_probe.dll')
    audit=reports['audit'];assert audit['compile_exit_code']==audit['compile_diagnostic_bytes']==0 and audit['expected_incomplete_link_exit_code']!=0
    assert len(audit['missing_genuine_providers'])==17 and len(audit['actual_unresolved_link_symbols'])==21
    assert sorted(audit['existing_exports_requiring_native_link_declaration_repairs'])==['camera_work','event_skipping','frame_redrawing','pad_records']
    assert not audit['entire_original_sound_object_linked_or_executed'] and not audit['dummy_services_introduced']
    for n,h in audit['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,key in (('external_full_sound.c','generated_source_sha256'),('sound.obj','object_sha256')):assert sha(a.audit/n)==audit[key]
    capture=a.startup/'movie0/native-reset-boot-logo.png';assert sha(capture)=='9b28f7b43d8786cfee116fc34b39ed33ff44fb8ed93a0f62b21477b4902d2071'
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selected=[dict(path=rel,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for _,rel,raw in qualified_native_builds(r)]
    assert selected==old['public_build_selection'] and len(selected)==10
    authored=['native/jp_native_sound_spatial.c','native/jp_native_sound_spatial.h','jp_port_tools/qsound_spatial_observation.inc','jp_port_tools/build_native_sound_spatial.py','jp_port_tools/verify_native_sound_spatial.py','jp_port_tools/sound_spatial_value_probe.c','jp_port_tools/audit_native_sound_after_spatial.py','jp_port_tools/package_native_sound_spatial.py','JP_NATIVE_SOUND_SPATIAL.txt']
    total=sum(reports[n]['runtime_checks'] for n in ('verification','qsound','clock','owner','exit'))+reports['saves']['checks']+reports['startup']['checks']
    result=dict(status='original_spatial_sound_shared_matrix_and_native_RZ_values_qualified_full_queue_frontier_recorded',runtime_checks=total,new_spatial_checks=v['runtime_checks'],complete_original_spatial_entry_calls=4600,
        current_QSound_checks=reports['qsound']['runtime_checks'],current_clock_checks=reports['clock']['runtime_checks'],current_owner_exit_save_startup_checks=9995,
        actual_GPU_images=570,actual_Presents=570,original_save_calls=29,
        prior_System_frame_chain_proofs_retained_not_rerun=True,retained_original_chain_checks_not_rerun=79103,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},
        support_source_sha256={path.relative_to(r).as_posix():sha(path) for path in (header,header.parent/'README.txt')},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualification=dict(path=str(old_path),sha256=sha(old_path)),all_prior_qualified_source_and_proofs_preserved=True,
        unchanged_prior_DLLs=unchanged,public_build_selection=selected,
        current_full_sound_missing_genuine_providers=audit['missing_genuine_providers'],current_full_sound_unresolved_symbols=audit['actual_unresolved_link_symbols'],current_data_import_binding_repairs=audit['existing_exports_requiring_native_link_declaration_repairs'],
        shared_approximation_model_hardware_accuracy_independently_reverified=False,
        full_game_queue_streams_acoustic_3D_main_gameplay_verified=False,combined_external_binaries_or_retail_media_published=False,
        publication_pending_next_daily_review=True,playable=False,overall_completion_percent=10)
    assert result['current_owner_exit_save_startup_checks']==sum(reports[n]['runtime_checks'] for n in ('owner','exit'))+reports['saves']['checks']+reports['startup']['checks']
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for name,source in files.items():
        target=p/'analysis'/('jp_native_sound_spatial'+('' if name=='verification' else '_'+name)+'.json');assert not target.exists()
        shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],total,'current checks;',v['runtime_checks'],'new spatial checks',flush=True)

if __name__=='__main__':main()
