"""Qualify five interface primitives and explicitly scoped original task paths."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','verification','copy_verification','tasks_verification','retained_verification','unbounded'):
        path=P/f'analysis/movie_input_interface_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_input_interface.dll')
    for name,expected in {'verification':(528,6816),'copy_verification':(84,1176),'retained_verification':(83,759)}.items():
        assert tuple(reports[name][k] for k in ('original_native_comparisons','independent_checks'))==expected
    for name in ('verification','copy_verification','tasks_verification','audit'):
        assert reports[name]['dll_sha256']==dll,name
    t=reports['translation'];c=reports['connection'];a=reports['audit'];tasks=reports['tasks_verification']
    assert (len(t['routines']),t['instruction_rows'],t['proposed_rows'],c['compiled_source_count'])==(5,24,24,225)
    assert reports['unbounded']['exit_code']==0 and reports['unbounded']['source_count']==225
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    assert tasks['lifecycles_passed']==38 and len(tasks['cases'])==38
    assert tasks['newly_qualified_nonlocal_routes']==['0x8c648426','0x8c660932','0x8c660b2e']
    for case in tasks['cases']:
        assert case['task_frames_never_repaired'] and case['completed_ids_never_injected']
        config=case['input'];phases=case['phases']
        if config['kind']=='core' and config['geometry']==511:
            if config['command']==1:assert any(p['stop_pc']=='0x8c64ae32' and p['status']==-2 for p in phases)
            else:assert any('0x8c6607fc' in p['targets'] for p in phases) and phases[-1]['phase']=='original_owner_cancel'
    coverage=reports['verification']['coverage_by_routine']+reports['copy_verification']['coverage_by_routine']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1508,1513)
    baseline=registry[:1508];routines=[]
    for body in t['routines']:
        cv=next(x for x in coverage if x['address']==body['address']);assert not cv['missing_rows'] and not cv['missing_outcomes']
        assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==body['address'] for x in baseline)
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);routines.append(row)
    assert sum(x['instruction_rows'] for x in routines)==24 and sum(x['executed_outcomes'] for x in routines)==2
    prior=P/'analysis/movie_input_submission_qualification.json';old=json.loads(prior.read_text());files.append(prior)
    comparisons=sum(reports[n]['original_native_comparisons'] for n in ('verification','copy_verification','tasks_verification','retained_verification'))
    checks=sum(reports[n]['independent_checks'] for n in ('verification','copy_verification','tasks_verification','retained_verification'))
    limitations=[
        'Five newly qualified primitives24original instructions/two conditional outcomes:four16bit endian callbacks and forwardwordcopy. Canonical647e16 initializer/647de8 selector and all existing integration bodies receive zero duplicate credit. Finite declared-domain tests are not exhaustive game or hardware coverage.',
        'Original initializer writes actual interface object8c89c538 through cell8c89c54c; original selector installs retail callback addresses. Indexed WRITE647e38 and aligned literal evidence establish this provenance, not an exhaustive all-game writer inventory.',
        '38task lifecycles cover channels0/7,commands1/18,two transport orders,two interface endians,invalid/valid geometry,null/unknown completion callbacks,device removal,retry andbad header. Original APIs create task frames/requests, submission hands them to inflight, declared raw device observations feed actual responsepump, then original contexts resume. No saved frame, completedID, decoded metadata or parser/callback result is injected. Retry budget0 is selected through original647534 API; original initializer default10 is not silently patched.',
        'Valid command18 completes two256byte sub-blocks via original660a7e/6607fc, resumes through660b2e and648426, returns original mode33 and calls completion before idle/owner teardown. Valid command1 reaches missing filesystem dependency64ae32 and remains pending without a fabricated callback. Removal resumes660932 with low-level-208,converted by original caller to completion-255; malformed geometry returns-125;bad header returns-255. Full6608a4 and660a7e remain partial/unregistered; their report lists unexecuted rows/outcomes.',
        'Exact prior224bridge inverse plusfive routes/exportprefix; returnmembership1971 unchanged, no new nonlocal exports or source waitbounds. Three existing resume routes are freshly qualified for these graphs, bringing11qualified/71unqualified of82; arbitrary task/parser graphs remain unqualified. OriginalCPU and flat execution code identical.',
        'The wordcopy retains do-while semantics,forward overlap,floor(bytecount/4) and short-count underflow. Boundary tests observe one valid word followed by mappedRAM fault, not all2^32 zero-count iterations. Direct helpers independently check entireRAM/GPR/SR/PR/FPU/special effects. Task comparisons retain every physicalFPUbank/FPSCR/FPUL value; no unsupported all-bank preservation or fixedFPSCR is imposed on a call stopped inside a missing callee. Retained context controls independently test restoration.',
        'Explicit external observations are busy-clear and raw RXbytes only;SDK automatic primaryrequests may also advance the real serial counter. Existing platformbus stores MMIO values but does not implement physicalMapleDMA traversal/controller acquisition/timing/automaticIRQ. Original side is projectSH4instructionmodel,not hardware.',
        'Fresh225softwareIRQ/contextSRFD controls pass; oldwarm216/cold211startup evidence remains historical,not newly rerun. Earlier18474comparisons are historical; combined evidence is not unique cases or codecoverage. All prior publicsource/proofs/builds preserved except reviewed catalog/status/docs.',
        'Wholegame and allPC/Android/3DS/N64/PS1/FPS/8K/SUPIR/settings remain unfinished. TextureOff disables sampling and uses vertexcolors. Goal active;approximately10%weightedmilestones,not measured codecoverage.'
    ]
    q=dict(status='partially_verified_original_interface_and_task_response_integration',compiled_source_count=225,newly_registered_entries=5,newly_qualified_rows=24,qualified_conditional_outcomes=2,routines=routines,canonical_duplicate_credit=0,coverage_by_routine=coverage,partial_reader_coverage=tasks['coverage_by_routine'],fresh_original_native_comparisons=comparisons,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=18474,total_evidence_original_native_comparisons=18474+comparisons,prior_registry_entries=1508,result_registry_entries=1513,result_unique_module_address_pairs=1510,flat_return_guard_sites=1971,new_resume_exports=0,newly_qualified_nonlocal_routes=tasks['newly_qualified_nonlocal_routes'],qualified_nonlocal_resume_routes_total=11,remaining_unqualified_resume_routes=71,original_task_lifecycles=38,task_original_native_comparisons=tasks['original_native_comparisons'],physical_maple_dma_complete=False,declared_storage_command18_complete=True,full_header_reader_qualified=False,full_block_reader_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Recover actually reached filesystem dependency64ae32 under valid command1 using original interface/task constructors, task-created requests and explicit raw device observations; preserve saved contexts and pending command at missing callees. PhysicalMaple acquisition remains separate unfinished work.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_input_interface_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED5new24rows/2outcomes;',comparisons,'fresh;',checks,'checks',flush=True)
if __name__=='__main__':main()
