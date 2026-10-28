"""Evidence-gated qualification of command13 and its original type-CC write helpers."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    names=('translation','connection','audit','guards_verification','edge_guards_verification','tasks_verification','extra_tasks_verification','retained_verification','frontier_verification','unbounded')
    for name in names:
        path=P/f'analysis/movie_filesystem_command13_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command13.dll')
    for name in ('audit','guards_verification','edge_guards_verification','tasks_verification','extra_tasks_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    expected={'guards_verification':(216,1512),'edge_guards_verification':(40,280),'retained_verification':(243,1683),'frontier_verification':(92,420)}
    for name,wanted in expected.items():assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==wanted,name
    tasks=reports['tasks_verification'];extra=reports['extra_tasks_verification']
    assert tasks['original_task_sessions']==len(tasks['cases'])==26 and extra['original_task_sessions']==len(extra['cases'])==3
    for r in (tasks,extra):
        assert r['original_native_comparisons']==sum(len(c['phases']) for c in r['cases'])
        assert r['independent_checks']==sum(c['independent_checks'] for c in r['cases'])
        assert all(all(p['status']==0 for p in c['phases']) for c in r['cases'])
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==239 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    t=reports['translation'];bodies=t['routines'];assert t['instruction_rows']==483 and len(bodies)==4 and t['canonical_reused_entries']==[] and sum(len(b['conditional_edges']) for b in bodies)==74
    suite_names=('guards_verification','edge_guards_verification','tasks_verification','extra_tasks_verification');coverage={}
    for body in bodies:
        cs=[c for name in suite_names for c in reports[name]['coverage_by_routine'] if c['address']==body['address']];rows=set.intersection(*(set(c['missing_rows']) for c in cs));edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cs))
        expected_edges={('0x8c648eba','0x8c648f2c')} if body['address']=='0x8c648e36' else {('0x8c64bad4','0x8c64bb00')} if body['address']=='0x8c64ba30' else set()
        assert not rows and edges==expected_edges,(body['address'],rows,edges)
        coverage[body['address']]=dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges'])-len(edges),domain_excluded_rows=[],domain_excluded_outcomes=[list(e) for e in sorted(edges)])
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little');long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert long(0x8c648ef8)==0x8c64b2dc and word(0x8c648eb4)==0x430b and word(0x8c648eb8)==0x4011 and word(0x8c648eba)==0x8b37
    assert word(0x8c64b2dc)==0x000b and word(0x8c64b2de)==0xe000 and long(0x8c649018)==0xffff and word(0x8c64bad4)==0x8d14
    assert word(0x8c660e30)==251 and word(0x8c660e32)==word(0x8c6477ae)==0xff40
    zero=P/'source/sa_movie_filesystem_delete.c';files.append(zero);part=zero.read_text().split('SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b2dc(')[1];assert 'r[0]=0u;return 0;' in part and 'status=call(' not in part
    bridge=(P/reports['connection']['source']).read_text();assert 'if(target==0x8c64b2dcu)return sa_movie_filesystem_delete_8c64b2dc(' in bridge
    alt=next(b for b in bodies if b['address']=='0x8c64ba30');rows=list(csv.DictReader(Path(alt['listing']).open(),delimiter='\t'))
    assert any(r['address']=='8c64bab8' and r['flows'].strip()=='8c648d24' for r in rows)
    proofs=[dict(kind='immutable_local_constant_return',outcome=['0x8c648eba','0x8c648f2c'],argument='Original64b2dc returns0 inRTSdelay;negativeCMP/PZ branch impossible underunchangedexactbinding.'),dict(kind='zero_count_standard_worker_raw_format_domain',outcome=['0x8c64bad4','0x8c64bb00'],argument='Count0 leaveswritehandleheadFFFF/count0;close serializeshead16FFFF. Standardraw16bituserlimit<=FFFF makesoriginalread-open64b300rejectheadFFFF beforeverifiedreadloop. Stableworkername+2c andcache disjointfromABIstack. Zero-loopskip AFTERsuccessfulreopen excludedonlyinthisstandardworker/raw-formatdomain,notglobalunreachability forarbitraryoutofformatmappedlimits/aliases/concurrentmutations. Allsource/branchesretained.')]
    files.extend([P/'source/sa_movie_filesystem_command11.c',P/'source/sa_movie_filesystem_command.c',P/'source/sa_movie_filesystem_chain.c',P/'source/sa_movie_filesystem_write.c',P/'analysis/movie-filesystem-chain/8c64b300_instructions.tsv',P/'analysis/movie-filesystem-write/8c6476e8_instructions.tsv'])
    routes={pc for name in ('tasks_verification','extra_tasks_verification') for pc in reports[name]['newly_observed_nonlocal_parent_routes']}
    assert routes=={'0x8c649572','0x8c647b22','0x8c64b9e8','0x8c64ba00','0x8c64ba9c','0x8c64bab2','0x8c64bae2','0x8c64bb0a'}
    for pc in routes:assert ('if(target=='+pc+'u)return ') in bridge
    fronts=[c['frontier'] for c in reports['frontier_verification']['cases']];assert len(fronts)==2 and all(f['pc']=='0x8c64bbb2' and f['pr']=='0x8c647b3c' and f['pending'] for f in fronts)
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1573,1577) and not any(r['address'] in {b['address'] for b in bodies} for r in registry[:1573])
    old_path=P/'analysis/movie_filesystem_command12_qualification.json';old=json.loads(old_path.read_text());files.append(old_path);assert old['total_evidence_original_native_comparisons']==46423
    counts=('guards_verification','edge_guards_verification','tasks_verification','extra_tasks_verification','retained_verification','frontier_verification');fresh=sum(reports[n]['original_native_comparisons'] for n in counts);checks=sum(reports[n]['independent_checks'] for n in counts)
    routines=[]
    for body in bodies:
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=coverage[body['address']]);routines.append(row)
    limits=[
        '4originalbodies483instructions/74conditionaloutcomes software-qualified:all483instructions and72outcomes execute;2outcomes have explicit domain-exclusion arguments. Canonicalzero negativeedge impossible underimmutablebinding. Zero-count read-loopskip afterSUCCESSFULreopen excludedonlystandardworker/raw16bitformat/stablecachedname/disjointABIstackdomain,NOTglobalunreachability. Source/branchesretained,no fakecalleeoutputs.',
        '26originalinitializedcommand13 tasks plus3highflagfailuretasks prove typeCC creation,normal/verifiedsequentialwrite0->1,close/reopen/readback/512comparison,exact128byteDATA/DIR/FAT/ROOTpackets/metadata/dirty/flags/timestamp/head/count/attribute1/counters/callback/72clear/idle/ownerteardown. Duplicate-name-250/existingCC-247/prefixcapacity-252/fullDIR-248/poolfull-249 leaveoldrecords/chainsunchanged. Dataabort-255 andspecialcommit251->-192 propagate withoutnext-blockretry/badmark;successfulprefixpreserved. OriginalmasterIGNORESclosereturn;closeIGNORESfinalFATreturn. SoftwareACKledgernotphysicalDMA/flashdurability/atomicity/powerlossproof.',
        '216admission/rootguard and40block/extent/parsedzero-capacity mappedABI comparisons useactualcallees andindependentfullRAM/error/output/cache/FPU/calleeGPR checks. Parsedzeroextent isDECLAREDhelperinput,notclaimednaturalrawheaderacquisition. Rawinterface fixesextent128;rawheaderfield18=0doesnotmakeparsedextent0. Initialtestoraclescorrectedfromdecoder/backend special-error literal andno-spacefixtureconsumesallleadingfreeblocks;nativecodeandwaitlimitsunchanged. Allfinalmatrix/proofs rerunagainstcorrectedsource.',
        '239-sourceDLL/lib andstrictunboundedWallWextraWerror pass.483freshretailrowsbytechecked;exactprevious238bridgeplus4bodies/32actualreturns/prefix.2220->2252returnmembership,360explicitnonfunctionexports. Eightrecordedpostyieldparentroutes qualify,61totalqualified/299remaining;exportsreceive0routinecredit. CPU/flatcode/guards/productionobserver/rawsession/ABIunchanged;nonewwaitlimits. Prior1573registryrows/source/proofs/2709buildspreserved.',
        '243freshretainedIRQ/context/taskcomparisons retainoldnumericassertions;historical46423notallrerun. Combinedevidenceisnotunique/codecoverage. No freshfullstartup;216warm/211coldhistorical. Twoactualinitializedcommand14frontiersreachmissing64bbb2,PR647b3c,pendingwithoutinventedcallback/teardown.660baeoptionalhook/fullphysicalfilesystem/game/portsunfinished. Retailpolicy66d3a4=0retained,explicitabortpolicy0/constructorNULLhook inrawtasks. PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOffdisablessampling/usesvertexcolors. Overall10%weightedmilestones,notmeasuredcoverage;researchDLLsnotplayableports.'
    ]
    q=dict(status='partially_verified_original_command13_type_CC_software_ACK_domains',compiled_source_count=239,newly_registered_entries=4,newly_qualified_rows=483,qualified_conditional_outcomes=74,executed_rows=483,executed_conditional_outcomes=72,canonical_adapter_rows=0,canonical_duplicate_credit=0,newly_translated_rows=483,domain_exclusion_arguments=proofs,coverage_by_routine=list(coverage.values()),routines=routines,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=46423,total_evidence_original_native_comparisons=46423+fresh,prior_registry_entries=1573,result_registry_entries=1577,result_unique_module_address_pairs=1574,prior_flat_return_guard_sites=2220,flat_return_guard_sites=2252,new_resume_exports=32,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=61,remaining_unqualified_resume_routes=299,explicit_nonfunction_resume_routes_total=360,completed_command13_cases=29,pending_command14_frontiers=fronts,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command13_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Recoveractualcommand14handler64bbb2,PR647b3c anditsoriginaldependencies. Preserveactualcallerinputs/frames;retain660baeoptionalhookunqualified.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_command13_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND13 QUALIFIED4/483rows/74outcomes;',fresh,checks,flush=True)
if __name__=='__main__':main()
