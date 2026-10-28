"""Qualify five original read-family bodies; retain dirty close as an unregistered proposal."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','handles_verification','guards_verification','tasks_verification','read_errors_verification','open_alias_verification','retained_verification','frontier_verification','unbounded'):
        path=P/f'analysis/movie_filesystem_command11_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command11.dll')
    for name in ('audit','handles_verification','guards_verification','tasks_verification','read_errors_verification','open_alias_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    expected={'handles_verification':(56,336),'guards_verification':(204,1424),'tasks_verification':(646,3109),'retained_verification':(243,1683),'frontier_verification':(92,420)}
    for name,wanted in expected.items():assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==wanted
    assert reports['tasks_verification']['original_task_sessions']==12 and reports['read_errors_verification']['original_task_sessions']==4 and reports['open_alias_verification']['original_initialized_helper_sessions']==2
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==237 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    t=reports['translation'];assert t['instruction_rows']==547 and len(t['routines'])==7 and t['canonical_reused_entries']==['0x8c64b2d4']
    coverage={}
    for body in t['routines']:
        cs=[c for name in ('handles_verification','guards_verification','tasks_verification','read_errors_verification','open_alias_verification') for c in reports[name]['coverage_by_routine'] if c['address']==body['address']]
        missing=set.intersection(*(set(c['missing_rows']) for c in cs));edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cs))
        coverage[body['address']]=dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(edges),missing_rows=sorted(missing),missing_outcomes=[list(e) for e in sorted(edges)])
    qualified={'0x8c64b67a','0x8c64a360','0x8c648d24','0x8c64965c','0x8c64b2a6'}
    size_missing={'0x8c64a3cc','0x8c64a3ce','0x8c64a3d0','0x8c64a3d2','0x8c64a3d4'}
    for a in qualified:
        assert set(coverage[a]['missing_rows'])==(size_missing if a=='0x8c64a360' else set()),(a,coverage[a])
        wanted=[['0x8c64a3ca','0x8c64a3cc']] if a=='0x8c64a360' else [['0x8c648d94','0x8c648db4']] if a=='0x8c648d24' else []
        assert coverage[a]['missing_outcomes']==wanted,(a,coverage[a])
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little');long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert word(0x8c64a3c4)==0xb78a and 0x8c64a3c8+0x78a*2==0x8c64b2dc
    assert word(0x8c64b2dc)==0x000b and word(0x8c64b2de)==0xe000 and word(0x8c64a3c8)==0x4011 and word(0x8c64a3ca)==0x8904
    assert long(0x8c648ddc)==0x8c64b2dc and word(0x8c648d8e)==0x430b and word(0x8c648d92)==0x4011 and word(0x8c648d94)==0x8b0e
    zero=P/'source/sa_movie_filesystem_delete.c';files.append(zero);part=zero.read_text().split('SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b2dc(')[1];assert 'r[0]=0u;return 0;' in part and 'status=call(' not in part
    bridge=(P/reports['connection']['source']).read_text();assert 'if(target==0x8c64b2dcu)return sa_movie_filesystem_delete_8c64b2dc(' in bridge
    fronts=[c['frontier'] for c in reports['frontier_verification']['cases']];assert len(fronts)==2 and all(f['pc']=='0x8c64b71c' and f['pr']=='0x8c647b02' and f['pending'] for f in fronts)
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1556,1561) and not any(r['address'] in qualified for r in registry[:1556])
    previous=P/'analysis/movie_filesystem_command10_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==40253
    names=('handles_verification','guards_verification','tasks_verification','read_errors_verification','open_alias_verification','retained_verification','frontier_verification');fresh=sum(reports[n]['original_native_comparisons'] for n in names);checks=sum(reports[n]['independent_checks'] for n in names)
    routines=[]
    for body in t['routines']:
        if body['address'] not in qualified:continue
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=coverage[body['address']]);routines.append(row)
    limits=[
        'Five original read-family bodies qualify382rows/68outcomes:377rows/66outcomes execute;5size-error rows and2negativecanonical outcomes have localbytechecked zero-returnproofs. Source/branches retained;no fabricatedcalleeoutput orduplicatehelpercredit. Readmaster64b67a70,size64a36077,open648d24114,read64965c98,allocator64b2a623. Canonicalclear64b2d4fourrows gets0newcredit. Close64919e161rows remainsPARTIALUNREGISTERED;dirtyclosewritebranches unqualified.',
        'Originalcommand11 zero requestedcount queries decodedfileblockcount;nonzero count bypasses sizequery. Open validates originalFATchain andallocatesfirstfree72bytehandle. Reads followoriginalFAT links in512byte strides;EOF returns0 without touchingremainingcallerbuffer,evenwhen requestedcount exceedsfilelength. Master preservesnegative readresult,alwayscloses aftersuccessfulopen,ignoresclosereturn. Exactoriginalbehavior retained,not repaired.',
        '12 original initialized command11 success/EOF/missing/invalidchain/poolfull tasks;bothbyteorders. Exactrawpayload/FATfollowedreadplan/metadata-preservation/counters/callback/idle/ownerteardown. Poolfull acquiredby8actualallocatorcalls,thenreleasedbyoriginalclear. Four initializedrawstatus7 errors retain-255 underexplicitsoftwarepolicy0/1;bothincrementprogress/errorcounters. Originalearlyabort comparisonis-208,soearlyabortnotclaimedhere. No suppliedcalleeoutputs,parsedmetadata,frames,IDs,PR orcallbackresults;notphysicalDMA/flashdurability/game.',
        'Two original rawinitialized helper sessions useexplicitcallerfilename alias atfreepoolrecord+12. Firstvalidation seesSONIC,actualallocation overwritesindex0/namebytes,secondlookup genuinely returns-1 storedglobally,openmaps-251andclearsrecord. ThisdeclaredmappedABIalias isnotnaturalworkerfilenameacquisition;ordinaryworkerfilenameisitsseparaterecord+2c. Theoriginalfailurepath isnotdiscardedasunreachable.',
        '56mappedallocator/clear and204mappedguard/EOF/NULLclose comparisons includeindependentwholeRAM/counter/cache/output/FPU/calleeGPR expectations acrossfourCPUFPUprofiles. Naturalacquisitionofeveryguard/invalidmarker/policyinput notclaimed.132/84/32byte uppernestedABIscratch bounds,fullstackstillcheckedbyoriginal/nativecomparison.',
        '237-sourceDLL andstrictunboundedWallWextraWerror pass.547freshoriginalrowsbytechecked;exact236bridgeplus7bodies/37actualreturns/exports/prefix. Returns2124->2161;269continuationexports,42priorqualified/227remaining. No newroute-specific creditassignedthisstage;exportsreceive0routinecredit. CPU/flatcode/guards/observer/rawsession/ABIunchanged;nonewwaitlimits. Prior1556registryrows/source/proofs/2705builds preserved.',
        '243freshretainedIRQ/context/taskcomparisons preservepriornumericexpectations. Prior40253comparisons historical,notallrerun;combinedtotalsnotunique/codecoverage. No freshfullstartup;216warm/211coldhistorical. Twofreshinitializedcommand12frontiers reachactualmissing64b71c,PR647b02 andremainpending. Nextrecoveractualwritepathandqualifyclosewritebranches. Backend660baeoptionalhook/fullfilesystem/game/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOff disablessampling/usesvertexcolors. GoalACTIVE10%weightedmilestones,notmeasuredcoverage;researchDLLsnotplayableports.'
    ]
    q=dict(status='partially_verified_original_command11_read_family',compiled_source_count=237,newly_registered_entries=5,newly_qualified_rows=382,qualified_conditional_outcomes=68,executed_rows=377,executed_conditional_outcomes=66,canonical_adapter_rows=4,canonical_duplicate_credit=0,partial_unregistered_gateway_rows=161,newly_translated_unregistered_rows=161,partial_unregistered_proposals=['0x8c64919e','0x8c660bae'],routines=routines,coverage_by_routine=list(coverage.values()),fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=40253,total_evidence_original_native_comparisons=40253+fresh,prior_registry_entries=1556,result_registry_entries=1561,result_unique_module_address_pairs=1558,prior_flat_return_guard_sites=2124,flat_return_guard_sites=2161,new_resume_exports=37,newly_qualified_nonlocal_routes=[],qualified_nonlocal_resume_routes_total=42,remaining_unqualified_resume_routes=227,explicit_nonfunction_resume_routes_total=269,completed_command11_cases=16,initialized_open_alias_cases=2,pending_command12_frontiers=fronts,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command11_qualified=False,dirty_close_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Recoveractualcommand12handler64b71c,PR647b02 andactualwrite/open dependencies;useoriginalwritepath toqualifyremaining161-rowclose64919e,withoutinventedframes/results. Keep660baeoptionalhookunqualified.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_command11_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND11 QUALIFIED5/382rows/68outcomes;close161partial;',fresh,checks,flush=True)
if __name__=='__main__':main()
