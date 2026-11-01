"""Qualify three whole originals; canonical reuse and partial pool get no credit."""
import ast,hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    paths=[P/'analysis'/name for name in ('player_work_lifecycle_verification.json','player_work_frontier_verification.json','player_work_lifecycle_retained_verification.json','player_work_lifecycle_unbounded.json')]
    direct,frontier,retained,compiler=[json.loads(p.read_text()) for p in paths]
    files=[Path(__file__),*paths,P/'audit_movie_linked_routines.py',P/'verify_player_work_lifecycle_retained.py',P/'verify_player_work_lifecycle_retained_checked.py']
    for report in (direct,frontier,retained,compiler):
        for path,expected in report.get('sha256',{}).items():assert sha(path)==expected,path;files.append(Path(path))
    for path,expected in compiler['source_sha256'].items():assert sha(path)==expected,path;files.append(Path(path))
    assert (direct['comparisons'],direct['independent_checks'],direct['constructor_cases'])==(267,12172,132)
    assert (frontier['comparisons'],frontier['independent_checks'])==(33,101)
    assert frontier['status_counts']=={'0':21,'-1':6,'-3':6}
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(17,183)
    assert retained['normal_completions']==16 and retained['declared_numeric_frontier_cases']==1
    assert compiler['source_count']==268 and compiler['exit_code']==0 and compiler['original_instruction_rows']==710
    scripts=sorted(P.glob('*player_work_*.py'))
    for script in scripts:ast.parse(script.read_text(encoding='utf-8-sig'),filename=str(script));files.append(script)
    dll=R/'work/sonic-adventure/build/player_work_allocation.dll';files.append(dll)
    assert direct['sha256'][str(dll)]==frontier['sha256'][str(dll)]==retained['dll_sha256']==sha(dll)
    selected={'0x8c06c350','0x8c04b440','0x8c07d67c'};routines=[]
    for prefix in ('player_work_initialize','player_work_dependencies','player_work_allocation'):
        path=P/f'analysis/{prefix}_translation.json';t=json.loads(path.read_text());files.append(path)
        connection=P/f'analysis/{prefix}_connection.json';c=json.loads(connection.read_text());files.append(connection)
        for report in (t,c):
            for path,expected in report['sha256'].items():assert sha(path)==expected,path;files.append(Path(path))
        for body in t['routines']:
            if body['address'] not in selected:continue
            coverage=next(x for x in direct['coverage_by_routine'] if x['address']==body['address'])
            assert not coverage['missing_rows'] and not coverage['missing_outcomes']
            row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
            row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=coverage);routines.append(row)
    assert len(routines)==3 and sum(x['instruction_rows'] for x in routines)==319
    assert sum(x['coverage']['executed_outcomes'] for x in routines)==42
    prior=json.loads((R/'publication/sonic-adventure/outputs/sonic-adventure/analysis/recovered_routines.json').read_text())['routines']
    assert len(prior)==1623 and not any(x['address'] in selected and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in prior)
    linked=inventory(dll,prior+routines);assert linked['confirmed_linked_unique_routines_by_name']==670
    partial=next(x for x in direct['coverage_by_routine'] if x['address']=='0x8c09844a')
    assert len(partial['missing_rows'])==3 and len(partial['missing_outcomes'])==5
    limitations=[
        'Three whole original instruction bodies:06c350(281rows/40outcomes),04b440(20/2),07d67c(18/0). All319rows/42outcomes execute. Original allocation dependencies and call-return state remain real;no supplied allocation outputs or execution frames.',
        '132normal constructors cover eight character kinds,individual/all/clear progression bits,both player indices,four bank/rounding profiles,and both default/nondefault actor modes. Original fixed0x90000-byte heap and4096-task pool initialization precede parent task construction. Host orchestrates the separate calls on carried state;this is not proof of the normal complete boot/caller chain.',
        '33extra comparisons include six raw PR/SZ/bank profiles,two unsupported rounding profiles,four invalid mapping inputs,and declared empty-free-head states. Six numeric and six mapping software stops match. Empty free-head is an input representing exhaustion;no claim of naturally exhausting the heap. These are not physical exceptions. Wide transfer-mode completions are differential evidence,not physical floating-point qualification.',
        '17prior live-record comparisons/183checks replay unchanged assertions through the newDLL,including16normal callbacks andone matched nonfinite FMAC stop. The retained report selects allocation-family metadata for its coverage section;it supplies regression comparisons,not new0777e0 coverage or routine credit.',
        '09068e and five registered heap/task entries are canonical reuse with zero duplicate credit. New flat translations retain their complete original bodies. The separately connected task-pool initializer09844a remains unregistered:111/114rows and11/16outcomes observed;constant-loop/empty-heap obligations remain. Historical original implementations and evidence are preserved.',
        'Constructor allocations lie inside the actual original heap8c7ee8e0..8c87e8e0 and below the declared8cf00000 test stack. This verifies the allocation data flow and disjointness for these runs. It does not establish all retail stack/caller/object/callback aliases or close the two globally unresolved0777e0sort outcomes. Stored child callback0b3a2e is not executed or qualified.',
        '268-sourceWall/Wextra/Werror build and strict unbounded compilation pass;710exported instruction rows byte-match the original RevA input. No new guest wait limits;existing4096host import floor is loaded before the retained suite. First retained launch stopped during host imports before guest execution;the checked entry only fixes load order and preserves all old assertions.',
        'PhysicalFPU/MMU/cache/exception/device behavior,full-game decompilation,playablePC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR andsettings remain unfinished. TextureOff must disable sampling/use vertex-material colours. NoSARecompimplementation imported. Overall10%weightedmilestones,not measured code coverage;goalactive/SolHigh.'
    ]
    result=dict(status='three_complete_original_player_constructor_helpers_software_qualified',routines=routines,new_original_routine_credit=3,newly_qualified_original_rows=319,executed_conditional_outcomes=42,
                fresh_original_native_comparisons=317,fresh_independent_checks=12456,normal_constructor_cases=132,compiled_source_count=268,
                linked_routine_inventory=linked,result_registry_entries=1626,result_unique_module_address_pairs=1623,
                canonical_reused_entries=['0x8c09068e','0x8c098a10','0x8c098a80','0x8c098b0c','0x8c09852e','0x8c0985ec'],canonical_duplicate_credit=0,
                partial_unregistered_task_pool=partial,caller_allocation_alias_contract_globally_proven=False,full_game_qualified=False,
                actual_heap_start=direct['actual_heap_start'],actual_heap_end_exclusive=direct['actual_heap_end_exclusive'],declared_test_stack=direct['declared_caller_stack'],
                new_nonlocal_resume_exports=33,new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=2662,
                supplied_game_callee_outputs=0,supplied_execution_frames=0,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
                next_task='Finish09844a pool initializer qualification and trace actual caller stack/object lifecycles before closing0777e0sort alias premises. Stored0b3a2e remains a separate dependency.',limitations=limitations,
                dll_sha256=sha(dll),sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_work_lifecycle_qualification.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('PLAYER WORK QUALIFIED3originals/319rows/42outcomes;317comparisons/12456checks;670linked',flush=True)
if __name__=='__main__':main()
