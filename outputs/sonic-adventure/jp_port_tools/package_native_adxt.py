"""Bind scoped original ADXT integration to current original and native evidence."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','verification','regressions','inputs','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    scripts={n:'verify_native_'+s+'.py' for n,s in [('pstm','pstm'),('joints','adx_joints'),('decoder','adx_stream_decoder'),('core','adx'),('math','sound_math'),('qsound','qsound'),('clock','sound_clock'),('media','adx_media'),('audio','adx_output'),('rna','rna_mirror'),('retail','retail_rna'),('adxstm','adxstm'),('adxf','adxf')]}
    files={'build':build/'build_verification.json','compile':build/'adxt_compile_attempt.json','runtime':a.verification/'verification.json','inputs':a.inputs/'verification.json','regression_commands':a.regressions/'commands.json'}
    files.update({n:a.regressions/('rna-current' if n=='rna' else n)/'verification.json' for n in scripts})
    files['audit']=a.regressions/'audit/dependency_inventory.json';reports={n:read(f) for n,f in files.items()};b=reports['build'];v=reports['runtime'];bh=sha(files['build'])
    assert len(reports['compile'])==5 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in reports['compile'])
    assert all(x['exit_code']==0 for x in reports['regression_commands'].values())
    assert v['runtime_checks']==40404 and v['build_verification_sha256']==bh and v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_adxt.py')
    assert set(v['cases'])=={'lifecycle','pending','voice_cpu','voice_dma','music_cpu','music_dma','timed'}
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,key in [('external_adxt.c','adxt_generated_source_sha256'),('sound_owner_adxt.c','adxt_owner_generated_source_sha256'),('external_game_sound.c','game_sound_generated_source_sha256'),('include/sofdec.h','adxt_generated_header_sha256')]:assert sha(build/n)==b[key],n
    for n,h in b['rna_generated_sources_sha256'].items():assert sha(build/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'] and len(b['adxt_exports'])==55
    assert sha(files['inputs'])==b['adxt_matching_inputs_proof_sha256']
    for n,script in scripts.items():assert reports[n]['build_verification_sha256']==bh and reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    audit=reports['audit'];assert audit['build_verification_sha256']==bh and audit['script_sha256']==sha(p/'jp_port_tools/audit_native_adx_stream.py')
    assert audit['genuine_frontend_providers_not_implemented']==[] and audit['actual_unresolved_link_symbols']==['adxpd_internal_error']
    for n,total,rate,dmas in [('voice',37975,22050,111),('music',214913,44100,597)]:
        cpu=v['cases'][n+'_cpu']['observations'];dma=v['cases'][n+'_dma']['observations']
        assert cpu['declared_samples']==dma['declared_samples']==total and cpu['sample_rate']==dma['sample_rate']==rate
        assert cpu['complete_retail_stream'] and dma['complete_retail_stream'] and cpu['PCM_sha256']==dma['PCM_sha256']
        assert cpu['independent_decoded_PCM_sha256']==dma['independent_decoded_PCM_sha256']
        assert cpu['new_actual_DMA_transfers']==0 and dma['new_actual_DMA_transfers']==dmas
    prior={}
    for name in ('jp_native_adxf_package.json','jp_native_adxstm_package.json','jp_native_retail_rna_package.json','jp_native_rna_package.json','jp_native_pstm_package.json'):
        path=p/'analysis'/name;old=read(path)
        for field in ('authored_source_sha256','proof_files_sha256'):
            for n,h in old[field].items():assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    unchanged={}
    for key,row in b['adxt_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert row['sha256']==b[key+'_sha256'];unchanged[key]=dict(current=b[key],sha256=row['sha256'])
    base=Path(b['adxt_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    assert sha(base/'build_verification.json')==b['adxt_baseline_sha256']
    for n in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/n)==sha(base/n);unchanged[n]=dict(current=str(build/n),sha256=sha(build/n))
    authored=['jp_port_tools/'+n for n in ('build_native_adxt.py','adxt_observation.inc','verify_native_adxt.py','verify_native_rna_mirror.py','package_native_adxt.py')]+['native/jp_native_adxt.h','native/jp_native_adxt.inc','JP_NATIVE_ADXT.txt']
    for n in authored:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    counts={n:reports[n]['runtime_checks'] for n in scripts};counts['adxt']=v['runtime_checks']
    result=dict(status='original_ADXT_native_frontend_complete_selected_voice_and_music_playback_verified_game_stream_integration_pending',runtime_checks=sum(counts.values()),checks_by_scope=counts,new_adxt_checks=v['runtime_checks'],new_fresh_process_cases=7,original_ADXT_source_functions=54,original_ADXT_and_timer_exports=55,
        real_finite_voice_and_music_headers_unchanged=True,independent_decoded_PCM_and_actual_sound_RAM_verified=True,complete_CPU_DMA_firmware_PCM_equal=True,
        actual_60Hz_AICA_frame_schedule_and_timed_voice_verified=True,partial_constructor_live_lower_record_preservation_and_ordered_global_cleanup_verified=True,
        actual_four_byte_CPU_wrap_mirror_preserves_selected_DMA_mode=True,second_clock_or_output_owner_created=False,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},proof_files_sha256={f.resolve().relative_to(r).as_posix():sha(f) for f in files.values()},
        prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,unchanged_prior_DLLs=unchanged,
        original_stream_missing_ADXT_functions=[],remaining_stream_data_import=['adxpd_internal_error'],original_game_stream_fixed_work_replaced=False,
        next_production_task='Integrate actual original game stream task with native owned music/voice work, shared error data and qualified60Hz scheduling; verify requests, simultaneous playback and lifecycle before looping/other stream cases.',
        original_game_stream_task_looping_simultaneous_streams_and_all_retail_clips_verified=False,English_localization_verified=False,
        combined_SDK_game_binaries_retail_inputs_RAM_PCM_captures_publishable=False,public_build_payloads_changed=False,publication_pending_review=True,goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for n,src in files.items():
        target=p/'analysis'/('jp_native_adxt_'+n+'.json');assert not target.exists();shutil.copyfile(src,target);assert sha(src)==sha(target)
    print(result['status'],result['runtime_checks'],'current scoped checks',flush=True)

if __name__=='__main__':main()
