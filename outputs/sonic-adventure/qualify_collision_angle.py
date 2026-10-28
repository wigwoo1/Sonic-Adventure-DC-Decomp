"""Qualify complete new bodies and keep uncovered wrappers explicitly unregistered."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','audit','direct_verification','frontier_verification','retained_verification','unbounded'):
  path=P/f'analysis/collision_angle_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pin,h in r.get('sha256',{}).items():assert digest(pin)==h,(name,pin);files.append(Path(pin))
 dll=digest(ROOT/'work/sonic-adventure/build/collision_angle.dll')
 for name in ('audit','direct_verification','frontier_verification'):assert reports[name]['dll_sha256']==dll,name
 for suite in reports['retained_verification']['suites']:
  path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==dll
  for pin,h in r['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
 retained=reports['retained_verification'];assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
 direct=reports['direct_verification'];frontier=reports['frontier_verification']
 assert (direct['original_native_comparisons'],direct['independent_checks'])==(1046,9116)
 assert (frontier['original_native_comparisons'],frontier['independent_checks'])==(96,672)
 assert direct['status_counts']=={'0':808,'-1':166,'1':36,'-3':36}
 assert frontier['status_counts']=={'0':32,'-3':28,'-1':36}
 for r in (direct,frontier):assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in r['cases'])
 t=reports['translation'];assert len(t['routines'])==5 and t['instruction_rows']==223 and t['canonical_reused_entries']==['0x8c10fbb8'] and len(t['resume_routes'])==3
 coverage=[];routines=[];qualified={'0x8c10e6a0','0x8c10fb94'};partial={'0x8c10d058','0x8c10d0f8'}
 for body in t['routines']:
  cvs=[next(c for c in r['coverage_by_routine'] if c['address']==body['address']) for r in (direct,frontier)]
  missing=set.intersection(*(set(c['missing_rows']) for c in cvs));missing_edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
  cv=dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges),domain_excluded_rows=[],domain_excluded_outcomes=[]);coverage.append(cv)
  if body['address'] in qualified:
   assert not missing and not missing_edges
   routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=cv);routines.append(routine)
  elif body['address'] in partial:assert len(missing)==3 and len(missing_edges)==1
  else:assert body['address']=='0x8c10fbb8' and body['canonical_registered_reuse'] and not missing and not missing_edges
 assert sum(c['executed_rows'] for c in coverage)==217 and sum(c['executed_outcomes'] for c in coverage)==28
 returns_path=P/'analysis/collision_angle_returns.json';returns=json.loads(returns_path.read_text());files.append(returns_path)
 assert returns['prior_return_sites']==2268 and len(returns['allowed_call_returns'])==2271 and len(returns['new_call_returns'])==3
 assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==245 and reports['unbounded']['exit_code']==0
 for pin,h in reports['unbounded']['source_sha256'].items():assert digest(pin)==h;files.append(Path(pin))
 previous=P/'analysis/collision_math_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==66842
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1584,1586)
 assert not any(r['address'] in qualified|partial and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry[:1584])
 fresh=1046+96+243;checks=9116+672+1683;assert (fresh,checks)==(1385,11471)
 limits=[
  'Two complete new original bodies qualify:quadrant10e6a0(35rows/4outcomes)andatanpolynomial10fb94(16rows/2outcomes). All51newrows/6outcomesexecute. Canonicalrange-reduce10fbb8 adapter114rows/16outcomesexecute butalreadyregistered,zero duplicatecredit. Prior standalone atan sources/proofs preserved;new adapters use current numeric/bus/observer ABI.',
  'Two29row publicwrappers10d058/10d0f8 are translated/integrated proposals,each26rows/3of4outcomesexecute. Infinity-result errno arms at10d082..10d088 and10d122..10d128 remain unexecuted:6rows/2outcomes. No exclusions or unreachable proof are claimed;wrappers remain UNREGISTERED. Do not inject calleeresults/frames or patch original constants to manufacture coverage.',
  '1046base +96targeted original/native comparisons pass. Base808completed/166unsupported/36numerictrap/36RAMguard;frontier32completed/36unsupported/28RAMguard. Entire CPU/FPU/RAM/device and ordered boundaries compared. Exceptional/unsupported stops are not successful completions. Actual original connected callees execute;no substituted game services. Numeric-probe classifies existing exceptional states;ordinary scalar arithmetic remains independent integer/dyadic reference.',
  'Finite host asin/atan2/atan sanity uses absolute tolerance2e-6,not arbitrary-bit physical FPU proof. Both FPbanks,rounding/DN/SZ/PR,selected enables/flags,NaN/infinity/subnormal/zero/largeinputs,MMUoffP0/P1/P2 views andguardprefixes tested. Targeted largequotients cover missing canonicalrange-reducer rows;SZscratch frontier uses legitimate initial FPbits and stops naturally. SR.FD clear callABI;physicalexception/address-error/MMUdelivery/timing/gameplay remain unqualified.',
  '245-sourceDLL/lib and strictunboundedWallWextraWerrorpass. Exact244bridge+5bodies/3continuations/3actualJSRreturns;returnmembership2271,exports379. None of the new nonlocal resume routes are independently qualified merely by export:prior71qualified,308remaining. CPU/flatcode/guards/observer/rawsession/ABI unchanged,nonewwaitlimits;2721oldbuilds preserved.',
  '243freshretainedIRQ/context/taskcomparisons/1683checks preserve prior numeric expectations. Prior66842historical comparisons are not all rerun;combined68227is not unique or codecoverage.216warm/211coldstartup andprior242full512write remain historical. No fresh245complete startup/gameplay/full-devicewrite claim.',
  'Fullcollisioncaller029b00 has1121staticrows and further normalization,inverse-matrix,rendering dependencies. Original exported caller/dependency evidence is private,not translated/qualified/credited. Next establish original infinity-result wrapper path reachability or rigorous proof,then continue collisioncaller dependency closure. Optional660baewritehook remains unqualified. Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings unfinished. TextureOffdisables sampling/usesvertexcolors.10%weightedmilestones,not coverage. Keep SolHigh;no pause.'
 ]
 q=dict(status='partially_verified_collision_angle_dependencies_with_explicit_wrapper_gaps',compiled_source_count=245,newly_registered_entries=2,newly_qualified_rows=51,qualified_conditional_outcomes=6,executed_rows=217,executed_conditional_outcomes=28,canonical_adapter_rows=114,canonical_duplicate_credit=0,newly_translated_rows=109,newly_translated_unregistered_rows=58,partial_unregistered_gateway_rows=58,domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=routines,fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_previous_evidence_original_native_comparisons=66842,total_evidence_original_native_comparisons=66842+fresh,prior_registry_entries=1584,result_registry_entries=1586,result_unique_module_address_pairs=1583,prior_flat_return_guard_sites=2268,flat_return_guard_sites=2271,new_resume_exports=3,newly_qualified_nonlocal_routes=[],qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=308,explicit_nonfunction_resume_routes_total=379,finite_sanity_cases=direct['finite_sanity_cases']+frontier['finite_sanity_cases'],upstream_game_implementation_imported=False,physical_fpu_all_inputs_qualified=False,physical_exception_delivery_qualified=False,sr_fd_clear_call_abi=True,physical_maple_dma_complete=False,full_filesystem_qualified=False,full_gameplay_qualified=False,fresh_warm_startup_comparisons=0,fresh_cold_profile=False,historical_warm_profile=old['historical_warm_profile'],historical_cold_profile=old['historical_cold_profile'],partial_unregistered_proposals=['0x8c660bae']+sorted(partial),next_task='Establish original infinity-result errno path reachability or rigorous proof in10d058/10d0f8;keep both unregistered until justified. Continue029b00 dependencyclosure without suppliedcalleeoutputs/frames or alteredoriginalconstants.',dll_sha256=dll,limitations=limits,overall_completion_percent=10,estimate_method='weighted milestones, not measured code coverage',sha256={str(p):digest(p) for p in files})
 from audit_movie_linked_routines import inventory
 q['linked_routine_inventory']=inventory(ROOT/'work/sonic-adventure/build/collision_angle.dll',registry if len(registry)==1586 else registry+routines)
 assert q['linked_routine_inventory']['registry_entries']==1586 and q['linked_routine_inventory']['registry_unique_module_address_pairs']==1583
 q['sha256'][str(P/'audit_movie_linked_routines.py')]=digest(P/'audit_movie_linked_routines.py')
 (P/'analysis/collision_angle_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('COLLISION ANGLE QUALIFIED2/51rows/6outcomes;2wrappersstillpartial;',fresh,checks,'confirmedlinked',q['linked_routine_inventory']['confirmed_linked_unique_routines_by_name'],flush=True)
if __name__=='__main__':main()
