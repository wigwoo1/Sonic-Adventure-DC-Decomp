"""Evidence-gated qualification of the original whole-device reader."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','tasks_verification','retained_verification','frontier_verification','unbounded'):
        path=P/f'analysis/movie_filesystem_command15_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command15.dll')
    for name in ('audit','tasks_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    for name,wanted in {'retained_verification':(243,1683),'frontier_verification':(92,420)}.items():assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==wanted,name
    tasks=reports['tasks_verification'];assert tasks['original_task_sessions']==len(tasks['cases'])==10
    assert tasks['original_native_comparisons']==sum(len(c['phases']) for c in tasks['cases']) and tasks['independent_checks']==sum(c['independent_checks'] for c in tasks['cases'])
    assert all(all(p['status']==0 for p in c['phases']) for c in tasks['cases'])
    full=[c for c in tasks['cases'] if c['configuration']['kind']=='full512'];assert len(full)==1;full=full[0]
    assert full['configuration']['geometry']==511 and full['result']=='0x0' and full['copied_bytes']==262144
    assert len(full['requests'])==512 and [r[1] for r in full['requests']]==list(range(512)) and [r[2] for r in full['requests']]==[0x8cda0010+i*512 for i in range(512)]
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==241 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    t=reports['translation'];assert len(t['routines'])==1 and t['instruction_rows']==54 and t['canonical_reused_entries']==[] and len(t['routines'][0]['conditional_edges'])==4
    body=t['routines'][0];c=tasks['coverage_by_routine'][0];assert c['address']==body['address'] and not c['missing_rows'] and not c['missing_outcomes'] and c['executed_rows']==54 and c['executed_outcomes']==4
    coverage=[dict(address=body['address'],static_rows=54,executed_rows=54,executed_outcomes=4,domain_excluded_rows=[],domain_excluded_outcomes=[])]
    routes=set(tasks['newly_observed_nonlocal_parent_routes']);assert routes=={'0x8c647abc','0x8c64b5e4'}
    bridge=(P/reports['connection']['source']).read_text()
    for pc in routes:assert ('if(target=='+pc+'u)return ') in bridge
    fronts=[c['frontier'] for c in reports['frontier_verification']['cases']];assert len(fronts)==2 and all(c['configuration']['kind']=='next_command16' for c in reports['frontier_verification']['cases'])
    assert all(f['pending'] and f['pc']==fronts[0]['pc'] and f['pr']==fronts[0]['pr'] for f in fronts)
    next_pc,next_pr=fronts[0]['pc'],fronts[0]['pr'];assert 'if(target=='+next_pc+'u)return ' not in bridge
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little');long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert long(0x8c64b5d8)==0x8c647610 and word(0x8c64b5c8)==512 and word(0x8c647ab6)==0x55e6 and word(0x8c647ab8)==0x430b
    assert word(0x8c64b5f6)==0x7301 and word(0x8c64b5f8)==0x3e33 and word(0x8c64b5fa)==0x8bef
    assert long(0x8c6476e4)==0xffff0000 and word(0x8c6476c8)==0xff01
    old_path=P/'analysis/movie_filesystem_command14_qualification.json';old=json.loads(old_path.read_text());files.append(old_path);assert old['total_evidence_original_native_comparisons']==51118
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1579,1580) and not any(r['address']==body['address'] for r in registry[:1579]) and not any(r['address']==next_pc and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry)
    count_names=('tasks_verification','retained_verification','frontier_verification');fresh=sum(reports[n]['original_native_comparisons'] for n in count_names);checks=sum(reports[n]['independent_checks'] for n in count_names)
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=54,coverage=coverage[0])
    limits=[
        'One new originalbody54instructions/4conditionaloutcomes all execute. Originalmetadataquery/readwrapper/SDK/arithmetic/callback helpers reusedwithoutnewcredit. No exclusion/canonical/duplicatecredit. This softwarequalification isnotphysicalstorage/fullfilesystem/game proof.',
        'Fullnormal512-blockdevicecopy usesactualrawgeometry511 acquiredbyoriginalquery18,includingoriginallast-blockclassification. Opcode15 readsblocks0..511 inclusive with512-bytecallerstride;262144independentcallerbytes verified plusguards,exactphysicalblock/destinationplan,progressaftereveryblock,metadata/poolpreservation,finalcounters/callback/idle/ownerteardown. Rawinitialquerylastblockandbulkreadsharecoherentstableper-blockimage. No reducedgeometry substitute fornormalcase.',
        'Nine smaller/errorcases spanbothchannels/SDKbyteorders/submissionorders,rawgeometries0/1/2/33,first/later/second-halferrors. Smallergeometries useoriginalquery18unsupported-125 pathanditsdecodedmetadata;parsedstateisNOTforced. Readresponse7->-255 and251->-65536 viaactualwrapper;successful/partialprefix retained,error-tail/guardsunchanged. Wholedevicemasterstopsatfirstnegativewrapperresult. Retrybudget0viaoriginalAPI,abortpolicy0explicit. No suppliedcalleeoutputs/parsedmetadata/frames/IDs/PR orinventedcompletion.',
        '241-sourceDLL/lib andstrictunboundedWallWextraWerrorpass.54freshretailrowsbytechecked;exactprevious240bridgeplus1body/1actualreturn/prefix.2258->2259returnmembership,367explicitnonfunctionexports.2recordedpostyieldparentroutesqualify;66totalqualified/301remaining. CPU/flatcode/guards/productionobserver/rawsession/ABIunchanged;nonewwaitlimits.1579priorregistryrows/source/proofs/2713buildspreserved.',
        '243freshretainedIRQ/context/taskcomparisons retainoldnumericassertions;historical51118notallrerun. Combinedevidenceisnotunique/codecoverage;216warm/211coldhistorical,no freshfullstartup. Twoactualinitializedopcode16frontiersremainpendingat '+next_pc+' /PR'+next_pr+' withoutinventedcallback/teardown. Optional660baewritehook/fullphysicalfilesystem/game/portsunfinished. Arbitrarymalformedcalleraliases/changingmetadata/concurrentdevicehistoriesnotfullyqualified.',
        'SARecompassessmentremainsseparate:PALv1.003 versusUSARevA,privateSDK/AOTbootstrapinputsnotpublic,project-widereuselicensenotconfirmed. Fourmatrixcounterpartsidentified(3exact,1literalrelocation),allalreadyrecovered;0newcredit/0upstreamruntimecodeimport. Potentialnonduplicategameplay/rendererintegrationstillrequiresmapping/reuse-terms/ABIadaptation/localproof. WholePC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOff disables sampling/usesvertexcolors.10%weightedmilestones,notmeasuredcoverage;researchDLLsnotplayableports.'
    ]
    q=dict(status='partially_verified_original_opcode15_whole_device_read_software_domains',compiled_source_count=241,newly_registered_entries=1,newly_qualified_rows=54,qualified_conditional_outcomes=4,executed_rows=54,executed_conditional_outcomes=4,canonical_adapter_rows=0,canonical_duplicate_credit=0,newly_translated_rows=54,domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=[routine],fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=51118,total_evidence_original_native_comparisons=51118+fresh,prior_registry_entries=1579,result_registry_entries=1580,result_unique_module_address_pairs=1577,prior_flat_return_guard_sites=2258,flat_return_guard_sites=2259,new_resume_exports=1,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=66,remaining_unqualified_resume_routes=301,explicit_nonfunction_resume_routes_total=367,completed_command15_cases=10,normal_device_blocks_read=512,normal_device_bytes_read=262144,completed_full_device_cases=1,completed_small_or_error_cases=9,pending_command16_frontiers=fronts,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command15_all_inputs_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Recoveractualopcode16handler'+next_pc+',PR'+next_pr+' fromfreshoriginalevidence. Preservecallerinputs/frames;retain660baeoptionalhookunqualified. EvaluateSARecompnonduplicatepermittedreuseafterPAL-USA/SDKcompatibilitymapping.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    from audit_movie_linked_routines import inventory
    prospective=registry if len(registry)==1580 else registry+[routine]
    q['linked_routine_inventory']=inventory(ROOT/'work/sonic-adventure/build/movie_filesystem_command15.dll',prospective)
    assert q['linked_routine_inventory']['registry_entries']==1580 and q['linked_routine_inventory']['registry_unique_module_address_pairs']==1577
    q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
    (P/'analysis/movie_filesystem_command15_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND15 QUALIFIED1/54rows/4outcomes;',fresh,checks,'next',next_pc,next_pr,flush=True)
if __name__=='__main__':main()
