"""Qualify complete path/directory bodies while retaining the initializer frontier."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
SELECTED={'0x8c64563e','0x8c64573e','0x8c6454fe','0x8c6002a2'};REUSED={'0x8c645374','0x8c64541c'}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_disc_paths_translation','movie_disc_paths_connection','movie_disc_paths_media','movie_disc_paths_verification','movie_disc_paths_invariant','movie_disc_paths_regression_verification','movie_disc_init_qualification','movie_disc_init_translation')]
 translation,connection,media,direct,proof,regression,prior,parent=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for report in (translation,connection,media,direct,proof,regression,prior,parent):
  for path,h in report['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert connection['compiled_source_count']==171 and translation['instruction_rows']==344 and len(translation['routines'])==4
 assert direct['cases_passed']==98 and direct['independent_checks']==598 and direct['status_counts']=={'0':93,'-4':1,'-3':4}
 assert direct['dll_sha256']==regression['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_disc_paths.dll')
 assert len(media['profiles'])==10 and not media['retail_bytes_included'] and not media['whole_disc_images']
 for profile in media['profiles'].values():assert digest(ROOT/profile['path'])==profile['sha256']
 rows=direct['coverage_by_routine'];assert len(rows)==6
 for row in rows:
  assert not row['missing_rows']
  assert row['missing_edges']==([['0x8c645854','0x8c645858']] if row['address']=='0x8c64573e' else [])
 assert sum(r['static_rows'] for r in rows if r['address'] in SELECTED)==499
 assert sum(r['conditional_edges_covered'] for r in rows if r['address'] in SELECTED)==99
 assert proof['unreachable_edge']==['0x8c645854','0x8c645858'] and proof['guard_dominates_later_test'] and proof['callee_preserves_r11'] and not proof['unreachable_instruction_rows']
 assert regression['retained_warm_startup_comparisons']==1 and regression['original_bridge_preserved_except_added_routes'] and regression['retained_cold_trace_has_no_added_routes'] and not regression['cold_start_reexecuted_in_new_dll']
 assert regression['warm_result']['clear_native']['writes']==0x200000
 boundaries=[c for c in direct['cases'] if c['input']['media_profile'] in ('crossing','exact_path')]
 assert len(boundaries)==8 and all(c['disc_read_attempts']==2 for c in boundaries)
 assert {c['guest_r0'] for c in boundaries}=={'0xafcd','0x0'}
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in direct['cases'])
 routines=[]
 for report in (translation,parent):
  for r in report['routines']:
   if r['address'] in SELECTED:routines.append(dict(r,module='1ST_READ.BIN',source=report['source'],source_sha256=digest(P/report['source'])))
 assert len(routines)==4
 files.append(Path(__file__))
 report=dict(routines=routines,newly_registered_entries=4,qualified_rows=499,executed_rows=499,qualified_conditional_outcomes=100,executed_conditional_outcomes=99,proven_unreachable_conditional_outcomes=1,unreachable_edge=proof['unreachable_edge'],
  reused_existing_entries=sorted(REUSED),reused_additional_credit=0,compiled_source_count=171,direct_comparisons=98,regression_startup_comparisons=1,total_comparisons=99,independent_direct_checks=598,generated_media_profiles=10,coverage_by_routine=rows,
  retained_cold_reads=72,retained_cold_boundary_comparisons=56648,cold_start_reexecuted_in_new_dll=False,
  next_task='Complete remaining original disc/library initializer media/error branches using explicit raw inputs; then join initialization to its original caller and MOVIE boot sequence. Keep the cold baseline and new path/directory evidence distinct.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Four complete original path resolvers, directory loader and directory-selection bodies qualify. All 499 rows execute; 99/100 outcomes execute and one is justified by byte-checked R11 preservation. Two existing string helpers receive zero extra credit. Native disc/platform implementations are unchanged.',
  limitations=[
   'Generated media are sixteen-sector raw test streams with invented path/directory records and separately declared namespaces. They contain no retail bytes and are not full disc images or measurements of physical drive behavior. Actual native transport reads their requested bytes; no helper/lookup/read result is substituted.',
   'Direct tests use the retained explicitly prepared library seed and initialize its linked temporary descriptor as a precondition. They do not claim that every context was constructed by original reset flow. The preceding 170-source cold initialization proof remains retained separately.',
   'The suite covers nested/missing/case-folded names, both separators, prefixed paths, record rollover and exact sector ends, cache hits, signed counts, buffer-full and first-record size changes, read failures, table selection and type-one post-processing. Independent namespace/record/hash/field assertions supplement full-state instruction comparisons.',
   'The one unexecuted outcome would branch on R11 equal to one after a dominating check established the opposite. The intervening corridor, its delay slots and its original comparison/uppercase callees preserve R11. The proof assumes immutable instructions, normal call/return ABI and no asynchronous CPU register mutation. Every original row and branch remains in C.',
   'An out-of-range path-table input retains the previous finite read-wait stop. Four invalid-memory controls match; a leaf helper that never accesses its stack can still return normally with a bad-stack fixture. Host zero is not synonymous with guest operation success.',
   'The new build directly replays retained warm earlier startup and its complete eight-MiB clear. The original bridge is byte-identical after removing only added routes/export renames, and the earlier 56648-boundary cold trace has none of those routes. This is source-level preservation evidence; that entire cold comparison was not rerun in the new DLL.',
   'Initializer 8c600038 remains partial at its retained 409/479 rows and 28/58 outcomes and receives no new credit. Complete reset-to-gameplay, physical I/O/timing, pixels, all playable ports and requested frame-rate/display/texture settings remain unfinished. Whole-project weighted estimate remains ten percent.'])
 (P/'analysis/movie_disc_paths_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified four original path/directory bodies: all 499 rows, 99 executed outcomes plus one invariant; two string helpers reused')
if __name__=='__main__':main()
