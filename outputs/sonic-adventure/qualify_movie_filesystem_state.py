"""Qualify the complete gateway/retry reader; keep the table loop partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','tasks_verification','table_verification','retained_verification','unbounded'):
        path=P/f'analysis/movie_filesystem_state_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_state.dll')
    for name in ('audit','tasks_verification','table_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p)
        assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    t=reports['translation'];tasks=reports['tasks_verification'];table=reports['table_verification'];retained=reports['retained_verification']
    assert (len(t['routines']),t['instruction_rows'],len(t['resume_routes']),reports['connection']['compiled_source_count'])==(3,146,3,226)
    assert tasks['lifecycles_passed']==36 and len(tasks['cases'])==36
    assert table['original_native_comparisons']==24 and table['independent_checks']==312 and table['newly_registered_entries']==0
    assert retained['original_native_comparisons']==243
    assert reports['unbounded']['exit_code']==0 and reports['unbounded']['source_count']==226
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1513,1515)
    old_registry=registry[:1513];qualified=[]
    for body in t['routines']:
        if body['address']=='0x8c64ae62':continue
        cv=next(x for x in tasks['coverage_by_routine'] if x['address']==body['address'])
        assert not cv['missing_rows'] and not cv['missing_outcomes'],cv
        assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==body['address'] for x in old_registry)
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);qualified.append(row)
    assert len(qualified)==2 and sum(x['instruction_rows'] for x in qualified)==102 and sum(x['executed_outcomes'] for x in qualified)==14
    partial=table['coverage_by_routine'][0];assert partial['missing_rows'] and partial['missing_outcomes']
    comparisons=sum(reports[n]['original_native_comparisons'] for n in ('tasks_verification','table_verification','retained_verification'))
    checks=sum(reports[n]['independent_checks'] for n in ('tasks_verification','table_verification','retained_verification'))
    prior=P/'analysis/movie_input_interface_qualification.json';old=json.loads(prior.read_text());files.append(prior)
    limitations=[
        'Two newly qualified original read bodies102instruction rows/14conditional outcomes:root gateway64ae32(12/0) andretry reader647610(90/14). Allrows/outcomes execute under declared coherent contexts andreal downstream services. Third table-reader proposal64ae62(44rows/6outcomes) remains partial/unregistered;24independent nonpositive-count vectors do not qualify its positive loop. Local names are interpretations,not recovered symbols.',
        '36actual command1 task lifecycles include16successful channel/byteorder/FPU combinations pluserror,removal,abortpolicy,retry exhaustion,success afterretry,protocolretry,live unlimitedretry API andnull/unknownprogresscallback cases. Original constructors/APIs create frames/requests;real submission/raw observations/responsepump completeIDs;original contexts resume. No savedframe,completedID,decodedmetadata,parser/callback result is supplied.',
        'Success assembles actual512raw bytes at original descriptor root destination,updates operation/error counts andcalls real progresscallback,then resumes caller64b21e andstops atmissing filesystem validator64a4b2. Command remains pending with no fabricatedcompletion. Invalid storage responses follow originalerror mappings andcompletion/idle/owner teardown. Original root-data validation andcomplete filesystem/game remain unfinished.',
        'Abort-policy word66d3a0 is an explicit coherent initial software-policy fixture input setbefore thecommand;natural acquisition/writerinventory is not proved. Retrybudget andprogresscallback changes use original647534/647c6c APIs while savedcontexts are checked unchanged. Live policy-1 tests preserve original unbounded retry behavior with finite externally supplied responses,not a source waitlimit. Statistics are independently projected from actual pre-operation inputs,not assumed zero.',
        'Previous225bridge/runtime/ABI exactly preserved except three bodies,three nonfunction exports,exportprefix andthree actual new callreturns647646/647694/64ae9a. OriginalCPU/flatcode andexisting guards unchanged. Returnmembership1971->1974 is exact,not arbitraryPR relaxation. Existing82nonfunctionroutes plus3new=85. Newlyqualified647646 andexisting64b21e give13qualified/72unqualified;647694 and64ae9a exports remain unqualified for nonlocal resumption. No functioncredit for any continuation.',
        '243fresh retained comparisons cover83softwareIRQ/contextSRFD controls and160phases across8selected prior tasks,with oldnumericexpectations preserved byAST. Allother previous19828comparisons are historical,not allrerun. Combined evidence is not unique cases or codecoverage. Oldwarm216/cold211startup remain historical;no fresh226fullstartup/hardware claim.',
        'Retained suite initially used an old observer adapter;DLLchecksumguard rejected that run. Adapter binding was corrected andthe suite rerun against the exact226DLL. The rejected old-DLL run receives no freshcomparisoncredit. Native/reference code andnumeric expectations did not change.',
        'PhysicalMapleDMA acquisition,traversal,timing andautomaticIRQ remain unfinished;busyclear/rawRXbytes are declared device observations. Original side is projectSH4 instruction model,not hardware. Wholegame/PC/Android/3DS/N64/PS1/FPS/8K/SUPIR/settings remain unfinished. TextureOff disables sampling anduses vertexcolors. Goal active,about10%weightedmilestones,not measured codecoverage.'
    ]
    q=dict(status='partially_verified_original_filesystem_read_integration',compiled_source_count=226,newly_registered_entries=2,newly_qualified_rows=102,qualified_conditional_outcomes=14,routines=qualified,canonical_duplicate_credit=0,proposed_adapter_rows=146,coverage_by_routine=tasks['coverage_by_routine'],partial_table_coverage=partial,unregistered_proposals=['0x8c64ae62'],fresh_original_native_comparisons=comparisons,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=19828,total_evidence_original_native_comparisons=19828+comparisons,prior_registry_entries=1513,result_registry_entries=1515,result_unique_module_address_pairs=1512,prior_flat_return_guard_sites=1971,flat_return_guard_sites=1974,new_resume_exports=3,newly_qualified_nonlocal_routes=['0x8c647646','0x8c64b21e'],qualified_nonlocal_resume_routes_total=13,remaining_unqualified_resume_routes=72,explicit_nonfunction_resume_routes_total=85,unqualified_new_resume_exports=['0x8c647694','0x8c64ae9a'],original_task_lifecycles=36,task_original_native_comparisons=tasks['original_native_comparisons'],physical_maple_dma_complete=False,original_root_block_read_complete_in_declared_domain=True,full_table_reader_qualified=False,full_filesystem_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Recover actually reached root-block validator64a4b2. Feed raw root storage data through original read/task path andtest parser validation without supplying decoded results orrepairing saved contexts. Then qualify positive table loop64ae62 andthe remaining write backend660bae as actually reached.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_state_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED2new102rows/14outcomes;',comparisons,'fresh;',checks,'checks',flush=True)
if __name__=='__main__':main()
