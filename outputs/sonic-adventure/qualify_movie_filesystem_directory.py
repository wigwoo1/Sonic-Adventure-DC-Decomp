"""Qualify only byte-audited bodies with complete observed instruction paths."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    names=('translation','connection','audit','lookup_verification','counts_verification','tasks_verification','frontier_verification','retained_verification','unbounded');reports={};files=[Path(__file__)]
    for name in names:
        p=P/f'analysis/movie_filesystem_directory_{name}.json';r=json.loads(p.read_text());reports[name]=r;files.append(p)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_directory.dll')
    for name in ('audit','lookup_verification','counts_verification','tasks_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    assert reports['lookup_verification']['original_native_comparisons']==512 and reports['lookup_verification']['independent_checks']==6656
    assert reports['counts_verification']['original_native_comparisons']==24 and reports['counts_verification']['independent_checks']==312
    assert reports['tasks_verification']['lifecycles_passed']==36
    assert reports['retained_verification']['original_native_comparisons']==243 and reports['retained_verification']['independent_checks']==1683
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==228 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1518,1520)
    coverage=[];routines=[]
    for body in reports['translation']['routines']:
        a=body['address'];candidates=[r for n in ('lookup_verification','counts_verification','tasks_verification') for r in reports[n]['coverage_by_routine'] if r['address']==a]
        missing=set.intersection(*(set(r['missing_rows']) for r in candidates));outcomes=set.intersection(*(set(map(tuple,r['missing_outcomes'])) for r in candidates));assert not missing and not outcomes,a
        cv=dict(address=a,static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges']),missing_rows=[],missing_outcomes=[]);coverage.append(cv)
        assert not any(r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==a for r in registry[:1518])
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=reports['translation']['source'],instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);routines.append(row)
    assert sum(r['instruction_rows'] for r in routines)==71 and sum(r['executed_outcomes'] for r in routines)==8
    prior=P/'analysis/movie_filesystem_root_qualification.json';old=json.loads(prior.read_text());files.append(prior)
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('lookup_verification','counts_verification','tasks_verification','frontier_verification','retained_verification'));checks=sum(reports[n]['independent_checks'] for n in ('lookup_verification','counts_verification','tasks_verification','frontier_verification','retained_verification'))
    limitations=[
        'Full directory55row reader and16row table-link lookup qualified against original flat instruction execution. Independent lookup ABI/fullRAM/exactstackwrites/FPU/SR/specialeffects across512vectors andsignednonpositivecounts24vectors. BE reader retains sign extension fromthe pre-swap load;initial mistaken oracle corrected withoutnative/reference changes. No hardware match claimed.',
        '36 original command1 task lifecycles through header/root/table/linked directory/completion/idle/teardown. Counts0/1/2,earlyend,countlimit,boundedcycle,first/late-255 andgeneric-65536 latch use rawresponsebytes andoriginal APIs/constructors/contexts. No parsedresults,requestIDs,taskframes orcallbackreturns supplied. Exactrequestedindices,bufferincrements,untouchedtails,counters/progress/completion checked. This initializes declared filesystem data;does not qualify full filesystem/file operations orphysical acquisition.',
        'Two new bodies71rows/8outcomes;no duplicate credit. Two new continuation exports andtwo actualreturn sites only;previous bridge inverse exact. 87nonfunction routes total17qualified/70unqualified. New64a9e0 andprevious64b246 qualified byactualnonlocalresumption.64a9f0 executes synchronously andremains unqualified fornonlocal entry. Continuations receive no function credit.',
        'Original command20 boundary observed onchannels0/7 at64bcd6,withstaticdispatchpointercheck andfulloriginal/nativeprefixmatch. No callee qualification credit. Command19 is the original waitpath,not the writebackend.',
        '228sourceDLL built;strict unboundedsyntax withWall/Wextra/Werror passed. Existing CPU/flatmodel,ABI/runtime/exception/SRFD/guard policies preserved.243 fresh retained comparisons withunchanged numeric assertions. Earlier22682comparisons historical,not allrerun;combined evidence isnot uniquecases orcoverage. No fresh228cold/warmstartup;old216warm/211cold historical.',
        'PhysicalMapleDMA acquisition/traversal/timing/automaticIRQ unfinished;busyclear/rawRXbytes aredeclared deviceinputs. Writebackend660bae andhigherfileoperations unfinished. Entiregame/PC/Android/3DS/N64/PS1/FPS/8K/SUPIR/settings incomplete. TextureOff disables sampling andusesvertexcolors. Goalactive;10%weightedmilestones,not measuredcodecoverage.'
    ]
    q=dict(status='partially_verified_original_filesystem_initialization',compiled_source_count=228,newly_registered_entries=2,newly_qualified_rows=71,qualified_conditional_outcomes=8,newly_translated_rows=71,reused_previously_unregistered_rows=0,routines=routines,coverage_by_routine=coverage,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=22682,total_evidence_original_native_comparisons=22682+fresh,prior_registry_entries=1518,result_registry_entries=1520,result_unique_module_address_pairs=1517,prior_flat_return_guard_sites=1977,flat_return_guard_sites=1979,new_resume_exports=2,newly_qualified_nonlocal_routes=['0x8c64a9e0','0x8c64b246'],qualified_nonlocal_resume_routes_total=17,remaining_unqualified_resume_routes=70,explicit_nonfunction_resume_routes_total=87,original_task_lifecycles=36,task_original_native_comparisons=reports['tasks_verification']['original_native_comparisons'],physical_maple_dma_complete=False,command1_initialization_complete_in_declared_domain=True,full_filesystem_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],next_task='Recover actual command20 callee64bcd6 reached byoriginal APIs/scheduler onchannels0/7. Its behavior remains unqualified. Independent writebackend660bae,physical acquisition andhigher filesystem/gameplay remain unfinished.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_directory_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('DIRECTORY QUALIFIED2/71rows/8outcomes',fresh,checks,flush=True)
if __name__=='__main__':main()
