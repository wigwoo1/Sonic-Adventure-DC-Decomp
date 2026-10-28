"""Qualify074ee4 for the explicitly bounded original-math scalar software profile."""
import hashlib,json,subprocess
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
BASE='06161710734a2a2603b9acc523e1dc8c4c6179bb'

def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    files=[Path(__file__),P/'audit_movie_linked_routines.py'];reports={}
    for suffix in ('certificate','audit','replay','direct_verification','range_verification'):
        path=P/f'analysis/movement_orientation_fold_{suffix}.json';r=json.loads(path.read_text());reports[suffix]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    proof=reports['certificate'];assert proof['status']=='certified_original_math_scalar_profile_angle_fold_bound'
    assert proof['scalar_profile_unreachable_rows']==['0x8c074fd6','0x8c074fd8','0x8c074fda']
    assert proof['scalar_profile_infeasible_outcomes']==[['0x8c074fd4','0x8c074fd6']]
    assert proof['unqualified_broader_outcomes']==proof['scalar_profile_infeasible_outcomes']
    assert not any(proof[k] for k in ('original_code_removed','runtime_guards_added','original_math_data_or_precision_mutation_qualified','universal_physical_infeasibility_proved'))
    assert len(proof['scaling_symbolic_exponent_classes'])==256
    direct,extra=reports['direct_verification'],reports['range_verification']
    for r in (direct,extra):assert r['test_AST_unchanged_except_report_destination'] and not r['native_code_changed']
    row=direct['coverage_by_routine'][0];other=extra['coverage_by_routine'][0]
    assert row['address']==other['address']=='0x8c074ee4'
    missing=sorted(set(row['missing_rows'])&set(other['missing_rows']))
    outcomes=sorted(set(map(tuple,row['missing_outcomes']))&set(map(tuple,other['missing_outcomes'])))
    assert missing==proof['scalar_profile_unreachable_rows'] and outcomes==list(map(tuple,proof['scalar_profile_infeasible_outcomes']))
    assert (row['static_rows'],row['executed_rows'],row['static_outcomes'],row['executed_outcomes'])==(157,154,24,23)
    assert (reports['replay']['original_native_comparisons'],reports['replay']['independent_checks'])==(465,4299)
    dll=ROOT/'work/sonic-adventure/build/movement_orientation_response.dll';sha=digest(dll)
    assert sha==reports['audit']['dll_sha256']==direct['dll_sha256']==extra['dll_sha256']
    assert reports['audit']['compiler_exit_code']==0 and reports['audit']['compiled_source_count']==259
    for name,h in reports['audit']['unchanged_compiled_source_sha256'].items():assert digest(name)==h,name
    previous=P/'analysis/movement_orientation_response_qualification.json';old=json.loads(previous.read_text());files.append(previous)
    assert old['proposed_owner_new_routine_credit']==0 and old['total_evidence_original_native_comparisons']==78481
    for name,h in old['sha256'].items():assert digest(name)==h,name
    manifest=P/'analysis/movement_orientation_response_translation.json';t=json.loads(manifest.read_text());files.append(manifest)
    body=next(x for x in t['routines'] if x['address']=='0x8c074ee4')
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
    routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=157)
    raw=subprocess.check_output([r'C:\Program Files\Git\cmd\git.exe','-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':outputs/sonic-adventure/analysis/recovered_routines.json'])
    baseline=json.loads(raw)['routines'];assert len(baseline)==1612
    assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==body['address'] for x in baseline)
    linked=inventory(dll,baseline+[routine]);assert linked['confirmed_linked_unique_routines_by_name']==657
    limitations=[
      'Software qualification is limited to normal entry, fixed original code/math literals/coefficient table, ordinary RAM stack without aliasing those fixed inputs, PR0/SZ0 and completed RM0/RM1 arithmetic. Full original source remains emitted.154rows/23outcomes execute;3rows/1outcome are accounted by the original-math scalar range analysis, not deleted or excluded from source.',
      'The classifier result is unconstrained. All selected atan formulas and fallback are bounded using the actual polynomial, power-of-two scaling, constants, rounding and preserved registers.256symbolic exponent classes cover every fraction for scaling arguments[0,1] with powers-2/-1. The final finite product lies in[-0.022887651,32768.03515625], strictly between-1and32769. Truncation yields0..32768. Original NaN handling stops or gives masked phase0. No ideal atan result or injected callee output/frame is assumed.',
      '465fresh full-state original/native comparisons and4299independent checks pass, with424included independent oracle cases. These replay the published tests with AST equality except output filenames. Retained243comparisons/1683checks are historical from the identical DLL, not newly rerun. Cumulative78946comparison evidence is not unique routines or measured code coverage.',
      'All259compiled source files and the tested DLL are unchanged. Strict unbounded Wall/Wextra/Werror compiler checks pass. No new native implementation, guard, ABI, observer, guest limit, call return or nonlocal resume route is introduced. This is qualification of a previously compiled original owner, not new C code or a playable port.',
      'Mutated/aliased math inputs, foreign mode changes, broader PR/SZ precision, physical FPU/MMU/exception delivery and hardware remain unqualified. The original074fd4->074fd6 branch remains in source and the broader obligation stays open. Older074626/6402c0 precision,028bfe/029b00 branches and0730d8 parent remain unfinished.',
      'Full game decompilation and PC60/120/240FPS8K,Android,stereoscopic3DS,N64/PS1,SUPIR and settings remain unfinished. TextureOff must disable texture sampling and use vertex/material colours.10% is a weighted milestone estimate. Goal remains active;SolHigh remains appropriate.'
    ]
    result=dict(status='original_owner_qualified_for_original_math_scalar_software_profile',
      compiled_source_count=259,native_sources_changed=0,dll_rebuilt=False,dll_sha256=sha,
      routine=routine,new_original_routine_credit=1,newly_qualified_original_rows=157,
      executed_owner_rows=154,scalar_profile_proved_unreachable_rows=missing,
      executed_owner_conditional_outcomes=23,scalar_profile_infeasible_outcomes=proof['scalar_profile_infeasible_outcomes'],
      unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes']+proof['unqualified_broader_outcomes'],
      result_registry_entries=1613,result_unique_module_address_pairs=1610,linked_routine_inventory=linked,
      fresh_original_native_comparisons=465,fresh_independent_checks=4299,independent_oracle_cases=424,
      retained_previous_evidence_original_native_comparisons=78481,total_evidence_original_native_comparisons=78946,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,canonical_duplicate_credit=0,
      flat_return_guard_sites=2501,explicit_nonfunction_resume_routes_total=609,
      qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=538,
      new_nonlocal_resume_routes_qualified=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      parent_movement_caller_still_unregistered='0x8c0730d8',world_owner_still_unregistered='0x8c028bfe',collision_caller_still_unregistered='0x8c029b00',
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      next_task='Resume ordinary-entry dependency recovery in0730d8 after this scalar gate;retain broader precision,math mutation,world/collision and physical obligations.',
      profile=proof['profile'],limitations=limitations,sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_orientation_fold_qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('FOLD OWNER SCALAR QUALIFICATION PASS157rows=154executed+3bounded;657linked;broadercasesOPEN',flush=True)
if __name__=='__main__':main()
