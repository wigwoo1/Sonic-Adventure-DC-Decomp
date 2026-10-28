"""Qualify the complete movement/model group under the current explicit software ABI."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','edges_verification','pipeline_verification','predicate_proof','retained_verification','unbounded'):
        path=P/f'analysis/movement_model_response_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_model_response.dll';sha=digest(dll)
    for name in ('audit','direct_verification','edges_verification','pipeline_verification'):assert reports[name]['dll_sha256']==sha
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    t=reports['translation'];direct=reports['direct_verification'];edges=reports['edges_verification'];proof=reports['predicate_proof']
    assert len(t['routines'])==3 and t['instruction_rows']==906 and len(t['resume_routes'])==66
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(666,6822,640)
    assert direct['status_counts']=={'0':640,'-1':18,'-3':8}
    assert (edges['original_native_comparisons'],edges['independent_checks'],edges['independent_oracle_cases'])==(176,2052,116) and edges['status_counts']=={'0':176}
    assert (reports['pipeline_verification']['original_native_comparisons'],reports['pipeline_verification']['independent_checks'],reports['pipeline_verification']['actual_nonempty_mesh_sequences'])==(40,260,20)
    assert reports['pipeline_verification']['supplied_game_callee_outputs']==reports['pipeline_verification']['supplied_execution_frames']==0
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    coverage=[]
    for a,b in zip(direct['coverage_by_routine'],edges['coverage_by_routine']):
        assert a['address']==b['address'];address=a['address']
        missing=sorted(set(a['missing_rows'])&set(b['missing_rows']))
        absent=sorted(set(map(tuple,a['missing_outcomes']))&set(map(tuple,b['missing_outcomes'])))
        integer=proof['integer_infeasible_outcomes'] if address=='0x8c036bc0' else []
        scalar=proof['scalar_profile_infeasible_outcomes'] if address=='0x8c0742d4' else []
        unreachable=proof['proved_unreachable_original_rows'] if address=='0x8c036bc0' else []
        assert missing==unreachable and absent==sorted(map(tuple,integer+scalar))
        coverage.append(dict(address=address,static_rows=a['static_rows'],executed_rows=a['static_rows']-len(missing),
          static_outcomes=a['static_outcomes'],executed_outcomes=a['static_outcomes']-len(absent),proved_unreachable_rows=unreachable,
          integer_infeasible_outcomes=integer,scalar_profile_infeasible_outcomes=scalar,unaccounted_software_profile_outcomes=[]))
    assert sum(r['executed_rows'] for r in coverage)==904 and sum(r['executed_outcomes'] for r in coverage)==112
    assert not proof['original_code_removed'] and not proof['domain_excluded_rows'] and not proof['domain_excluded_outcomes']
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==257 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    retpath=P/'analysis/movement_model_response_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2392 and len(ret['new_call_returns'])==66 and len(ret['allowed_call_returns'])==2458
    actual={b['pr'] for report in (direct,edges,reports['pipeline_verification']) for case in report['cases'] for b in case['boundaries']}
    assert set(ret['original_call_returns'])<=actual
    previous=P/'analysis/movement_mode_query_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==75497
    routines=[]
    for body in t['routines']:
        r={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};r.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(c for c in coverage if c['address']==body['address']));routines.append(r)
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1602,1605);baseline=registry[:1602]
    assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==648;files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_original_movement_model_response_group',compiled_source_count=257,newly_registered_entries=3,new_original_routine_credit=3,newly_qualified_original_rows=906,
      canonical_duplicate_credit=0,executed_original_rows=904,proved_unreachable_original_rows=2,executed_conditional_outcomes=112,
      integer_proved_infeasible_outcomes=1,scalar_profile_proved_infeasible_outcomes=1,
      scalar_predicate_profile=proof['scalar_profile'],unqualified_broader_precision_outcomes=proof['scalar_profile_infeasible_outcomes'],
      unaccounted_software_profile_outcomes=[],domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=routines,
      prior_registry_entries=1602,result_registry_entries=1605,result_unique_module_address_pairs=1602,linked_routine_inventory=linked,
      fresh_original_native_comparisons=1125,fresh_independent_checks=10817,direct_status_counts=direct['status_counts'],independent_oracle_cases=756,
      actual_nonempty_mesh_sequences=20,actual_original_call_returns_executed=66,supplied_game_callee_outputs=0,supplied_execution_frames=0,
      flat_return_guard_sites=2458,explicit_nonfunction_resume_routes_total=566,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=495,
      new_nonlocal_resume_exports=66,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,
      retained_previous_evidence_original_native_comparisons=75497,total_evidence_original_native_comparisons=76622,
      world_owner_still_unregistered='0x8c028bfe',world_owner_unaccounted_outcomes=old['world_owner_unaccounted_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',
      collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=old['collision_caller_unaccounted_outcomes'],
      fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Recover actual0730d8 dependency075a40 at call73a50;verify original arguments and real helpers. Preserve broader626precision and older world/collision/math obligations. Later canonical055d5a is a reuse candidate.',
      limitations=[
        'Original0742d4 movement response(519rows),036bc0recursive modelwalk(377rows),036f12render-state wrapper(10rows). '
        '906originalrowsretained,904execute. IntegerAND7selectorproof accounts036c12taken andits2unreachablerows;all8types/draw/child/sibling/orderflags tested. '
        '074626taken is proved impossible only in the explicit current PR0/SZ0 scalar softwareprofile:prior cap>speed andlater opposite speed>cap readstableRAMwithoutcalls/stores. '
        'BroaderphysicalSZ1/foreignFPmode changesareNOTprovedunreachable;thebranchremainsinsource andbroaderprecisionworkremainsopen. No originalcode orbranchremoved.',
        '666directcomparisons/6822checks:640complete,18existingunsupportednumericstops,8mappedguards. '
        '176additionalangle/commit/reversed-trianglecomparisons/2052checks allcomplete. All519movementrows execute across the two suites. '
        'Independentmodelmeshorder/nodeimmutability/matrixbalance/queryrestore/inputrecord/returnoracles cover756includedcases. '
        'Originalmatrix/rotation/math/angle/collision/rendercallbacks run;no suppliedcalleeoutputs/frames. SuccessfularithmeticprofilesPR0/SZ0;selectedrawmodesaredocumentedstops.',
        '40comparisons/260checkscover20realrendererinitializer->walk/wrapper->acceptedmeshsequences:solid/textured/alpha paths,bothFPbanks andselectedRM1. '
        'Originalcull/project/shade/material/primitive/flagcallbacks executewith4authoredrawvertices. Independentexactdyadicprojection/cache/packet/context/stackchecks;fullsoftwaredevice/queuecompare. '
        'Wrapperinstallsitsoriginalrenderer-recordpacketbuffer;barewalkuseslivecursor. These differ andthetestderiveseachfrominputrecords. NotGPU pixels/fullgame/completeold037098callbackqualification.',
        '243freshretainedIRQ/context/taskcomparisons/1683checks preserveoldnumericassertionsbyAST.1125freshcomparisons/10817checks;76622cumulativeevidence isnotunique/codecoverage. '
        'Priorstartup/full-devicewrite remainhistorical,notrerun. Physicalalignment/FPU/exception/MMU/bootROM/GPU/gameplay/frametimingunqualified.',
        '257-sourceDLL/library andstrictunboundedWall/Wextra/Werror pass. Exact256bridge+3entries/66actualreturns/66continuationexports. '
        'All1602priorregistryrows and2747oldbuilds preserved;newDLL/libadditional.566explicitresumes,71qualifiednonlocal/495remaining;new66exportsnotindependentlyqualifiednonlocalroutes. '
        'No newguestwaitbounds/CPU/guard/observerchanges. Hostimportfloor4096retained.',
        'A scaffoldnumericreplacement accidentallyrenamedSHA256intworeportwriters. Correctedmetadata-onlyscripts;finalstrictandretainedchecks rerun successfully. '
        'Pipelineassertionswerecorrectedfromassumedprimitiveaddressrange/fixedbuffertoexistingprimitiveconstantsandoriginalwrapperbufferdataflow. NativeCneedednochanges. '
        'Onlyfinalsuccessfulreportsarecounted;no duplicatecreditfromdiagnostic/initialruns.',
        'Broader626precision,028bfe null-readoutcomes,029b00 A48cskip andthreeoldmathwrapperprecisionpaths remainopen;0730d8 staysunregistered. '
        'No upstreamimplementationcodecopied. Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. '
        'TextureOffdisablessampling/usesvertexcolours.10%weightedmilestones;researchDLLnonplayable;goalactive,keepSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_model_response_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('MODEL RESPONSE QUALIFIED3software-profile routines/906rows;1125comparisons/10817checks;648linked')

if __name__=='__main__':main()
