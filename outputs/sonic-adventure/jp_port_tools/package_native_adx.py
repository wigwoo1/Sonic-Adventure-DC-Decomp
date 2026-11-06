"""Bind native ADX/retail PCM/real output proofs without publishing retail media."""
import argparse,hashlib,json,shutil,sys
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','media','audio','math','qsound','clock','audit','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files=dict(build=build/'build_verification.json',verification=a.check/'verification.json',media=a.media/'verification.json',audio=a.audio/'verification.json',math=a.math/'verification.json',qsound=a.qsound/'verification.json',clock=a.clock/'verification.json',audit=a.audit/'dependency_inventory.json')
    reports={n:read(path) for n,path in files.items()};b=reports['build'];v=reports['verification'];bh=sha(files['build'])
    for n in ('verification','media','audio','math','qsound','clock','audit'):assert reports[n]['build_verification_sha256']==bh,n
    assert v['runtime_checks']==31714 and not v['original_function_hooks'] and v['all_8192_mono_scales_independent'] and v['all_256_code_bytes']
    assert v['default_500Hz_44100Hz_coefficients']==[7334,-3283] and v['native_expander_pointer_bits']==64
    assert reports['media']['runtime_checks']==60 and reports['media']['frames_verified']==108680 and len(reports['media']['assets'])==6
    assert reports['media']['voice_archive_file_count']==2088 and reports['media']['complete_original_entry_calls']==12
    assert reports['media']['core_proof_sha256']==sha(files['verification'])
    assert reports['audio']['media_proof_sha256']==sha(files['media']) and reports['audio']['frames']==88200
    out=reports['audio']['actual_output'];assert out['submitted']==out['consumed']==88200 and not out['discarded'] and not out['queued_frames']
    assert out['source_hash']==reports['audio']['actual_delivered_float_FNV'] and reports['audio']['only_unique_session_muted']
    assert reports['math']['runtime_checks']==293998 and reports['qsound']['runtime_checks']>18000 and reports['clock']['runtime_checks']>3000
    assert all(not x['exit_code'] and not x['diagnostic_bytes'] for x in b['commands'][:9])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(build/n)==h,n
    assert sha(build/'external_adx.c')==b['ADX_generated_source_sha256'] and sha(build/'adx_runtime_constants.h')==b['ADX_constants_sha256']
    assert sha(build/'external_game_sound.c')==b['sound_math_generated_source_sha256']
    old_path=p/'analysis/jp_native_sound_math_package.json';old=read(old_path)
    for name in ('jp_native_sound_math_package.json','jp_native_sound_spatial_package.json','jp_native_qsound_package.json','jp_native_sound_clock_package.json'):
        for group in ('authored_source_sha256','proof_files_sha256'):
            for n,h in read(p/'analysis'/name)[group].items():assert sha(r/n)==h,n
    base_file=next(r/n for n in old['proof_files_sha256'] if n.endswith('/build_verification.json') and sha(r/n)==b['ADX_baseline_sha256'])
    unchanged={}
    for key,row in b['ADX_preserved_prior_DLLs'].items():
        assert sha(row['path'])==row['sha256'] and sha(b[key])==b[key+'_sha256'],key
        if key not in ('game_sound_DLL','sound_SDK_DLL'):assert b[key+'_sha256']==row['sha256'];unchanged[key]=row
    for n in ('jp_native_g2_audio.dll','jp_native_aica.dll','jp_native_audio.dll'):
        assert sha(build/n)==sha(base_file.parent/n);unchanged[n]=dict(path=str(base_file.parent/n),sha256=sha(build/n))
    for n,script in (('verification','verify_native_adx.py'),('media','verify_native_adx_media.py'),('audio','verify_native_adx_output.py'),('math','verify_native_sound_math.py'),('qsound','verify_native_qsound.py'),('clock','verify_native_sound_clock.py')):
        assert reports[n]['verifier_sha256']==sha(p/'jp_port_tools'/script),n
    assert reports['media']['fixture_reader_sha256']==sha(p/'jp_port_tools/adx_media_fixtures.py')
    wav=Path(reports['media']['private_decoder_demo_WAV']);assert sha(wav)==reports['media']['private_decoder_demo_WAV_sha256']==reports['audio']['private_decoder_WAV_sha256']
    audit=reports['audit'];assert audit['compile_exit_code']==audit['compile_diagnostic_bytes']==0 and audit['expected_incomplete_link_exit_code']!=0
    assert len(audit['actual_unresolved_link_symbols'])==13 and len(audit['genuine_frontend_providers_not_implemented'])==12
    assert audit['implemented_core_state_needing_same_module_or_explicit_data_import']==['adxpd_internal_error']
    assert not audit['original_stream_module_linked_or_executed'] and not audit['fixed_work_replaced'] and not audit['dummy_ADXT_or_stream_services']
    assert audit['script_sha256']==sha(p/'jp_port_tools/audit_native_adx_stream.py')
    assert sha(a.audit/'external_original_stream.c')==audit['generated_source_sha256'] and sha(a.audit/'stream.obj')==audit['object_sha256']
    sys.path.insert(0,str(p));from publish_github_snapshot import qualified_native_builds
    selected=[dict(path=rel,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()) for _,rel,raw in qualified_native_builds(r)]
    assert selected==old['public_build_selection'] and len(selected)==10
    authored=['native/jp_native_adx_numeric.c','native/jp_native_adx_numeric.h','jp_port_tools/adx_observation.inc','jp_port_tools/build_native_adx.py','jp_port_tools/verify_native_adx.py','jp_port_tools/adx_media_fixtures.py','jp_port_tools/verify_native_adx_media.py','jp_port_tools/verify_native_adx_output.py','jp_port_tools/audit_native_adx_stream.py','jp_port_tools/package_native_adx.py','JP_NATIVE_ADX.txt']
    support=['source/sh4_float_i32.c','source/sh4_float_i32.h','source/sh4_truncate_i32.c','source/stage00_task.h','source/level_variant.h','third_party/skmp-sh4-fpu/sh4_fpu_approx.h','third_party/skmp-sh4-fpu/README.txt','sh4_binary32.py','verify_startup.py','jp_port_tools/model_reference.py','jp_port_tools/primitive_reference.py','jp_port_tools/camera_reference_numeric.py','jp_port_tools/verify_native_audio.py']
    total=sum(reports[n]['runtime_checks'] for n in ('verification','media','audio','math','qsound','clock'))
    result=dict(status='native_original_ADX_codec_expander_real_JP_music_voice_and_actual_PCM_output_qualified_live_streaming_unfinished',runtime_checks=total,
        core_ADX_checks=v['runtime_checks'],retail_media_checks=60,actual_output_checks=reports['audio']['runtime_checks'],
        current_math_checks=reports['math']['runtime_checks'],current_QSound_checks=reports['qsound']['runtime_checks'],current_clock_checks=reports['clock']['runtime_checks'],
        complete_original_core_entry_calls=v['complete_original_entry_calls'],complete_original_retail_entry_calls=12,retail_frames_verified=108680,actual_WASAPI_frames=88200,Japanese_voice_archive_file_count=2088,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},reference_support_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in support},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in files.values()},
        prior_qualification=dict(path=str(old_path),sha256=sha(old_path)),all_prior_qualified_source_and_proofs_preserved=True,
        unchanged_prior_DLLs=unchanged,public_build_selection=selected,live_stream_frontend_missing_providers=audit['genuine_frontend_providers_not_implemented'],
        decoded_WAV_retail_payload_or_combined_SDK_game_binaries_published=False,original_track_preserved=True,
        stream_work_ownership_real_GDFS_ADXT_RNA_scheduling_looping_or_main_gameplay_verified=False,
        earlier_graphics_save_owner_spatial_proofs_retained_not_rerun=True,publication_pending_review=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for name,source in files.items():
        target=p/'analysis'/('jp_native_adx'+('' if name=='verification' else '_'+name)+'.json');assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],total,'current checks;108680 retail frames;88200 actual output frames',flush=True)
if __name__=='__main__':main()
