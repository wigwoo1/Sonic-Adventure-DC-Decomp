"""Qualify original rename in mapped ABI/raw ACK domains, preserving code/proofs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','tasks_verification','retained_verification','frontier_verification','unbounded'):
        p=P/f'analysis/movie_filesystem_command8_{name}.json';r=json.loads(p.read_text());reports[name]=r;files.append(p)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command8.dll')
    for name in ('audit','direct_verification','tasks_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    assert (reports['direct_verification']['original_native_comparisons'],reports['direct_verification']['independent_checks'])==(56,504)
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    task=reports['tasks_verification'];assert task['original_task_sessions']==16
    assert {c['configuration']['kind'] for c in task['cases']}=={'happy_a','happy_b','happy_c','happy_d','old_missing','duplicate_old','duplicate_other','exact_old','case_sensitive','dir_abort','dir_nospace','dir_reloc_first','dir_reloc_second','dir_fat_abort','dir_root_abort','spaces'}
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==234 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    body,=reports['translation']['routines'];assert body['instruction_rows']==116 and len(body['conditional_edges'])==18
    cvs=[cv for name in ('direct_verification','tasks_verification') for cv in reports[name]['coverage_by_routine'] if cv['address']==body['address']]
    missing=set.intersection(*(set(c['missing_rows']) for c in cvs));outcomes=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
    assert missing=={'0x8c649fa0','0x8c649fa2','0x8c649fa4'} and outcomes=={('0x8c649f9e','0x8c649fa0')},(missing,outcomes)
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
    long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert word(0x8c649f94)==0xd31d and long(0x8c64a00c)==0x8c64b2dc and word(0x8c649f98)==0x430b
    assert word(0x8c64b2dc)==0x000b and word(0x8c64b2de)==0xe000 and word(0x8c649f9c)==0x4011 and word(0x8c649f9e)==0x8902
    assert long(0x8c649ff8)==0x8c673bc4 and long(0x8c649de4)==0x8c673bc0,'rename/delete initial counters are separate fields'
    bridge=(P/reports['connection']['source']).read_text();assert 'if(target==0x8c64b2dcu)return sa_movie_filesystem_delete_8c64b2dc(' in bridge
    zero_source=P/'source/sa_movie_filesystem_delete.c';files.append(zero_source);code=zero_source.read_text();part=code[code.index('SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b2dc('):];assert 'r[0]=0u;return 0;' in part and 'status=call(' not in part
    routes={'0x8c64a02a','0x8c647a86'};assert routes<=set(task['newly_observed_nonlocal_parent_routes'])
    for pc in routes:assert ('if(target=='+pc+'u)return ') in bridge
    frontier=reports['frontier_verification'];assert frontier['original_task_sessions']==2
    fronts=[c['frontier'] for c in frontier['cases']];assert all(f and f['pc']=='0x8c64a828' and f['pr']=='0x8c647aac' and f['pending'] for f in fronts)
    assert long(0x8c647c20)==0x8c64a828
    import csv
    worker=P/'analysis/movie-channel-worker/8c647980_instructions.tsv';files.append(worker)
    wr={int(r['address'],16):r for r in csv.DictReader(worker.open(),delimiter='\t')}
    for pc in (0x8c647aa2,0x8c647aa6,0x8c647aa8,0x8c647aaa):assert word(pc)==int.from_bytes(bytes.fromhex(wr[pc]['bytes']),'little')
    for c in frontier['cases']:
        ch=c['configuration']['channel'];record=long(0x8c6479bc)+ch*128
        assert [int(a,16) for a in c['frontier']['arguments']]==[ch,0,0,0]
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1546,1547) and not any(r['address']==body['address'] for r in registry[:1546])
    prior=P/'analysis/movie_delete_gateway_qualification.json';old=json.loads(prior.read_text());files.append(prior);assert old['total_evidence_original_native_comparisons']==35108
    profiles=P/'analysis/movie_filesystem_persist_qualification.json';profile=json.loads(profiles.read_text());files.append(profiles)
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('direct_verification','tasks_verification','retained_verification','frontier_verification'));checks=sum(reports[n]['independent_checks'] for n in ('direct_verification','tasks_verification','retained_verification','frontier_verification'))
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=reports['translation']['source'],instruction_rows=116,executed_rows=113,executed_outcomes=17,statically_proven_nonexecuted_rows=sorted(missing),statically_proven_impossible_outcomes=[list(x) for x in sorted(outcomes)])
    limits=[
        'Original command8 rename649f1e qualifies116rows/18conditionaloutcomes:113executed/17outcomes plus3localconstant-proof rows/1impossible outcome. Immutableoriginalcall64a00c->64b2dc,rtsdelaymov0R0,cmp/pz/bt prove negativebranch649fa0/fa2/fa4 impossible;branch/source retained,no fabricatedreturn orduplicatehelpercredit. Notglobalreachability/selfmodification proof.',
        'Originaloldname lookup rejects missing with-251;destinationlookup rejects anyexistingname(includingolditself) with-250. Acceptedrename copiesexactly12bytes into directoryrecord+4..+15,marksitssector dirty andcallsoriginal64ab36. No inventedNULpadding,filenamevalidation,casefolding orfilechainfree. Empty/embeddedNUL/fullwidth/space/case inputs retainoriginalbehavior;otherrecordfields anddataFATchains unchanged exceptoriginalmetadata-block relocation.',
        'Sixteen originalconstructor/rawheader/root/FAT/linkedDIR acquisitions thenactualcommand8 APIs/scheduler. Literalindependenttransactionplans check data0C/commit0D unit/block/phase/source/128bytepayloads,whole RAM root/FAT/DIR/dirtybuffers,counters,callback/idle/ownerteardown andACKledger. First/secondsector badblockrelocation,FAT/root abort/no-space anddirectoryabort execute actualcallees. Successfulnonrelocatedrename writesDIR only;no inventedtrailingFATpass. ACKledger notphysicalDMA/flashdurability/powerloss proof. No parsedmetadata/frame/ID/PR/calleeoutput/callbackreturn supplied. NULLwritehook/explicitabortpolicy0 coherentinputdomain only.',
        '56directguardcomparisons preserve independentRAM/counter/callorder/FPU/calleeGPR and132byte nestedABIframebound acrosschannels0/7/fourCPUFPUprofiles. Naturalacquisitionofeveryadmission/presence/busy/cache/root/latchstate isnotclaimed. Renameinitialcounter uses673bc4 plusoriginalslot*80;deleteusesdistinct673bc0. Newtestoracles correctedfrombytecheckedrename649ff8 literal;nativeC unchanged.',
        '234-source DLL andstrictunbounded Wall/Wextra/Werror pass.116freshretailrows bytechecked;exactprevious233bridge exceptonebody/tenactualreturns/exports/exportprefix. Returns2074->2084,192nonfunctionexports;35qualified/157unqualified afteractualrename64a02a andworker647a86 nonlocalresumptions. CPU/flatcode/guards/observer/session/ABI unchanged;no newwaitlimits. No routinecreditfromexports.',
        '243freshretainedIRQ/context/taskcomparisons preservepriornumericassertions. Earlier35108comparisons historical,notallrerun;combinedtotalsnotunique/codecoverage. No freshfullstartup;216warm/211cold historical. Earlier1546registryrows/source/proofs/2699builds preserved. Twofreshinitializedcommand9 tasksreachactualmissing64a828,PR647aac withoriginalchannel/zero arguments;remainpending withoutinventedcallback/teardown. Recoverthisactualdependency next. Backend660bae optionalhook/fullfilesystem/game/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings unfinished. TextureOff disables sampling andusesvertexcolors. ResearchDLLsnotplayableports;goalACTIVE10%weightedmilestones,notmeasuredcodecoverage.'
    ]
    q=dict(status='partially_verified_original_command8_rename_raw_ACK_domain',compiled_source_count=234,newly_registered_entries=1,newly_qualified_rows=116,qualified_conditional_outcomes=18,executed_rows=113,executed_conditional_outcomes=17,statically_proven_nonexecuted_rows=sorted(missing),statically_proven_impossible_outcomes=[list(x) for x in sorted(outcomes)],canonical_duplicate_credit=0,routines=[routine],fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=35108,total_evidence_original_native_comparisons=35108+fresh,prior_registry_entries=1546,result_registry_entries=1547,result_unique_module_address_pairs=1544,prior_flat_return_guard_sites=2074,flat_return_guard_sites=2084,new_resume_exports=10,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=35,remaining_unqualified_resume_routes=157,explicit_nonfunction_resume_routes_total=192,original_task_sessions=18,completed_initializations=18,completed_command8_cases=16,pending_command9_frontiers=fronts,task_original_native_comparisons=task['original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command8_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=profile['historical_warm_profile'],historical_cold_profile=profile['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Freshbytecheck/recover actualcommand9 handler64a828 reachedbyoriginalworker PR647aac. Preserveoriginalchannel/argumentinput acquisition,frames/IDs/PR andrecoveractualcallees;keep660bae optionalhookunqualified.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_command8_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND8 QUALIFIED1/116rows (113executed+3constantproof)/18outcomes',fresh,checks,flush=True)
if __name__=='__main__':main()
