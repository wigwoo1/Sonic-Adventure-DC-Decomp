"""Evidence-gated qualification of the original whole-device writer."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','tasks_verification','retained_verification','frontier_verification','unbounded'):
        path=P/f'analysis/movie_filesystem_command16_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command16.dll')
    for name in ('audit','tasks_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    for name,wanted in {'retained_verification':(243,1683),'frontier_verification':(92,420)}.items():assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==wanted,name
    tasks=reports['tasks_verification'];assert tasks['original_task_sessions']==len(tasks['cases'])==10
    assert tasks['original_native_comparisons']==sum(len(c['phases']) for c in tasks['cases']) and tasks['independent_checks']==sum(c['independent_checks'] for c in tasks['cases'])
    assert all(all(p['status']==0 for p in c['phases']) for c in tasks['cases'])
    assert all(c['ack_ledger_not_physical_storage'] and not c['failed_block_storage_state_proven'] for c in tasks['cases'])
    full=[c for c in tasks['cases'] if c['configuration']['kind']=='full512'];assert len(full)==1;full=full[0]
    assert full['configuration']['geometry']==511 and full['result']=='0x0' and full['acknowledged_bytes']==262144
    assert full['attempted_blocks']==full['acknowledged_blocks']==list(range(512)) and full['source_sha256']==full['acknowledged_image_sha256']
    assert len(full['packets'])==2560 and [(r['block'],r['phase'],r['status']) for r in full['packets']]==[(b,p,7) for b in range(512) for p in range(5)]
    assert len(full['progress'])==512
    errors={'first_data_error':(0,0,8,0xffffff01),'later_data_error':(1,0,8,0xffffff01),'mid_data_error':(1,2,8,0xffffff01),'commit_error':(0,4,251,0xffffff40)}
    for case in tasks['cases']:
        config=case['configuration'];kind=config['kind']
        assert case['attempted_blocks']==list(range(len(case['attempted_blocks'])))
        if kind in errors:
            block,phase,status,result=errors[kind];assert case['result']==hex(result)
            assert case['acknowledged_blocks']==list(range(block)) and case['acknowledged_bytes']==block*512
            assert (case['packets'][-1]['block'],case['packets'][-1]['phase'],case['packets'][-1]['status'])==(block,phase,status)
            assert len(case['packets'])==block*5+phase+1
        else:assert case['result']=='0x0' and case['acknowledged_bytes']==(config['geometry']+1)*512
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==242 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    t=reports['translation'];assert len(t['routines'])==1 and t['instruction_rows']==54 and t['canonical_reused_entries']==[] and len(t['routines'][0]['conditional_edges'])==4
    body=t['routines'][0];c=tasks['coverage_by_routine'][0];assert c['address']==body['address']=='0x8c64b60e' and not c['missing_rows'] and not c['missing_outcomes'] and c['executed_rows']==54 and c['executed_outcomes']==4
    coverage=[dict(address=body['address'],static_rows=54,executed_rows=54,executed_outcomes=4,domain_excluded_rows=[],domain_excluded_outcomes=[])]
    routes=set(tasks['newly_observed_nonlocal_parent_routes']);assert routes=={'0x8c647acc','0x8c64b650'}
    bridge=(P/reports['connection']['source']).read_text()
    for pc in routes:assert ('if(target=='+pc+'u)return ') in bridge
    fronts=[c['frontier'] for c in reports['frontier_verification']['cases']];assert len(fronts)==2 and all(c['configuration']['kind']=='next_command17' for c in reports['frontier_verification']['cases'])
    assert all(f['pending'] and f['pc']=='0x8c649e00' and f['pr']=='0x8c647b4e' for f in fronts)
    next_pc,next_pr=fronts[0]['pc'],fronts[0]['pr'];assert 'if(target=='+next_pc+'u)return ' not in bridge
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little');long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert long(0x8c64b6b4)==0x8c6476e8 and word(0x8c64b6a8)==512 and word(0x8c647ac6)==0x55e6 and word(0x8c647ac8)==0x420b
    assert word(0x8c64b662)==0x7301 and word(0x8c64b664)==0x3e33 and word(0x8c64b666)==0x8bef
    assert word(0x8c660e30)==251 and word(0x8c660e32)==0xff40 and word(0x8c6477ae)==0xff40 and word(0x8c6477ac)==0xff01
    old_path=P/'analysis/movie_filesystem_command15_qualification.json';old=json.loads(old_path.read_text());files.append(old_path);assert old['total_evidence_original_native_comparisons']==54984
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1580,1581) and not any(r['address']==body['address'] for r in registry[:1580]) and not any(r['address']==next_pc and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry)
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('tasks_verification','retained_verification','frontier_verification'));checks=sum(reports[n]['independent_checks'] for n in ('tasks_verification','retained_verification','frontier_verification'))
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=54,coverage=coverage[0])
    limits=[
        'One new original body:54instructions/4conditionaloutcomes all execute,no exclusions/canonical/duplicatecredit. Existing6476e8wrapper/backend/SDK/callbacks reusedwithoutnewcredit. ACKledger verifies software request payloads and acknowledged blocks,not physical flash/MapleDMA/atomicity/powerloss/durability/fullfilesystem/game.',
        'Normal rawgeometry511 acquired viaoriginalquery18 includinglastblock511classification. Opcode16 writes0..511 inclusive:4exact128-byteDATApackets+emptyCOMMITperblock,2560packets/262144sourcebytes. Whole acknowledged image reconstructs independent source. Literal unit/block/phase/payload/source512stride/waitframes/progress/counters/resultcallback/idle/teardown checked. Entire caller source/guards andmetadata/pool checked aftereveryresponse. No reducedgeometry substitute.',
        'Nine smaller/errorcases spanbothchannels/SDKbyteorders/submissionorders,rawgeometry0/1/2/33,first/later/midDATA8->-255 andCOMMIT251->-192. Originalquery18 rejects smallgeometry-125 while retaining its decodedmetadata;no forcedparsedfields. Stopfirstnegativewrapperresult;successfulACKprefix checked,failedphysicalblockstateNOTproved. Retry0viaoriginalAPI,abortpolicy0explicit,constructorNULLoptionalwritehook retained. No suppliedcalleeoutput/frame/ID/PR/inventedcompletion.',
        '242-sourceDLL/lib andstrictunboundedWallWextraWerrorpass.54freshretailrowsbytechecked;exactprevious241bridgeplus1body/1actualreturn/prefix.2259->2260returnmembership,368explicitnonfunctionexports. Actualroutes647acc/64b650qualify:68total/300remaining. CPU/flatcode/guards/productionobserver/rawsession/ABIunchanged;nonewwaitlimits.1580priorregistryrows/source/proofs/2715buildspreserved.',
        '243freshretainedIRQ/context/taskcomparisons retainoldnumericassertions;historical54984notallrerun. Combinedevidenceisnotunique/codecoverage.216warm/211coldhistorical,no freshfullstartup. Twoactualinitializedopcode17frontierspendingat'+next_pc+'/PR'+next_pr+' withoutinventedcallback/teardown. Optional660baewritehook unqualified. Concurrentmediachanges/arbitrarymalformedaliases/fullfilesystem/game/portsunfinished.',
        'SARecompassessment separate:PALv1.003 vsUSARevA,requiredSDK/AOTbootstrapinputsnotpublic,project-widereuselicensenotconfirmed. Fourmatrixcounterparts allalreadyrecovered;two nonduplicate collisionmath fingerprints found in separate follow-up,unqualified/uncredited.0newcredit/0upstreamruntimecodeimport. Nonduplicatereuse requiresregion/ABI/reuse-terms/localproof. PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. TextureOff disables sampling/usesvertexcolors.10%weightedmilestones,notmeasuredcoverage;researchDLLnotplayableport.'
    ]
    q=dict(status='partially_verified_original_opcode16_whole_device_write_software_domains',compiled_source_count=242,newly_registered_entries=1,newly_qualified_rows=54,qualified_conditional_outcomes=4,executed_rows=54,executed_conditional_outcomes=4,canonical_adapter_rows=0,canonical_duplicate_credit=0,newly_translated_rows=54,domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=[routine],fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=54984,total_evidence_original_native_comparisons=54984+fresh,prior_registry_entries=1580,result_registry_entries=1581,result_unique_module_address_pairs=1578,prior_flat_return_guard_sites=2259,flat_return_guard_sites=2260,new_resume_exports=1,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=68,remaining_unqualified_resume_routes=300,explicit_nonfunction_resume_routes_total=368,completed_command16_cases=10,normal_device_blocks_acknowledged=512,normal_device_bytes_acknowledged=262144,normal_device_write_packets=2560,completed_full_device_cases=1,completed_small_or_error_cases=9,pending_command17_frontiers=fronts,physical_maple_dma_complete=False,physical_write_durability_proven=False,failed_block_storage_state_proven=False,full_filesystem_qualified=False,full_command16_all_inputs_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Recover actual initialized opcode17 dependency '+next_pc+', PR'+next_pr+' fromfreshoriginalevidence;retain660baeoptionalhookunqualified. Continue permitted nonduplicate SARecomp assessment after region/ABI mapping.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    from audit_movie_linked_routines import inventory
    q['linked_routine_inventory']=inventory(ROOT/'work/sonic-adventure/build/movie_filesystem_command16.dll',registry if len(registry)==1581 else registry+[routine])
    assert q['linked_routine_inventory']['registry_entries']==1581 and q['linked_routine_inventory']['registry_unique_module_address_pairs']==1578
    q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
    (P/'analysis/movie_filesystem_command16_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND16 QUALIFIED1/54rows/4outcomes;',fresh,checks,'next',next_pc,next_pr,flush=True)
if __name__=='__main__':main()
