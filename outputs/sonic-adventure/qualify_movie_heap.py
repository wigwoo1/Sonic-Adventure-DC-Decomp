"""Qualify heap bodies, retaining startup's real missing-initialization fault."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_heap_translation','movie_heap_connection','movie_heap_verification','movie_heap_initialization_verification','movie_heap_prefix_verification','movie_heap_state_probe','movie_heap_frontier','movie_render_setup_qualification')]
 translation,connection,direct,initial,prefix,probe,frontier,prior=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,initial,prefix,probe,frontier,prior):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert direct['dll_sha256']==initial['dll_sha256']==prefix['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_heap.dll')
 assert direct['cases_passed']==238 and direct['status_counts']=={'0':228,'-3':8,'-4':2} and direct['independent_checks']==1624
 assert initial['cases_passed']==127 and initial['status_counts']=={'0':122,'-4':2,'-3':3} and initial['independent_checks']==738
 coverage=[]
 for row in direct['coverage_by_routine']:
  chosen=next((r for r in initial['coverage_by_routine'] if r['address']==row['address']),row)
  assert not chosen['missing_rows'] and not chosen['missing_edges'],chosen
  coverage.append(chosen)
 assert sum(r['static_rows'] for r in coverage)==180 and sum(r['conditional_edges_covered'] for r in coverage)==22
 assert prefix['cases_passed']==6 and prefix['independent_checks']==120 and prefix['status_counts']=={'-3':6}
 assert prefix['next_dependencies']==['0x8c10d4bc','0x8c6021fe']
 for c in prefix['cases']:
  assert c['heap_root_at_stop']=='0x0'
  if c['input']['initialize_context']:assert c['stop_pc']=='0x8c6021fe' and c['heap_cursor_register_at_stop']=='0x0'
  else:assert c['stop_pc']=='0x8c10d4bc'
 assert probe['cursor']=='0x0' and probe['invalid_cursor'] and probe['request_bytes']==80752
 routines=[]
 for original in translation['routines']:
  if original['address']!='0x8c6021d2':routines.append(dict(original,module='1ST_READ.BIN',source=translation['source'],source_sha256=digest(P/translation['source'])))
 assert len(routines)==3 and sum(r['instruction_rows'] for r in routines)==178
 files.append(Path(__file__))
 report=dict(routines=routines,coverage_by_routine=coverage,newly_registered_entries=3,reused_existing_thunks=1,qualified_rows=178,qualified_conditional_outcomes=22,integrated_rows=180,
  compiled_source_count=160,direct_comparisons=365,normal_returns=350,memory_faults=11,privilege_faults=4,independent_direct_checks=2362,allocation_sequence_comparisons=36,
  startup_profiles=6,startup_independent_checks=120,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,
  actual_startup_heap_root='0x0',actual_startup_allocation_request=80752,next_dependency='Original initialization caller 8c010180 and its preceding services must execute before allocator-dependent startup; do not inject a heap root or allocation result.',
  dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Original allocation, initialization and release/coalescing qualify in complete-state native comparisons. Existing allocation thunk gains native integration without duplicate credit. Original staged boot executes allocation and stops on its actual null heap; original earlier initialization caller remains exported only.',
  limitations=['Direct well-formed circular lists and pool/size/payload/SR/stack inputs are explicit fixtures, not startup heap initialization. Corrupted cycles that cannot return to the cursor are outside these fixtures.',
              'All guest CPU/RAM/FPU/banked/special state and ordered software-platform effects compare. Failed operations retain actual earlier instruction effects; no whole-call rollback or CPU exception-vector delivery is asserted.',
              'Four -4 direct cases are privilege faults; eleven -3 direct cases are invalid memory/stack faults. No result or successful allocation is fabricated.',
              'The original allocator rounds with 32-bit wrapping arithmetic, including very large requests; tests preserve that behavior rather than adding new allocation validation.',
              'Six boot profiles retain prior driver/context/runtime fixtures. Their missing earlier heap setup is recorded, not bypassed. No parent startup or original caller receives recovery credit.',
              'No natural complete boot, pixels, playable port, actual FPS/resolutions/settings or completed platform target is established.'])
 (P/'analysis/movie_heap_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified three heap bodies: 178 rows / 22 outcomes; thunk reused; actual null-heap startup fault retained')
if __name__=='__main__':main()
