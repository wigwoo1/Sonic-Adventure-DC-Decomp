"""Qualify two complete helpers; keep the expanded initialization caller partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','verification','retained_verification','startup_verification'):
  path=P/f'analysis/movie_earlier_initialize_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['translation'];d=reports['verification'];retained=reports['retained_verification'];startup=reports['startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_earlier_initialize.dll')
 assert len(t['routines'])==6 and t['instruction_rows']==101 and reports['connection']['compiled_source_count']==213
 assert (d['cases_passed'],d['independent_checks'])==(408,6120) and (retained['total_original_native_comparisons'],retained['independent_checks'])==(35,335)
 assert (startup['cases_passed'],startup['independent_checks'],startup['actual_next_frontier'],startup['current_warm_boundaries'])==(1,75,'0x8c051790',4709)
 assert all(r['dll_sha256']==dll for r in (d,retained,startup)) and not startup['fresh_cold_profile']
 registry_path=P/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text())['routines'];previous=registry[:1469];assert len(previous)==1469 and len(registry) in (1469,1471)
 new={0x8c5fbb60:'sa_movie_earlier_initialize_8c5fbb60',0x8c6033e4:'sa_movie_earlier_initialize_8c6033e4'};canonical={0x8c644d84,0x8c5fbcb6,0x8c647ca4};qualified=[];reused=[]
 for routine in t['routines']:
  address=int(routine['address'],16);coverage=next(r for r in d['coverage_by_routine'] if r['address']==routine['address'])
  if address==0x8c053ac0:assert coverage['executed_rows']==0;continue
  assert not coverage['missing_rows'] and not coverage['missing_outcomes']
  matches=[r for r in previous if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)&0x1fffffff==address&0x1fffffff]
  if address in canonical:
   assert len(matches)==1 and matches[0]['retail_span_sha256']==routine['retail_span_sha256'];old=matches[0];files.extend((P/old['source'],P/old['verification']));reused.append(dict(address=routine['address'],canonical_name=old['name'],additional_credit=0))
  else:
   assert address in new and not matches
   qualified.append(dict(module='1ST_READ.BIN',name=new[address],address=routine['address'],end_exclusive=routine['end_exclusive'],source=t['source'],retail_span_sha256=routine['retail_span_sha256'],instruction_rows=routine['instruction_rows'],executed_rows=coverage['executed_rows'],executed_outcomes=coverage['executed_outcomes']))
 assert sum(r['instruction_rows'] for r in qualified)==36 and sum(r['executed_outcomes'] for r in qualified)==4 and len(reused)==3
 capture_path=ROOT/'work/sonic-adventure/movie_earlier_initialize_startup_warm_capture.json';capture=json.loads(capture_path.read_text());seen=set(capture['executed_rows']);body=next(r for r in t['routines'] if r['address']=='0x8c053ac0');missing=sorted(set(body['reachable_addresses'])-seen);observed={tuple(e) for e in capture['executed_edges']};missing_edges=sorted(set(map(tuple,body['conditional_edges']))-observed);assert len(missing)==26 and len(missing_edges)==1
 prior=P/'analysis/movie_record_address_qualification.json';previous_q=json.loads(prior.read_text());assert previous_q['total_evidence_original_native_comparisons']==8424;files.extend((prior,capture_path))
 limitations=['Two new helpers fully execute36rows/four conditional outcomes within declared immutable-code/literal, coherent mapped-RAM and disjoint-stack/globals domain. Three canonical helpers22rows receive zero extra recovery credit. Finite differential tests are not exhaustive whole-game ABI proof.',
 'Earlier053ac0 caller remains partial:17of43rows/one of two outcomes execute in fresh warm startup;26rows and retry-taken outcome remain unqualified. Its research-only two-taken-test retry guard returns incomplete status-4 under SA_BOUND_RETAIL_STALLS; no successful result is substituted, and that taken retry guard was not exercised by this zero-status warm run.',
 'Fresh warm startup executes real memory setup then prior0546c0 and record poll before missing051790. Complete original/native state/boundaries match. Historical212nested4702 target sequence only is retained; changed enclosing registers/stack/PR mean no full historical boundary-field identity claim.',
 '35retained context/SR/FD cases replay unchanged fixtures/oracles in213DLL. Older8424evidence not all rerun;211fullcold remains historical/not rerun or relabeled. Not physical reset/boot/gameplay proof. Broader652210/656940 remain partial.',
 'Whole game/ports/FPS8K/SUPIR/settings unfinished. TextureOff disables sampling/uses vertex colors. Goal ACTIVE about10% weighted milestones, not code coverage.']
 report=dict(newly_registered_entries=2,newly_qualified_rows=36,executed_new_rows=36,qualified_conditional_outcomes=4,canonical_reuse=reused,canonical_integration_rows=22,reused_additional_credit=0,compiled_source_count=213,routines=qualified,coverage_by_routine=d['coverage_by_routine'],partial_caller=dict(address='0x8c053ac0',static_rows=43,executed_rows=17,missing_rows=missing,executed_outcomes=1,missing_outcomes=missing_edges,registered=False),new_direct_original_native_comparisons=408,new_direct_independent_checks=6120,fresh_retained_original_native_comparisons=35,fresh_retained_independent_checks=335,fresh_warm_startup_comparisons=1,fresh_warm_startup_checks=75,fresh_original_native_comparisons=444,fresh_independent_checks=6530,retained_previous_evidence_original_native_comparisons=8424,total_evidence_original_native_comparisons=8868,prior_registry_entries=1469,result_registry_entries=1471,result_unique_module_address_pairs=1468,actual_next_frontier='0x8c051790',warm_boundaries=4709,fresh_cold_profile=False,historical_cold_profile=retained['historical_cold_profile'],remaining_unqualified_graphics_root_outcomes=previous_q['remaining_unqualified_graphics_root_outcomes'],partial_unregistered_dispatcher='0x8c652210',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_earlier_initialize_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED2new36rows/4outcomes;3canonical22rows;callerpartial;444cases6530checks')
if __name__=='__main__':main()
