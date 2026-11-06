"""Bind complete original ADXSTM integration, fresh regressions and unchanged prior evidence."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','verification','regressions','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files={'build':build/'build_verification.json','compile':build/'adxstm_compile_attempt.json','runtime':a.verification/'verification.json',
        'regression_commands':a.regressions/'commands.json'}
    scripts={'pstm':'verify_native_pstm.py','joints':'verify_native_adx_joints.py','decoder':'verify_native_adx_stream_decoder.py',
        'core':'verify_native_adx.py','math':'verify_native_sound_math.py','qsound':'verify_native_qsound.py','clock':'verify_native_sound_clock.py',
        'media':'verify_native_adx_media.py','audio':'verify_native_adx_output.py','rna':'verify_native_rna.py','retail':'verify_native_retail_rna.py'}
    files.update({n:a.regressions/n/'verification.json' for n in scripts});files['audit']=a.regressions/'audit/dependency_inventory.json'
    reports={n:read(path) for n,path in files.items()};b=reports['build'];bh=sha(files['build']);v=reports['runtime']
    assert len(reports['compile'])==4 and all(row['exit_code']==row['diagnostic_bytes']==0 for row in reports['compile'])
    assert all(row['exit_code']==0 for row in reports['regression_commands'].values())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ('game_sound_DLL','sound_SDK_DLL'):assert sha(b[n])==b[n+'_sha256']
    assert sha(build/'external_adx_stms.c')==b['adxstm_generated_source_sha256']
    assert sha(build/'sound_owner_adxstm.c')==b['adxstm_owner_generated_source_sha256']
    assert sha(build/'include/shinobi/gdfs.h')==b['adxstm_GDFS_native_header_sha256']
    assert sha(build/'external_game_sound.c')==b['pstm_generated_source_sha256']
    assert v['runtime_checks']==23752 and v['fresh_process_cases']==8 and v['build_verification_sha256']==bh
    assert v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_adxstm.py')
    for key,name in [('fixture_source_sha256','rna_test_runtime.py'),('oracle_source_sha256','adx_step_oracle.py'),('disc_fixture_source_sha256','adx_media_fixtures.py')]:assert v[key]==sha(p/'jp_port_tools'/name)
    cpu=v['cases']['title_cpu']['observations'];dma=v['cases']['title_dma']['observations']
    for row in (cpu,dma):
        assert row['retail_prefix_frames']==88192 and row['firmware_output_frames']==89344 and row['ring_wraps']==10
        assert row['continuously_refilled_original_ADXSTM'] and not row['header_rewritten'] and row['offline_render_quantum']==128
        assert sha(row['private_demo_WAV'])==row['private_demo_WAV_sha256']
    assert cpu['PCM_sha256']==dma['PCM_sha256'] and cpu['RAM_sha256']==dma['RAM_sha256']
    assert cpu['new_actual_DMA_transfers']==0 and dma['new_actual_DMA_transfers']==1091
    full=v['cases']['file']['observations'];assert full['independent_full_file_compared'] and full['retail_file_bytes']==2857986 and full['sector_bytes_compared']==2859008
    for n,script in scripts.items():assert reports[n]['build_verification_sha256']==bh and reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    assert reports['audit']['build_verification_sha256']==bh and reports['audit']['script_sha256']==sha(p/'jp_port_tools/audit_native_adx_stream.py')
    assert len(reports['audit']['genuine_frontend_providers_not_implemented'])==12
    assert reports['media']['frames_verified']==108680 and reports['audio']['frames']==88200
    prior={}
    for name in ('jp_native_retail_rna_package.json','jp_native_rna_package.json','jp_native_pstm_package.json','jp_native_adx_stream_decoder_package.json'):
        path=p/'analysis'/name;old=read(path)
        for field in ('authored_source_sha256','proof_files_sha256'):
            for n,h in old[field].items():assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    unchanged={}
    for key,row in b['adxstm_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert row['sha256']==b[key+'_sha256'],key;unchanged[key]=dict(current=b[key],sha256=row['sha256'])
    base=Path(b['adxstm_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    for name in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/name)==sha(base/name),name;unchanged[name]=dict(current=str(build/name),sha256=sha(build/name))
    authored=['jp_port_tools/'+n for n in ('build_native_adxstm.py','adxstm_observation.inc','verify_native_adxstm.py','package_native_adxstm.py')]
    authored+=['native/jp_native_adxstm.h','native/jp_native_adxstm.inc','JP_NATIVE_ADXSTM.txt']
    for n in authored:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    counts={n:reports[n]['runtime_checks'] for n in scripts};counts['adxstm']=v['runtime_checks']
    result=dict(status='complete_original_ADXSTM_module_native_file_refill_and_lifetime_retail_music_prefix_verified_full_ADXT_frontend_pending',
        runtime_checks=sum(counts.values()),checks_by_scope=counts,new_adxstm_checks=23752,new_fresh_process_cases=8,genuine_original_stream_exports=28,
        complete_retail_file_bytes_compared=2857986,sector_rounded_bytes_compared=2859008,retail_music_prefix_frames=88192,actual_firmware_output_frames=89344,
        CPU_DMA_complete_PCM_and_2MiB_RAM_match=True,automatic_shared_file_service_and_System_cleanup_verified=True,
        input_joint_ownership_and_actual_cancellation_rejection_preserved=True,second_clock_or_output_client_created=False,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,unchanged_prior_DLLs=unchanged,
        full_frontend_missing_services=reports['audit']['genuine_frontend_providers_not_implemented'],
        next_production_task='Complete original ADXF file/archive partition layer, then ADXT initialization/talk/server integration with native owned music/voice buffers in this same service/lifecycle.',
        full_song_looping_live_timed_retail_ADXT_AFS_voice_or_gameplay_verified=False,English_localization_verified=False,
        physical_console_DMA_timing_or_speaker_acoustics_qualified=False,
        combined_SDK_game_binaries_retail_inputs_RAM_PCM_and_WAV_publishable=False,public_build_payloads_changed=False,
        publication_pending_review=True,goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for name,src in files.items():
        target=p/'analysis'/('jp_native_adxstm_'+name+'.json');assert not target.exists();shutil.copyfile(src,target);assert sha(src)==sha(target)
    print(result['status'],result['runtime_checks'],'current scoped checks',flush=True)

if __name__=='__main__':main()
