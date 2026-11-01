"""Qualify initializer/query and reuse two canonical original helpers without duplicate credit."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_child_callback_qualification.json';assert not target.exists()
    paths=[P/f'analysis/player_child_callback_{x}.json' for x in ('verification','allocated_verification','dependencies_verification','compile_checks')]
    direct,allocated,deps,compiler=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
    for report in (direct,allocated,deps,compiler):
        for p,value in report['sha256'].items():assert sha(p)==value,p;files.append(Path(p))
    for p,value in compiler['source_sha256'].items():assert sha(p)==value,p;files.append(Path(p))
    assert (direct['original_native_comparisons'],direct['checks'],direct['independent_exact_oracles'])==(88,983,292)
    assert (allocated['original_native_comparisons'],allocated['checks'],allocated['allocated_child_lifecycles'])==(50,326,16)
    assert (deps['original_native_comparisons'],deps['checks'],deps['canonical_duplicate_credit'])==(5,37,0)
    assert compiler['source_count']==276 and compiler['compiler_exit_code']==0 and compiler['original_instruction_rows_byte_checked']==74
    dll=R/'work/sonic-adventure/build/player_child_callback_dependencies.dll';digest=sha(dll)
    assert all(x['dll_sha256']==digest for x in (direct,allocated,deps))
    mp=P/'analysis/player_child_callback_translation.json';t=json.loads(mp.read_text());files.append(mp);routines=[]
    for b in t['routines']:
        cov=next(x for x in direct['coverage_by_routine'] if x['address']==b['address'])
        assert cov['executed_rows']==b['instruction_rows'] and not cov['missing_rows'] and not cov['unresolved_outcomes']
        assert cov['executed_outcomes']+len(cov['proven_infeasible_outcomes'])==len(b['conditional_edges'])
        row={k:b[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=b['instruction_rows'],coverage=cov);routines.append(row)
    assert len(routines)==2 and sum(x['instruction_rows'] for x in routines)==68
    assert sum(x['coverage']['executed_outcomes'] for x in routines)==7
    assert sum(len(x['coverage']['proven_infeasible_outcomes']) for x in routines)==1
    prior=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1634];assert len(prior)==1634
    assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address'] in ('0x8c0b3a2e','0x8c07ce40') for x in prior)
    linked=inventory(dll,prior+routines);assert linked['confirmed_linked_unique_routines_by_name']==680
    limitations=[
        'Complete56-row child initializer0b3a2eand12-row player character query07ce40are software-qualified. All68rows and7feasible conditional outcomes execute. The eighth static outcome is proven infeasible from the actual original042dc4constant-zero return and immediate callerTST/BF. No instruction is removed and no nonzero callee result is supplied to force that branch.',
        '88normal full-state comparisons/983checks include292independent exact callback/reset/table/byte-index/unsigned16fallback oracles. Original character table IDs2,7,0,3,5are used. The initializer installs callbacks0b3b1a/0b3f2c/0b3afc,queries player0,andpreserves other child fields including floating player index. It does not itself call the movement owner.',
        '50comparisons/326checks cover16real heap/pool/parent/constructor/child lifecycles across eight character IDs andtwo player indices. The actual stored callback runs on the actual allocated child;exact installed callbacks,selected table,reset fields,query counts andfallback membership pass. The absent-player0caseuses a declared global character input and executes the actual fallback getter. Retail scheduler order and full movement startup remain open.',
        'Two already qualified originals042dc4/04f698are integrated as6original instruction rows in uniform adapters,with5direct comparisons/37checks andzero duplicate routine credit. Adapter names differ from their historical registered names,so the exact-name DLL inventory conservatively excludes those aliases. The initial275-source proposal and its missing-dependency frontier are preserved;qualification applies to the276-source dependency-connected build.',
        'Both275-source proposal and276-source dependency build compile. The276sources also pass a new strict unbounded Wall/Wextra/Werror check. All74newly emitted original rows byte-match the local RevA input. Only two realJSRreturn sites are added;existing CPU/FPU/bus/ABI/guest limits stay unchanged and no new guest wait bound or independently qualified nonlocal resumption is introduced.',
        'Assigned subsequent callbacks are stored but not executed here. Full renderer,owner/routing,retail collision validity/startup/stack invariants,physical hardware andfullgame/ports/settings remain unfinished. Planned PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR andtexture/display settings retain their scope. TextureOff must disable sampling anduse vertex/material colours.10%is weighted milestones,not measured code coverage;goal active/SolHigh.'
    ]
    report=dict(status='complete_original_child_initializer_and_character_query_software_qualified',routines=routines,new_original_routine_credit=2,newly_qualified_original_rows=68,executed_conditional_outcomes=7,proven_infeasible_conditional_outcomes=1,unresolved_conditional_outcomes=0,
        fresh_original_native_comparisons=143,fresh_checks=1346,independent_exact_oracles=292,actual_allocated_child_lifecycles=16,compiled_source_count=276,new_emitted_original_rows=74,canonical_reused_entries=['0x8c042dc4','0x8c04f698'],canonical_reused_original_rows=6,canonical_duplicate_credit=0,
        result_registry_entries=1636,result_unique_module_address_pairs=1633,linked_routine_inventory=linked,dll_sha256=digest,original_constant_return_gate_proof=direct['original_constant_return_gate_proof'],stored_next_callbacks=['0x8c0b3b1a','0x8c0b3f2c','0x8c0b3afc'],subsequent_callbacks_executed=False,
        flat_return_guard_sites=compiler['flat_return_membership'],new_nonlocal_resume_routes_qualified=0,full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
        next_task='Recover the actual installed execution callback0b3b1aand its dependencies from the real child lifecycle;retain separate movement startup,collision producer,owner/routing and renderer coverage obligations.',limitations=limitations,sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED2originals/68rows/7feasible outcomes+1infeasible proof;143comparisons/1346checks;680confirmedlinks',flush=True)
if __name__=='__main__':main()
