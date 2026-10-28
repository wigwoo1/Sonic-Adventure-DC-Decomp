"""Qualify original record update in mapped ABI/raw ACK domains, preserving code/proofs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','tasks_verification','retained_verification','frontier_verification','unbounded'):
        p=P/f'analysis/movie_filesystem_command10_{name}.json';r=json.loads(p.read_text());reports[name]=r;files.append(p)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_command10.dll')
    for name in ('audit','direct_verification','tasks_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    assert (reports['direct_verification']['original_native_comparisons'],reports['direct_verification']['independent_checks'])==(56,504)
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    task=reports['tasks_verification'];assert task['original_task_sessions']==16
    assert {c['configuration']['kind'] for c in task['cases']}=={'happy_a','happy_b','happy_c','happy_d','old_missing','policy_negative','zero_word','no_op','high_field','dir_abort','dir_nospace','dir_reloc_first','dir_reloc_second','dir_fat_abort','dir_root_abort','index15'}
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==236 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    body,=reports['translation']['routines'];assert body['instruction_rows']==115 and len(body['conditional_edges'])==16
    cvs=[cv for name in ('direct_verification','tasks_verification') for cv in reports[name]['coverage_by_routine'] if cv['address']==body['address']]
    missing=set.intersection(*(set(c['missing_rows']) for c in cvs));outcomes=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
    assert missing=={'0x8c64a2ce','0x8c64a2d0','0x8c64a2d2'} and outcomes=={('0x8c64a2cc','0x8c64a2ce')},(missing,outcomes)
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
    long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert word(0x8c64a2c2)==0xd322 and long(0x8c64a34c)==0x8c64b2dc and word(0x8c64a2c6)==0x430b
    assert word(0x8c64b2dc)==0x000b and word(0x8c64b2de)==0xe000 and word(0x8c64a2ca)==0x4011 and word(0x8c64a2cc)==0x8902
    assert long(0x8c64a338)==0x8c673bcc and long(0x8c649ff8)==0x8c673bc4,'update/rename initial counters are separate fields'
    bridge=(P/reports['connection']['source']).read_text();assert 'if(target==0x8c64b2dcu)return sa_movie_filesystem_delete_8c64b2dc(' in bridge
    zero_source=P/'source/sa_movie_filesystem_delete.c';files.append(zero_source);code=zero_source.read_text();part=code[code.index('SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b2dc('):];assert 'r[0]=0u;return 0;' in part and 'status=call(' not in part
    routes={'0x8c64a31e','0x8c647a9c'};assert routes<=set(task['newly_observed_nonlocal_parent_routes'])
    for pc in routes:assert ('if(target=='+pc+'u)return ') in bridge
    frontier=reports['frontier_verification'];assert frontier['original_task_sessions']==2
    fronts=[c['frontier'] for c in frontier['cases']];assert all(f and f['pc']=='0x8c64b67a' and f['pr']=='0x8c647ae2' and f['pending'] for f in fronts)
    assert long(0x8c647c2c)==0x8c64b67a
    import csv
    worker=P/'analysis/movie-channel-worker/8c647980_instructions.tsv';files.append(worker)
    wr={int(r['address'],16):r for r in csv.DictReader(worker.open(),delimiter='\t')}
    for pc in (0x8c647ad4,0x8c647ad6,0x8c647ade,0x8c647ae0):assert word(pc)==int.from_bytes(bytes.fromhex(wr[pc]['bytes']),'little')
    for c in frontier['cases']:
        ch=c['configuration']['channel'];record=long(0x8c6479bc)+ch*128
        assert [int(a,16) for a in c['frontier']['arguments']]==[ch,record+0x2c,0,0]
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1555,1556) and not any(r['address']==body['address'] for r in registry[:1555])
    prior=P/'analysis/movie_filesystem_command9_qualification.json';old=json.loads(prior.read_text());files.append(prior);assert old['total_evidence_original_native_comparisons']==38695
    profiles=P/'analysis/movie_filesystem_persist_qualification.json';profile=json.loads(profiles.read_text());files.append(profiles)
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('direct_verification','tasks_verification','retained_verification','frontier_verification'));checks=sum(reports[n]['independent_checks'] for n in ('direct_verification','tasks_verification','retained_verification','frontier_verification'))
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=reports['translation']['source'],instruction_rows=115,executed_rows=112,executed_outcomes=15,statically_proven_nonexecuted_rows=sorted(missing),statically_proven_impossible_outcomes=[list(x) for x in sorted(outcomes)])
    limits=[
        'Original command10 record updater64a244 qualifies115rows/16conditionaloutcomes:112executed/15outcomes plus3localconstant-proof rows/1impossible outcome. Immutableoriginalcall64a34c->64b2dc,rtsdelaymov0R0,cmp/pz/bt prove negativebranch64a2ce/d0/d2 impossible;branch/source retained,no fabricatedreturn orduplicatehelpercredit. Notglobalreachability/selfmodification proof.',
        'Original name lookup rejects missing with-251. Accepted update always storeslow8 ofR7 atrecord+1. Onlywhen softwareword66d3a8 isnonzero,originalSDKselected endianwriter storeslow16 ofR6 atrecord+26. Otherrecordbytes/name/dataFAT unchanged exceptoriginalmetadata-block relocation. Evenunchangedfieldvalues markdirty/write. No unsupported semanticname assigned tofields. DIRwriter64ab36return propagated;no inventedtrailingFATpass.',
        'Sixteen originalconstructor/rawheader/root/FAT/linkedDIR acquisitions thenactualcommand10 APIs/scheduler. Literalindependenttransactionplans check data0C/commit0D unit/block/phase/source/128bytepayloads,whole RAM root/FAT/DIR/dirtybuffers,counters,callback/idle/ownerteardown andACKledger. First/secondsector badblockrelocation,FAT/root abort/no-space anddirectoryabort executeactualcallees. ACKledger notphysicalDMA/flashdurability/powerloss proof. No parsedmetadata/frame/ID/PR/calleeoutput/callbackreturn supplied. NULLwritehook/explicitabortpolicy0 coherentinputdomain only.',
        'Policy66d3a8 retailinitialword0 ispreserved byoriginalconstruction/rawacquisition andassertedbeforetestinput. Nonzero1/FFFFFFFF areexplicitcoherentinitial software-state inputs,notcalleeoutputs. Naturalnondefault activation/writerAPI remainsunproved. Boundedexactmainimage literal/load inventory isnotexhaustive writerinventory,globalunreachability ordeadcode proof. Bothwordbranches qualifyonlywithinthisdeclaredinputdomain.',
        '56directguardcomparisons preserveindependentRAM/counter/callorder/FPU/calleeGPR and140byte nestedABIframebound acrosschannels0/7/fourCPUFPUprofiles. Naturalacquisitionofeveryadmission/presence/busy/cache/root/latchstate isnotclaimed. Updateinitialcounter uses673bcc plusoriginalslot*80;rename673bc4/delete673bc0 aredistinct. OriginalargumentAPI647cc4 andworker acquireactualname/R6/R7.',
        '236-source DLL andstrictunbounded Wall/Wextra/Werror pass.115freshretailrows bytechecked;exactprevious235bridge exceptonebody/tenactualreturns/exports/exportprefix. Returns2114->2124,232nonfunctionexports;42qualified/190unqualified afteractualupdate64a31e andworker647a9c nonlocalresumptions. CPU/flatcode/guards/observer/session/ABI unchanged;nonewwaitlimits. No routinecreditfromexports.',
        '243freshretainedIRQ/context/taskcomparisons preservepriornumericalassertions. Earlier38695comparisons historical,notallrerun;combinedtotalsnotunique/codecoverage. No freshfullstartup;216warm/211cold historical. Earlier1555registryrows/source/proofs/2703builds preserved. Twofreshinitializedcommand11 tasksreachactualmissing64b67a,PR647ae2 withoriginalchannel/name/zero arguments;remainpending withoutinventedcallback/teardown. Recoverthisactualdependency next. Backend660bae optionalhook/fullfilesystem/game/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. TextureOff disables sampling andusesvertexcolors. ResearchDLLsnotplayableports;goalACTIVE10%weightedmilestones,notmeasuredcodecoverage.'
    ]
    q=dict(status='partially_verified_original_command10_record_update_raw_ACK_domain',compiled_source_count=236,newly_registered_entries=1,newly_qualified_rows=115,qualified_conditional_outcomes=16,executed_rows=112,executed_conditional_outcomes=15,statically_proven_nonexecuted_rows=sorted(missing),statically_proven_impossible_outcomes=[list(x) for x in sorted(outcomes)],canonical_duplicate_credit=0,routines=[routine],fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=38695,total_evidence_original_native_comparisons=38695+fresh,prior_registry_entries=1555,result_registry_entries=1556,result_unique_module_address_pairs=1553,prior_flat_return_guard_sites=2114,flat_return_guard_sites=2124,new_resume_exports=10,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=42,remaining_unqualified_resume_routes=190,explicit_nonfunction_resume_routes_total=232,original_task_sessions=18,completed_initializations=18,completed_command10_cases=16,pending_command11_frontiers=fronts,task_original_native_comparisons=task['original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command10_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=profile['historical_warm_profile'],historical_cold_profile=profile['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Freshbytecheck/recover actualcommand11 handler64b67a reachedbyoriginalworker PR647ae2. Preserveoriginalchannel/argumentinput acquisition,frames/IDs/PR andrecoveractualcallees;keep660bae optionalhookunqualified.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    q.update(policy_retail_initial_word=0,nondefault_policy_natural_acquisition_proved=False,policy_writer_inventory_complete=False)
    (P/'analysis/movie_filesystem_command10_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COMMAND10 QUALIFIED1/115rows (112executed+3constantproof)/16outcomes',fresh,checks,flush=True)
if __name__=='__main__':main()
