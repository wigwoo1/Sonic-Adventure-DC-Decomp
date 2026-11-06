"""Bind original ADXF archive integration and current native regression evidence."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','verification','regressions','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files={'build':build/'build_verification.json','compile':build/'adxf_compile_attempt.json','runtime':a.verification/'verification.json',
        'regression_commands':a.regressions/'commands.json'}
    scripts={'pstm':'verify_native_pstm.py','joints':'verify_native_adx_joints.py','decoder':'verify_native_adx_stream_decoder.py',
        'core':'verify_native_adx.py','math':'verify_native_sound_math.py','qsound':'verify_native_qsound.py','clock':'verify_native_sound_clock.py',
        'media':'verify_native_adx_media.py','audio':'verify_native_adx_output.py','rna':'verify_native_rna.py','retail':'verify_native_retail_rna.py','adxstm':'verify_native_adxstm.py'}
    files.update({n:a.regressions/n/'verification.json' for n in scripts});files['audit']=a.regressions/'audit/dependency_inventory.json'
    reports={n:read(path) for n,path in files.items()};b=reports['build'];bh=sha(files['build']);v=reports['runtime']
    assert len(reports['compile'])==5 and all(x['exit_code']==x['diagnostic_bytes']==0 for x in reports['compile'])
    assert all(x['exit_code']==0 for x in reports['regression_commands'].values())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert sha(build/'external_adx_fs.c')==b['adxf_generated_source_sha256']
    assert sha(build/'sound_owner_adxf.c')==b['adxf_owner_generated_source_sha256']
    assert sha(build/'external_adx_stms.c')==b['adxstm_generated_source_sha256']
    assert sha(build/'external_game_sound.c')==b['pstm_generated_source_sha256']
    base=Path(b['adxf_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    assert sha(base/'build_verification.json')==b['adxf_baseline_sha256']
    assert (build/'external_adx_stms.c').read_bytes()==(base/'external_adx_stms.c').read_bytes()+b'\n'+(p/'jp_port_tools/adxstm_adxf_bridge.inc').read_bytes()
    assert v['runtime_checks']==4627 and v['fresh_process_cases']==6 and v['build_verification_sha256']==bh
    assert v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_adxf.py')
    for key,name in [('fixture_source_sha256','rna_test_runtime.py'),('ring_fixture_source_sha256','verify_native_adxstm.py'),('disc_fixture_source_sha256','adx_media_fixtures.py')]:assert v[key]==sha(p/'jp_port_tools'/name)
    assert v['cases']['archive']['observations']['real_voice_archive_entries']==2088
    assert v['cases']['archive']['observations']['every_independent_range_verified'] and v['cases']['overlay']['observations']['real_added_archive_entries']==2088
    assert v['cases']['pool']['observations']['live_unrelated_lower_stream_preserved_across_init_and_shutdown']
    assert v['cases']['reads']['observations']['open_file_survives_partition_unload']
    clips=v['cases']['reads']['observations']['actual_voice_reads'];assert [x['index'] for x in clips]==[0,1,2,2087]
    assert sum(x['real_entry_bytes'] for x in clips)==477558 and sum(x['compared_sector_bytes'] for x in clips)==483328
    for n,script in scripts.items():assert reports[n]['build_verification_sha256']==bh and reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    assert reports['audit']['build_verification_sha256']==bh and reports['audit']['script_sha256']==sha(p/'jp_port_tools/audit_native_adx_stream.py')
    assert len(reports['audit']['genuine_frontend_providers_not_implemented'])==11 and 'ADXF_LoadPartition' not in reports['audit']['actual_unresolved_link_symbols']
    prior={}
    for name in ('jp_native_adxstm_package.json','jp_native_retail_rna_package.json','jp_native_rna_package.json','jp_native_pstm_package.json'):
        path=p/'analysis'/name;old=read(path)
        for field in ('authored_source_sha256','proof_files_sha256'):
            for n,h in old[field].items():assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    unchanged={}
    for key,row in b['adxf_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):
            assert row['sha256']==b[key+'_sha256'];unchanged[key]=dict(current=b[key],sha256=row['sha256'])
    for name in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/name)==sha(base/name);unchanged[name]=dict(current=str(build/name),sha256=sha(build/name))
    authored=['jp_port_tools/'+n for n in ('build_native_adxf.py','adxstm_adxf_bridge.inc','adxf_observation.inc','verify_native_adxf.py','package_native_adxf.py')]
    authored+=['native/jp_native_adxf.h','native/jp_native_adxf.inc','JP_NATIVE_ADXF.txt']
    for n in authored:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    counts={n:reports[n]['runtime_checks'] for n in scripts};counts['adxf']=v['runtime_checks']
    result=dict(status='complete_original_ADXF_native_AFS_partition_ranges_clip_reads_and_owned_lifetime_verified_ADXT_frontend_pending',
        runtime_checks=sum(counts.values()),checks_by_scope=counts,new_adxf_checks=4627,new_fresh_process_cases=6,genuine_original_ADXF_exports=20,
        all_2088_Japanese_voice_ranges_verified=True,all_2088_added_archive_ranges_verified=True,selected_voice_content_bytes_verified=477558,selected_voice_sector_bytes_verified=483328,
        actual_partition_rollback_replace_unload_and_open_file_lifetime_verified=True,live_lower_streams_preserved_and_parent_cleanup_order_verified=True,
        actual_timed_archive_read_and_pending_System_cleanup_verified=True,second_clock_or_catalog_or_output_owner_created=False,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,unchanged_prior_DLLs=unchanged,
        full_frontend_missing_services=reports['audit']['genuine_frontend_providers_not_implemented'],
        next_production_task='Complete ADXT init/talk/settings/server ownership in this same sound/System graph, preserving active pools and replacing fixed game music/voice work storage.',
        audible_ADXT_voice_full_song_looping_or_gameplay_verified=False,English_localization_verified=False,
        combined_SDK_game_binaries_retail_inputs_and_clip_captures_publishable=False,public_build_payloads_changed=False,
        publication_pending_review=True,goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for name,src in files.items():
        target=p/'analysis'/('jp_native_adxf_'+name+'.json');assert not target.exists();shutil.copyfile(src,target);assert sha(src)==sha(target)
    print(result['status'],result['runtime_checks'],'current scoped checks',flush=True)

if __name__=='__main__':main()
