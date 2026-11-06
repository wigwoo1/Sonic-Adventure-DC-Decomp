"""Bind actual retail loop/header qualification to the unchanged native graph."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
from build_native_texture_runtime import function
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','verification','inventory','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();build=a.build.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    files={'runtime':a.verification/'verification.json','inventory':a.inventory/'verification.json','build':build/'build_verification.json'}
    v,inventory,b=(read(files[n]) for n in ('runtime','inventory','build'));bh=sha(files['build'])
    assert v['runtime_checks']==323947 and inventory['runtime_checks']==755
    assert v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_game_loops.py') and inventory['verifier_sha256']==sha(p/'jp_port_tools/inventory_native_game_loops.py')
    assert v['build_verification_sha256']==inventory['build_verification_sha256']==bh
    assert set(v['cases'])=={'cpu','dma','restart','timed'}
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    unchanged={}
    for key,path in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(path,str) and path.endswith('.dll'):
            assert sha(path)==b[key+'_sha256'];unchanged[key]=dict(path=path,sha256=b[key+'_sha256'])
    original=a.upstream/'sdk/sofdec/adx_tsvr.c';assert sha(original)==b['adxt_original_source_sha256']['sdk/sofdec/adx_tsvr.c']
    raw=(build/'external_adxt.c').read_bytes();bodies={}
    for n in ('adxt_trap_entry','adxt_eos_entry'):
        before,after=function(original.read_bytes(),n),function(raw,n);assert before==after
        bodies[n]=hashlib.sha256(before).hexdigest()
    cpu,dma=(v['cases'][n]['observations'] for n in ('cpu','dma'))
    for n in ('PCM_sha256','RAM_sha256','independent_PCM_sha256','firmware_frames'):assert cpu[n]==dma[n],n
    assert cpu['actual_DMA_transfers']==0 and dma['actual_DMA_transfers']>0
    for row in (cpu,dma):
        assert [x['cumulative_samples'] for x in row['trap_events']]==[1194788,2253224]
        assert len(row['actual_EOS_seeks'])>=2 and all(x['to_sector']==75 for x in row['actual_EOS_seeks'])
        assert row['actual_output_capacity_pressure'] and row['independent_decoder_steps']>0 and row['unmodified_source']
    assert v['cases']['restart']['observations']['boundary_restart_verified'] and v['cases']['timed']['observations']['timed_automatic']
    assert inventory['music_files']==109 and inventory['looping_tracks']==79 and inventory['declared_music_IDs']==110 and inventory['missing_declared_music_files']==['NIGHTS_K.ADX']
    prior={}
    for name in ('jp_native_game_stream_package.json','jp_native_adxt_package.json','jp_native_adxf_package.json','jp_native_adxstm_package.json','jp_native_retail_rna_package.json','jp_native_rna_package.json','jp_native_pstm_package.json'):
        path=p/'analysis'/name;old=read(path)
        for field in ('authored_source_sha256','proof_files_sha256'):
            for n,h in old[field].items():assert sha(r/n)==h,n
        prior[name]=dict(path=path.relative_to(r).as_posix(),sha256=sha(path))
    base=read(p/'analysis/jp_native_game_stream_package.json');assert base['runtime_checks']==564316
    for row in read(p/'analysis/jp_native_game_stream_audit.json')['actual_linked_objects']:assert sha(row['path'])==row['sha256']
    authored=['jp_port_tools/'+n for n in ('verify_native_game_loops.py','inventory_native_game_loops.py','package_native_game_loops.py')]+['JP_NATIVE_GAME_LOOPS.txt']
    for n in authored:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    result=dict(status='original_game_retail_music_loop_EOS_trap_pressure_restart_and_timed_playback_verified_effect_queue_pending',new_loop_runtime_checks=v['runtime_checks'],new_header_inventory_checks=inventory['runtime_checks'],new_checks=v['runtime_checks']+inventory['runtime_checks'],prior_unchanged_graph_qualified_checks=base['runtime_checks'],combined_qualified_scoped_checks=base['runtime_checks']+v['runtime_checks']+inventory['runtime_checks'],new_fresh_process_cases=4,
        production_code_objects_and_DLLs_changed=False,C_rebuild_required=False,prior_regressions_rerun=False,reason_prior_proofs_remain_applicable='Exact qualified production source, object and DLL hashes are unchanged.',unchanged_DLLs=unchanged,unchanged_original_loop_callback_body_sha256=bodies,
        two_full_unmodified_retail_loops_and_continuous_decoder_history_verified=True,complete_CPU_DMA_PCM_and_full_RAM_equal=True,actual_original_EOS_seek_and_trap_contexts_verified=True,natural_output_capacity_limits_and_boundary_restart_verified=True,actual_existing_timed_output_owner_reused=True,
        declared_music_IDs=110,present_standalone_music_files=109,looping_metadata_tracks=79,missing_declared_music_files=['NIGHTS_K.ADX'],all79_tracks_runtime_verified=False,
        authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in authored},proof_files_sha256={f.resolve().relative_to(r).as_posix():sha(f) for f in files.values()},prior_qualifications_preserved=prior,all_prior_qualified_source_and_proof_hashes_preserved=True,
        next_production_task='Integrate complete original effects request/slot queue, sound_send_queue and actual bank loading with existing stream/MIDI/QSound/native owners; audit already qualified renamed providers, then verify actual effect-bank playback and shared lifetimes.',
        full_game_effects_audio_English_or_gameplay_verified=False,retail_audio_assets_or_combined_game_SDK_binaries_publishable=False,public_build_payloads_changed=False,publication_pending_review=True,goal_active=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');files['package']=d/'qualification.json'
    for n,src in files.items():
        dst=p/'analysis'/('jp_native_game_loops_'+n+'.json');assert not dst.exists();shutil.copyfile(src,dst);assert sha(src)==sha(dst)
    print(result['status'],result['combined_qualified_scoped_checks'],'combined qualified scoped checks',flush=True)
if __name__=='__main__':main()
