"""Accept the complete collision child and line wrapper; retain caller invariants."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/collision_response_qualification.json';assert not target.exists()
    paths=[P/'analysis'/n for n in ('player_collision_response_verification.json','collision_response_line_wrapper_verification.json','collision_response_caller_contracts.json','collision_response_compile_checks.json')]
    child,wrapper,contract,compiler=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
    for report in (child,wrapper,contract,compiler):
        for p,value in report['sha256'].items():assert sha(p)==value,p;files.append(Path(p))
    for p,value in compiler['source_sha256'].items():assert sha(p)==value,p;files.append(Path(p))
    assert (child['original_native_comparisons'],child['checks'],child['independent_exact_oracles'],child['independent_bounded_direction_oracles'])==(111,619,153,30)
    assert child['status_counts']=={'0':102,'-1':9}
    assert (wrapper['original_native_comparisons'],wrapper['checks'])==(12,109)
    assert (contract['original_native_comparisons'],contract['checks'])==(3,24) and contract['stack_bytes_seeded_or_repaired']==0
    assert not contract['retail_validity_invariant_proven'] and not contract['caller_checks_child_result_before_output_copy']
    assert compiler['source_count']==274 and compiler['compiler_exit_code']==0 and compiler['original_instruction_rows_byte_checked']==526
    dll=R/'work/sonic-adventure/build/player_response_clip.dll';digest=sha(dll)
    assert all(x['dll_sha256']==digest for x in (child,wrapper,contract,compiler))
    routines=[]
    for prefix,entry,report in (('player_movement_dependencies','0x8c0344dc',child),('player_movement_shapes','0x8c043a40',wrapper)):
        mp=P/f'analysis/{prefix}_translation.json';t=json.loads(mp.read_text());files.append(mp)
        b=next(x for x in t['routines'] if x['address']==entry);cov=report['coverage']
        assert cov['executed_rows']==b['instruction_rows'] and not cov['missing_rows'] and not cov['missing_outcomes']
        assert cov['executed_outcomes']==len(b['conditional_edges'])
        row={k:b[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=b['instruction_rows'],coverage=cov);routines.append(row)
    assert sum(x['instruction_rows'] for x in routines)==526
    prior=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1632];assert len(prior)==1632
    assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address'] in ('0x8c0344dc','0x8c043a40') for x in prior)
    linked=inventory(dll,prior+routines);assert linked['confirmed_linked_unique_routines_by_name']==678
    limitations=[
        'The complete 476-row collision child0344dc and50-row debug-line wrapper043a40 are software-qualified in the declared domains. All526rows and52conditional outcomes execute.111child comparisons include102normal returns and9matched downstream63d2d8 numerical stops;12direct wrapper cases return normally through the real renderer.',
        'The child tests include153independent exact result/rejection/direction checks and30bounded cardinal-direction checks. Only the mathematical type4 direction oracle permits absolute2^-20 rotation error;the entire original/native state and ordered boundaries still match exactly. All six shape types,early rejection,zero fallbacks,rotations,direction choices,flags,debug modes,index=count and six declared output aliases are exercised.',
        'The wrapper tests read the actual63ce24 boundary after original code constructs its descriptor. All six floating point arguments,two exact endpoint colours,descriptor pointers/count/flags andactual0x34frame match independent expectations. No callee result or private frame is supplied. This does not qualify the complete renderer or arbitrary hardware/device/numerical domains.',
        'A genuine two-call response sequence proves that a rejected child leaves its output unchanged and072946reads it without checking the return value. The first real successful call supplies the prior stack contents;no stack bytes are seeded or repaired. Cold backing bytes reach a numeric stop;reused nonzero bytes can yield a normal return. Whether retail collision flags guarantee valid child inputs remains unproven. The larger072946response remains unregistered;do not invent zero initialization or discard its original reads.',
        'Native C and the existing274-source DLL are unchanged and hash-checked. All274sources pass a new strict unbounded Wall/Wextra/Werror syntax compile,andall526subject instruction encodings match the original input. The existing verified DLL is reused;it is not claimed to have been rebuilt in this milestone. No CPU/FPU/bus/ABI/guest-limit or return-membership change and no new nonlocal qualification.',
        'The larger renderer,movement owner/routing,retail allocation/stack provenance,physical hardware and full game/ports/settings remain incomplete. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR and texture/display settings remain planned. TextureOff must disable sampling and retain vertex/material colours. Whole-project10%is a weighted milestone estimate,not measured code coverage;goal active/SolHigh.'
    ]
    report=dict(status='complete_collision_child_and_line_wrapper_software_qualified;response_caller_invariants_open',routines=routines,new_original_routine_credit=2,newly_qualified_original_rows=526,executed_conditional_outcomes=52,
        fresh_original_native_comparisons=126,fresh_checks=752,child_independent_exact_oracles=153,child_bounded_direction_oracles=30,compiled_source_count=274,existing_native_DLL_reused=True,native_C_changed=False,
        result_registry_entries=1634,result_unique_module_address_pairs=1631,linked_routine_inventory=linked,dll_sha256=digest,response_helper_registered=False,retail_collision_validity_invariant_proven=False,
        renderer_registered=False,new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=compiler['flat_return_membership'],full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
        next_task='Trace actual collision flag/info production and original allocated owner calls to072946;prove the retail validity/alias contracts or retain those gaps. Expand remaining real renderer and movement-owner/routing paths without supplying outputs or frames.',limitations=limitations,sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED2 originals/526rows/52outcomes;126comparisons/752checks;678confirmedlinks;response validity contract open',flush=True)
if __name__=='__main__':main()
