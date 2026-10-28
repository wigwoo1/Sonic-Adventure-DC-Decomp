"""Qualify complete original glyph/text helpers in the existing software ABI domains."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','audit','direct_verification','retained_verification','unbounded'):
  path=P/f'analysis/collision_text_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 dll=ROOT/'work/sonic-adventure/build/collision_text.dll';h=digest(dll)
 for name in ('audit','direct_verification'):assert reports[name]['dll_sha256']==h
 retained=reports['retained_verification']
 for suite in retained['suites']:
  path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==h
  for pin,sha in r['sha256'].items():assert digest(pin)==sha,(suite['suite'],pin);files.append(Path(pin))
 t=reports['translation'];direct=reports['direct_verification'];assert len(t['routines'])==2 and t['instruction_rows']==315 and len(t['resume_routes'])==2 and not t['canonical_reused_entries']
 assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_quad_oracle_cases'])==(243,2709,175) and direct['status_counts']=={'0':207,'-1':24,'-3':12}
 cv=direct['coverage_by_routine'];assert len(cv)==2 and sum(c['executed_rows'] for c in cv)==315 and sum(c['executed_outcomes'] for c in cv)==4
 assert all(not c['missing_rows'] and not c['missing_outcomes'] for c in cv)
 targets={int(b['target'],16) for c in direct['cases'] for b in c['boundaries']};assert targets=={0x8c6407b0,0x8c640922,0x8c10d3f0}
 long=[c for c in direct['cases'] if c['configuration']['kind']=='length'];assert [c['configuration']['length'] for c in long]==[32768,65536,65537]
 assert [sum(b['target']=='0x8c6407b0' for b in c['boundaries']) for c in long]==[0,0,1]
 assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
 assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==248 and reports['unbounded']['exit_code']==0
 for pin,sha in reports['unbounded']['source_sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 returns=P/'analysis/collision_text_returns.json';r=json.loads(returns.read_text());files.append(returns);assert r['prior_return_sites']==2292 and len(r['allowed_call_returns'])==2294 and r['new_call_returns']==['0x8c6409d6','0x8c6409ea']
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1587,1589)
 assert not any(x['address'] in ('0x8c6407b0','0x8c640922','0x8c10d058','0x8c10d0f8') and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in registry[:1587])
 previous=P/'analysis/collision_render_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==70524
 routines=[]
 for body in t['routines']:
  routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(c for c in cv if c['address']==body['address']));routines.append(routine)
 q=dict(status='partially_verified_original_glyph_and_text_software_abi_domains',compiled_source_count=248,newly_registered_entries=2,new_original_routine_credit=2,newly_qualified_rows=315,executed_rows=315,static_conditional_outcomes=4,executed_conditional_outcomes=4,proven_infeasible_conditional_outcomes=0,unaccounted_outcomes=[],domain_exclusion_arguments=[],coverage_by_routine=cv,routines=routines,canonical_duplicate_credit=0,new_direct_entry_routes_qualified=2,new_nonlocal_resume_exports=2,new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=2294,explicit_nonfunction_resume_routes_total=402,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=331,prior_registry_entries=1587,result_registry_entries=1589,result_unique_module_address_pairs=1586,fresh_original_native_comparisons=486,fresh_independent_checks=4392,direct_status_counts=direct['status_counts'],independent_quad_oracle_cases=175,reached_actual_existing_game_callees=['0x8c10d3f0'],supplied_game_callee_outputs=0,supplied_execution_frames=0,test_only_external_QACR_input_seed=True,retained_previous_evidence_original_native_comparisons=70524,total_evidence_original_native_comparisons=71010,physical_fpu_all_inputs_qualified=False,physical_exception_delivery_qualified=False,full_gameplay_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,wrappers_still_unregistered=['0x8c10d058','0x8c10d0f8'],optional_660bae_still_unqualified=True,upstream_game_implementation_imported=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Recover full029b00collisioncaller usingclosedrenderer/text/canonicalmathdependencies;verifyrealoriginalcalleesandderiveactualanglewrapperinputconstraints. Keep10d058/10d0f8unregistereduntiltheirremainingpathsarejustified;no inventedcalleeoutputs/frames.',dll_sha256=h,limitations=[
 'Two complete original routines:6407b0 glyph quad emitter185instructions/0branches/0calls;640922 string drawing130instructions/2conditionalbranches/2calls. Bothnewfunctions andthe existing10d3f0string-lengthcalleeexecuteactualoriginalbehavior;noSARecompimplementation/gameoutputs/executionframes supplied. All315instructions/4conditionaloutcomesexecute,noexclusions/infeasibleoutcomes/duplicatecredit.',
 '243direct fullCPU/FPU/RAM/device/softwarequeue/orderedboundary comparisons:207completed,24existingunsupportednumericstops,12mappedRAMguards. BothFPbanks/PR/SZ/RM0/1,finitecoordinates/halfsteps,selectedNaN/infinity/subnormal/maxfinitefontsizes,mappedMMUoffP0/P1/P2aliases,cursoroffsets,colours,characterlow/highbytes andstringsincludingempty/embeddedNUL/highbytes/longsignedlengthscovered. Existingnumericprobebackends retained;nonzerostopsarenotcompletions;physicalhardwareexception/MMUdelivery/GPU pixelsunqualified;SRFDclearfixedcodeABI.',
 '175independentordinaryfinitecasescheck allsevenmeaningfulwordsofeachquadvertex:control,signed16positions,depth1,exact16x16atlasUVwith1/1024biasand63/1024span,colour. Alsofourvertexorder,finalcursor,originaltextheaderwordorder,state marker,inputstringpreservationandcommittedrendererpointer. Stringlengths32768/65536/65537followoriginalsigned16count:0/0/1glyphs. These175areincludedin243,notadditionalcomparisoncredit. Padding/queue/devicebehavioralsoexactlycomparedagainstoriginalexecution.',
 'Directglyphleafcallers mustconfigurestorequeuesfirst. InitialsmokeleftQACRzeroandbothoriginal/nativestoppedattheunconfiguredqueueflush. Added explicit external-deviceINPUTQACR12onbothsidesbeforeexecutionforthedeviceleafcases;no suppliedgameoutput/frame. TextcallerstartsQACRzeroandexecutesitsownoriginalQACRwrites. Test-onlycomparisonsetupaddsseedassignmentonly;allfullstate/orderedcomparisonlogicandproductionobserverunchanged.',
 'GhidradraftincorrectlyremovedFLOATinstructions6409bc/6409caas unreachable;bothraworiginalinstructionsarepreservedandexecute. Instructionlistingsandbytechecks,notdraftpseudocode,areauthoritative. All oldtemplates/nativecallees/CPU/flatinterpreter/guards/rawsession/ABI implementations preserved.248-sourceDLL/libandstrictunboundedWallWextraWerrorpass;no newwaitbounds.',
 'Exactprevious247bridgeplus2bodies/2actualreturns/2continuationexports. Returnmembership2294;402explicitresumeexports,71qualifiednonlocalroutes/331remaining. Newcontinuationexportsarenotindependentlyqualifiednonlocalresumes.2729oldbuildartifactspreserved,newDLL/libadditional.1587oldregistryrowsandrequirements/proofs/sourceunchangedexceptreviewedmutablecatalogs/docs.',
 '243freshretainedIRQ/context/taskcomparisons/1683checksretainpreviousnumericassertions(AST). Total486freshcomparisons/4392checks;cumulative71010ishistorical-plus-fresh,notunique/codecoverage.216warm/211coldstartupandprior242full512write remainhistorical;nofreshcomplete248startup/gameplay/full-deviceclaim. Full029b00collisioncallerandtwoanglewrapperpaired-FMOVerrorpathsremainunfinished.',
 'Allgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished;TextureOffdisablessampling/usesvertexcolours.10%weightedmilestones,notcoverage;researchDLLnonplayable. KeepgoalactiveandSolHighforreverseengineering.'
 ],sha256={str(p):digest(p) for p in files})
 from audit_movie_linked_routines import inventory
 q['linked_routine_inventory']=inventory(dll,registry if len(registry)==1589 else registry+routines);assert q['linked_routine_inventory']['confirmed_linked_unique_routines_by_name']==631
 q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
 (P/'analysis/collision_text_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('TEXT QUALIFIED2routines/315rows/4outcomes;486comparisons/4392checks;631linked')
if __name__=='__main__':main()
