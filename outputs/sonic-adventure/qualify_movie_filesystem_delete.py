"""Qualify complete RAM-removal helpers; keep gateway/persistence partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','tasks_verification','retained_verification','unbounded'):
        p=P/f'analysis/movie_filesystem_delete_{name}.json';r=json.loads(p.read_text());reports[name]=r;files.append(p)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/movie_filesystem_delete.dll')
    for name in ('audit','direct_verification','tasks_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        p=P/suite['proof'];r=json.loads(p.read_text());files.append(p);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h,(suite['suite'],pin);files.append(Path(pin))
    assert (reports['direct_verification']['original_native_comparisons'],reports['direct_verification']['independent_checks'])==(672,5704)
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    task=reports['tasks_verification'];assert task['original_task_sessions']==8 and task['original_native_comparisons']==370
    assert sum(c['pending_persistence'] for c in task['cases'])==7
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==232 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    routines=[];coverage=[]
    for body in reports['translation']['routines']:
        cvs=[cv for name in ('direct_verification','tasks_verification') for cv in reports[name]['coverage_by_routine'] if cv['address']==body['address']]
        missing=set.intersection(*(set(cv['missing_rows']) for cv in cvs));edges=set.intersection(*(set(map(tuple,cv['missing_outcomes'])) for cv in cvs))
        cv=dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(edges),missing_rows=sorted(missing),missing_outcomes=sorted(edges));coverage.append(cv)
        if body['address']=='0x8c649d20':assert missing and edges;continue
        assert not missing and not edges,body['address']
        if body['address']=='0x8c64b2dc':continue
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=reports['translation']['source'],instruction_rows=body['instruction_rows'],executed_rows=cv['executed_rows'],executed_outcomes=cv['executed_outcomes']);routines.append(row)
    assert len(routines)==3 and sum(r['instruction_rows'] for r in routines)==108 and sum(r['executed_outcomes'] for r in routines)==8
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1535,1538)
    assert not ({r['address'] for r in routines}&{r['address'] for r in registry[:1535]})
    prior=P/'analysis/movie_write_removal_integration_verification.json';old=json.loads(prior.read_text());files.append(prior);assert old['total_evidence_original_native_comparisons']==30771
    profile=P/'analysis/movie_filesystem_write_qualification.json';profiles=json.loads(profile.read_text());files.append(profile)
    fresh=sum(reports[n]['original_native_comparisons'] for n in ('direct_verification','tasks_verification','retained_verification'));checks=sum(reports[n]['independent_checks'] for n in ('direct_verification','tasks_verification','retained_verification'))
    limitations=[
        'RAM removal649c68 qualifies70rows/6outcomes withactualoriginalsearch64ad56/entryaddress64b274/chainvalidator64b300/FATreader64b1c0/newsetter/memoryfill/dirtymarker. Missingname returns-251 withoutmutation;foundentryalwaysclears32bytes/marksdirectorydirty. ValidchainsfreeeachcountboundedlinktoFFFC;invalidchainsstillclearentry butdoNOTfreeFAT. Originalbehavior retained,notreplacedwithinventedrepair/validation. ExactfullnonstackRAM/184byte maxnestedstack/ownGPRPRsaves/callorder oracles.',
        'FATsetter64b18e qualifies25rows/2outcomes:originalsigned inclusiveindex<=rootlimit guard andactualSDK-selectedBELEwriter. Low16value/uint32shiftwrap/negative/highindices/511/512/rootlimits testedinmappedRAM;notvalidcardformatclaimforallvectors. Dirtymarker64b28c qualifies13rows/0outcomes withexactarithmeticrightshiftby4,fullRAM/allGPR/specialstate oracles. Canonical64b2dc zero-return2rows fullyreplayedbutZEROduplicatecredit;originalregisteredsource/proofunchanged.',
        'Eightactualoriginalcommand1 rawheader/root/FAT/linked-directory acquisitions thencommand7 APIs/scheduler668e60. Channels0/7/allbyteorders,validone/two/threeblocks/zero/index16/badchain/missingname. Independent root/FAT/directory/dirtybuffers/noextraI/O oracles. One missing-namecommand completes-251callback/idle/ownerteardown;seven RAMmutations reachactualmissingpersistence64ab36 withPR649db6/r4channel. No diskpersistence orcompletion/teardown ofpendingcommands claimed. Callerfilename/rawdevicebytes areinputs;no parsedmetadata,taskframe,ID,PR,calleeoutput orcallbackresult supplied.',
        'Gateway649d20 remains92rows PARTIAL/UNREGISTERED;positivepath nowreaches64ab36 afterRAMmutation. Canonicalzero-returnmeansitsnegative-returncheck cannotbeusedtoinventafailurepath;branch/sourcekept. Persistence64ab36/64afe0/relatedcallees mustbefreshbytechecked/tracednext. Olderwritebackend660bae optionalhook remains10rows/3outcomesunqualified;no hookoutputs invented orglobaldeadcodeclaim.',
        '232sourceDLL buildandstrictunbounded Wall/Wextra/Werror syntaxpass.202rowsbytechecked=200proposed+2canonical. Exact231bridgeinverseexcept5bodies/14continuations/14actualreturns/exportprefix;returns2036->2050. CPU/flatcode/guards/observer/rawinputsession/ABI/runtime/SRFD unchanged;no newwaitlimits.158nonfunctionexports25qualified/133unqualified;all14newroutes unqualifiedforNONLOCALresumption andreceivenoroutinecredit. SynchronousRAMexecution isnotcallbackyieldproof.',
        '243freshretainedIRQ/context/taskcontrols preservetheoriginalnumericassertions. Earlier30771comparisons historical,notallrerun;combinedevidence notuniquecases orcoverage. No freshwarm/coldstartup;216warm/211coldhistorical. Prior14685selectedsource/proofs and2695builds preserved except10reviewedcatalog/status/docpaths;requirementsunchanged. PhysicalMaple/filesystem/gameplay/ports/FPS/8K/SUPIR/settingsunfinished. TextureOff disablestexturesamplingandusesvertexcolors. ResearchDLLsnotplayableports;goalACTIVE10%weightedmilestones,notmeasuredcodecoverage.'
    ]
    q=dict(status='partially_verified_command7_RAM_removal_persistence_frontier',compiled_source_count=232,newly_registered_entries=3,newly_qualified_rows=108,qualified_conditional_outcomes=8,newly_translated_unregistered_rows=200,canonical_adapter_rows=2,canonical_duplicate_credit=0,canonical_reused_entries=['0x8c64b2dc'],partial_unregistered_proposals=['0x8c649d20'],routines=routines,coverage_by_routine=coverage,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=30771,total_evidence_original_native_comparisons=30771+fresh,prior_registry_entries=1535,result_registry_entries=1538,result_unique_module_address_pairs=1535,prior_flat_return_guard_sites=2036,flat_return_guard_sites=2050,new_resume_exports=14,newly_qualified_nonlocal_routes=[],qualified_nonlocal_resume_routes_total=25,remaining_unqualified_resume_routes=133,explicit_nonfunction_resume_routes_total=158,original_task_sessions=8,completed_initializations=8,completed_command7_missing_name_cases=1,pending_persistence_cases=7,task_original_native_comparisons=370,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_command7_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=profiles['historical_warm_profile'],historical_cold_profile=profiles['historical_cold_profile'],next_task='Recover actuallyreached persistence64ab36 fromcommand7 RAMremoval;freshbytecheck/traceitsactualcallees,includingknown64afe0/64ae4a asreached. Keep649d20gatewaypartialuntilpersistence/completionpathsqualify. Originaldirtyrecord/FATchangespending,notpersisted;noframe/ID/PR/resultrepair. Keepolder660baeoptionalhookpartial.',dll_sha256=dll,limitations=limitations,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    (P/'analysis/movie_filesystem_delete_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('DELETE QUALIFIED3/108rows/8outcomes',fresh,checks,flush=True)
if __name__=='__main__':main()
