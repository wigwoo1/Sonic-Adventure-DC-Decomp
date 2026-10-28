"""Qualify the complete triangle wrapper within the existing software ABI domains."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','audit','direct_verification','retained_verification','unbounded','predicate_proof'):
  path=P/f'analysis/collision_render_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 dll=ROOT/'work/sonic-adventure/build/collision_render.dll';h=digest(dll)
 for name in ('audit','direct_verification'):assert reports[name]['dll_sha256']==h
 retained=reports['retained_verification']
 for suite in retained['suites']:
  path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==h
  for pin,sha in r['sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 t=reports['translation'];direct=reports['direct_verification'];proof=reports['predicate_proof'];assert len(t['routines'])==1 and t['instruction_rows']==332 and len(t['resume_routes'])==20 and not t['canonical_reused_entries']
 assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_geometry_oracle_cases'])==(382,4352,332) and direct['status_counts']=={'0':354,'-1':22,'-3':6}
 cv=direct['coverage_by_routine'];assert len(cv)==1 and cv[0]['executed_rows']==332 and cv[0]['executed_outcomes']==31 and not cv[0]['missing_rows'] and cv[0]['missing_outcomes']==proof['infeasible_outcomes'] and not cv[0]['unaccounted_outcomes']
 assert proof['conditional_outcomes']==32 and proof['feasible_conditional_outcomes']==31 and not proof['domain_excluded_rows'] and not proof['domain_excluded_outcomes']
 targets={int(b['target'],16) for c in direct['cases'] for b in c['boundaries']}
 callees={0x8c605dac,0x8c605e0a,0x8c605e68,0x8c605f3c,0x8c605fca,0x8c60607a,0x8c6069a4,0x8c606a08,0x8c638ecc,0x8c64f5bc,0x8c64f840,0x8c64fae8,0x8c64fc68,0x8c64fe0c,0x8c650144}
 assert targets==callees|{0x8c63d66c}
 assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
 assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==247 and reports['unbounded']['exit_code']==0
 for pin,sha in reports['unbounded']['source_sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 returns=P/'analysis/collision_render_returns.json';r=json.loads(returns.read_text());files.append(returns);assert r['prior_return_sites']==2272 and len(r['allowed_call_returns'])==2292 and len(r['new_call_returns'])==20
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1586,1587)
 assert not any(x['address'] in ('0x8c63d66c','0x8c10d058','0x8c10d0f8') and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in registry[:1586])
 previous=P/'analysis/collision_inverse_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==69899
 body=t['routines'][0];routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=332,coverage=cv[0])
 q=dict(status='partially_verified_original_triangle_renderer_software_abi_domains',compiled_source_count=247,newly_registered_entries=1,new_original_routine_credit=1,newly_qualified_rows=332,executed_rows=332,static_conditional_outcomes=32,executed_conditional_outcomes=31,proven_infeasible_conditional_outcomes=1,unaccounted_outcomes=[],domain_exclusion_arguments=[],coverage_by_routine=cv,routines=[routine],canonical_duplicate_credit=0,new_direct_entry_routes_qualified=1,new_nonlocal_resume_exports=20,new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=2292,explicit_nonfunction_resume_routes_total=400,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=329,prior_registry_entries=1586,result_registry_entries=1587,result_unique_module_address_pairs=1584,fresh_original_native_comparisons=625,fresh_independent_checks=6035,direct_status_counts=direct['status_counts'],independent_geometry_oracle_cases=332,reached_actual_game_callees=sorted(map(hex,callees)),supplied_game_callee_outputs=0,supplied_execution_frames=0,retained_previous_evidence_original_native_comparisons=69899,total_evidence_original_native_comparisons=70524,physical_fpu_all_inputs_qualified=False,physical_exception_delivery_qualified=False,full_gameplay_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,wrappers_still_unregistered=['0x8c10d058','0x8c10d0f8'],optional_660bae_still_unqualified=True,upstream_game_implementation_imported=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Recover original text/glyph dependencies640922/6407b0,thencontinue full029b00callerclosure. Existingstrlen10d3f0 isreused;no injectedcalleeoutputs/frames andno duplicateknowncode.',dll_sha256=h,limitations=[
 'Newcompleteoriginalroutine63d66c has332instructions/20calls/15uniquecallees. It assembles indexed triangle fan/strip/list inputs,transforms threepoints,culls against farvisibility,clipsnearplane,andselectscolour/UV orprojectededgeemitters. All15actualgamecallees alreadyexistandexecute;no implementationimportedfromSARecomp,providedcalleeresults,orexecutionframes.',
 'All332instructionsand31conditionaloutcomesexecutein382fullCPU/FPU/RAM/device/queue/orderedboundarycomparisons. Oneof32syntacticoutcomes isprovedinfeasiblefromnormalentry:atd6e0theunchangedR11wasalreadyknownnonzerofromd6c4/d6c6. ThecheckedCFG/predicateproofdoesnotexcludeanyvalidinput,removeoriginalcode,orclaimarbitrarymidfunctionentryqualification.',
 '354completedcalls,22existingunsupportednumericstopsand6reachedRAMguardsmatchoriginalexecution. BothFPbanks,PR/SZ,RM0/1,finite/NaN/infinity/subnormal/maxfiniteinputz,mappedMMUoffP0/P1/P2views,cursoroffsets,rendererflag/swapstateandfan/strip/listtrianglescovered. Existingnumericprobe/backends retained;physicalFPU/exception/MMU/address-error delivery unqualified;SRFDclearcallABI. Nonzero statuses arenotcompletions.',
 '332independentfinitegeometrycaseschecktriangleindices/transformcounts,farrejection,nearclippedcounts,emitterchoice,header/vertex/edgepacketcountsandcursor. Finiteunclippedcolour/UVstripcasesalsocheckprojectedposition/depth/termination/colours/signedUVwords. These332areincludedinthe382,notextracomparisoncredit. Clippedarbitraryattributeandwideedgecoordinates relyonoriginalinstructioncomparison,notaseparatecompletehardwarepixeloracle.',
 'Initialfixturemade shaderdestination slot88fcd0 contain a sentinel ratherthan a validpointer. Original stoppedat606a52 while oldnativecallee hadnomappedpointerguard. Correctedtheauthoredvalidinputpointer;no nativeimplementationchanged. Invalidlegacycallee pointersareoutsidequalification;the six guardcasesexercise newwrapper/guardedpointtransform accesses,not an arbitrarymalformedrendererclaim.',
 '247-sourceDLL/libandstrictunboundedWallWextraWerrorpass. Exactprevious246bridgeplus1wrapper/20actualreturns/20continuationexports. Oldcallees,CPU/flatinterpreter/guards/productionobserver/rawsession/ABIunchanged,no newwaitlimits.400exportsareexplicitcontinuations,71qualifiednonlocalroutes/329remaining;exportaloneisnotnonlocalbehaviorqualification.2727oldbuilds preserved.',
 '243freshretainedIRQ/context/taskcomparisons/1683checkskeepoldnumericassertions. Total625freshcomparisons/6035checks;cumulative70524ishistorical-plus-fresh,notunique/codecoverage.216warm/211coldstartupandprior242full512write remainhistorical;nofreshcomplete247startup/gameplay/full-deviceclaim. Full029b00caller/textdependenciesandanglewrapperpaired-FMOVerrorpathsremainunfinished.',
 'Allgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOffdisablessampling/usesvertexcolors.10%weightedmilestones,notcoverage;researchDLLnonplayable. KeepgoalactiveandSolHighforreverseengineering.'
 ],sha256={str(p):digest(p) for p in files})
 from audit_movie_linked_routines import inventory
 q['linked_routine_inventory']=inventory(dll,registry if len(registry)==1587 else registry+[routine]);assert q['linked_routine_inventory']['confirmed_linked_unique_routines_by_name']==629
 q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
 (P/'analysis/collision_render_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('RENDER QUALIFIED1routine/332rows/31executed+1infeasible outcome;625comparisons/6035checks;629linked')
if __name__=='__main__':main()
