"""Qualify canonical inverse/determinant integration without duplicate recovery credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','audit','direct_verification','retained_verification','unbounded'):
  path=P/f'analysis/collision_inverse_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 dll=ROOT/'work/sonic-adventure/build/collision_inverse.dll';h=digest(dll)
 for name in ('audit','direct_verification'):assert reports[name]['dll_sha256']==h
 for suite in reports['retained_verification']['suites']:
  path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==h
  for pin,sha in r['sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 t=reports['translation'];direct=reports['direct_verification'];retained=reports['retained_verification']
 assert len(t['routines'])==2 and t['instruction_rows']==1191 and len(t['resume_routes'])==1 and t['canonical_reused_entries']==['0x8c6390b0','0x8c64f3ec']
 assert (direct['original_native_comparisons'],direct['independent_checks'])==(624,5862) and direct['status_counts']=={'0':511,'-1':97,'-3':16}
 assert sum(c['executed_rows'] for c in direct['coverage_by_routine'])==1191 and sum(c['executed_outcomes'] for c in direct['coverage_by_routine'])==8
 assert all(not c['missing_rows'] and not c['missing_outcomes'] for c in direct['coverage_by_routine'])
 assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
 assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==246 and reports['unbounded']['exit_code']==0
 for pin,sha in reports['unbounded']['source_sha256'].items():assert digest(pin)==sha;files.append(Path(pin))
 returns=P/'analysis/collision_inverse_returns.json';r=json.loads(returns.read_text());files.append(returns);assert r['prior_return_sites']==2271 and len(r['allowed_call_returns'])==2272 and r['new_call_returns']==['0x8c6390ba']
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1586
 for body in t['routines']:
  row=next(x for x in registry if x['address']==body['address'] and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN');assert row['name']==body['name'] and row['retail_span_sha256']==body['retail_span_sha256']
 assert not any(x['address'] in ('0x8c10d058','0x8c10d0f8') and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in registry)
 previous=P/'analysis/collision_normalize_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==69032
 oracle_fields=('exact_determinant_oracle_cases','exact_inverse_identity_cases','approximate_inverse_identity_cases','singular_sentinel_oracle_cases');oracles={k:direct[k] for k in oracle_fields};assert sum(oracles.values())==432
 q=dict(status='partially_verified_canonical_inverse_determinant_runtime_integration',compiled_source_count=246,newly_registered_entries=0,new_original_routine_credit=0,canonical_adapter_rows=1191,canonical_duplicate_credit=0,new_direct_entry_routes_qualified=2,original_canonical_rows_executed=1191,qualified_conditional_outcomes=8,executed_conditional_outcomes=8,coverage_by_routine=direct['coverage_by_routine'],domain_exclusion_arguments=[],canonical_entries=t['canonical_reused_entries'],new_nonlocal_resume_exports=1,new_nonlocal_resume_routes_qualified=0,flat_return_guard_sites=2272,explicit_nonfunction_resume_routes_total=380,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=309,registry_entries=1586,registry_unique_module_address_pairs=1583,fresh_original_native_comparisons=867,fresh_independent_checks=7545,direct_status_counts=direct['status_counts'],retained_previous_evidence_original_native_comparisons=69032,total_evidence_original_native_comparisons=69899,physical_fpu_all_inputs_qualified=False,physical_exception_delivery_qualified=False,enabled_exception_domains_qualified=False,full_gameplay_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,wrappers_still_unregistered=['0x8c10d058','0x8c10d0f8'],optional_660bae_still_unqualified=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Continue029b00 dependency closure:recover remaining original rendering dependencies63d66c/640922 andtheiractualtransitivecalls,using alreadyrecovered canonical code whereproperABIallows. Keepwrapperpaired-FMOVerrorpaths open,no inventedcalleeoutputs/frames.',dll_sha256=h,**oracles,limitations=[
 'Two alreadyregistered canonical functions gain currentABI implementations/directentryroutes:inverse6390b0(1019rows)anddeterminant64f3ec(172rows). All1191rows/8conditionaloutcomesexecute,no exclusions or duplicate recoverycredit. Oldcanonical sources/proofs/ABIs preserved. NewCABI is the currentfullcontext/bus/observer interface in this new researchDLL,not the old standalone entrysignature.',
 'The actual inverse JSR6390b6 calls originaldeterminant64f3ec withPR6390ba andproperstack/state;no substitutedresults/frames. Newactualreturn membership2272 andone exportedcontinuation;exportalone doesnot qualify nonlocalresume. Existing71qualified/309remaining,380explicitexports. CPU/flatcode/guards/observer/rawsession unchanged,no newsourcewaitbounds.',
 '624fresh fullCPU/FPU/RAM/device/orderedboundary comparisons:511completed,97existingunsupportednumericstops,16mappedmatrixguards. BothFPbanks/RM0/1/DN,PR/SZ/presetflags,explicit/implicitmatrices,mappedMMUoffP0/P1/P2views,normal/singular/NaN/infinity/subnormal/maxfiniteinputs tested. Enabledexception andphysicaldeliverydomains remainunqualified. Nonzero statuses are not completions.',
 '432independent ordinary finite-matrix checks comprise24term integerpermutationdeterminants,exact rational A*inverse(A)=I forpoweroftwotriangular/permutation/translationcases,bounded denseproducts atabsolute1e-4,andoriginalsingularsentinel/zero-return cases. Firstindependentoracle incorrectlyassumedzero fill;originalbyte7f7fc99e proves finite singularsentinel. InitialSZ1footprint assumedforward writes;originalpointerwalks prove backward[M-64,M+64) union. Threefocusedcases andcorrectedfullmatrixpass,nativebehaviorunchanged.',
 'Existing rawFLDS emitter fromrecover_draw_clipping_state.py was missing from inheritedgenerator;added only to newfamily. No oldtemplates/source/proofs rewritten.246-sourceDLL/lib andstrictunboundedWallWextraWerrorpass;2725olderbuildfiles preserved, newDLL/libadditional. Generated adapters preserve originalinstructionorder/FRbank/SZswitches/FPUL/register/stack effects andfinalRTSdelayword6398ac;8literalbytes excluded frominstructionlist,not missingcode.',
 '243freshretainedIRQ/context/taskcomparisons/1683checks keepoldnumericassertions. Combined69899ishistorical-plus-fresh,notunique/codecoverage.216warm/211coldstartup andprior242full512write remainhistorical. No freshcomplete246startup/gameplay/full-devicewrite claim. Full029b00caller andrenderingclosure remainunfinished;bothanglewrappers stayunregistered.',
 'Allgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings unfinished;TextureOffdisablessampling/usesvertexcolors.10%weightedmilestones,notcoverage;researchDLLnonplayable. Keepgoalactive,SolHigh forreverseengineering;no upstreamimplementationcopy.'
 ],sha256={str(p):digest(p) for p in files})
 from audit_movie_linked_routines import inventory
 q['linked_routine_inventory']=inventory(dll,registry);assert q['linked_routine_inventory']['confirmed_linked_unique_routines_by_name']==628
 q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
 (P/'analysis/collision_inverse_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('INVERSE INTEGRATION QUALIFIED1191rows/8outcomes;867comparisons/7545checks;628linked,0newroutines',oracles)
if __name__=='__main__':main()
