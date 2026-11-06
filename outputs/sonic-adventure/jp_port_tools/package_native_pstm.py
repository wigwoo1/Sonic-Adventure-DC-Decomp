"""Bind the real PCM-port graph and regression evidence; retain prior proofs."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path): return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','pstm','joints','decoder','core','math','qsound','clock','media','audio','audit','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve(); p=r/'outputs/sonic-adventure'; build=a.build.resolve(); d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files={'build':build/'build_verification.json'}
    files.update({n:getattr(a,n)/('dependency_inventory.json' if n=='audit' else 'verification.json') for n in ('pstm','joints','decoder','core','math','qsound','clock','media','audio','audit')})
    reports={n:read(path) for n,path in files.items()}; b=reports['build']; bh=sha(files['build'])
    for n in files:
        if n!='build': assert reports[n]['build_verification_sha256']==bh,n
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    assert sha(build/'external_game_sound.c')==b['pstm_generated_source_sha256']
    assert all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'][:2])
    for n,h in b['pstm_private_matching_data_sha256'].items(): assert sha(build/n)==h,n
    v=reports['pstm']; assert v['runtime_checks']==22957 and v['original_complete_entry_calls']==4227
    assert v['cases']['contracts']['contract_part']=='all' and len(v['cases'])==5 and not v['original_function_hooks']
    assert v['cases']['contracts']['observations']['controlled_real_queue_count_fixture_reset_between_independent_batches']
    raw=v['cases']['driver_reference']['observations']
    for n in ('driver_cpu','driver_dma'):
        item=v['cases'][n]['observations']; assert item['PCM_frames']==26460 and item['nonzero_PCM']
        assert item['PCM_sha256']==raw['PCM_sha256'] and item['RAM_sha256']==raw['RAM_sha256']
    assert not v['cases']['timed']['observations']['second_output_client_created']
    assert v['cases']['timed']['observations']['actual_stream_sample_frames']>1000
    script_map={'pstm':'verify_native_pstm.py','joints':'verify_native_adx_joints.py','decoder':'verify_native_adx_stream_decoder.py','core':'verify_native_adx.py','math':'verify_native_sound_math.py','qsound':'verify_native_qsound.py','clock':'verify_native_sound_clock.py','media':'verify_native_adx_media.py','audio':'verify_native_adx_output.py'}
    for n,script in script_map.items(): assert reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    assert reports['media']['frames_verified']==108680 and reports['audio']['frames']==88200
    assert reports['audio']['media_proof_sha256']==sha(files['media'])
    prior={}
    for name in ('jp_native_adx_stream_decoder_package.json','jp_native_adx_package.json','jp_native_sound_math_package.json','jp_native_sound_spatial_package.json','jp_native_qsound_package.json','jp_native_sound_clock_package.json'):
        path=p/'analysis'/name; package=read(path)
        for group in ('authored_source_sha256','proof_files_sha256'):
            for n,h in package[group].items(): assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    unchanged={}
    for key,row in b['pstm_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert row['sha256']==b[key+'_sha256'],key
            unchanged[key]=dict(current=b[key],sha256=row['sha256'])
    base=Path(b['pstm_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    for n in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/n)==sha(base/n),n
        unchanged[n]=dict(current=str(build/n),sha256=sha(build/n))
    sys.path.insert(0,str(p)); import publish_github_snapshot as publisher
    selected=publisher.qualified_native_builds(r); assert len(selected)==10
    authored=['jp_port_tools/'+n for n in ('build_native_pstm.py','pstm_observation.inc','verify_native_pstm.py','pstm_reference_bus.py','package_native_pstm.py')]+['JP_NATIVE_PSTM.txt']
    total=sum(reports[n]['runtime_checks'] for n in script_map); assert total==438234
    audit=reports['audit']; assert len(audit['genuine_frontend_providers_not_implemented'])==12
    result=dict(status='original_PCM_stream_port_family_MIDI_status_actual_firmware_and_shared_native_output_qualified_frontend_unfinished',
        runtime_checks=total,checks_by_scope={n:reports[n]['runtime_checks'] for n in script_map},new_complete_original_entry_calls=4227,
        original_PCM_port_exports=25,original_MIDI_status_exports=11,actual_PSTM_port_capacity=4,
        independent_PCM_test_frames=26460,independent_raw_CPU_DMA_full_RAM_and_PCM_match=True,
        single_existing_timed_WASAPI_output_verified=True,actual_System_delegated_shutdown_verified=True,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,
        unchanged_prior_DLLs=unchanged,public_support_artifacts=[dict(path=rel,bytes=len(data),sha256=hashlib.sha256(data).hexdigest()) for path,rel,data in selected],
        public_support_artifact_count=10,full_frontend_missing_services=audit['genuine_frontend_providers_not_implemented'],
        original_G2_contract_uses_explicit_static_sound_RAM_DMA_idle_and_FIFO_empty_fixtures=True,
        original_function_hooks=False,physical_console_bus_timing_or_waveform_qualified=False,
        full_ADXT_RNA_GDFS_AFS_music_voice_streaming_or_gameplay_verified=False,
        combined_SDK_game_binaries_or_retail_data_publishable=False,publication_pending_review=True,
        goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir(); (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n'); files['package']=d/'qualification.json'
    for name,src in files.items():
        target=p/'analysis'/('jp_native_pstm_'+name+'.json'); assert not target.exists()
        shutil.copyfile(src,target); assert sha(src)==sha(target)
    print(result['status'],total,'current checks',flush=True)
if __name__=='__main__': main()
