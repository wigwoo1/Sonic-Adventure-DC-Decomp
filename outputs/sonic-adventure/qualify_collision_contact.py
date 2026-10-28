"""Record complete-body execution and explicit unresolved outcomes, without registration."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','audit','direct_verification','frontier_verification','retained_verification','unbounded'):
  path=P/f'analysis/collision_contact_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 dll=ROOT/'work/sonic-adventure/build/collision_contact.dll';h=digest(dll)
 for name in ('audit','direct_verification','frontier_verification'):assert reports[name]['dll_sha256']==h
 retained=reports['retained_verification']
 for suite in retained['suites']:
  path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==h
  for pin,sha in r['sha256'].items():assert digest(pin)==sha,(suite['suite'],pin);files.append(Path(pin))
 t=reports['translation'];direct=reports['direct_verification'];frontier=reports['frontier_verification']
 assert len(t['routines'])==1 and t['instruction_rows']==1121 and len(t['resume_routes'])==32 and not t['canonical_reused_entries']
 assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_contact_oracle_cases'])==(199,956,62) and direct['status_counts']=={'0':166,'-1':29,'-3':4}
 assert (frontier['original_native_comparisons'],frontier['independent_checks'],frontier['independent_contact_oracle_cases'])==(2,12,2)
 d,f=direct['coverage_by_routine'][0],frontier['coverage_by_routine'][0];missing=sorted(set(d['missing_rows'])&set(f['missing_rows']));outcomes=sorted(set(map(tuple,d['missing_outcomes']))&set(map(tuple,f['missing_outcomes'])))
 assert not missing and outcomes==[('0x8c02a48c','0x8c02a4a8'),('0x8c02a592','0x8c02a5f4')]
 cv=dict(address='0x8c029b00',static_rows=1121,executed_rows=1121,static_conditional_outcomes=74,executed_outcomes=72,missing_rows=missing,missing_outcomes=outcomes,proven_infeasible_outcomes=[],domain_excluded_rows=[],domain_excluded_outcomes=[])
 assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
 assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==249 and reports['unbounded']['exit_code']==0
 for pin,sha in reports['unbounded']['source_sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 returns=P/'analysis/collision_contact_returns.json';r=json.loads(returns.read_text());files.append(returns);assert r['prior_return_sites']==2294 and len(r['allowed_call_returns'])==2326 and len(r['new_call_returns'])==32
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1589
 partial={'0x8c029b00','0x8c10d008','0x8c10d058','0x8c10d0f8'}
 assert not any(x['address'] in partial and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in registry)
 previous=P/'analysis/collision_text_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==71010
 # Confirm all32 original call sites return to a real observed callee boundary.
 actual_returns={b['pr'] for c in direct['cases']+frontier['cases'] for b in c['boundaries']}
 assert set(r['original_call_returns'])<=actual_returns
 observations={address:[] for address in ('0x8c10d008','0x8c10d058','0x8c10d0f8')}
 for c in direct['cases']+frontier['cases']:
  for b in c['boundaries']:
   if b['target'] in observations:observations[b['target']].append(b['angle_input'])
 assert all(observations.values())
 q=dict(status='partially_verified_complete_collision_caller_with_two_unqualified_outcomes',compiled_source_count=249,newly_registered_entries=0,new_original_routine_credit=0,newly_translated_rows=1121,executed_rows=1121,static_conditional_outcomes=74,executed_conditional_outcomes=72,proven_infeasible_conditional_outcomes=0,unaccounted_outcomes=outcomes,domain_exclusion_arguments=[],coverage_by_routine=[cv],canonical_duplicate_credit=0,new_direct_entry_routes_partially_verified=1,new_nonlocal_resume_exports=32,new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=2326,explicit_nonfunction_resume_routes_total=434,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=363,registry_entries=1589,registry_unique_module_address_pairs=1586,fresh_original_native_comparisons=444,fresh_independent_checks=2651,direct_status_counts={'0':168,'-1':29,'-3':4},independent_contact_oracle_cases=64,actual_original_call_sites_executed=32,observed_angle_wrapper_inputs=observations,observations_are_not_unreachable_or_interval_proofs=True,supplied_game_callee_outputs=0,supplied_execution_frames=0,retained_previous_evidence_original_native_comparisons=71010,total_evidence_original_native_comparisons=71454,physical_fpu_all_inputs_qualified=False,physical_exception_delivery_qualified=False,full_gameplay_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,partial_unregistered_proposals=sorted(partial),optional_660bae_still_unqualified=True,upstream_game_implementation_imported=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Resolve029b00outcomesA48c->A4a8andA592->A5f4fromoriginalnormalentry/dataflow. EstablishcalleeR10/SPpreservationandCOUNTmemoryalias/stabilityfactsorconstructactualoriginalinputsthatreachthem. DoNOTassumeunreachablefromfinite testsorABIwithoutproof;keepcallerand3mathwrappersunregistereduntilqualified. Continuegamecaller/initializationinvariantsasneeded,nosuppliedframes/results.',dll_sha256=h,limitations=[
 'Completeoriginal029b00body is translated/integrated:1121instructions/74syntacticconditionaloutcomes/32calls. Everyinstructionand72outcomesexecute,all32callsitesobservedwithrealcallees. The twooutcomesA48c->A4a8andA592->A5f4remainUNQUALIFIED,notprovedinfeasibleandnotexcluded. CallerremainsUNREGISTERED;no newroutinecredit. Threeexistingmathwrappers10d008/10d058/10d0f8alsoremainunregistered.',
 '201directcallercomparisons:168completed,29existingunsupportednumericstops,4mappedRAMguards. FullCPU/FPU/RAM/device/softwarequeue/orderedboundariesmatch. Actualmatrix/cross/length/normalization/acos/asin/atan/rotation/inverse/determinant/transform/copy/render/text calleesexecute,no providedcalleeoutputs/frames. ObservedFR4/FR5/FPSCRinputsto3wrappersarecapturedread-only;finiteobservationsarenotunreachableorintervalproofs.',
 '64independentsemanticcasescovernonpositiveradiuspreservingquery/counter,empty/unmatchedworldreset,stationaryfinitehorizontaltriangledecisions,repeated-facecontactcount/16cap,andextended-edge rejection. Otherfinitegeometry/motion/responseaveraging/sorting/debug renderinguseoriginalinstructioncomparisons,notaseparatecompletephysicsoracle. Authoredworldlinkedlist/triangles/matrix/render/fontinputsarenotnaturalgameinitializationproof.',
 'The derived extended-edge cases set x+z=2 with x,zoppositesigns. Middle signed area4-2x-2z iszero,thefirsttwosignedproductsarezero,andthethirdrejects. ThisexecutesF5e->FA0withoutinventedcalleeoutputsorparameterfuzzing. Materialflag0x800000andactualmotion/plane inputscovernormal/signbranches. UnsupportedPR/SZandselectedNaN/infinity/subnormal/maxfinitecasesretainexistingsoftwarestop policies;nonzerostopsarenotcompletions.',
 'InheritedgeneratorlackedFABS;newfamilyreusesexistingrawFABS/PRstop emitterfromrecover_draw_distance_helpers.py,no oldtemplate/nativebackendchange. InitialfixturediscardedrequiredsharedABIdefaults(other),fixedbyinheritingtheestablishedrenderinputconfiguration. Initialnear-endquerycouldreadzeroandearlyexit;guardfixturecorrectedtoactuallyunmappedquery. NativeCunchangedbyfixture/refinementcorrections. Misreadpartialshardcoveragewascheckedagainstsinglecase96;identicalrecords/currentpinsandvalidunion,noruntime nondeterminismfound.',
 '249-sourceDLL/libandstrictunboundedWallWextraWerrorpass. Exactprevious248bridgeplus1body/32actualreturns/32continuationexports. Oldcallees,CPU/flatinterpreter/guards/productionobserver/rawsession/ABI unchanged,nonewwaitbounds. InheritedtestonlyfontQACRinputseedplusnewreadonlyFPargumentcapture,allcomparisonlogicunchanged.434explicitresumeexports,71qualifiednonlocalroutes/363remaining;newexportsnotindependentlyqualified.2731oldbuildartifactspreserved,newDLL/libadditional.',
 '243freshretainedIRQ/context/taskcomparisons/1683checks preserveoldnumericassertions(AST). Total444freshcomparisons/2651checks;cumulative71454ishistorical-plus-fresh,notunique/codecoverage.216warm/211coldstartup/prior242full512write remainhistorical;no freshcomplete249startup/gameplay/full-deviceclaim. PhysicalFPU/exception/MMU/address-error delivery/GPU pixelsunqualified;SRFDclearfixedcode/mappedsoftwareABI.',
 'No newregistrationorqualificationclaimforremainingoutcomes. Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOffdisablessampling/usesvertexcolours.10%weightedmilestones,notcoverage;researchDLLnonplayable. KeepgoalactiveandSolHighforremainingreverseengineering.'
 ],sha256={str(p):digest(p) for p in files})
 from audit_movie_linked_routines import inventory
 q['linked_routine_inventory']=inventory(dll,registry);assert q['linked_routine_inventory']['confirmed_linked_unique_routines_by_name']==631
 q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
 (P/'analysis/collision_contact_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('CONTACT PARTIAL1121rows/72of74outcomes;444comparisons/2651checks;631linked;0routinecredit')
if __name__=='__main__':main()
