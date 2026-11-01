"""Accept four whole shape helpers;retain the larger movement-path gaps explicitly."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    names=('player_movement_shapes_verification','player_movement_owner_verification','player_movement_allocated_verification','player_movement_retained_verification','player_movement_path_compile_checks')
    paths=[P/f'analysis/{n}.json' for n in names];shapes,owner,allocated,retained,compiler=[json.loads(p.read_text()) for p in paths]
    files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
    for report in (shapes,owner,allocated,retained,compiler):
        for path,expected in report.get('sha256',{}).items():assert sha(path)==expected,path;files.append(Path(path))
    for path,expected in compiler['source_sha256'].items():assert sha(path)==expected,path;files.append(Path(path))
    assert (shapes['original_native_comparisons'],shapes['checks'],shapes['independent_semantic_oracle_cases'])==(256,1176,148)
    assert (owner['original_native_comparisons'],owner['checks'],owner['status_counts'])==(96,672,{'0':96})
    assert (allocated['comparisons'],allocated['checks'],allocated['movement_lifecycles'])==(26,142,8)
    assert (retained['original_native_comparisons'],retained['independent_checks'],retained['status_counts'])==(17,183,{'0':16,'-1':1})
    assert compiler['source_count']==272 and compiler['compiler_exit_code']==0 and compiler['original_instruction_rows_byte_checked']==4063
    dll=R/'work/sonic-adventure/build/player_movement_path.dll';digest=sha(dll)
    assert shapes['dll_sha256']==owner['dll_sha256']==allocated['dll_sha256']==retained['dll_sha256']==digest
    families={};new_resumes=0
    for prefix in ('player_movement_owner','player_movement_dependencies','player_movement_route','player_movement_shapes'):
        path=P/f'analysis/{prefix}_translation.json';t=json.loads(path.read_text());files.append(path);families[prefix]=t
        for path,expected in t['sha256'].items():assert sha(path)==expected,path;files.append(Path(path))
        new_resumes+=len(t['resume_routes'])
    selected={'0x8c02fc12','0x8c02fd32','0x8c02fe5c','0x8c031a84'};routines=[]
    for body in families['player_movement_shapes']['routines']:
        if body['address'] not in selected:continue
        coverage=next(x for x in shapes['coverage_by_routine'] if x['address']==body['address']);assert not coverage['missing_rows'] and not coverage['missing_outcomes']
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=families['player_movement_shapes']['source'],instruction_rows=body['instruction_rows'],coverage=coverage);routines.append(row)
    assert len(routines)==4 and sum(x['instruction_rows'] for x in routines)==425 and sum(x['coverage']['executed_outcomes'] for x in routines)==56
    previous=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1627];assert len(previous)==1627
    assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address'] in selected for x in previous)
    linked=inventory(dll,previous+routines);assert linked['confirmed_linked_unique_routines_by_name']==675
    owner_row=next(x for x in owner['coverage_by_routine'] if x['address']=='0x8c072b44');route_row=next(x for x in owner['coverage_by_routine'] if x['address']=='0x8c0730d8')
    assert (owner_row['executed_rows'],owner_row['executed_outcomes'])==(328,39) and (route_row['executed_rows'],route_row['executed_outcomes'])==(251,5)
    limitations=[
        'Four complete original helpers02fc12(128rows/18outcomes),02fd32(130/18),02fe5c(110/18),031a84(57/2):all425rows/56outcomes execute.256full-state comparisons/1176checks include148independent exact absolute/translated/zero-angle/difference results. Both rotation orders and individual/all nonzero axes execute with original matrix services;shared numerical integration is not physical-hardware qualification.',
        'The whole629-row072b44 owner,2041-row0730d8 routing body,476-row0344dc,236-row072946,and50-row043a40 debug wrapper are retained in native source but remain unregistered.96whole-owner ordinary-entry cases complete:328/629owner rows and39/92outcomes;251/2041route rows and5/234outcomes. Coverage gaps remain;no branch is removed or silently declared infeasible. The debug wrapper still needs real63ce24 submission when reached.',
        '26comparisons/142checks cover eight original heap->task pool->parent task->06c350->072b44->0730d8->0777e0lifecycles. Actual allocated actor/aux/state/query pointers remain live and pass through the real calls. The actual caller frame is0x4c in the declared narrow-transfer cases. Host phase order,collision inputs,velocity and8cf00000stack are declared. This is not retail stack provenance or universal alias exclusion;two globally unresolved0777e0sort outcomes remain open.',
        '17prior live-record comparisons/183checks pass unchanged through the272-sourceDLL,including16normal cases andone matched nonfinite FMAC software stop. Coverage metadata selects the actual old0777e0subject via a read-only namespace view;runtime and old assertions are unchanged. No new special-tail or physical exception credit.',
        '272-source build and strict unboundedWall/Wextra/Werror checks pass. All4063newly emitted instruction rows byte-match the original RevA input;realJSR/BSRreturns are checked. Existing CPU/observer/numeric/bus/ABI behavior and guest limits stay unchanged;no new waits or supplied results/frames. Canonical055d5a/06c880reuse gets zero duplicate credit. New continuation exports are not independently qualified nonlocal resumptions.',
        'Full game decompilation,normal complete boot,physicalFPU/MMU/exceptions/devices andplayablePC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR/settings remain unfinished. TextureOff must disable sampling/use vertex-material colours. NoSARecompimplementation imported;goalactive/SolHigh;10%weightedmilestones,not measured code coverage.'
    ]
    report=dict(status='four_complete_shape_helpers_software_qualified;whole_movement_owner_and_route_remain_partial',routines=routines,new_original_routine_credit=4,newly_qualified_original_rows=425,executed_conditional_outcomes=56,
                fresh_original_native_comparisons=395,fresh_checks=2173,independent_shape_semantic_oracle_cases=148,compiled_source_count=272,new_emitted_original_rows=4063,
                result_registry_entries=1631,result_unique_module_address_pairs=1628,linked_routine_inventory=linked,canonical_reused_entries=['0x8c055d5a','0x8c06c880'],canonical_duplicate_credit=0,
                owner_partial_coverage=owner_row,route_partial_coverage=route_row,owner_registered=False,route_registered=False,caller_allocation_alias_contract_globally_proven=False,
                original_allocated_movement_lifecycles=8,new_nonlocal_resume_exports=new_resumes,new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=compiler['flat_return_membership'],guest_execution_limits_unchanged=True,
                full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=digest,
                next_task='Target missing072b44/0730d8branches with faithful collision/world inputs;finish0344dc/072946asreached. Recover real63ce24ifdebugwrapper path is exercised. Keep actual caller/stack/alias obligations and constructor owner discovery open.',limitations=limitations,
                sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_movement_path_qualification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
    print('MOVEMENT PATH QUALIFIED4shape helpers/425rows/56outcomes;395comparisons/2173checks;675confirmedlinks;owner/routePARTIAL',flush=True)
if __name__=='__main__':main()
