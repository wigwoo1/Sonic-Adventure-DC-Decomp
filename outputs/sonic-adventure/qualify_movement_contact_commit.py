"""Qualify both complete original contact routines from fresh behavioral evidence."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__),P/'audit_movie_linked_routines.py']
    for suffix in ('translation','connection','audit','direct_verification','fallback_verification','retained_verification','unbounded','preservation'):
        path=P/f'analysis/movement_contact_commit_{suffix}.json';r=json.loads(path.read_text());reports[suffix]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    t=reports['translation'];d=reports['direct_verification'];e=reports['fallback_verification'];retained=reports['retained_verification']
    assert len(t['routines'])==2 and t['instruction_rows']==384 and len(t['resume_routes'])==23
    assert (d['original_native_comparisons'],d['independent_checks'],d['independent_oracle_cases'])==(307,2551,270)
    assert (e['original_native_comparisons'],e['independent_checks'],e['independent_oracle_cases'])==(16,160,16)
    coverage=[]
    for body in t['routines']:
        address=body['address'];parts=[next(x for x in r['coverage_by_routine'] if x['address']==address) for r in (d,e)]
        missing=set(parts[0]['missing_rows'])&set(parts[1]['missing_rows']);edges=set(map(tuple,parts[0]['missing_outcomes']))&set(map(tuple,parts[1]['missing_outcomes']))
        assert not missing and not edges,(address,missing,edges)
        n=body['instruction_rows'];outcomes=len(body['conditional_edges'])
        coverage.append(dict(address=address,static_rows=n,executed_rows=n,static_outcomes=outcomes,executed_outcomes=outcomes,missing_rows=[],missing_outcomes=[]))
    assert sum(x['executed_rows'] for x in coverage)==384 and sum(x['executed_outcomes'] for x in coverage)==32
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    dll=ROOT/'work/sonic-adventure/build/movement_contact_commit.dll';sha=digest(dll)
    assert sha==reports['audit']['dll_sha256']==d['dll_sha256']==e['dll_sha256']
    for suite in retained['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    u=reports['unbounded'];assert u['source_count']==reports['connection']['compiled_source_count']==263 and u['exit_code']==0
    for name,h in u['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    retpath=P/'analysis/movement_contact_commit_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2534 and len(ret['allowed_call_returns'])==2557 and len(ret['new_call_returns'])==23
    observed={b['pr'] for report in (d,e) for case in report['cases'] for b in case['boundaries']};assert set(ret['original_call_returns'])<=observed
    p=reports['preservation'];assert p['preserved_previous_selected_files_outside_four_mutables']==16366 and p['preserved_prior_registry_rows']==1619 and p['preserved_archived_builds']==2759
    previous=P/'analysis/movement_heading_query_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==80696
    routines=[]
    for body,cov in zip(t['routines'],coverage):
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=cov);routines.append(row)
    baseline=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1619];assert len(baseline)==1619
    for row in routines:assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==row['address'] for x in baseline)
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==665
    limitations=[
      'Original074afc223rows/18outcomes and076640161rows/14outcomes all execute;384rows/32outcomes in union. No discarded Ghidra-unreachable comparisons,assumed infeasible outcomes,new wait bounds,supplied game results or execution frames. All actual math dependencies execute.',
      '074afc always copies query normal to state. Grounded displacement uses target minus actor;zero displacement falls back to negative auxiliary velocity. Angle4000 cutoff can reject before commit. Air path uses negative auxiliary velocity andflag200000 selects status update. Successful commit copies anchor,subtracts caller offset intoactorposition andcopies response delta intomotion.',
      '076640 preserves state/query input objects and uses original speed/status,parallel-vector andzero-delta gates. It subtracts oriented angle fromactorheading,adds8000 onlywhen actual circular distance exceeds6000,thenstorescharacterheading. Response delta below original0x3ecccccc leavescharacter speed;otherwise original0x3f199999 threshold and0x3f75c28f damping scale redirected velocity.',
      '307directcomparisons/2551checks/270semanticcases plus16realzero-displacementfallbackwitnesses/160checks/16semanticcases. Sixteenwitnesses close074bce->074bd4 inbothbanks/rounding/flags. RawPR/SZ/FD/numeric/mapped stops remain documented original software behavior;no broadphysicalqualification.',
      'An initial independentoracle incorrectly assumed decimal equal directions yield exactzeroangle. Original normalization of0.1produces0x3f7fffff andrealnonzeroangle;native matched beforetheoraclefailed. Decimalcases retained in fullstatecomparisons;addedexact-power-of-two cases verifyzero-angle6000/8000contracts. No native/reference/observer fix or weakened executiongate.',
      '243retainedinterrupt/context/taskcomparisons/1683checks preservepreviousnumericexpectationsbyAST.566freshcomparisons/4394checks;286includedsemanticcases.81262cumulativecomparisons areevidencecounts,notuniqueroutines ormeasuredcoverage. Otherhistoricalstartup/hardwareevidence notnewlyrerun.',
      '263sourcebuild andstrictunboundedWall/Wextra/Werrorpass. Exactprevious262bridgeplus2entries/23realreturns/23continuationexports;2557flatreturns,665explicitresumes,71qualifiednonlocal/594remaining. Newexportsarenotnewnonlocalqualification. All2759oldbuilds/1619oldregistryrows/16366immutablepublishedfiles preserved.',
      'Parents0730d8/0777e0 andworld/collisionowners028bfe/029b00 remainunregistered;nextspecialtaildependencies075686/075f8c. Older callbackassignment/missingcallback/074ee4scalar/broaderprecision/physicalFPU/MMU/exception/device obligations remain. Fullgame/PC60/120/240FPS8K/Android/stereo3DS/N64/PS1/SUPIR/settings unfinished. TextureOff disables sampling/usesvertex-materialcolours. NoSARecompimplementationcopied. Goalactive/SolHigh;10%weightedmilestones.'
    ]
    result=dict(status='complete_original_contact_commit_pair_software_qualified',compiled_source_count=263,routines=routines,new_original_routine_credit=2,newly_qualified_original_rows=384,coverage_by_routine=coverage,executed_original_rows=384,executed_conditional_outcomes=32,infeasible_outcomes_claimed=[],canonical_duplicate_credit=0,result_registry_entries=1621,result_unique_module_address_pairs=1618,linked_routine_inventory=linked,fresh_original_native_comparisons=566,fresh_independent_checks=4394,independent_oracle_cases=286,retained_previous_evidence_original_native_comparisons=80696,total_evidence_original_native_comparisons=81262,supplied_game_callee_outputs=0,supplied_execution_frames=0,flat_return_guard_sites=2557,explicit_nonfunction_resume_routes_total=665,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=594,new_nonlocal_resume_exports=23,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',world_owner_still_unregistered='0x8c028bfe',collision_caller_still_unregistered='0x8c029b00',special_tail_still_unregistered='0x8c0777e0',full_gameplay_qualified=False,physical_exception_delivery_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,next_task='Recover remaining specialtail0777e0dependencies075686 and075f8c,thenverifytail andparent0730d8. Preserveolderworld/collision/precision/callback/physical obligations.',limitations=limitations,sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_contact_commit_qualification.json').write_text(json.dumps(result,indent=2)+'\n');print('CONTACT COMMIT QUALIFIED384rows/32outcomes;566comparisons/4394checks;665linked',flush=True)
if __name__=='__main__':main()
