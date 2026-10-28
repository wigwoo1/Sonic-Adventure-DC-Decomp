"""Evidence-gated qualification of original range-reader and FAT seek helper."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    names=('translation','connection','audit','seek_verification','guards_verification','tasks_verification','retained_verification','frontier_verification','unbounded');reports={};files=[Path(__file__)]
    for name in names:
        path=P/f'analysis/movie_filesystem_command14_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command14.dll')
    for name in ('audit','seek_verification','guards_verification','tasks_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    for name,wanted in {'seek_verification':(80,560),'guards_verification':(56,392),'retained_verification':(243,1683),'frontier_verification':(92,420)}.items():assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==wanted,name
    tasks=reports['tasks_verification'];assert tasks['original_task_sessions']==len(tasks['cases'])==16
    assert tasks['original_native_comparisons']==sum(len(c['phases']) for c in tasks['cases']) and tasks['independent_checks']==sum(c['independent_checks'] for c in tasks['cases'])
    assert all(all(p['status']==0 for p in c['phases']) for c in tasks['cases'])
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==240 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    t=reports['translation'];assert len(t['routines'])==2 and t['instruction_rows']==128 and t['canonical_reused_entries']==[] and sum(len(b['conditional_edges']) for b in t['routines'])==22
    coverage=[]
    for body in t['routines']:
        cs=[c for name in ('seek_verification','guards_verification','tasks_verification') for c in reports[name]['coverage_by_routine'] if c['address']==body['address']]
        rows=set.intersection(*(set(c['missing_rows']) for c in cs));edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cs));assert not rows and not edges,(body['address'],rows,edges)
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges']),domain_excluded_rows=[],domain_excluded_outcomes=[]))
    routes=set(tasks['newly_observed_nonlocal_parent_routes']);assert routes=={'0x8c647b3c','0x8c64bc22','0x8c64bc3a'}
    bridge=(P/reports['connection']['source']).read_text()
    for pc in routes:assert ('if(target=='+pc+'u)return ') in bridge
    fronts=[c['frontier'] for c in reports['frontier_verification']['cases']];assert len(fronts)==2 and all(f['pc']=='0x8c64b584' and f['pr']=='0x8c647abc' and f['pending'] for f in fronts)
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little');long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert long(0x8c64bc50)==0xfffa and long(0x8c64bc54)==0xffff0000
    assert word(0x8c647b32)==0x2f26 and word(0x8c647b34)==0x56e7 and word(0x8c647b36)==0x57e8 and word(0x8c647b38)==0x430b
    assert word(0x8c64bbe4)==0x3d38 and word(0x8c64bc0c)==0xbf9e and word(0x8c64bc10)==0xeb00
    old_path=P/'analysis/movie_filesystem_command13_qualification.json';old=json.loads(old_path.read_text());files.append(old_path);assert old['total_evidence_original_native_comparisons']==49797
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1577,1579) and not any(r['address'] in {b['address'] for b in t['routines']} for r in registry[:1577])
    count_names=('seek_verification','guards_verification','tasks_verification','retained_verification','frontier_verification');fresh=sum(reports[n]['original_native_comparisons'] for n in count_names);checks=sum(reports[n]['independent_checks'] for n in count_names)
    routines=[]
    for body in t['routines']:
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(c for c in coverage if c['address']==body['address']));routines.append(row)
    limits=[
        'Two new original bodies128instructions/22conditionaloutcomes all execute in declared software domains. No scoped exclusion or duplicate/canonical routine credit. Original metadata/filename/file-size/open/read/close helpers reused without new credit. Physical filesystem/game/ports unfinished.',
        '16actual raw-initialized command14 tasks use original argument/opcode APIs/scheduler/transport/pump. Offset atcallerAPIstack0,requestedcount atstack1;worker passesrecord+20 asoffset andrecord+24 ascount. Count0 becomes file-size minusoffset;onlynonunderflowauto-count cases completed. Explicitoffset may exceedEOF. ExactFATfollowedphysicalreadplan/rangepayload/EOF-error untouchedtail/rootFATDIR/handles/counters/callback/idle/teardown independentlychecked. Originalseekreturnignored;emptyfileexplicitoffset1 proves this behavior. Readerrors preserveprefix andpropagate-255. No suppliedcalleeoutput/parsedmetadata/savedframe/ID/PR;softwareinputsnotphysicalDMAproof.',
        '80declaredmappedABI seek comparisons coverbothSDKbyteorders/2channels/4CPUFPUprofiles:0/1/2/end/pastend/unsignedmax finitechains,FFFA/remaining0 versusinconsistentcount/head cases. Current resets tohead;remainingNOTreset. FFFA+zero returns0;FFFA+nonzero orremaining0 beforeend returns-65536. ActualSDKreader executes;entireRAM/return/FPU/calleeGPR checks supplementoriginal/nativeCPU/stack/ordered effects. Inconsistenthelperinputsarenotclaimednaturalrawacquisition.56master admission/root comparisons extendnegative size-query/errorpropagation.',
        'Auto-countoffset>file-size wraps modulo32bits and can produce an enormous original read loop. This arithmetic/controlflow is preserved,not patched or hiddenbehindnewbounds. Completedraw-task matrixdoesNOTclaimthisunderflow/concurrentmalformed-inputdomain verified. Seek canresetcurrentwithoutrestoringremainingonrepeatedcalls;originalbehaviorretained,notmodernized.',
        '240-sourceDLL/lib andstrictunboundedWallWextraWerror pass.128freshretailrowsbytechecked;exactprevious239bridgeplus2bodies/6actualreturns/prefix.2252->2258returnmembership,366explicitnonfunctionexports.3recordedpostyieldparentroutesqualify;64totalqualified/302remaining. CPU/flatcode/guards/productionobserver/rawsession/ABIunchanged;nonewwaitlimits.1577priorregistryrows/source/proofs/2711buildspreserved. Initial scaffold self-importfixedbeforecompilation/proofs;no guestbehaviorchange.',
        '243freshretainedIRQ/context/taskcomparisons retainpreviousnumericassertions;historical49797notallrerun. Combinedevidenceisnotunique/codecoverage;216warm/211coldhistorical,no freshfullstartup.2actualinitializedcommand15frontiersreach64b584,PR647abc,pendingwithoutinventedcallback/teardown.660baeoptionalhook/fullphysicalfilesystem/game/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOff disables sampling/usesvertexcolors. Overall10%weightedmilestones,notmeasuredcoverage;researchDLLsnotplayableports.'
    ]
    q=dict(status='partially_verified_original_command14_range_read_and_seek_software_domains',compiled_source_count=240,newly_registered_entries=2,newly_qualified_rows=128,qualified_conditional_outcomes=22,executed_rows=128,executed_conditional_outcomes=22,canonical_adapter_rows=0,canonical_duplicate_credit=0,newly_translated_rows=128,domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=routines,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=49797,total_evidence_original_native_comparisons=49797+fresh,prior_registry_entries=1577,result_registry_entries=1579,result_unique_module_address_pairs=1576,prior_flat_return_guard_sites=2252,flat_return_guard_sites=2258,new_resume_exports=6,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=64,remaining_unqualified_resume_routes=302,explicit_nonfunction_resume_routes_total=366,completed_command14_cases=16,pending_command15_frontiers=fronts,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command14_qualified=False,auto_count_underflow_completed_task_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Recoveractualcommand15handler64b584,PR647abc anditsoriginaldependencies. Preserveactualcallerinputs/frames;retain660baeoptionalhookunqualified.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_command14_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND14 QUALIFIED2/128rows/22outcomes;',fresh,checks,flush=True)
if __name__=='__main__':main()
