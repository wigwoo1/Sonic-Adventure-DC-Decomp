"""Qualify six submission bodies without claiming physical DMA or task parsing."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','verification','lifecycle_verification','faults_verification','retained_verification','unbounded'):
        path=P/f'analysis/movie_input_submission_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    expected={'verification':(684,13464),'lifecycle_verification':(192,1792),'faults_verification':(48,432),'retained_verification':(323,2607)}
    dll=digest(ROOT/'work/sonic-adventure/build/movie_input_submission.dll')
    for name,pair in expected.items():
        assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==pair,name
        if 'dll_sha256' in reports[name]:assert reports[name]['dll_sha256']==dll
    t=reports['translation'];c=reports['connection'];a=reports['audit'];r=reports['verification']
    assert (len(t['routines']),t['instruction_rows'],t['proposed_rows'],t['canonical_adapter_rows'],c['compiled_source_count'])==(7,384,351,33,224)
    assert reports['unbounded']['exit_code']==0 and reports['unbounded']['source_count']==224
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    assert (r['executed_adapter_rows'],r['executed_adapter_outcomes'],r['new_body_rows'],r['new_body_outcomes'])==(384,52,351,46)
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1502,1508)
    baseline=registry[:1502];routines=[]
    for body in t['routines']:
        cv=next(x for x in r['coverage_by_routine'] if x['address']==body['address']);assert not cv['missing_rows'] and not cv['missing_outcomes']
        existing=next((x for x in baseline if x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==body['address']),None)
        if existing:
            assert body['address']=='0x8c65f1aa';continue
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);routines.append(row)
    assert len(routines)==6 and sum(x['instruction_rows'] for x in routines)==351 and sum(x['executed_outcomes'] for x in routines)==46
    prior=P/'analysis/movie_input_pump_qualification.json';old=json.loads(prior.read_text());files.append(prior)
    limitations=[
        'Six newly qualified original bodies351instructions/46outcomes, plus canonical preflight33instructions/6outcomes with zero duplicate credit. All384adapter rows/52outcomes execute in declared coherent SDK/stack/buffer inputs. Local names are interpretations, not recovered symbols. Finite tests are not exhaustive ABI/game coverage.',
        'Original65efda submission and65eeda preparation execute real report/division/packet/terminate/address callees. Independent packet/context/buffer projections, all256TASbytes/purge/neighbors, readiness/busy/lock priority, command dispatch and payload byte order, all24record transitions, two buffer banks, signed timing-product edges and exact MMIO order pass.',
        'Canonical preflight body is byte-identical to prior guarded adapter after name change; its shared guard now admits four observed aligned32bit transport register addresses. Exact prior bridge inverse verifies six route additions, one canonical replacement, return membership and export prefix only. Existing exception/IRQ/runtime/worker continuations and external ABI unchanged. No new wait bounds or nonlocal continuation routes.',
        '64three-phase lifecycles192comparisons:original API creates queued ID, original submission moves it to inflight and writes DMA buffer/start registers, original responsepump completes ID and rearms. Busy-clear/raw RX bytes are explicit external device observations. Neither completed IDs nor saved task frames nor parser/callback results are supplied. No complete active task parser continuation is established yet.',
        'Existing startup-platform bus provides explicit software register storage. It does not implement Maple DMA traversal, physical controller acquisition, device timing or automatic interrupt completion. Actual four MMIO accesses use bus callbacks; RAM shadow backing is unchanged. No physical cache/TLB or concurrent host atomicity claim.',
        '48reached bus/null/misalignment/signed-clock comparisons preserve exact prefixes and pending IDs.323fresh retained queued-task/pending/cancel/softwareIRQ/contextSRFD comparisons retain old numeric expectations. Original execution is the project SH4 instruction reference, not hardware or whole-game execution. No fresh224 warm/cold startup; old216warm/211cold historical.',
        '1247fresh original/native comparisons18295independent checks. Earlier17227comparisons are historical, not all rerun;18474combined evidence is not unique cases or code coverage. Larger602cb8input/652210handler,6608a4activeIO and arbitrary task/parser graphs remain partial/unregistered where documented.',
        'Wholegame and allPC/Android/3DS/N64/PS1/FPS/8K/SUPIR/settings requirements remain unfinished. TextureOff disables sampling and uses vertex colors. Goal stays active, approximately10%weightedmilestones, not measured code coverage.'
    ]
    q=dict(status='partially_verified_submission_response_integration',compiled_source_count=224,newly_registered_entries=6,newly_qualified_rows=351,qualified_conditional_outcomes=46,routines=routines,canonical_reuse=t['canonical_reuse'],canonical_duplicate_credit=0,new_guarded_adapter_rows=384,canonical_adapter_rows=33,executed_integration_rows=384,executed_integration_conditional_outcomes=52,coverage_by_routine=r['coverage_by_routine'],fresh_original_native_comparisons=1247,fresh_independent_checks=18295,retained_previous_evidence_original_native_comparisons=17227,total_evidence_original_native_comparisons=18474,prior_registry_entries=1502,result_registry_entries=1508,result_unique_module_address_pairs=1505,flat_return_guard_sites=a['guard_call_return_sites'],new_resume_exports=0,qualified_nonlocal_resume_routes_total=8,remaining_unqualified_resume_routes=74,request_response_lifecycles=64,request_response_original_native_comparisons=192,physical_maple_dma_complete=False,natural_response_to_task_parser_complete=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Connect original task-created queued request through verified submission and response completion, then resume the original waiting task without modifying saved frames or completed IDs. Recover actual remaining callback-reader/global setup below6608a4 as reached. Physical Maple DMA transport remains a separate unfinished device boundary.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_input_submission_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED6new351rows/46outcomes;',q['fresh_original_native_comparisons'],'fresh;',q['fresh_independent_checks'],'checks',flush=True)
if __name__=='__main__':main()
