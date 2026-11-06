"""Bind current streaming prerequisite proofs and preserve all prior qualified evidence."""
import argparse, hashlib, json, shutil, sys
from pathlib import Path
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path): return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','joints','decoder','core','math','qsound','clock','media','audio','audit','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve(); p=r/'outputs/sonic-adventure'; d=a.output.resolve(); build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files={'build':build/'build_verification.json'}
    files.update({n:getattr(a,n)/('dependency_inventory.json' if n=='audit' else 'verification.json') for n in ('joints','decoder','core','math','qsound','clock','media','audio','audit')})
    reports={n:read(path) for n,path in files.items()}; b=reports['build']; bh=sha(files['build'])
    for n in files:
        if n!='build': assert reports[n]['build_verification_sha256']==bh,n
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items(): assert sha(build/n)==h,n
    assert sha(build/'external_adx_joints.c')==b['joints_generated_source_sha256']
    for n,h in b['stream_decoder_generated_source_sha256'].items(): assert sha(build/n)==h,n
    assert all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'][:7])
    expected={'joints':63993,'decoder':2981,'core':31714,'math':293998,'qsound':18469,'media':60}
    for n,count in expected.items(): assert reports[n]['runtime_checks']==count,n
    assert reports['joints']['complete_original_entry_calls']==4752 and reports['joints']['actual_deferred_DMA_callbacks']==1
    assert reports['decoder']['complete_original_entry_calls']==874 and len(reports['decoder']['cases'])==12
    assert reports['decoder']['native_error_varargs_verified'] and reports['decoder']['native_null_subordinate_creation_rollback_verified']
    assert not reports['joints']['original_function_hooks'] and not reports['decoder']['original_function_hooks']
    assert sum(x['output_backpressure_passes'] for x in reports['decoder']['cases'])==14
    assert sum(x['declared_samples'] for x in reports['decoder']['cases'])==1864
    assert reports['media']['frames_verified']==108680 and reports['audio']['frames']==88200
    assert reports['audio']['media_proof_sha256']==sha(files['media'])
    assert reports['clock']['actual_original_driver_bank_MIDI_and_Windows_output']
    prior={}
    for name in ('jp_native_adx_package.json','jp_native_sound_math_package.json','jp_native_sound_spatial_package.json','jp_native_qsound_package.json','jp_native_sound_clock_package.json'):
        path=p/'analysis'/name; package=read(path)
        for group in ('authored_source_sha256','proof_files_sha256'):
            for n,h in package[group].items(): assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    unchanged={}
    for key,row in b['stream_decoder_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert row['sha256']==b[key+'_sha256'],key
            unchanged[key]=dict(current=b[key],sha256=row['sha256'])
    for name in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/name)==sha(Path(b['stream_decoder_preserved_prior_DLLs']['game_sound_DLL']['path']).parent/name),name
        unchanged[name]=dict(current=str(build/name),sha256=sha(build/name))
    sys.path.insert(0,str(p)); import publish_github_snapshot as publisher
    selected=publisher.qualified_native_builds(r); assert len(selected)==10
    authored=['jp_port_tools/'+n for n in ('build_native_adx_joints.py','verify_native_adx_joints.py','build_native_adx_stream_decoder.py','verify_native_adx_stream_decoder.py','package_native_adx_stream_decoder.py')]+['JP_NATIVE_ADX_STREAM_DECODER.txt']
    total=sum(reports[n]['runtime_checks'] for n in ('joints','decoder','core','math','qsound','clock','media','audio'))
    audit=reports['audit']; assert len(audit['genuine_frontend_providers_not_implemented'])==12
    result=dict(status='original_native_joint_and_SJD_ADXB_ADXPD_stream_decoder_prerequisites_verified_full_frontend_unfinished',
        runtime_checks=total,checks_by_scope={n:reports[n]['runtime_checks'] for n in ('joints','decoder','core','math','qsound','clock','media','audio')},
        new_complete_original_entry_calls=5626,streaming_synthetic_frames=1864,streaming_synthetic_channel_samples=3146,
        retail_codec_regression_frames=108680,actual_output_regression_frames=88200,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,
        original_source_adaptations=b['stream_decoder_source_adaptations'],
        original_source_bodies_adapted=True,legacy_build_original_source_bodies_changed_flag_applies_only_to_joint_stage=True,
        unchanged_prior_DLLs=unchanged,public_support_artifacts=[dict(path=rel,bytes=len(data),sha256=hashlib.sha256(data).hexdigest()) for path,rel,data in selected],
        public_support_artifact_count=10,full_frontend_missing_services=audit['genuine_frontend_providers_not_implemented'],
        implemented_error_flag_requires_colocation_or_data_import=True,
        full_ADXT_GDFS_RNA_PCM_game_streaming_verified=False,WAV_or_SPSD_playback_qualified=False,
        malformed_media_or_raw_pointer_safety_qualified=False,physical_console_interrupt_timing_qualified=False,
        combined_original_SDK_game_binaries_or_retail_assets_publishable=False,
        earlier_graphics_save_full_owner_exit_proofs_retained_not_rerun=True,
        publication_pending_review=True,goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir(); (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    files['package']=d/'qualification.json'
    for name,src in files.items():
        target=p/'analysis'/('jp_native_adx_stream_decoder_'+name+'.json')
        assert not target.exists(); shutil.copyfile(src,target); assert sha(src)==sha(target)
    print(result['status'],total,'current checks',flush=True)
if __name__=='__main__': main()
