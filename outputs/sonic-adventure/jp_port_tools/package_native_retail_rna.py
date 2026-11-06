"""Freeze real native-filesystem retail audio evidence without rewriting previous proofs."""
import argparse,ast,hashlib,json,shutil,subprocess
from pathlib import Path
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','verification','earlier','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(a.build/'build_verification.json');v=read(a.verification/'verification.json')
    assert v['runtime_checks']==9159 and v['build_verification_sha256']==sha(a.build/'build_verification.json')
    assert v['actual_original_GDFS_input_verified'] and v['compressed_input_source']=='gdfs_async'
    assert not v['retail_header_rewritten'] and not v['synthetic_end_marker_added'] and not v['full_song_or_gameplay_verified']
    for field,name in [('verifier_sha256','jp_port_tools/verify_native_retail_rna.py'),('oracle_source_sha256','jp_port_tools/adx_step_oracle.py'),
        ('fixture_source_sha256','jp_port_tools/rna_test_runtime.py'),('bit_arithmetic_source_sha256','sh4_binary32.py'),
        ('disc_fixture_source_sha256','jp_port_tools/adx_media_fixtures.py')]:assert v[field]==sha(p/name),field
    cpu,dma=(v['cases'][n] for n in ('cpu','dma'))
    for row in (cpu,dma):
        assert row['prefix_frames']==88192 and row['actual_firmware_rendered_frames']==90112 and row['ring_wraps']==10
        assert row['original_declared_song_frames']==2540372 and len(row['decoder_steps'])==345
        assert row['compressed_input_source']=='gdfs_async' and row['actual_GDFS_observations']['read_end_callbacks']==1
        assert row['actual_GDFS_observations']['native_input_bytes']==100352
        assert sha(row['private_demo_WAV'])==row['private_demo_WAV_sha256']
    assert cpu['PCM_sha256']==dma['PCM_sha256'] and cpu['final_RAM_sha256']==dma['final_RAM_sha256']
    assert cpu['actual_new_DMA_transfers']==0 and dma['actual_new_DMA_transfers']>0
    prior_path=p/'analysis/jp_native_rna_package.json';prior=read(prior_path)
    assert prior['runtime_checks']==443687
    for field in ('authored_source_sha256','proof_files_sha256'):
        for n,h in prior[field].items():assert sha(r/n)==h,n
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for key,value in b.items():
        if isinstance(value,str) and key.endswith('_DLL') and key+'_sha256' in b:assert sha(value)==b[key+'_sha256'],key
    earlier=a.earlier/'verification.json';ev=read(earlier);saved=a.earlier/'verifier_before_GDFS_input.py'
    assert ev['verifier_sha256']==sha(saved) and ev['runtime_checks']==9125
    assert all(ev['cases'][n]['PCM_sha256']==v['cases'][n]['PCM_sha256'] for n in ('cpu','dma'))
    upstream={}
    for name in ('adx_stms','adx_fs','adx_inis','adx_tlk','adx_tlk2','adx_tsvr'):
        n='sdk/sofdec/'+name+'.c';source=a.upstream/n
        assert sha(source)==hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(a.upstream),'show',PIN+':'+n])).hexdigest()
        upstream[n]=sha(source)
    authored=['jp_port_tools/'+n for n in ('adx_step_oracle.py','verify_native_retail_rna.py','package_native_retail_rna.py')]+['JP_NATIVE_RETAIL_RNA.txt']
    for n in authored:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    result=dict(status='bounded_Japanese_title_native_GDFS_SJD_RNA_firmware_CPU_DMA_pipeline_verified_full_frontend_pending',
        runtime_checks=452846,new_retail_pipeline_checks=9159,prior_current_scoped_checks=443687,
        original_GDFS_nonblocking_read_and_callback_verified=True,actual_retail_stereo_frames=88192,actual_firmware_output_frames=90112,
        every_decoded_sample_and_full_sound_rings_independently_verified=True,CPU_DMA_complete_PCM_and_2MiB_RAM_match=True,
        native_production_source_or_build_bytes_changed=False,retail_header_rewritten=False,synthetic_end_marker_added=False,
        full_song_live_retail_WASAPI_ADXT_AFS_or_gameplay_verified=False,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},
        proof_files_sha256={path.resolve().relative_to(r).as_posix():sha(path) for path in
            (a.build/'build_verification.json',a.verification/'verification.json',prior_path,earlier,saved)},
        prior_RNA_package_sha256=sha(prior_path),prior_source_and_proof_hashes_preserved=True,
        earlier_fixture_evidence_preserved_but_not_double_counted=True,next_original_frontend_source_sha256=upstream,
        upstream_pin=PIN,next_production_task='Complete original ADXSTM file streaming in the existing System/sound owner, then ADXF/ADXT with native work buffers and ordered cancellation/cleanup.',
        original_game_source_retail_inputs_RAM_PCM_and_WAV_publishable=False,public_build_payloads_changed=False,
        publication_pending_review=True,goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    for name,src in [('runtime',a.verification/'verification.json'),('package',d/'qualification.json')]:
        target=p/'analysis'/('jp_native_retail_rna_'+name+'.json');assert not target.exists();shutil.copyfile(src,target);assert sha(src)==sha(target)
    print(result['status'],result['runtime_checks'],'current scoped checks',flush=True)

if __name__=='__main__':main()
