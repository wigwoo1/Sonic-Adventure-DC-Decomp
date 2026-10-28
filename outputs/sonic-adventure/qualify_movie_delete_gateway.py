"""Qualify delete gateway with executable coverage and a bytechecked constant proof."""
import ast,csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    files=[Path(__file__)];reports={};pins={}
    for name in ('direct','async','tasks_0','tasks_1','tasks_2'):
        p=P/f'analysis/movie_delete_gateway_{name}_verification.json';r=json.loads(p.read_text());reports[name]=r;files.append(p)
        for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);pins[pin]=h;files.append(Path(pin))
    direct=reports['direct'];async_=reports['async'];tasks=[reports['tasks_'+str(i)] for i in range(3)]
    assert (direct['original_native_comparisons'],direct['independent_checks'])==(56,504)
    assert (async_['original_task_sessions'],async_['original_native_comparisons'],async_['independent_checks'])==(2,144,854)
    cases=[c for r in tasks for c in r['cases']];expected={(ch,kind) for ch in (0,7) for kind in ('missing','cache_clear','raw_absent','next_command8')}
    assert len(cases)==8 and {(c['configuration']['channel'],c['configuration']['kind']) for c in cases}==expected
    frontiers=[c['frontier'] for c in cases if c['frontier']];assert len(frontiers)==2
    assert all(c['pc']=='0x8c649f1e' and c['pr']=='0x8c647a86' and c['pending'] for c in frontiers)
    for c in cases:
        if c['configuration']['kind']=='raw_absent':assert c['raw_device_event']['unplug'] and c['raw_device_event']['request_ids_unchanged']
        if not c['frontier']:assert c['phases'][-1]['phase']=='original_completed_owner_teardown'
    cp=P/'analysis/movie_filesystem_persist_connection.json';connection=json.loads(cp.read_text());files.append(cp)
    for pin,h in connection['sha256'].items():assert digest(pin)==h,pin;files.append(Path(pin))
    strict=P/'analysis/movie_filesystem_persist_unbounded.json';s=json.loads(strict.read_text());files.append(strict);assert s['exit_code']==0 and s['source_count']==233
    for pin,h in s['source_sha256'].items():assert digest(pin)==h,pin;files.append(Path(pin))
    dll=ROOT/'work/sonic-adventure/build/movie_filesystem_persist.dll';h=digest(dll);assert h=='554408fa7619e4d55556b9f731551478b265bf81d850a9c5477e9bfe42a65835'
    assert all(r['dll_sha256']==h for r in reports.values())
    old=P/'analysis/movie_filesystem_persist_qualification.json';prior=json.loads(old.read_text());files.append(old);assert prior['total_evidence_original_native_comparisons']==34522
    for pin,hsh in prior['sha256'].items():assert digest(pin)==hsh,pin;files.append(Path(pin))
    mp=P/'analysis/movie_filesystem_delete_translation.json';manifest=json.loads(mp.read_text());files.append(mp);body=next(b for b in manifest['routines'] if b['address']=='0x8c649d20');zero=next(b for b in manifest['routines'] if b['address']=='0x8c64b2dc')
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0
    for routine in (body,zero):
        p=Path(routine['listing']);files.append(p)
        for row in csv.DictReader(p.open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==94 and body['instruction_rows']==92 and len(body['conditional_edges'])==16
    word=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
    long=lambda a:int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert word(0x8c649d94)==0xd318 and long(0x8c649df8)==0x8c64b2dc and word(0x8c649d98)==0x430b
    assert word(0x8c64b2dc)==0x000b and word(0x8c64b2de)==0xe000
    assert word(0x8c649d9c)==0x4011 and word(0x8c649d9e)==0x8902
    bridge=(P/connection['source']).read_text();assert 'if(target==0x8c64b2dcu)return sa_movie_filesystem_delete_8c64b2dc(' in bridge
    source=P/manifest['source'];files.append(source);code=source.read_text();start=code.index('SA_STAGE_EXPORT int sa_movie_filesystem_delete_8c64b2dc(');part=code[start:];assert 'r[0]=0u;return 0;' in part and 'status=call(' not in part
    # Immutable call target; helper delay slot sets R0=0; cmp/pz sets T=1;
    # bt is necessarily taken. No foreign code runs between these operations.
    cvs=[cv for r in reports.values() for cv in r['coverage_by_routine'] if cv['address']==body['address']]
    missing=set.intersection(*(set(c['missing_rows']) for c in cvs));outcomes=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
    assert missing=={'0x8c649da0','0x8c649da2','0x8c649da4'} and outcomes=={('0x8c649d9e','0x8c649da0')},(missing,outcomes)
    wp=P/'analysis/movie-channel-worker/8c647980_instructions.tsv';files.append(wp);rows={int(r['address'],16):r for r in csv.DictReader(wp.open(),delimiter='\t')}
    worker_points=(0x8c647a80,0x8c647a82,0x8c647a84,0x8c647b78,0x8c647b7a,0x8c647b7c,0x8c647b7e,0x8c647b80,0x8c647b82)
    for pc in worker_points:assert word(pc)==int.from_bytes(bytes.fromhex(rows[pc]['bytes']),'little')
    assert long(0x8c647c48)==0x8c64850a and long(0x8c647c18)==0x8c649f1e
    for case in cases:
        if case['frontier']:
            ch=case['configuration']['channel'];record=long(0x8c6479bc)+ch*128
            assert [int(x,16) for x in case['frontier']['arguments']]==[ch,record+0x2c,record+0x3c,0]
    assert '0x8c64af0e' in async_['newly_observed_nonlocal_parent_routes'] and '0x8c64af0c' not in async_['newly_observed_nonlocal_parent_routes']
    assert 'if(target==0x8c64af0eu)return sa_movie_filesystem_persist_resume_8c64af0e(' in bridge
    bm=P/'analysis/movie_delete_gateway_previous_build_manifest.json';build=json.loads(bm.read_text());files.append(bm);assert len(build['files'])==2699
    for row in build['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    scripts=[P/n for n in ('verify_movie_delete_gateway_direct.py','verify_movie_delete_gateway_async.py','verify_movie_delete_gateway_tasks.py','qualify_movie_delete_gateway.py')]
    for p in scripts:ast.parse(p.read_text())
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1545,1546) and not any(r['address']==body['address'] for r in registry[:1545])
    fresh=sum(r['original_native_comparisons'] for r in reports.values());checks=sum(r['independent_checks'] for r in reports.values())
    limits=[
        'Gateway649d20 has92 original rows:89 executed with15 conditional outcomes;three negative-canonical-result rows and one outcome are impossible under the bytechecked immutable64b2dc call target. Original rts delay slot setsR0=0 before cmp/pz and bt;branch/source retained, no fabricated negative return. This local constant proof is not global game reachability or self-modifying-code support.',
        '56 direct coherent mapped-ABI cases acrosschannels0/7/fourCPUFPUprofiles cover inactive/presence/function/busy/cache/root-signature/latch guards. Original constructors/status/readiness/invalidation/rootcheck execute. Independent nonstackRAM/cache/counters/callorder/84byte ABIframe/FPU/calleeGPR expectations;original/native comparison checks entire machine state. Direct fixtures do not claim natural acquisition of every guard input.',
        'Eight additional rawinitialized sessions cover missingname, originalcache-clear API, primaryscanner rawremoval and nextcommand8, bothchannels/order/interface choices. Six rejectedcommand7 cases preserveacquireddata, completecallbacks/idle/ownerteardown withoutwrites. Commonworker -255 cleanup also calls64850a atPR647b82;rawabsence additionally invalidatesinsidegateway atPR649d64. Rawevents do not repairframes/IDs/PR/results.',
        'Two freshinitializedcommand7 success/data-abort lifecycles preserve published independenttransaction/ACK/buffer/counter/callback/cleanup assertions. Correct nonlocal FATpass return64af0e now directlyobserved afterrealresponse wait/resume;precall64af0c gets no credit. Cumulative33 of182nonfunctionexportsqualified,149unqualified;exports get no routinecredit.',
        'Existing233-source nativeC,CPU,flatcode,guards,observer,ABI,DLLandall2699builds unchanged. Previous boundedDLL andunbounded Wall/Wextra/Werror sourcepins verified current;no freshnative rebuild claimed. All earlier1545 registry rows/source/proofs preserved. Earlier34522 comparisons historical,notallrerun;combined totalsnotunique/codecoverage. PhysicalMaple/flashdurability/powerloss/fullfilesystem/fullgenericcommand7/game/ports unfinished;NULLwritehook andexplicitabortpolicy0 inputdomain retained.',
        'Actualcommand8 reachesmissing649f1e,PR647a86,channel plusoriginalworker filename/buffer arguments. Two frontiercommands remainpending;no callback/teardown invented. Recoverthisactualdependency next. Overall10%weightedmilestones,notcodecoverage;goalACTIVE. PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOff disables sampling andusesvertexcolors. ResearchDLLsnotplayableports.'
    ]
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=manifest['source'],instruction_rows=92,executed_rows=89,statically_proven_nonexecuted_rows=sorted(missing),executed_outcomes=15,statically_proven_impossible_outcomes=[list(x) for x in sorted(outcomes)])
    q=dict(status='partially_verified_delete_gateway_declared_ABI_raw_ACK_domain',compiled_source_count=233,native_sources_changed=False,fresh_native_rebuild=False,previous_native_build_and_strict_source_pins_current=True,newly_registered_entries=1,reused_previously_unregistered_rows=92,newly_qualified_rows=92,qualified_conditional_outcomes=16,executed_rows=89,executed_conditional_outcomes=15,statically_proven_nonexecuted_rows=sorted(missing),statically_proven_impossible_outcomes=[list(x) for x in sorted(outcomes)],canonical_duplicate_credit=0,routines=[routine],fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=34522,total_evidence_original_native_comparisons=34522+fresh,prior_registry_entries=1545,result_registry_entries=1546,result_unique_module_address_pairs=1543,new_resume_exports=0,newly_qualified_nonlocal_routes=['0x8c64af0e'],qualified_nonlocal_resume_routes_total=33,remaining_unqualified_resume_routes=149,explicit_nonfunction_resume_routes_total=182,flat_return_guard_sites=2074,original_task_sessions=10,completed_initializations=10,completed_command7_cases=8,pending_command8_frontiers=frontiers,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command7_qualified=False,partial_unregistered_proposals=['0x8c660bae'],next_task='Fresh-bytecheck/recover actualcommand8 handler649f1e reachedfromoriginalworker PR647a86, usingoriginalchannel/name/bufferargs. Preservependingframes/IDs and recoveractualcallees ratherthan suppliedresults. Keep660bae optionalhookunqualified.',dll_sha256=h,prior_build_files_preserved=2699,instruction_rows_byte_checked=103,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_delete_gateway_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('DELETE GATEWAY QUALIFIED1/92rows (89executed+3constantproof)/16outcomes',fresh,checks,flush=True)
if __name__=='__main__':main()
