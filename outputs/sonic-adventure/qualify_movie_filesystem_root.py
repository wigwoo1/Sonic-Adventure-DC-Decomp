"""Qualify new root code and two unchanged, previously unregistered helpers."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','tasks_verification','table_verification','retained_verification','unbounded'):
        path=P/f'analysis/movie_filesystem_root_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_root.dll')
    for name in ('audit','direct_verification','tasks_verification','table_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    assert reports['direct_verification']['original_native_comparisons']==576 and reports['direct_verification']['independent_checks']==8600
    assert reports['tasks_verification']['lifecycles_passed']==34 and reports['tasks_verification']['original_native_comparisons']==968 and reports['tasks_verification']['independent_checks']==5032
    assert reports['table_verification']['original_native_comparisons']==24 and reports['table_verification']['independent_checks']==312
    assert reports['retained_verification']['original_native_comparisons']==243 and reports['retained_verification']['independent_checks']==1683
    assert len(reports['translation']['routines'])==1 and reports['translation']['instruction_rows']==68 and not reports['translation']['resume_routes']
    assert reports['connection']['compiled_source_count']==227 and reports['unbounded']['source_count']==227 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    old_status=P/'analysis/movie_countdown_translation.json';sm=json.loads(old_status.read_text());files.append(old_status)
    old_table=P/'analysis/movie_filesystem_state_translation.json';tm=json.loads(old_table.read_text());files.append(old_table)
    candidates=[(reports['translation']['routines'][0],reports['translation']['source']),
        (next(r for r in sm['routines'] if r['address']=='0x8c660dac'),sm['source']),
        (next(r for r in tm['routines'] if r['address']=='0x8c64ae62'),tm['source'])]
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1515,1518)
    coverage=reports['direct_verification']['coverage_by_routine']+[r for r in reports['tasks_verification']['coverage_by_routine'] if r['address']=='0x8c64ae62'];routines=[]
    for body,source in candidates:
        cv=next(r for r in coverage if r['address']==body['address']);assert not cv['missing_rows'] and not cv['missing_outcomes']
        assert not any(r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==body['address'] for r in registry[:1515])
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=source,instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);routines.append(row);files.append(P/source)
    assert sum(r['instruction_rows'] for r in routines)==144 and sum(r['executed_outcomes'] for r in routines)==24
    prior=P/'analysis/movie_filesystem_state_qualification.json';old=json.loads(prior.read_text());files.append(prior)
    limitations=[
        'Three newly qualified original routines144rows/24outcomes:root checker64a4b2(68/14,new source),status660dac(32/4,unchanged countdown source),table reader64ae62(44/6,unchanged226source). Both reused bodies were previously unregistered,not duplicate credit. Allrows/outcomes execute. Registered cleanup64850a(21rows) reused with zero duplicate credit. Local names are interpretations,not original symbols.',
        '576direct root ABI comparisons cover availability exits,decoded24byte record versusseparate44byte transfer status,presence/function/busy/cache gates,all16signature mismatch positions withfourbadbytes,latch0/1/ffffffff,twochannels/fourCPUFPUprofiles. Real status/readiness/cleanup code executes;no helper return supplied. Independent nonstackRAM/cache invalidation and64byte ABI scratchbound,return/preservedregister/FPU/SR/callselection oracles complement strict original/native entirestate includingstack.',
        '34actual command1 task lifecycles use raw metadata tablecounts0/1/2 and16leading55rootbytes. Original APIs/constructors/requestsubmission/responsepump/contextrestore own every frame andID. Raw root/table bytes,actual requested blockindices/bufferaddresses,counters/progress/completion anduntouchedbuffer tails are independently checked. No savedframe,completedID,decodedmetadata orparser/callback result is supplied. Valid path reaches real missingdirectory reader64a9a8;complete filesystem/game remains unfinished.',
        'Bad root signatures clear cache flags andthe original caller completes result0;this is not proof of a formatted/usable filesystem. Table reader-255 propagates to completion. Generic table error-65536 sets original channel error latch andthe caller completes0;subsequent real root check returns-253 fromthatlatched state. Positive table loop andexisting nonlocalroutes64ae9a/64b236 are qualified through actual yields/resumes.24additional signed nonpositive-count ABI vectors retain independent exact stack/machine effects.',
        'Only68instructions newly translated. Existing status/table/cleanup C androutes remain byte-identical;one new root route/exportprefix andthreeactual root JSRreturn sites are the only previousbridge changes. Allowedreturns1974->1977,not arbitraryPR relaxation. No new continuation exports orwaitbounds. Existing85nonfunctionexports unchanged;15qualified/70unqualified after qualifying64ae9a and64b236. Othergraphs andcallback-yield routes remain unqualified.',
        'OriginalCPU andflat execution code,ABI/runtime/exception/SRFD/guard policies unchanged.243fresh retained controls comprise83softwareIRQ/contextSRFD comparisons and160phases across8selected older task contracts;numeric expectations unchanged byAST. Allother earlier20871comparisons are historical,not allrerun. Combined evidence is not unique cases orcodecoverage. No fresh227fullstartup;old216warm/211cold remain historical.',
        'An initial direct absent-device fixture cleared only transfer status;the originalavailability helper reads decodedrecord646856. The unchanged original/native executions both reachedthe sameNULL-read prefix. Fixture inputs were corrected tokeep bothrecords coherent;no native orreference behavior was changed. Only final576passing vectors count as fresh proof.',
        'PhysicalMapleDMA acquisition/traversal/timing/automaticIRQ are unfinished;busyclear/rawRXbytes are declared device inputs. Direct coherentSDK/cache/latch fixtures are helper inputs,not proof ofnatural acquisition. Original sideprojectSH4 instructionmodel,not hardware. Wholegame/PC/Android/3DS/N64/PS1/FPS/8K/SUPIR/settings remain unfinished. TextureOff disables sampling andusesvertexcolors. Goalactive,about10%weightedmilestones,not measured codecoverage.'
    ]
    q=dict(status='partially_verified_root_validation_and_table_reads',compiled_source_count=227,newly_registered_entries=3,newly_qualified_rows=144,qualified_conditional_outcomes=24,newly_translated_rows=68,reused_previously_unregistered_rows=76,registered_cleanup_duplicate_credit=0,routines=routines,coverage_by_routine=coverage,fresh_original_native_comparisons=1811,fresh_independent_checks=15627,retained_previous_evidence_original_native_comparisons=20871,total_evidence_original_native_comparisons=22682,prior_registry_entries=1515,result_registry_entries=1518,result_unique_module_address_pairs=1515,prior_flat_return_guard_sites=1974,flat_return_guard_sites=1977,new_resume_exports=0,newly_qualified_nonlocal_routes=['0x8c64ae9a','0x8c64b236'],qualified_nonlocal_resume_routes_total=15,remaining_unqualified_resume_routes=70,explicit_nonfunction_resume_routes_total=85,original_task_lifecycles=34,task_original_native_comparisons=968,physical_maple_dma_complete=False,root_and_table_reads_complete_in_declared_domain=True,full_table_reader_qualified=True,full_filesystem_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Recover actually reached directory reader64a9a8 using original command1 task/raw root andtable responses. Derive directory metadata/data from original code,then qualify positive directory reads/returns without supplying parsed results orrepairing taskframes. Physical acquisition andwritebackend660bae remain independent unfinished work.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_root_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED3new144rows/24outcomes;1811fresh/15627checks',flush=True)
if __name__=='__main__':main()
