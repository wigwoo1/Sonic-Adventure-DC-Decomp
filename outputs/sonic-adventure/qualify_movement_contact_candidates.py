"""Qualify complete candidate scan/clamp/response from fresh behavioral evidence."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__),P/'audit_movie_linked_routines.py']
    suffixes=('translation','connection','audit','direct_verification','minimum_verification','cardinal_verification','edges_verification','retained_verification','unbounded','preservation')
    for suffix in suffixes:
        path=P/f'analysis/movement_contact_candidates_{suffix}.json';r=json.loads(path.read_text());reports[suffix]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    t=reports['translation'];evidence=[reports[k] for k in ('direct_verification','minimum_verification','cardinal_verification','edges_verification')];retained=reports['retained_verification']
    assert len(t['routines'])==2 and t['instruction_rows']==645 and len(t['resume_routes'])==28
    for r,counts in zip(evidence,((304,2401,272),(8,72,8),(48,576,48),(16,156,16))):
        assert (r['original_native_comparisons'],r['independent_checks'],r['independent_oracle_cases'])==counts
        assert r['supplied_game_callee_outputs']==r['supplied_execution_frames']==0
    coverage=[]
    for body in t['routines']:
        parts=[next(x for x in r['coverage_by_routine'] if x['address']==body['address']) for r in evidence]
        missing=set(parts[0]['missing_rows']);edges=set(map(tuple,parts[0]['missing_outcomes']))
        for p in parts[1:]:missing&=set(p['missing_rows']);edges&=set(map(tuple,p['missing_outcomes']))
        assert not missing and not edges,(body['address'],missing,edges)
        n=body['instruction_rows'];outcomes=len(body['conditional_edges']);coverage.append(dict(address=body['address'],static_rows=n,executed_rows=n,static_outcomes=outcomes,executed_outcomes=outcomes,missing_rows=[],missing_outcomes=[]))
    assert sum(x['executed_rows'] for x in coverage)==645 and sum(x['executed_outcomes'] for x in coverage)==54
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    dll=ROOT/'work/sonic-adventure/build/movement_contact_candidates.dll';sha=digest(dll)
    assert reports['audit']['dll_sha256']==sha and all(r['dll_sha256']==sha for r in evidence)
    for suite in retained['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    u=reports['unbounded'];assert u['source_count']==reports['connection']['compiled_source_count']==264 and u['exit_code']==0
    for name,h in u['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    retpath=P/'analysis/movement_contact_candidates_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2557 and len(ret['allowed_call_returns'])==2585 and len(ret['new_call_returns'])==28
    observed={b['pr'] for r in evidence for case in r['cases'] for b in case['boundaries']};assert set(ret['original_call_returns'])<=observed
    p=reports['preservation'];assert p['preserved_previous_selected_files_outside_four_mutables']==16411 and p['preserved_prior_registry_rows']==1621 and p['preserved_archived_builds']==2761
    previous=P/'analysis/movement_contact_commit_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==81262
    routines=[]
    for body,cov in zip(t['routines'],coverage):
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=cov);routines.append(row)
    baseline=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1621];assert len(baseline)==1621
    for row in routines:assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==row['address'] for x in baseline)
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==667
    fresh=sum(r['original_native_comparisons'] for r in evidence)+retained['original_native_comparisons'];checks=sum(r['independent_checks'] for r in evidence)+retained['independent_checks'];semantic=sum(r['independent_oracle_cases'] for r in evidence)
    assert (fresh,checks,semantic)==(619,4888,344)
    limitations=[
      '075686198rows/20outcomes and075f8c447rows/34outcomes all execute;645rows/54outcomes acrossfourfreshdirectsuites. All28originalJSRreturnsobserved. No discardedGhidra-unreachableblocks,infeasibilityassumptions,newwaitbounds,suppliedgameoutputs ormid-executionframes. Actualmath/gamecallees execute.',
      'Both scan countfromquery+28 acrossglobal6bacf0,58-stride records,usingactualpointdistance/angle/dot tests.075686 clamps motion toselecteddistance;075f8c retainscandidateheight/angle/dot/speed/state4000 gates,addsdelta,copiesnormal,formsactualplaneintersection,selectsdirection andcopiesauxvelocitytoresponse delta. Completedredirectreturns12;earlyreturn0 canfollowpartialwrites.',
      '304directcomparisons/2401checks/272semanticcases covercounts0/1/2/16,nearest/tiedcandidates,normal/motiongeometry,empty lists,zero/added/cancellingdelta,state4000/auxzero,bank/rounding/rawprecision/FD/numeric/mapped stops. Counts andraw modes arepreserved,notrepaired orboundedbynewsourceguards. Rawstopsarenotplayablecompletions.',
      '48cardinalplanecomparisons/576checks independentlyverifybothintersectionaxes/directions andspeeds0.5/1/2,exactdeltacopy andpreservedqueryposition/motion/anchor.16edgecomparisons/156checks closeglobal-reference/heightrejectionandfiniteunderflowdirectionflip:delta(2,0,-2^-70/-80) crossnormrounds/flushe stozero,querynormalfallbackthenactualnegative-dotflip;velocity(0,0,-1)independentlychecked.',
      '8finiteunset-minimumcomparisons/72checks retainoriginalinitializer1000/index-1 behavior. Eligiblepointdistance1024neverselectsminimum;finite tiny motion haszero squaredlength butnegative dot. Originalreadsdeclaredpre-entrycallerpadding-1 atdistance[-1],thenexecuteszero-motioncommit. No mathreturnvalue orexecutionframeinjected;thisisoriginal behavior,notrepairedmemorylogic.',
      'Independenttestassumptionswereresolvedfromoriginalinstructions:redirectR0=12,32bitrawnormalcopy instead ofPython64bitdecimal,FSRRA(9)rounding1/3towardzero producingnormalized0x3f7fffff. Alloriginalcasesretained;exactrationaloracleadded. No native/reference/observer fixesneeded;allcompletedoriginal/nativecomparisonsmatched beforeoracleassertions.',
      '243retainedIRQ/context/taskcomparisons/1683checks preserveoldnumericexpectationsbyAST.619freshcomparisons/4888checks,344includedsemanticcases;81881cumulativecomparisons areevidencecounts,notuniqueroutines ormeasuredcoverage. Otherhistoricalstartup/hardwareevidencenotnewlyrerun.',
      '264sourcebuild/strictunboundedWallWextraWerrorpass. Exactprevious263bridgeplus2entries/28returns/28continuationexports;2585flatreturns,693explicitresumes,71qualifiednonlocal/622remaining. Newexportsnotnewnonlocalqualification. All2761oldbuilds/1621oldrows/16411immutablepublishedfilespreserved. CPU/guards/observer/ABI/guestlimitsunchanged.',
      'Nextqualify0777e0specialtailthen0730d8parent;connectivityaloneisnotqualification.028bfe/029b00/callbackassignment/missingcallbacks/074ee4scalar/broaderprecision/physicalFPU-MMU-device-exception/fullgame remainunfinished. PC60/120/240FPS8K/Android/stereo3DS/N64/PS1/SUPIR/settingsunfinished. TextureOffdisablessampling/usesvertex-materialcolours. NoSARecompimplementationcopied. Goalactive/SolHigh;10%weightedmilestones.'
    ]
    limitations=[x.replace('rounds/flushe stozero','rounds/flushes tozero') for x in limitations]
    result=dict(status='complete_original_contact_candidates_software_qualified',compiled_source_count=264,routines=routines,new_original_routine_credit=2,newly_qualified_original_rows=645,coverage_by_routine=coverage,executed_original_rows=645,executed_conditional_outcomes=54,infeasible_outcomes_claimed=[],canonical_duplicate_credit=0,result_registry_entries=1623,result_unique_module_address_pairs=1620,linked_routine_inventory=linked,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,independent_oracle_cases=semantic,retained_previous_evidence_original_native_comparisons=81262,total_evidence_original_native_comparisons=81881,supplied_game_callee_outputs=0,supplied_execution_frames=0,flat_return_guard_sites=2585,explicit_nonfunction_resume_routes_total=693,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=622,new_nonlocal_resume_exports=28,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',world_owner_still_unregistered='0x8c028bfe',collision_caller_still_unregistered='0x8c029b00',special_tail_still_unregistered='0x8c0777e0',full_gameplay_qualified=False,physical_exception_delivery_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,next_task='Recover andqualifyfull0777e0specialtail withactual dependencies,then0730d8parent. Preserveolderworld/collision/precision/callback/physical obligations.',limitations=limitations,sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_contact_candidates_qualification.json').write_text(json.dumps(result,indent=2)+'\n');print('CONTACT CANDIDATES QUALIFIED645rows/54outcomes;619comparisons/4888checks;667linked',flush=True)
if __name__=='__main__':main()
