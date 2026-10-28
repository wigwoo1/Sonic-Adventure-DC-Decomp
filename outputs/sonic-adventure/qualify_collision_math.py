"""Qualify two original math leaves without importing upstream implementations."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','retained_verification','unbounded'):
        path=P/f'analysis/collision_math_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
    dll=digest(ROOT/'work/sonic-adventure/build/collision_math.dll')
    for name in ('audit','direct_verification'):assert reports[name]['dll_sha256']==dll,name
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
        for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    retained=reports['retained_verification'];assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    direct=reports['direct_verification'];assert direct['existing_numeric_probe_mode'] and direct['original_native_comparisons']==len(direct['cases'])==892
    assert direct['independent_checks']==sum(c['independent_checks'] for c in direct['cases'])==8472
    assert (direct['independent_cross_oracle_cases'],direct['independent_length_oracle_cases'])==(332,76)
    assert direct['status_counts']=={'0':648,'-1':136,'1':76,'-3':32}
    assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in direct['cases'])
    assert {c['configuration']['view'] for c in direct['cases']}=={'p0','p1','p2'}
    t=reports['translation'];assert len(t['routines'])==2 and t['instruction_rows']==41 and t['canonical_reused_entries']==[] and t['resume_routes']==[]
    wanted={'0x8c027360':(33,'ab64ed43a74ef8ae8bc802e9a03ac1ff70c19370a74186906ecf5878a3149dd8'),'0x8c63a75c':(8,'184ec57b105022cf5a5df589f52fed8dc017109b7c0ff5626bfaf6c31c5a39dd')}
    coverage=[];routines=[]
    for body in t['routines']:
        rows,h=wanted[body['address']];assert body['instruction_rows']==rows and body['retail_span_sha256']==h and body['conditional_edges']==[]
        c=next(c for c in direct['coverage_by_routine'] if c['address']==body['address']);assert not c['missing_rows'] and not c['missing_outcomes'] and c['executed_rows']==rows
        cv=dict(address=body['address'],static_rows=rows,executed_rows=rows,executed_outcomes=0,domain_excluded_rows=[],domain_excluded_outcomes=[]);coverage.append(cv)
        routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=rows,coverage=cv);routines.append(routine)
    returns_path=P/'analysis/collision_math_returns.json';returns=json.loads(returns_path.read_text());files.append(returns_path)
    assert returns['prior_return_sites']==len(returns['allowed_call_returns'])==2268 and returns['new_call_returns']==[]
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==244 and reports['unbounded']['exit_code']==0
    for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    previous=P/'analysis/movie_filesystem_command17_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==65707
    reuse_path=P/'analysis/sarecomp_gameplay_compatibility_probe.json';reuse=json.loads(reuse_path.read_text());files.append(reuse_path)
    candidates={r['name']:r for r in reuse['results']}
    assert candidates['cross']['exact_matches']==[dict(usa_address='0x8c027360',registered=[])] and candidates['length']['exact_matches']==[dict(usa_address='0x8c63a75c',registered=[])]
    for pin,h in reuse['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1582,1584)
    assert not any(r['address'] in wanted and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry[:1582])
    fresh=direct['original_native_comparisons']+retained['original_native_comparisons'];checks=direct['independent_checks']+retained['independent_checks'];assert (fresh,checks)==(1135,10155)
    limits=[
        'Two independently recovered original functions:cross027360(33rows)andlength63a75c(8rows). All41instructions execute,no original conditionaledges/exclusions/canonicalduplicatecredit. Exactoriginalbytefingerprints matchtheSARecompidentityreferences,but NOupstreamimplementationcode iscopied/merged. Existingnormalize63a94c andnumerichelpers receiveNOnewcredit.',
        '892fullCPU/FPU/RAM/ordered-boundary comparisons:648complete,76expectednumerictrapstops,136existingunsupportednumericstops,32reachedRAMguardstops. Everycasecompared;nonzero statuses areNOTfabricatedcompletions. Independent408finiteoracles comprise332dyadiccross geometry/alias casesand76exact-rational-sqrtlength cases. Theseareincludedinthe892,notadditionalcomparisoncredit.',
        'Original fmov/fsub/fmul/FIPR/FSQRT/integer/RTSdelay ordering retained. Cross loadsallinputs beforestores,mutatesR4/5/6 by8 andR0=8 inordinaryPR0/SZ0mode;lengthR4+=12 andreturnsFR7throughFR0 intheRTSdelay. Tests coveroutput/inputaliases,bothFPbanks/SZ/PR/DN,RM0/1plus2/3guards,selectedenable/flagcombinations,NaN/infinity/denormal/zero/overflow inputs,mappedMMU-offP0/P1/P2 viewsandRAMguardprefixes. GeometriccontractsareordinaryPR0/SZ0;othermodespreserveopcodeeffects orstopbyexistingpolicy.',
        'Existing numeric-probe mode classifiesstop/trapstate;ordinary completedscalararithmetic retainsindependentPythoninteger/dyadicexecution. Initialbroadtestneededprobe=True forenabledexceptions/denormals;targeted4casesandfullmatrixthenpasswithoutnativechanges. SharedpinnedFIPRvendorprimitive verifiesinstructionplumbing,NOTindependentarbitraryhardwarebits. Separatefinitegeometry/rationalrootoracles donotproveallhardwareFIPRprecision. PhysicalFPexception/SRFD/address-error/MMUdelivery/timing unqualified. SR.FD clear isdeclaredcallABI.',
        'Inheritedgenerationtemplate lackedFSQRT;newfamilyreusesexistingemitter/backendwithoutchangingoldsource.244-sourceDLL/lib andstrictunboundedWallWextraWerrorpass. Exactprevious243bridgeplus2entryroutes/prefix,0newJSRreturns/continuationexports. Returnmembership2268,exports376,qualifiedpostyieldroutes71/remaining305 unchanged. CPU/flatcode/guards/productionobserver/rawsession/ABIunchanged,nonewwaitlimits.2719oldbuildspreserved.',
        '243freshretainedIRQ/context/taskcomparisons retainoldnumericassertions;historical65707notallrerun. Combinedevidenceisnotunique/codecoverage.216warm/211coldstartupandprior242full512writeproofhistoric,no fresh244fullstartup/gameplay/full-deviceclaim. SDKnaturalbusy/latchacquisition,optional660baewritehook,physicalstorage/fullfilesystem/game/portsunfinished.',
        'Nextrecover original callers ofthese collisionmathroutines fromverifiedreferences andextendthenativecollisionfamily. SARecompPAL/USA/privateSDK-AOT/reuse-termslimitsremainforanydirectcodeintegration;noauthorcontact. FullPC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. TextureOff disables sampling/usesvertexcolors.10%weightedmilestones,notmeasuredcoverage;researchDLLnotplayableport.'
    ]
    q=dict(status='partially_verified_original_collision_math_software_abi_domains',compiled_source_count=244,newly_registered_entries=2,newly_qualified_rows=41,qualified_conditional_outcomes=0,executed_rows=41,executed_conditional_outcomes=0,canonical_adapter_rows=0,canonical_duplicate_credit=0,newly_translated_rows=41,domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=routines,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=65707,total_evidence_original_native_comparisons=65707+fresh,prior_registry_entries=1582,result_registry_entries=1584,result_unique_module_address_pairs=1581,prior_flat_return_guard_sites=2268,flat_return_guard_sites=2268,new_resume_exports=0,newly_qualified_nonlocal_routes=[],qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=305,explicit_nonfunction_resume_routes_total=376,direct_status_counts=direct['status_counts'],independent_cross_oracle_cases=332,independent_length_oracle_cases=76,upstream_game_implementation_imported=False,original_calls_in_new_functions=0,physical_fpu_all_inputs_qualified=False,physical_exception_delivery_qualified=False,sr_fd_clear_call_abi=True,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_gameplay_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae'],next_task='Recover original callers of027360/63a75c fromverifiedstatic/runtime references,thenextendthecollisionfamily usingexistingexactnumericalworkflow. Preserveoptional660baeunqualified;no upstreamimplementationcopywithoutresolvedterms.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
    from audit_movie_linked_routines import inventory
    q['linked_routine_inventory']=inventory(ROOT/'work/sonic-adventure/build/collision_math.dll',registry if len(registry)==1584 else registry+routines)
    assert q['linked_routine_inventory']['registry_entries']==1584 and q['linked_routine_inventory']['registry_unique_module_address_pairs']==1581
    q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
    (P/'analysis/collision_math_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COLLISION MATH QUALIFIED2/41rows;',fresh,checks,'confirmedlinked',q['linked_routine_inventory']['confirmed_linked_unique_routines_by_name'],flush=True)
if __name__=='__main__':main()
