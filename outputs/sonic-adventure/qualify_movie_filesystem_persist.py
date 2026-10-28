"""Qualify original persistence helpers in explicit mapped/ACK input domains."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','tasks_verification','retained_verification','unbounded'):
        p=P/f'analysis/movie_filesystem_persist_{name}.json';r=json.loads(p.read_text());reports[name]=r;files.append(p)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_persist.dll')
    for name in ('audit','direct_verification','tasks_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    assert (reports['direct_verification']['original_native_comparisons'],reports['direct_verification']['independent_checks'])==(544,5088)
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    task=reports['tasks_verification'];assert task['original_task_sessions']==17
    assert all(c['requests'] and c['phases'] for c in task['cases'])
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==233 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    routines=[];coverage=[]
    for body in reports['translation']['routines']:
        cvs=[cv for name in ('direct_verification','tasks_verification') for cv in reports[name]['coverage_by_routine'] if cv['address']==body['address']]
        missing=set.intersection(*(set(cv['missing_rows']) for cv in cvs));edges=set.intersection(*(set(map(tuple,cv['missing_outcomes'])) for cv in cvs))
        cv=dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(edges),missing_rows=sorted(missing),missing_outcomes=sorted(edges));coverage.append(cv)
        assert not missing and not edges,(body['address'],missing,edges)
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=reports['translation']['source'],instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);routines.append(row)
    assert len(routines)==7 and sum(r['instruction_rows'] for r in routines)==447 and sum(r['executed_outcomes'] for r in routines)==56
    # The task trace also watches 64af0c (a pre-call delay slot). It is NOT a
    # continuation export and receives no route credit. Only declared actual
    # return exports observed after genuine asynchronous resumes qualify here.
    routes={'0x8c64ab88','0x8c64b00a','0x8c64b06c','0x8c64ac38','0x8c64ac42','0x8c649db6','0x8c647a70'}
    assert routes<=set(task['newly_observed_nonlocal_parent_routes'])
    new_exports={r['pc'] for r in reports['translation']['resume_routes']}
    old_translation=json.loads((P/'analysis/movie_filesystem_delete_translation.json').read_text());old_exports={r['pc'] for r in old_translation['resume_routes']}
    bridge=(P/reports['connection']['source']).read_text()
    assert all(('if(target=='+pc+'u)return ') in bridge for pc in routes)
    assert '0x8c64af0c' not in new_exports|old_exports
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1538,1545)
    assert not ({r['address'] for r in routines}&{r['address'] for r in registry[:1538] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'})
    prior=P/'analysis/movie_filesystem_delete_qualification.json';old=json.loads(prior.read_text());files.append(prior);assert old['total_evidence_original_native_comparisons']==32056
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('direct_verification','tasks_verification','retained_verification'));checks=sum(reports[n]['independent_checks'] for n in ('direct_verification','tasks_verification','retained_verification'))
    limitations=[
        'Seven original persistence/allocator bodies qualify 447 rows and 56 conditional outcomes in declared mapped helper and initialized raw-device ACK domains. This is not whole-filesystem, physical flash durability, power-loss atomicity or gameplay qualification.',
        'Directory64ab36 writes dirty 512-byte sectors, follows original FAT links, preserves immediate -255 abort and -252 no-space, marks bad blocks FFFF, relocates into original two-region allocator, updates root head or previous FAT link, and propagates final FAT/root negative results. Cached directory head is not repaired by the recovery.',
        'FAT64aecc marks each successful physical block FFFA or failed block FFFF. Persistence64afe0 relocates only for -192, preserves -255, and retains original ignored other errors including device-removal -65536 and ignored root-write return after relocation. A successful command callback can therefore coexist with unacknowledged metadata; no stronger save guarantee is claimed.',
        'Allocator64894a rejects found block zero and falls back to the second region. Scanner6489d4 is inclusive descending with SDK-selected low16 reader. Contiguous64af4a retains original zero-count found+1 and search-span-below-pool behavior; mapped edge vectors are not valid-card-format claims.',
        'Seventeen original constructor/raw-header/root/FAT/linked-directory acquisitions then actual command7 APIs/scheduler execute all data/commit requests, raw ACK/error/removal inputs, original completion callback, idle resumption and owner teardown. One two-dirty case calls original RAM removal while idle before command7. Independent literal plans check unit/block/phase/source/128-byte input payloads, RAM root/FAT/DIR/dirty bytes, counters and callback. ACK ledger records acknowledged images, not physical DMA/flash. Initial abort policy0 is an explicit coherent software-policy input; natural acquisition remains unproved. Optional writehook remains constructor-cleared NULL; no supplied hook/callee output, decoded metadata, requestID, PR or saved frame.',
        '233-source DLL and unbounded Wall/Wextra/Werror compilation pass. 447 freshly exported original rows byte checked. Exact previous232 bridge except seven bodies, 24 actual return routes and export prefix; membership2050->2074. CPU, flat execution, guards, observer, session and ABI unchanged; no new wait limits. Seven additional observed actual parent return routes qualify, leaving150 of182 exports unqualified. Pre-call64af0c gets zero route credit;64af0e remains conservatively outside this route-specific inventory. Continuations get no routine credit.',
        '243 fresh retained IRQ/context/task comparisons preserve prior numeric expectations. Earlier32056 comparisons historical, not all rerun; combined totals are not unique cases or code coverage. No fresh full startup;216warm/211cold historical. Old1538 registry rows/source/proofs and2697 archived builds preserved. Gateway649d20 and backend660bae remain partial/unregistered; full command7/full filesystem unfinished. Research DLLs not playable ports. GoalACTIVE about10% weighted milestones, not measured coverage;PC/Android/3DS/N64/PS1/FPS/8K/SUPIR/settings unfinished. TextureOff disables sampling and uses vertex colors.'
    ]
    q=dict(status='partially_verified_original_persistence_raw_ACK_domain',compiled_source_count=233,newly_registered_entries=7,newly_qualified_rows=447,qualified_conditional_outcomes=56,newly_translated_unregistered_rows=447,canonical_adapter_rows=0,canonical_duplicate_credit=0,partial_unregistered_proposals=['0x8c649d20','0x8c660bae'],routines=routines,coverage_by_routine=coverage,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=32056,total_evidence_original_native_comparisons=32056+fresh,prior_registry_entries=1538,result_registry_entries=1545,result_unique_module_address_pairs=1542,prior_flat_return_guard_sites=2050,flat_return_guard_sites=2074,new_resume_exports=24,newly_qualified_nonlocal_routes=sorted(routes),qualified_nonlocal_resume_routes_total=32,remaining_unqualified_resume_routes=150,explicit_nonfunction_resume_routes_total=182,original_task_sessions=17,completed_initializations=17,completed_command7_cases=17,task_original_native_comparisons=task['original_native_comparisons'],physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command7_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Qualify remaining gateway649d20 guards with actual caller/input acquisition, retain impossible canonical negative branch without fabricated output; identify next reached original filesystem dependency. Keep optional660bae hook and unqualified return exports explicit.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_persist_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('PERSIST QUALIFIED7/447rows/56outcomes',fresh,checks,flush=True)
if __name__=='__main__':main()
