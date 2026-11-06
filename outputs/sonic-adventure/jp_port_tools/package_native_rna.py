"""Bind scoped RNA qualification and unchanged-component regressions to exact bytes."""
import argparse, hashlib, json, shutil
from pathlib import Path

def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path): return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','rna','regressions','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';build=a.build.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files={'build':build/'build_verification.json','inputs':a.inputs/'verification.json','runtime':a.rna/'verification.json',
           'compile':build/'rna_compile_attempt.json','regression_commands':a.regressions/'commands.json'}
    scripts={'pstm':'verify_native_pstm.py','joints':'verify_native_adx_joints.py','decoder':'verify_native_adx_stream_decoder.py',
        'core':'verify_native_adx.py','math':'verify_native_sound_math.py','qsound':'verify_native_qsound.py',
        'clock':'verify_native_sound_clock.py','media':'verify_native_adx_media.py','audio':'verify_native_adx_output.py'}
    files.update({n:a.regressions/n/'verification.json' for n in scripts})
    files['audit']=a.regressions/'audit/dependency_inventory.json'
    reports={n:read(path) for n,path in files.items()};b=reports['build'];bh=sha(files['build']);v=reports['runtime']
    assert len(reports['compile'])==9 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in reports['compile'])
    assert all(x['exit_code']==0 for x in reports['regression_commands'].values())
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert sha(build/'sound_owner_rna.c')==b['rna_owner_generated_source_sha256']
    assert sha(build/'external_game_sound.c')==b['pstm_generated_source_sha256']
    for n,h in b['rna_generated_sources_sha256'].items(): assert sha(build/n)==h,n
    assert sha(files['inputs'])==b['rna_matching_inputs_sha256']
    inputs=reports['inputs'];assert not inputs['placeholder_texts_used'] and inputs['original_matching_files_unchanged']
    for n,row in inputs['generated'].items():
        assert sha(a.inputs/'includes'/n)==sha(build/'rna_inputs'/n)==row['sha256'],n
    for n,h in inputs['authored_source_sha256'].items(): assert sha(r/n)==h,n
    assert v['runtime_checks']==5436 and v['fresh_process_cases']==9 and v['build_verification_sha256']==bh
    assert v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_rna.py')
    assert v['fixture_source_sha256']==sha(p/'jp_port_tools/rna_test_runtime.py')
    cpu=v['cases']['copy_cpu']['observations'];dma=v['cases']['copy_dma']['observations']
    assert cpu['source_frames']==dma['source_frames']==24576 and cpu['rendered_frames']==dma['rendered_frames']==26112
    assert cpu['PCM_sha256']==dma['PCM_sha256'] and cpu['RAM_sha256']==dma['RAM_sha256']
    assert cpu['new_DMA_transfers']==0 and dma['new_DMA_transfers']>0
    for rate in (60,120,240):
        row=v['cases']['timed'+str(rate)]['observations']
        assert row['actual_stream_sample_frames']>1000 and row['service_calls']>0 and row['consumed']>0 and not row['second_output_client_created']
    for n,script in scripts.items():
        assert reports[n]['build_verification_sha256']==bh and reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    assert reports['audit']['build_verification_sha256']==bh
    assert reports['audit']['script_sha256']==sha(p/'jp_port_tools/audit_native_adx_stream.py')
    assert len(reports['audit']['genuine_frontend_providers_not_implemented'])==12
    assert reports['media']['frames_verified']==108680 and reports['audio']['frames']==88200
    assert reports['audio']['media_proof_sha256']==sha(files['media'])
    prior={}
    for name in ('jp_native_pstm_package.json','jp_native_adx_stream_decoder_package.json','jp_native_adx_package.json',
        'jp_native_sound_math_package.json','jp_native_sound_spatial_package.json','jp_native_qsound_package.json','jp_native_sound_clock_package.json'):
        path=p/'analysis'/name;old=read(path)
        for field in ('authored_source_sha256','proof_files_sha256'):
            for n,h in old[field].items(): assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    unchanged={}
    for key,row in b['rna_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert row['sha256']==b[key+'_sha256'],key
            unchanged[key]=dict(current=b[key],sha256=row['sha256'])
    base=Path(b['rna_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    for n in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/n)==sha(base/n),n
        unchanged[n]=dict(current=str(build/n),sha256=sha(build/n))
    authored=['jp_port_tools/'+n for n in ('prepare_native_rna_inputs.py','build_native_rna.py','rna_observation.inc',
        'rna_test_runtime.py','verify_native_rna.py','package_native_rna.py')]
    authored+=['native/'+n for n in ('jp_native_rna.h','jp_native_rna.inc','jp_native_rna_clock.c')]+['JP_NATIVE_RNA.txt']
    counts={n:reports[n]['runtime_checks'] for n in scripts};counts['rna']=v['runtime_checks']
    result=dict(status='scoped_original_RNA_middleware_shared_owner_and_regressions_verified_retail_frontend_pending',
        runtime_checks=sum(counts.values()),checks_by_scope=counts,new_RNA_checks=5436,new_RNA_fresh_process_cases=9,
        genuine_middleware_exports=len(b['rna_exports']),actual_RNA_and_PCM_port_capacity=4,
        authored_stereo_source_frames=24576,actual_firmware_rendered_frames=26112,CPU_DMA_complete_PCM_and_2MiB_RAM_match=True,
        independent_actual_chunk_order_and_full_ring_contents_verified=True,automatic_shared_clock_wait_rates_tested=[60,120,240],
        actual_System_pending_DMA_and_borrowed_chunk_cleanup_verified=True,second_audio_output_client_created=False,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,unchanged_prior_DLLs=unchanged,
        full_frontend_missing_services=reports['audit']['genuine_frontend_providers_not_implemented'],
        retail_codec_and_RNA_tests_are_separate=True,retail_SJD_to_RNA_pipeline_verified=False,
        full_ADXT_GDFS_AFS_voice_streaming_or_gameplay_verified=False,English_localization_verified=False,
        original_hardware_timer_or_physical_console_bus_timing_qualified=False,
        combined_SDK_game_binaries_retail_inputs_and_RAM_PCM_captures_publishable=False,
        public_research_build_payloads_changed=False,publication_pending_review=True,playable=False,goal_active=True,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for n,src in files.items():
        target=p/'analysis'/('jp_native_rna_'+n+'.json');assert not target.exists()
        shutil.copyfile(src,target);assert sha(src)==sha(target)
    print(result['status'],result['runtime_checks'],'current scoped checks',flush=True)

if __name__=='__main__':main()
