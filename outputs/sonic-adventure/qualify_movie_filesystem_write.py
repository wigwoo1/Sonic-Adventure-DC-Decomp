"""Credit only complete helper/wrapper paths; keep optional-hook backend partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','packets_verification','tasks_verification','retained_verification','unbounded'):
        p=P/f'analysis/movie_filesystem_write_{name}.json';r=json.loads(p.read_text());reports[name]=r;files.append(p)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_write.dll')
    for name in ('audit','packets_verification','tasks_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    assert reports['packets_verification']['original_native_comparisons']==720 and reports['packets_verification']['independent_checks']==10072
    assert reports['retained_verification']['original_native_comparisons']==243 and reports['retained_verification']['independent_checks']==1683
    task=reports['tasks_verification'];assert task['task_sessions_passed']==task['completed_initializations']==23
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==231 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    prior_translation=P/'analysis/movie_filesystem_command_translation.json';pt=json.loads(prior_translation.read_text());files.append(prior_translation)
    bodies=reports['translation']['routines']+[next(r for r in pt['routines'] if r['address']=='0x8c64bcd6')]
    coverage=[];routines=[]
    for body in bodies:
        cvs=[cv for n in ('packets_verification','tasks_verification') for cv in reports[n]['coverage_by_routine'] if cv['address']==body['address']]
        assert cvs,body['address']
        missing=set.intersection(*(set(cv['missing_rows']) for cv in cvs));outcomes=set.intersection(*(set(map(tuple,cv['missing_outcomes'])) for cv in cvs))
        cv=dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes));coverage.append(cv)
        if body['address']=='0x8c660bae':
            assert missing and outcomes,'optional hook must remain explicitly unqualified';continue
        assert not missing and not outcomes,(body['address'],missing,outcomes)
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source='source/sa_movie_filesystem_command.c' if body['address']=='0x8c64bcd6' else reports['translation']['source'],instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);routines.append(row)
    assert len(routines)==8 and sum(r['instruction_rows'] for r in routines)==333 and sum(r['executed_outcomes'] for r in routines)==42
    saved={'0x8c660be6','0x8c660c6a','0x8c660c7c','0x8c660ce4','0x8c660cf6'};assert set(task['genuine_saved_resume_pcs'])==saved
    routes=sorted(saved|{'0x8c64771e','0x8c64bd5c','0x8c647b68'})
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1527,1535)
    assert not ({r['address'] for r in routines}&{r['address'] for r in registry[:1527] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'})
    oldp=P/'analysis/movie_filesystem_chain_qualification.json';old=json.loads(oldp.read_text());files.append(oldp);assert old['total_evidence_original_native_comparisons']==27599
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('packets_verification','tasks_verification','retained_verification'));checks=sum(reports[n]['independent_checks'] for n in ('packets_verification','tasks_verification','retained_verification'))
    limitations=[
        'SixSDK/data0C/commit0D mutexandpacket helpers149rows/8outcomes qualifyincoherentRAM/singleguestexecutor domain.720directcomparisons includeall256mutexbytes eachlock,independent serializedpacket/context/serialwrap/bank/order/padding/sourcebytes andmandatoryTASpurge oracles;actual diagnostic/commonbuilder/autopump execute. Automaticpumpfixtureusesoriginal busy gatefc0=1;doesnotclaim physicalDMA/atomicity.',
        'Retry6476e8 qualifies96rows/16outcomes withactualbackend660bae undertheoriginalconstructor-clearedNULLhook contract. OriginalpolicyAPI exercisesbudget0/1 andlive0->-1 retry;rawACK/status8/251/252/removal driveactual results. Abortpolicy isdeclaredoriginalsoftwareinput;no calleeoutputorcallbackresult supplied. Wrapper64bcd6 qualifiesall88rows/18outcomes withmissing/type/allrangeguards,zero/one/twoblockcounts,chainerrors,success/error/retry paths. Originalstartcoordinateisretained bybackend;no invented FAT-to-physicalblock remapping.',
        '23actualtasks acquire rawheader/root/FAT/directory withoriginalconstructors,submit/pump/scheduler beforecommand20. Nativeandoriginal fullstate/ordered effects agree; independentrequestinputs/data,progresscounters,completioncallback,idlefields andownerteardown checked. Four128bytedata requests plusonecommit completea512bytewrite indefaultcapabilitydomain. Readonly observercapturesbuilderinputs,nottransmittedbytes;serializedoutput isindependentlyverified separately. No decodedmetadata,taskframe,requestID,PR orreturnrepair.',
        'Backend660bae206rows/34outcomes remainsPARTIAL/UNREGISTERED withzero routinecredit. OriginalsourceincludesoptionalnonNULLhook andotherunexercisedbranches;qualificationrecords exactmissingrows/outcomes. No fabricatedhookreturn orclaimhookunreachable. Fullbackend/fullcommand20/fullfilesystem/physicalMaple/game remainunfinished.',
        '231sourceDLL builtandstrictunbounded Wall/Wextra/Werror syntaxpassed.451newrowsbytechecked;exact230bridgeinverseexcept8bodies/31continuations/31actualreturns/exportprefix. Returns2005->2036. Flatcode/ABI/runtime/SRFD/guards unchanged. ReferenceCPUdelta onlyaddsmandatoryTAS eventobservation at66054c/6605b6;integer/memory/TASexecution unchanged. OriginalCPU/proofs preserved;fullmemory/effectcomparisons remainstrict. Eightgenuine nonlocalroutesqualify throughsavedframes/parentunwind;other119of144 remainunqualified. No routinecredit fromcontinuationexports.',
        '243freshretainedIRQ/context/taskcontrols preserveoriginalnumericexpectations. Earlier27599comparisons historical,notallrerun. Combinedevidence isnotuniquecases orcoverage;216warm/211coldstartup profiles historical. Oldsource/proofs/2693builds preserved. ResearchDLLsnotplayableports. GoalACTIVEabout10%weightedmilestones,notmeasuredcodecoverage. PC/Android/3DS/N64/PS1,FPS/8K/SUPIR/settings remainunfinished;TextureOff disablestexturesampling andusesvertexcolors.'
    ]
    q=dict(status='partially_verified_original_write_path_optional_hook_backend_incomplete',compiled_source_count=231,newly_registered_entries=8,newly_qualified_rows=333,qualified_conditional_outcomes=42,newly_translated_rows=451,reused_previously_unregistered_rows=88,canonical_duplicate_credit=0,partial_unregistered_proposals=['0x8c660bae'],routines=routines,coverage_by_routine=coverage,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=27599,total_evidence_original_native_comparisons=27599+fresh,prior_registry_entries=1527,result_registry_entries=1535,result_unique_module_address_pairs=1532,prior_flat_return_guard_sites=2005,flat_return_guard_sites=2036,new_resume_exports=31,newly_qualified_nonlocal_routes=routes,qualified_nonlocal_resume_routes_total=25,remaining_unqualified_resume_routes=119,explicit_nonfunction_resume_routes_total=144,original_task_sessions=23,completed_initializations=23,task_original_native_comparisons=task['original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command20_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Resolve actual optionalwritehook ownership/installation and remaining660bae branchinputs without fabricatedhookresults;recover reachable gamecaller if hook genuinelyunreachable onlyafter boundedallmoduleevidence. Keep660bae partialuntilcomplete. Preserveactual scheduler668e60/rawdeviceflow andoriginalframes/IDs/PR.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_write_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('WRITE QUALIFIED8/333rows/42outcomes',fresh,checks,flush=True)
if __name__=='__main__':main()
