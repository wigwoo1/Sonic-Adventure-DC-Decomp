"""Bind exact math/shared-data proofs, appropriate regressions and current frontier."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','spatial','qsound','clock','audit','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files=dict(build=build/'build_verification.json',verification=a.check/'verification.json',spatial=a.spatial/'verification.json',qsound=a.qsound/'verification.json',clock=a.clock/'verification.json',audit=a.audit/'dependency_inventory.json')
    reports={n:read(path) for n,path in files.items()};b=reports['build'];v=reports['verification'];bh=sha(files['build'])
    assert all(reports[n]['build_verification_sha256']==bh for n in ('verification','spatial','qsound','clock','audit'))
    assert v['runtime_checks']==293998 and v['complete_original_entry_calls']==143504
    assert v['all_65536_angle_phases_checked'] and not v['original_function_hooks'] and v['shared_globals_imported_without_duplicates']
    assert v['controlled_state_fixtures_not_physical_input_or_camera_gameplay'] and v['native_pointer_bits']==64
    assert reports['spatial']['runtime_checks']==55295 and reports['spatial']['complete_original_entry_calls']==4600
    assert reports['qsound']['runtime_checks']>18000 and reports['qsound']['original_complete_instruction_calls']==3292
    assert reports['clock']['runtime_checks']>3000 and reports['clock']['fresh_process_cases']==19
    assert all(not row['exit_code'] and not row['diagnostic_bytes'] for row in b['commands'][:5])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(build/n)==h,n
    for n,h in b['sound_state_import_header_sha256'].items():assert sha(build/'sound_imports'/n)==h,n
    assert sha(build/'external_game_sound.c')==b['sound_math_generated_source_sha256']==b['spatial_generated_source_sha256']==b['qsound_generated_source_sha256']==b['game_sound_generated_source_sha256']
    assert sha(build/'sound_owner_qsound.c')==b['qsound_owner_generated_sha256']
    old_path=p/'analysis/jp_native_sound_spatial_package.json';old=read(old_path)
    for name in ('jp_native_sound_spatial_package.json','jp_native_qsound_package.json','jp_native_sound_clock_package.json'):
        for group in ('authored_source_sha256','proof_files_sha256'):
            for n,h in read(p/'analysis'/name)[group].items():assert sha(r/n)==h,n
    base_file=next(r/n for n in old['proof_files_sha256'] if n.endswith('/build_verification.json') and sha(r/n)==b['sound_math_baseline_sha256'])
    unchanged={}
    for key,row in b['sound_math_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):assert b[key+'_sha256']==row['sha256'];unchanged[key]=row
    for n in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/n)==sha(base_file.parent/n),n
        unchanged[n]=dict(path=str(base_file.parent/n),sha256=sha(build/n))
    for n,script in (('verification','verify_native_sound_math.py'),('spatial','verify_native_sound_spatial.py'),('qsound','verify_native_qsound.py'),('clock','verify_native_sound_clock.py')):
        assert reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    assert v['probe_source_sha256']==sha(p/'jp_port_tools/sound_math_value_probe.c') and v['probe_DLL_sha256']==sha(a.check/'sound_math_value_probe.dll')
    audit=reports['audit'];assert audit['compile_exit_code']==audit['compile_diagnostic_bytes']==0 and audit['expected_incomplete_link_exit_code']!=0
    assert len(audit['missing_genuine_providers'])==len(audit['actual_unresolved_link_symbols'])==13 and not audit['existing_exports_requiring_native_link_declaration_repairs']
    assert not audit['entire_original_sound_object_linked_or_executed'] and not audit['dummy_services_introduced']
    assert audit['sound_state_import_header_sha256']==b['sound_state_import_header_sha256']
    for n,h in audit['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,key in (('external_full_sound.c','generated_source_sha256'),('sound.obj','object_sha256')):assert sha(a.audit/n)==audit[key]
    header=p/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h';assert sha(header)=='cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
    assert v['pinned_vector_instruction_model_DLL_sha256']=='2e56428fadb1903c319da319cf6fa3731839eab1b16163e4e820821d6e4c2db2'
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selected=[dict(path=rel,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for _,rel,raw in qualified_native_builds(r)]
    assert selected==old['public_build_selection'] and len(selected)==10
    authored=['native/jp_native_sound_math.c','native/jp_native_sound_math.h','jp_port_tools/sound_state_imports.py','jp_port_tools/sound_state_observation.inc','jp_port_tools/build_native_sound_math.py','jp_port_tools/sound_math_value_probe.c','jp_port_tools/verify_native_sound_math.py','jp_port_tools/audit_native_sound_after_math.py','jp_port_tools/package_native_sound_math.py','JP_NATIVE_SOUND_MATH.txt']
    dependencies=[header,header.parent/'README.txt',p/'jp_port_tools/camera_reference_numeric.py',p/'jp_port_tools/model_reference.py',p/'jp_port_tools/primitive_reference.py',p/'sh4_binary32.py',p/'verify_startup.py',p/'jp_port_tools/native_task_test_support_v2.py',p/'jp_port_tools/verify_native_input_backend.py']
    total=sum(reports[n]['runtime_checks'] for n in ('verification','spatial','qsound','clock'))
    result=dict(status='original_sound_math_and_actual_native_shared_state_imports_qualified_full_stream_gameplay_unfinished',runtime_checks=total,new_math_state_checks=v['runtime_checks'],complete_original_math_entry_calls=143504,
        current_spatial_checks=reports['spatial']['runtime_checks'],current_QSound_checks=reports['qsound']['runtime_checks'],current_clock_checks=reports['clock']['runtime_checks'],
        all_65536_phase_sine_cosine_checked=True,native_shared_pointer_bits=64,four_existing_data_import_bindings_repaired=True,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},
        reference_support_source_sha256={x.relative_to(r).as_posix():sha(x) for x in dependencies},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualification=dict(path=str(old_path),sha256=sha(old_path)),all_prior_qualified_source_and_proofs_preserved=True,
        unchanged_prior_DLLs=unchanged,public_build_selection=selected,
        prior_owner_exit_save_System_frame_startup_chain_proofs_retained_not_rerun=True,
        current_missing_genuine_sound_providers=audit['missing_genuine_providers'],current_unresolved_sound_link_symbols=audit['actual_unresolved_link_symbols'],
        current_existing_export_binding_repairs=[],shared_approximation_new_hardware_validation=False,
        full_sound_module_queue_streams_camera_player_physical_controls_or_main_gameplay_verified=False,
        combined_external_binaries_or_retail_media_published=False,publication_pending_next_daily_review=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for name,source in files.items():
        target=p/'analysis'/('jp_native_sound_math'+('' if name=='verification' else '_'+name)+'.json');assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],total,'current checks;',v['runtime_checks'],'new math/state checks',flush=True)
if __name__=='__main__':main()
