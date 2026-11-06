"""Bind full original game stream integration to actual tasks/audio and prior proofs."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','verification','regressions','audit','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    scripts={n:'verify_native_'+s+'.py' for n,s in [('pstm','pstm'),('joints','adx_joints'),('decoder','adx_stream_decoder'),('core','adx'),('math','sound_math'),('qsound','qsound'),('clock','sound_clock'),('media','adx_media'),('audio','adx_output'),('rna','rna_mirror'),('retail','retail_rna'),('adxstm','adxstm'),('adxf','adxf'),('adxt','adxt')]}
    files={'build':build/'build_verification.json','compile':build/'game_stream_compile_attempt.json','runtime':a.verification/'verification.json','audit':a.audit/'verification.json','regression_commands':a.regressions/'commands.json'}
    files.update({n:a.regressions/n/'verification.json' for n in scripts});reports={n:read(f) for n,f in files.items()};b=reports['build'];v=reports['runtime'];audit=reports['audit'];bh=sha(files['build'])
    assert len(reports['compile'])==4 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in reports['compile'])
    assert all(x['exit_code']==0 for x in reports['regression_commands'].values())
    assert v['runtime_checks']==42706 and v['build_verification_sha256']==bh and v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_game_stream.py')
    assert set(v['cases'])=={'lifecycle','requests','pool_failure','mix_cpu','mix_dma','interlude','global_pending','timed'}
    assert audit['build_verification_sha256']==bh and audit['auditor_sha256']==sha(p/'jp_port_tools/audit_native_game_stream.py')
    assert audit['original_functions']==24 and audit['original_exported_entries']==19 and len(audit['actual_unique_ADXPD_error_definition'])==1
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,key in [('external_game_stream.c','game_stream_generated_source_sha256'),('sound_owner_stream.c','game_stream_owner_generated_source_sha256'),('external_adx_fs.c','game_stream_adxf_generated_source_sha256')]:assert sha(build/n)==b[key],n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    for n,script in scripts.items():assert reports[n]['build_verification_sha256']==bh and reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    cpu=v['cases']['mix_cpu']['observations'];dma=v['cases']['mix_dma']['observations']
    for key in ('PCM_sha256','RAM_sha256','independent_PCM_sha256','decoded_samples','firmware_frames'):assert cpu[key]==dma[key],key
    assert cpu['decoded_samples']==[214913,37975] and cpu['firmware_frames']==218368
    assert cpu['real_DMA_transfers']==0 and dma['real_DMA_transfers']==711
    assert cpu['simultaneous_play_observations']>0 and v['cases']['interlude']['observations']['natural_interlude_resume']
    assert v['cases']['timed']['observations']['automatic_elapsed_time']
    assert v['cases']['global_pending']['observations']['actual_pending_read_and_failed_external_task_cleanup_retried_by_System']
    prior={}
    for name in ('jp_native_adxt_package.json','jp_native_adxf_package.json','jp_native_adxstm_package.json','jp_native_retail_rna_package.json','jp_native_rna_package.json','jp_native_pstm_package.json'):
        path=p/'analysis'/name;old=read(path)
        for field in ('authored_source_sha256','proof_files_sha256'):
            for n,h in old[field].items():assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    solo=read(p/'analysis/jp_native_adxt_runtime.json')['cases']
    assert cpu['independent_PCM_sha256']==[solo['music_cpu']['observations']['independent_decoded_PCM_sha256'],solo['voice_cpu']['observations']['independent_decoded_PCM_sha256']]
    unchanged={}
    for key,row in b['game_stream_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert row['sha256']==b[key+'_sha256'];unchanged[key]=dict(current=b[key],sha256=row['sha256'])
    base=Path(b['game_stream_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    assert sha(base/'build_verification.json')==b['game_stream_baseline_sha256']
    for n in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/n)==sha(base/n);unchanged[n]=dict(current=str(build/n),sha256=sha(build/n))
    authored=['jp_port_tools/'+n for n in ('build_native_game_stream.py','game_stream_observation.inc','verify_native_game_stream.py','audit_native_game_stream.py','package_native_game_stream.py')]+['native/jp_native_game_stream.h','native/jp_native_game_stream.inc','JP_NATIVE_GAME_STREAM.txt']
    for n in authored:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    counts={n:reports[n]['runtime_checks'] for n in scripts};counts['game_stream']=v['runtime_checks']
    result=dict(status='complete_original_game_music_voice_task_native_requests_concurrent_playback_interlude_and_lifetimes_verified_looping_pending',runtime_checks=sum(counts.values()),checks_by_scope=counts,new_game_stream_checks=v['runtime_checks'],new_fresh_process_cases=8,original_game_stream_functions=24,original_game_stream_exports=19,
        actual_original_task_scheduler_and_children_used=True,original_task_heap_or_pool_reinitialized=False,native_owned_music_voice_and_partition_work_verified=True,
        actual_unique_original_decoder_error_flag_verified=True,real_full_simultaneous_voice_and_music_verified=True,independent_decoded_PCM_matches_prior_solo_qualification=True,
        complete_CPU_DMA_firmware_PCM_and_full_AICA_RAM_equal=True,natural_full_interlude_resume_verified=True,pending_failed_task_cleanup_survives_heap_reset_and_System_retry=True,
        existing_60Hz_ADXT_audio_service_and_unique_output_worker_reused=True,authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},proof_files_sha256={f.resolve().relative_to(r).as_posix():sha(f) for f in files.values()},
        prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,unchanged_prior_DLLs=unchanged,original_game_stream_fixed_work_replaced=True,original_game_stream_link_complete=True,
        next_production_task='Qualify actual retail looping music through original game task, ADXT trap and EOS-seek/refill path, including boundary stop/restart and retained ownership; continue full game sound integration.',
        looping_all_retail_clips_full_game_audio_and_gameplay_verified=False,English_localization_verified=False,
        combined_SDK_game_binaries_retail_inputs_RAM_PCM_captures_publishable=False,public_build_payloads_changed=False,publication_pending_review=True,goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for n,src in files.items():
        target=p/'analysis'/('jp_native_game_stream_'+n+'.json');assert not target.exists();shutil.copyfile(src,target);assert sha(src)==sha(target)
    print(result['status'],result['runtime_checks'],'current scoped checks',flush=True)
if __name__=='__main__':main()
