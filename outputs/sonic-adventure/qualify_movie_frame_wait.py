"""Credit only the complete display synchronizer; preserve partial wait/queue work."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports={};files=[Path(__file__)]
 for name in ('translation','connection','verification','retained_verification','startup_verification'):
  path=P/f'analysis/movie_frame_wait_{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
  for pinned,h in report['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['translation'];d=reports['verification'];retained=reports['retained_verification'];startup=reports['startup_verification'];dll=digest(ROOT/'work/sonic-adventure/build/movie_frame_wait.dll')
 assert len(t['routines'])==10 and t['instruction_rows']==433 and reports['connection']['compiled_source_count']==214
 assert (d['cases_passed'],d['independent_checks'])==(220,2636)
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(35,335)
 assert (startup['cases_passed'],startup['independent_checks'],startup['actual_wait_pc'],startup['frame_counter_unchanged'])==(1,79,'0x8c051798',2)
 assert all(r['dll_sha256']==dll for r in (d,retained,startup)) and not startup['fresh_cold_profile']
 cov={r['address']:r for r in d['coverage_by_routine']};display=next(r for r in t['routines'] if r['address']=='0x8c6433e0');row=cov[display['address']];assert (row['static_rows'],row['executed_rows'],row['executed_outcomes'])==(66,66,2) and not row['missing_rows'] and not row['missing_outcomes']
 assert cov['0x8c055c4c']['executed_rows']>60 and cov['0x8c055c4c']['executed_outcomes']>=5,'Nonempty queue requests must really execute; legacy fixture defaults may not overwrite counts'
 assert cov['0x8c608c82']['executed_rows']>23 and cov['0x8c6088bc']['executed_rows']==40
 registry_path=P/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text())['routines'];previous=registry[:1471];assert len(previous)==1471 and len(registry) in (1471,1472)
 canonical={0x8c6088bc,0x8c608914,0x8c608922,0x8c608e24,0x8c64ddc0};reused=[];partial=[];binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();files.append(binary)
 for body in t['routines']:
  address=int(body['address'],16);matches=[r for r in previous if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)&0x1fffffff==address&0x1fffffff]
  if address in canonical:
   assert len(matches)==1;old=matches[0];start=int(old['address'],16);end=int(old['end_exclusive'],16);assert hashlib.sha256(raw[start-0x8c010000:end-0x8c010000]).hexdigest()==old['retail_span_sha256'];files.extend((P/old['source'],P/old['verification']));reused.append(dict(address=body['address'],canonical_name=old['name'],additional_credit=0,historical_end_exclusive=old['end_exclusive'],integration_end_exclusive=body['end_exclusive'],historical_span_checked_against_same_binary=True,coverage=cov[body['address']]))
  else:
   assert not matches
   if address!=0x8c6433e0:partial.append(cov[body['address']]|dict(registered=False))
 assert len(reused)==5 and len(partial)==4
 prior=P/'analysis/movie_earlier_initialize_qualification.json';old_q=json.loads(prior.read_text());assert old_q['total_evidence_original_native_comparisons']==8868;files.append(prior)
 routine=dict(module='1ST_READ.BIN',name=display['name'],address=display['address'],end_exclusive=display['end_exclusive'],source=t['source'],retail_span_sha256=display['retail_span_sha256'],instruction_rows=66,executed_rows=66,executed_outcomes=2)
 limitations=['Only6433e0 display synchronization is newly qualified:all66rows/two outcomes;96display cases include both register banks and all independent RAM/stack/MMIO argument expectations. Actual existing MMIO and buffer-selection/copy callees execute. Immutable code/literals, mapped coherent buffers and disjoint private frames apply. Finite tests are not exhaustive hardware/game ABI proof.',
 'Frame051790 remains partial:positive counter wait executes, unchanged positive value and research-only two-test incomplete-4 observed. Zero/negative completion paths and asynchronous VBlank progress are not qualified. No counter clearing, direct callback or invented IRQ substitutes for progress. Earlier053ac0 remains partial/unregistered.',
 'Batch055c4c,submission608c82 and dispatcher64c34c remain partial. Nonempty batches build real stack descriptors and triplets, perform actual optional release, reset error state and reach real dispatcher tails64c374/64d1a6 as missing-2. Resource64ddc0 reaches actual missing classifier64f206 on active-resource path. Five canonical helpers get zero additional credit; per-body coverage remains explicit.',
 'Full Ghidra64c34c export merges2558rows. A byte-checked20row entry prefix ends only after actual indirect jump/delay slot. Full original export preserved; downstream tail bodies remain explicit reached dependencies, not omitted behavior or completed dispatcher credit.',
 'Fixture preserves requested case parameters across shared legacy preparation; each request checked after setup, nonempty/negative queue coverage asserted. Earlier erroneous220-case report with overwritten counts is superseded by final corrected suite, never counted again. Complete original/native CPU/FPU/RAM/device/ordered state comparison plus declared independent checks; no mock helper returns.',
 '256fresh comparisons3050checks:220direct2636,35retained335,one warm79. Previous8868evidence not all rerun;9124combined. Fresh warm4709boundaries retains exact previous213 recorded boundary fields and stops at051798 with counter2;old final machine identity after newly executed instructions not claimed. Old211cold historical/not rerun. Not physical timing/reset/gameplay proof.',
 'Whole game/ports/FPS8K/SUPIR/settings unfinished. TextureOff disables sampling/uses vertex colors. Broader652210/656940partial;goal ACTIVE about10% weighted milestones,not code coverage.']
 report=dict(newly_registered_entries=1,newly_qualified_rows=66,executed_new_rows=66,qualified_conditional_outcomes=2,compiled_source_count=214,routines=[routine],canonical_reuse=reused,canonical_integration_rows=186,reused_additional_credit=0,partial_entries=partial,coverage_by_routine=d['coverage_by_routine'],new_direct_original_native_comparisons=220,new_direct_independent_checks=2636,fresh_retained_original_native_comparisons=35,fresh_retained_independent_checks=335,fresh_warm_startup_comparisons=1,fresh_warm_startup_checks=79,fresh_original_native_comparisons=256,fresh_independent_checks=3050,retained_previous_evidence_original_native_comparisons=8868,total_evidence_original_native_comparisons=9124,prior_registry_entries=1471,result_registry_entries=1472,result_unique_module_address_pairs=1469,actual_wait_pc='0x8c051798',frame_counter_unchanged=2,warm_boundaries=4709,fresh_cold_profile=False,historical_cold_profile=retained['historical_cold_profile'],next_task='Recover/connect canonical countdown051810 and actual VBlank handler progress;do not force counter zero or substitute callbacks.',remaining_unqualified_graphics_root_outcomes=old_q['remaining_unqualified_graphics_root_outcomes'],partial_unregistered_dispatcher='0x8c652210',dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_frame_wait_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('QUALIFIED display1/66rows/2outcomes;wait/queuepartial;256fresh/3050checks')
if __name__=='__main__':main()
