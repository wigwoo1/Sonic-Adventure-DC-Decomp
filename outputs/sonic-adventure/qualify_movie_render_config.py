"""Qualify closed renderer bodies and explicitly preserve partial constructors."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_render_config_verification','movie_render_edges_verification','movie_render_static_verification','movie_render_config_prefix_verification','movie_render_frontier')];direct,extra,static,prefix,frontier=[json.loads(p.read_text()) for p in paths]
 assert direct['cases_passed']==673 and direct['status_counts']=={'0':669,'-8':1,'-3':3} and direct['independent_checks']==2698
 assert extra['cases_passed']==80 and extra['status_counts']=={'0':80} and extra['independent_checks']==252
 assert static['constant_r4']==1 and static['abstract_states_explored']==184 and static['proven_unreachable_edges']==[['0x8c6527c6','0x8c6527cc']]
 assert prefix['cases_passed']==6 and prefix['independent_checks']==120 and prefix['status_counts']=={'-2':4,'-3':2} and prefix['next_dependencies']==['0x8c10d4bc','0x8c653386']
 files=list(paths)
 for proof in (direct,extra,static,prefix,frontier):
  if 'dll_sha256' in proof:assert proof['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_render_config.dll')
  for path,h in proof['sha256'].items():assert digest(path)==h,path
  files.extend(Path(path) for path in proof['sha256'])
 additional=set(extra['covered_addresses']);extra_edges={tuple(e) for e in extra['covered_conditional_edges']};proven={tuple(e) for e in static['proven_unreachable_edges']};coverage=[]
 for original in direct['coverage_by_routine']:
  row=dict(original);p=next(r for r in prefix['coverage_by_routine'] if r['address']==row['address']);row['missing_rows']=[pc for pc in row['missing_rows'] if pc in p['missing_rows'] and pc not in additional];row['covered_rows']=row['static_rows']-len(row['missing_rows'])
  missing=[e for e in row['missing_edges'] if e in p['missing_edges'] and tuple(e) not in extra_edges];routine=next(r for r in direct['routines'] if r['address']==row['address']);row['conditional_edges_covered']=len(routine['conditional_edges'])-len(missing);row['proven_unreachable_edges']=[e for e in missing if tuple(e) in proven];row['missing_edges']=[e for e in missing if tuple(e) not in proven];row['conditional_outcomes_accounted']=len(routine['conditional_edges'])-len(row['missing_edges']);coverage.append(row)
 closed={r['address'] for r in coverage if not r['missing_rows'] and not r['missing_edges']};selected={'0x8c6526bc','0x8c6525c0','0x8c65796e','0x8c658ae0','0x8c657c60','0x8c6582e0'};reused={'0x8c665a80','0x8c10d3c8','0x8c10db1c','0x8c6513ee'}
 assert closed==selected|reused and sum(r['static_rows'] for r in coverage)==1814
 routines=[]
 for original in direct['routines']:
  if original['address'] in selected:r=dict(original);r.update(module='1ST_READ.BIN',source='source/sa_movie_render_config.c',source_sha256=digest(P/'source/sa_movie_render_config.c'));routines.append(r)
 assert len(routines)==6 and sum(r['instruction_rows'] for r in routines)==822 and sum(len(r['conditional_edges']) for r in routines)==60
 assert sum(r['conditional_edges_covered'] for r in coverage if r['address'] in selected)==59
 partial=[r for r in coverage if r['address'] not in closed];assert len(partial)==3
 files.append(Path(__file__))
 report=dict(routines=routines,coverage_by_routine=coverage,newly_registered_entries=6,reused_qualified_bodies=4,qualified_rows=822,qualified_static_conditional_outcomes=60,qualified_executed_conditional_outcomes=59,qualified_proven_unreachable_outcomes=1,integrated_static_rows=1814,integrated_tested_rows=sum(r['covered_rows'] for r in coverage),partial_constructors=partial,direct_comparisons=753,independent_direct_checks=2950,startup_profiles=6,startup_independent_checks=120,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependency='Integrate existing five-row helper 8c653386; finish untested paths in timing constructors 8c657e00, 8c6585c0 and 8c658804.',dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=direct['limitations']+static['preconditions']+['One original configuration branch is proven unreachable by conservative original-code dataflow; it is not counted as dynamically executed.','The three larger timing constructors remain partial, despite their tested default/variant paths. They receive no recovery credit.','The test labeled unknown_callback selects the reserved configured firmware target with invalid read arguments; the real host disc service returns -8. This is a retained platform-error case, not an invented callback result.','Pending-word completion edge cases invoke the actual original memory-fill body as an explicitly configured direct callback fixture; natural callback installation is not claimed.','Native register writes establish software state only; no pixels, playable output, requested display settings or completed full renderer are established.'])
 (P/'analysis/movie_render_config_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified six new bodies: 822 rows, 59 executed outcomes and one proven unreachable; three constructors remain partial')
if __name__=='__main__':main()
