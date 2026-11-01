"""Record allocated response lifecycle evidence without awarding owner recovery credit."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/movement_response_owner_research.json';assert not target.exists()
    paths=[P/f'analysis/movement_response_owner_{n}.json' for n in ('gate_proof','allocated_verification','compile_checks')]
    gate,life,compiler=[json.loads(p.read_text()) for p in paths];files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
    for report in (gate,life,compiler):
        for p,value in report['sha256'].items():assert sha(p)==value,p;files.append(Path(p))
    for p,value in compiler['source_sha256'].items():assert sha(p)==value,p;files.append(Path(p))
    assert gate['infeasible_call']=='0x8c072e84' and gate['structurally_gated_row_count']==31 and gate['native_instructions_removed']==0
    assert (life['original_native_comparisons'],life['checks'],life['lifecycles'])==(50,311,8)
    assert life['normal_initialized_owner_response_returns']==life['matched_cold_numeric_frontiers']==life['shape_flag_control_returns']==8
    assert life['old_normal_directly_seeded'] is False and not life['retail_collision_validity_invariant_proven']
    assert compiler['source_count']==274 and compiler['compiler_exit_code']==0 and compiler['original_instruction_rows_byte_checked']==1146
    dll=R/'work/sonic-adventure/build/player_response_clip.dll';digest=sha(dll)
    assert life['dll_sha256']==compiler['dll_sha256']==digest
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1634
    linked=inventory(dll,registry);assert linked['confirmed_linked_unique_routines_by_name']==678
    report=dict(status='allocated_initialization_and_response_call_verified;ordinary_dead_gate_proven;broader_contracts_open',new_original_routine_credit=0,registry_entries=1634,unique_module_address_pairs=1631,linked_routine_inventory=linked,
        fresh_original_native_comparisons=50,fresh_checks=311,original_allocated_lifecycles=8,normal_initialized_owner_response_returns=8,matched_cold_normal_numeric_frontiers=8,paired_shape_flag_control_returns=8,
        infeasible_normal_entry_call='0x8c072e84',observed_real_response_call='0x8c072f58',retained_structurally_gated_rows=31,native_instructions_removed=0,coverage_by_routine=life['coverage_by_routine'],
        existing_verified_DLL_reused=True,compiled_source_count=274,original_instruction_rows_byte_checked=1146,dll_sha256=digest,flat_return_guard_sites=compiler['flat_return_membership'],
        owner_registered=False,response_helper_registered=False,routing_body_registered=False,retail_collision_validity_invariant_proven=False,retail_stack_proven=False,new_nonlocal_resume_routes_qualified=0,
        full_game_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
        next_task='Trace collision info/flag producers and actual retail owner invocation order;expand remaining owner/routing/renderer paths. Retain all original gated code and distinguish authored collision input from proven retail invariants.',
        limitations=[
            'Actual original heap/task/constructor/movement state is carried through eight lifecycles. A zero-motion flags2 owner update produces the old surface normal;initialized flags1 shape0x800 cases then call072946from072f58and return normally. No allocator output,private frame or actual allocated old-normal vector is supplied.',
            'The cold constructor normal is zero and the real owner response reaches matching639b68 numerical stops in eight cases. These are retained software frontiers,not normal completions. A paired scene control differs only by four0x800shape bits and does not execute the response call. Host phase order and scene/collision data are authored;normal complete boot and retail producer invariants remain unproven.',
            'Original byte-backed ordinary-entry CFG and two unchangedR5comparisons require4and5on the same path. The second taken gate is impossible;cutting only that edge disconnects31retained rows including072e84while retaining072f58. This is not a proof for arbitrary intermediate PCs,asynchronous register corruption or nonlocal resumes;no original instruction or runtime branch is removed.',
            'Observed current cases cover321/629owner rows and27/92outcomes,200/236response rows and9/18outcomes,and20/2041routing rows and1/234outcomes. This is specific lifecycle coverage,not a replacement or reduction of prior historical coverage. Whole-owner,whole-routing and broader response qualification remain open.',
            'All274unchanged native sources pass a new strict unbounded Wall/Wextra/Werror syntax compile.1146subject instruction rows byte-match the original input. The existing verified DLL is hash-checked and reused;no new DLL build,native source change,guest bound or CPU/FPU/bus/ABI modification is claimed.',
            'Physical hardware,full game decompilation/playable boot,PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR and texture/display settings remain unfinished. TextureOff must disable sampling and retain vertex/material colours. Goal active;SolHigh remains appropriate;10%weighted milestones,not measured code coverage.'
        ],sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n')
    sp=P/'project_status.json';status=json.loads(sp.read_text());data={k:v for k,v in report.items() if k!='sha256'}
    data.update(verification=target.relative_to(P).as_posix(),verification_sha256=sha(target),guide='MOVEMENT_RESPONSE_OWNER_LIFECYCLE.txt')
    status['verified_progress']['movement_response_owner_lifecycle']=data;sp.write_text(json.dumps(status,indent=2)+'\n')
    print('RESEARCH50comparisons/311checks/eight allocated lifecycles;normal call072f58verified;31gatedrows retained;678links unchanged',flush=True)
if __name__=='__main__':main()
