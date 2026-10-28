"""Evidence-gated original batch deletion, with one proven impossible outcome."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','tasks_verification','guards_verification','retained_verification','unbounded','zero_preflight_proof'):
        path=P/f'analysis/movie_filesystem_command17_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command17.dll')
    for name in ('audit','tasks_verification','guards_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    tasks=reports['tasks_verification'];guards=reports['guards_verification']
    assert tasks['original_task_sessions']==len(tasks['cases'])==18 and guards['original_task_sessions']==len(guards['cases'])==10
    for suite in (tasks,guards):
        assert suite['original_native_comparisons']==sum(len(c['phases']) for c in suite['cases']) and suite['independent_checks']==sum(c['independent_checks'] for c in suite['cases'])
        assert all(all(p['status']==0 for p in c['phases']) for c in suite['cases'])
    assert all(c['ack_ledger_not_physical_durability'] and not c['failed_block_storage_state_proven'] for c in tasks['cases'])
    cases={c['configuration']['kind']:c for c in tasks['cases']}
    for kind in ('missing','mixed','duplicate','missing_first'):assert cases[kind]['result']=='0xffffff0a'
    assert cases['mixed']['delete_results']==[0,-251,0] and cases['duplicate']['delete_results']==[0,-251]
    for kind in ('zero','negative'):assert cases[kind]['result']=='0x0' and cases[kind]['delete_results']==[] and [x['role'] for x in cases[kind]['transactions']]==['fat','fat']
    for kind in ('dir_abort','later_dir_abort','fat_abort','fat_late_abort','missing_dir_abort','missing_fat_abort'):assert cases[kind]['result']=='0xffffff01'
    assert cases['dir_no_space']['result']=='0xffffff04' and cases['fat_no_space']['result']=='0xffffff40'
    assert all(c['result']==('0xffffff03' if c['configuration']['kind']=='bad_root' else '0xffffff01') for c in guards['cases'])
    busy=[c for c in guards['cases'] if c['configuration']['kind']=='busy'];assert len(busy)==2 and all(c['device_busy_input_is_declared_mapped_state'] and not c['natural_busy_acquisition_proven'] for c in busy)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==243 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    t=reports['translation'];assert len(t['routines'])==1 and t['instruction_rows']==123 and len(t['routines'][0]['conditional_edges'])==28 and t['canonical_reused_entries']==[] and len(t['resume_routes'])==8
    body=t['routines'][0];assert body['address']=='0x8c649e00'
    cvs=[r['coverage_by_routine'][0] for r in (tasks,guards)];rows=set.intersection(*(set(c['missing_rows']) for c in cvs));outcomes=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
    assert not rows and outcomes=={('0x8c649e94','0x8c649eda')}
    proof=reports['zero_preflight_proof'];assert tuple(proof['excluded_outcome'])==next(iter(outcomes)) and not proof['excluded_rows'] and proof['new_routine_credit']==proof['canonical_duplicate_credit']==0 and proof['original_helper_bytes']=='0b0000e0'
    coverage=[dict(address=body['address'],static_rows=123,executed_rows=123,executed_outcomes=27,qualified_outcomes=28,structurally_unreachable_outcomes=[proof['excluded_outcome']],domain_excluded_rows=[],domain_excluded_outcomes=[dict(edge=proof['excluded_outcome'],proof='analysis/movie_filesystem_command17_zero_preflight_proof.json',reason=proof['reason'])])]
    routes=set(tasks['newly_observed_nonlocal_parent_routes']);assert routes=={'0x8c647b4e','0x8c649ec0','0x8c649ecc'}
    bridge=(P/reports['connection']['source']).read_text()
    for pc in routes:assert ('if(target=='+pc+'u)return ') in bridge
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
    word=lambda a:int.from_bytes(binary[a-base:a-base+2],'little');signed=lambda a:int.from_bytes(binary[a-base:a-base+2],'little',signed=True);long=lambda a:int.from_bytes(binary[a-base:a-base+4],'little')
    assert long(0x8c647c3c)==0x8c649e00 and word(0x8c647b46)==0x55e6 and word(0x8c647b48)==0x56e7 and word(0x8c647b4a)==0x420b
    assert [signed(a) for a in (0x8c649ee2,0x8c649ee4,0x8c649ee6)]==[-255,-253,-246]
    assert long(0x8c649f00)==0x8c64b2dc and long(0x8c649f04)==0x8c64afe0
    assert signed(0x8c6479c8+19*2)+0x8c6479b8==0x8c647b6e
    old_path=P/'analysis/movie_filesystem_command16_qualification.json';old=json.loads(old_path.read_text());files.append(old_path);assert old['total_evidence_original_native_comparisons']==63860
    reuse_path=P/'analysis/sarecomp_gameplay_compatibility_probe.json';reuse=json.loads(reuse_path.read_text());files.append(reuse_path)
    candidates={r['name']:r for r in reuse['results']}
    for name,address in (('cross','0x8c027360'),('length','0x8c63a75c')):assert candidates[name]['exact_matches']==[dict(usa_address=address,registered=[])]
    for pin,h in reuse['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1581,1582) and not any(r['address']==body['address'] for r in registry[:1581])
    for address in ('0x8c027360','0x8c63a75c'):assert not any(r['address']==address and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry)
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('tasks_verification','guards_verification','retained_verification'));checks=sum(reports[n]['independent_checks'] for n in ('tasks_verification','guards_verification','retained_verification'));assert (fresh,checks)==(1847,11167)
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=123,coverage=coverage[0])
    limits=[
        'One new original batch-delete body123rows ALLexecute;27of28conditionaloutcomes execute. Remaining649e94->649eda is structurally impossible: original64b2dc RTS/MOV#0,R0 alwayszero;separatebyte/literal/currentbridge/historicaldirectproof. No excluded instructionrow,forcedcalleeoutput,canonicalduplicatecredit or helper recredit.',
        '18actual initialized originaltasks:one/two/reverse/missing/mixed/duplicate/missingfirst,zero/negativecounts,badchain,first/laterDIR/FATdataerrors,DIR/FATcommit251 no-space anddelete-error versuspersistence-errorprecedence. Independent complete RAMroot/FAT/DIR/dirty bytes,callernamearray/strings/guards/pool/metadata,payload/pointer/block/phase,ACKledger,preflightthenallnames,deletioncontinuesaftermissing,DIRthenFATflush,progress/counters/callback/idle/teardown. No suppliedparsedmetadata/frame/ID/PR/returnresult.',
        'Master returns-246 onlyafterboth persistence stages if any removalfailed;successfulprior/laterremovals stillpersist. Persistence negative returntakesprecedence. Zero/negativecounts skipbothnameloops butstill invokeDIR/FATflush,andFATwritesstilloccur. Originalbadchain behavior retained. DATA8=-255,DIRcommit251/noallocation=-252,FATcommit251/noallocation=-192.',
        '10guardtasks useoriginalinterface mask0,constructor-only missingmetadata,rawprimaryscan removal,originalcommand1 acquisition ofdeliberatelybadrawroot,plus2declared mappedSDKbusybyte1 cases. Originalquery18 alone doesNOTacquireadmissionflag;rejectedfixturecorrected usingoriginalcommand1(no nativechange). Busybytecase isexplicitABIstate,NOTnaturallyacquired contention. Worker invalidates only-255;badroot master-253 doesnot. No parsedmetadata/calleeoutputs supplied.',
        '243-sourceDLL/lib andstrictunboundedWallWextraWerrorpass.123freshretailrowsbytechecked;exactprevious242bridgeplus1body/8actualreturns/prefix.2260->2268returnmembership;376explicitnonfunctionexports. Actualpostyieldroutes647b4e/649ec0/649ecc qualify:71total/305remaining. Othernewcontinuations are not credited merely for being exported. CPU/flatcode/guards/productionobserver/rawsession/ABIunchanged,nonewwaitlimits.1581priorregistryrows/source/proofs/2717buildspreserved.',
        '243freshretainedIRQ/context/taskcomparisons retainoldnumericassertions;historical63860notallrerun. Combinedevidenceisnotunique/codecoverage.216warm/211coldstartup historical,no freshcomplete startup. Priorfull512write proof belongs to242build,not anew243full-device test. Optional660baewritehook,naturalbusy/latchacquisition,physicalMapleDMA/VMU/atomicity/powerloss/durability/concurrentmedia/arbitrarymalformedaliases/fullfilesystem/game remainunqualified.',
        'Opcode19 maps to original idlebranch647b6e;do notinventa next missingcommand19. Next independent recovery targets are byte-matched nonduplicate collisionmath027360 and63a75c fromSARecompfollow-up;their nativebehavior isnotyetqualified. No upstreamruntimecode imported;PAL-USA/SDK/reuse-terms limits remain. FullPC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings unfinished. TextureOff disables sampling/usesvertexcolors.10%weightedmilestones,notmeasuredcoverage;researchDLLnotplayableport.'
    ]
    q=dict(status='partially_verified_original_opcode17_batch_delete_software_domains',compiled_source_count=243,newly_registered_entries=1,newly_qualified_rows=123,qualified_conditional_outcomes=28,executed_rows=123,executed_conditional_outcomes=27,structurally_unreachable_conditional_outcomes=1,canonical_adapter_rows=0,canonical_duplicate_credit=0,newly_translated_rows=123,domain_exclusion_arguments=coverage[0]['domain_excluded_outcomes'],coverage_by_routine=coverage,routines=[routine],fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=63860,total_evidence_original_native_comparisons=63860+fresh,prior_registry_entries=1581,result_registry_entries=1582,result_unique_module_address_pairs=1579,prior_flat_return_guard_sites=2260,flat_return_guard_sites=2268,new_resume_exports=8,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=305,explicit_nonfunction_resume_routes_total=376,completed_command17_batch_cases=18,completed_command17_guard_cases=10,next_opcode19_is_idle=True,physical_maple_dma_complete=False,physical_write_durability_proven=False,failed_block_storage_state_proven=False,natural_busy_acquisition_proven=False,full_filesystem_qualified=False,full_command17_all_inputs_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Recover original collision cross-product027360 andvector-length63a75c identifiedbylocalbytefingerprints,usingexistingexactSH4FPUworkflow. No upstreamimplementationcopy ornewcreditwithoutlocalbehaviorproof. Keep660baeoptionalhookunqualified;opcode19isidle.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    from audit_movie_linked_routines import inventory
    q['linked_routine_inventory']=inventory(ROOT/'work/sonic-adventure/build/movie_filesystem_command17.dll',registry if len(registry)==1582 else registry+[routine])
    assert q['linked_routine_inventory']['registry_entries']==1582 and q['linked_routine_inventory']['registry_unique_module_address_pairs']==1579
    q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
    (P/'analysis/movie_filesystem_command17_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND17 QUALIFIED1/123rows/27executed+1impossibleoutcomes;',fresh,checks,flush=True)
if __name__=='__main__':main()
