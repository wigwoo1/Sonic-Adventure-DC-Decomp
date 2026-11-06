"""Freeze scoped effects qualification; never package SDK/game binaries/assets."""
import argparse,ast,hashlib,json,shutil
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','check','audit','inventory','regressions','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(a.build/'build_verification.json');v=read(a.check/'verification.json');au=read(a.audit/'verification.json');inv=read(a.inventory/'verification.json')
    bh=sha(a.build/'build_verification.json')
    assert v['verifier_sha256']==sha(p/'jp_port_tools/verify_native_effects.py')
    assert au['auditor_sha256']==sha(p/'jp_port_tools/audit_native_effects.py')
    assert inv['inventory_tool_sha256']==sha(p/'jp_port_tools/inventory_native_effect_banks.py')
    for record in (v,au):assert record['build_verification_sha256']==bh
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'] and au['retained_original_functions']==66 and au['new_original_sound_functions']==51
    assert len(v['cases'])==11 and set(v['cases'])=={'lifecycle','banks_cpu','banks_dma','queue_cpu','queue_dma','streams_cpu','streams_dma','reservations','slot_rules','voice_queue','stage_banks'}
    assert v['checks']==sum(c['checks'] for c in v['cases'].values())
    for left,right in (('queue_cpu','queue_dma'),('streams_cpu','streams_dma')):
        for key in ('PCM_sha256','RAM_sha256'):assert v['cases'][left]['observations'][key]==v['cases'][right]['observations'][key]
    assert v['cases']['banks_cpu']['observations']['files']==v['cases']['banks_dma']['observations']['files']
    assert v['cases']['reservations']['observations']['actual_eight_ring_overlap_rejected']
    assert v['cases']['voice_queue']['observations']['voice_archive_entries']==2088 and v['cases']['voice_queue']['observations']['first_voice_effect_index']==594
    assert inv['bank_files']==112 and inv['positive_payloads']==711 and not inv['missing_literal_references']
    assert inv['stream_ring_compatibility'][2]['conflicting_files']==0 and inv['stream_ring_compatibility'][4]['conflicting_files']==109
    commands=read(a.regressions/'commands.json');reports={}
    assert set(commands)=={'pstm','qsound','clock','rna','retail','adxt','game_stream','loops'}
    for name,row in commands.items():
        assert row['exit_code']==0,name;path=a.regressions/name/'verification.json';report=read(path)
        assert report['build_verification_sha256']==bh,name
        reports[name]=dict(path=str(path.resolve()),sha256=sha(path),checks=report.get('runtime_checks',report.get('checks')))
        assert reports[name]['checks']>0,name
    prior_path=p/'analysis/jp_native_game_loops_package.json';prior=read(prior_path)
    assert prior['combined_qualified_scoped_checks']==889018
    for field in ('authored_source_sha256','proof_files_sha256'):
        for n,h in prior[field].items():assert sha(r/n)==h,n
    for record in prior['prior_qualifications_preserved'].values():
        assert sha(r/record['path'])==record['sha256'];q=read(r/record['path'])
        for field in ('authored_source_sha256','proof_files_sha256'):
            for n,h in q[field].items():assert sha(r/n)==h,n
    authored=[p/'native/jp_native_effects.h',p/'native/jp_native_effects.inc',p/'jp_port_tools/build_native_effects.py',p/'jp_port_tools/effects_memory_boundary.inc',p/'jp_port_tools/effects_observation.inc',p/'jp_port_tools/verify_native_effects.py',p/'jp_port_tools/inventory_native_effect_banks.py',p/'jp_port_tools/audit_native_effects.py',Path(__file__).resolve(),p/'JP_NATIVE_EFFECTS.txt',p/'JP_NATIVE_EFFECTS_DRAFT.txt']
    for path in authored:
        assert path.is_file()
        if path.suffix=='.py':ast.parse(path.read_text(encoding='utf-8-sig'))
    evidence={'build':a.build/'build_verification.json','runtime':a.check/'verification.json','audit':a.audit/'verification.json','inventory':a.inventory/'verification.json'}
    evidence.update({'regression_'+n:a.regressions/n/'verification.json' for n in reports})
    proof={};d.mkdir()
    for name,source in evidence.items():
        target=p/'analysis'/('jp_native_effects_'+name+'.json');assert not target.exists()
        shutil.copyfile(source,target);assert sha(source)==sha(target);proof[target.relative_to(r).as_posix()]=sha(target)
    regression_checks=sum(x['checks'] for x in reports.values());new=v['checks']+inv['checks']
    result=dict(status='original_effects_queue_owned_banks_voice_handoff_shared_streams_and_memory_lifetimes_scoped_qualification',
        new_runtime_checks=v['checks'],new_independent_inventory_checks=inv['checks'],new_checks=new,
        prior_reported_qualified_scopes=889018,combined_qualified_scoped_checks=889018+new,selected_prior_regressions_rerun=True,selected_regression_checks=regression_checks,
        regressions=reports,new_original_sound_functions=51,retained_original_sound_functions=66,original_module_functions=68,additional_original_stage_helpers=6,
        real_bank_files=112,real_payloads=711,real_bank_records=2324,all59_literal_sound_bank_references_present=True,
        actual_frame_wait_queue_dispatch_verified=True,original_outer_and_intermediate_queue_callers_audited=True,full_game_main_loop_executed=False,
        real_CPU_DMA_effects_and_complete_music_voice_effects_PCM_and_RAM_equal=True,live_stream_bank_overlap_rejected_both_creation_orders=True,
        actual_original_voice_queue_offset_and_archive_bounds_verified=True,stage_character_selection_explicit_fixtures_verified=True,
        forced_mid_bank_IO_or_DMA_device_failure_verified=False,positive_standalone_shot_sequence_program_file_loads_verified=False,
        all_new_spatial_float_operations_bitwise_SH4_qualified=False,placement_zone_and_orbit_drawing_callbacks_integrated=False,
        additional_missing_graphics_callbacks=au['still_unintegrated_placement_drawing_callbacks'],
        build_verification_sha256=bh,private_sound_DLL_sha256=b['game_sound_DLL_sha256'],
        prior_qualifications_preserved={'game_loops':dict(path=prior_path.relative_to(r).as_posix(),sha256=sha(prior_path))},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},proof_files_sha256=proof,
        original_source_and_previous_proofs_preserved=True,combined_game_SDK_binaries_publishable=False,retail_assets_or_captures_packaged=False,
        public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
    qualification=d/'qualification.json';qualification.write_text(json.dumps(result,indent=2)+'\n')
    target=p/'analysis/jp_native_effects_package.json';assert not target.exists();shutil.copyfile(qualification,target)
    print(result['status'],new,'new scoped checks;',regression_checks,'selected regression checks;',result['combined_qualified_scoped_checks'],'combined qualified scopes',flush=True)
if __name__=='__main__':main()
