"""Qualify complete platform bodies while keeping the real caller partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_startup_translation','movie_startup_connection','movie_startup_verification','startup_platform_bus_verification','movie_startup_prefix_verification','movie_startup_frontier','movie_heap_qualification')]
 translation,connection,direct,bus,prefix,frontier,prior=[json.loads(p.read_text()) for p in paths];files=list(paths)
 for proof in (translation,connection,direct,bus,prefix,frontier,prior):
  for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 assert direct['dll_sha256']==bus['dll_sha256']==prefix['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_startup.dll')
 assert direct['cases_passed']==60 and direct['status_counts']=={'0':44,'-2':8,'-4':5,'-3':3} and direct['independent_checks']==136
 assert bus['successful_writes']==4388 and bus['successful_reads']==12122 and bus['new_word_slots']==1289 and bus['invalid_access_checks']==64
 assert bus['old_route_cases']==4 and bus['old_route_checks']==11 and bus['trace_checks']==2 and bus['trace_dropped']==12446 and bus['retained_clock_routing_checks']==5
 assert prefix['cases_passed']==6 and prefix['independent_checks']==120 and prefix['status_counts']=={'-3':6} and prefix['next_dependencies']==['0x8c10d4bc','0x8c6021fe']
 assert all(c['heap_root_at_stop']=='0x0' for c in prefix['cases'])
 selected={'0x8c6024d0','0x8c602570','0x8c645de0','0x8c6025e8','0x8c5fc81c','0x8c5fc848','0x8c5fc06c'}
 rows=direct['coverage_by_routine'];parent=next(r for r in rows if r['address']=='0x8c010180');assert parent['static_rows']==109 and parent['covered_rows']==40 and len(parent['missing_edges'])==10
 for row in rows:
  if row['address']!='0x8c010180':assert not row['missing_rows'] and not row['missing_edges'],row
 assert sum(r['static_rows'] for r in rows)==551 and sum(r['covered_rows'] for r in rows)==482
 assert sum(r['static_rows'] for r in rows if r['address'] in selected)==337
 assert sum(r['conditional_edges_covered'] for r in rows if r['address'] in selected)==26
 callers=[c for c in direct['cases'] if c['input']['entry']==0x8c010180 and c['status']==-2];assert len(callers)==2
 for c in callers:
  assert c['stop_pc']=='0x8c604702' and c['heap_root_at_stop']=='0x8c88ef1c'
  targets=[b['target'] for b in c['boundaries']]
  ordered=['0x8c010180','0x8c6024d0','0x8c602570','0x8c645de0','0x8c645f80','0xac5fc06c','0x8c5fc81c','0xac5fc848','0x8c6025e8','0x8c02e720','0x8c602128','0x8c602288','0x8c604702']
  assert targets==ordered,targets
  allocation=next(b for b in c['boundaries'] if b['target']=='0x8c602128');assert allocation['arguments'][:2]==[0x8cf00000,0x100000]
 routines=[dict(r,module='1ST_READ.BIN',source=translation['source'],source_sha256=digest(P/translation['source'])) for r in translation['routines'] if r['address'] in selected]
 assert len(routines)==7
 files.append(Path(__file__))
 report=dict(routines=routines,coverage_by_routine=rows,newly_registered_entries=7,qualified_rows=337,qualified_conditional_outcomes=26,reused_empty_bodies=2,reused_clock_regression_cases=2,
  compiled_source_count=162,unique_physical_rows=450,integrated_static_rows=551,integrated_executed_rows=482,alias_duplicate_credit=0,parent_rows_covered=40,parent_static_rows=109,parent_additional_recovery_credit=0,
  direct_comparisons=60,independent_direct_checks=136,storage_writes=4388,storage_reads=12122,storage_invalid_checks=64,storage_old_route_checks=11,storage_trace_checks=2,retained_clock_routing_checks=5,
  actual_caller_profiles=2,actual_initialized_heap_start='0x8cf00000',actual_initialized_heap_bytes=0x100000,actual_initialized_heap_root='0x8c88ef1c',
  retained_late_boot_profiles=6,retained_late_boot_checks=120,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,
  next_dependency='0x8c604702',dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
  scope='Seven complete original platform/vector/cache bodies qualify. Two original empty bodies and clock leaf are reused. The original early caller runs 40/109 rows, creates its heap by calling real 8c602128 with original computed arguments, then stops at its actual missing driver. No driver result or heap is inserted.',
  limitations=['Original table data, declared VBR/runtime state, software storage, raw clock/scan observations and coherent-RAM cache policies remain explicit fixtures. No physical IRQ acceptance, exception-vector delivery, timer progression, DMA, MMU/cache timing or pixels are established.',
              'System/counter reset writes are stored; guest timer reads retain the existing raw observation stream and do not restart it. A corrected reference adapter and two actual original-clock comparisons verify that routing.',
              'Vector templates have two handler-pointer fields patched by the original routine before copying. Purge bases are the separate original literals VBR+400/1000/1800, not inferred from copy destinations 100/400/600.',
              'P2 aliases move instruction PCs, labels and PC-relative addresses; loaded data stays unchanged, including the cached 8c5fc81c helper pointer. Aliases and the shared return tail receive no duplicate credit.',
              'The canonical earlier caller source/proof remains intact. Its new complete-state connection is partial: 40/109 rows and none of ten conditional outcomes execute before driver setup. No additional parent credit is awarded.',
              'The earlier caller has not completed, so it is not silently chained into retained late-boot tests. Those six controls still have their genuine zero heap and prior null-heap/context faults.',
              'Requested FPS/resolutions/settings, natural complete boot, playable PC/Android/3DS/demake targets and full-game decompilation remain unfinished.'])
 (P/'analysis/movie_startup_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified seven platform bodies: 337 rows / 26 outcomes; real caller initializes heap and reaches driver, remaining partial')
if __name__=='__main__':main()
