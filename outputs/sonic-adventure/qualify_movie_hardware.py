"""Qualify four new bodies and retain eight canonical helper registrations."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_hardware_verification','movie_hardware_edges_verification','movie_hardware_backend_verification','movie_hardware_prefix_verification','movie_hardware_context_frontier')];direct,extra,backend,prefix,frontier=[json.loads(p.read_text()) for p in paths]
 assert direct['cases_passed']==396 and direct['status_counts']=={'0':372,'-4':16,'-2':4,'-3':4} and direct['independent_checks']==2430
 assert extra['cases_passed']==4 and extra['independent_checks']==20 and extra['status_counts']=={'-4':4}
 assert backend['fifo_sampled_words_passed']==5000 and backend['control_subword_cases']==168 and backend['backend_checks']==40
 assert prefix['cases_passed']==4 and prefix['status_counts']=={'-3':4} and prefix['independent_checks']==80 and prefix['next_dependencies']==['0x8c10d4bc']
 for case in prefix['cases']:assert case['transfer_context_at_stop']=='0x0' and case['fault_address']=='0xd4' and case['fifo_cursor']==0 and not case['hardware_events']
 files=list(paths)
 for proof in (direct,extra,backend,prefix,frontier):
  if 'dll_sha256' in proof:assert proof['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_hardware.dll')
  for path,h in proof['sha256'].items():assert digest(path)==h,path
  files.extend(Path(path) for path in proof['sha256'])
 additional_edges={tuple(e) for e in extra['covered_conditional_edges']};additional_rows=set(extra['covered_addresses']);coverage=[]
 for row in direct['coverage_by_routine']:
  row=dict(row);row['missing_rows']=[pc for pc in row['missing_rows'] if pc not in additional_rows];row['covered_rows']=row['static_rows']-len(row['missing_rows']);row['missing_edges']=[e for e in row['missing_edges'] if tuple(e) not in additional_edges]
  routine=next(r for r in direct['routines'] if r['address']==row['address']);row['conditional_edges_covered']=len(routine['conditional_edges'])-len(row['missing_edges']);assert not row['missing_rows'] and not row['missing_edges'];coverage.append(row)
 assert sum(r['static_rows'] for r in coverage)==560 and sum(r['conditional_edges_covered'] for r in coverage)==92
 selected={'0x8c6046c0','0x8c10d460','0x8c10d770','0x8c10ebac'};routines=[]
 for original in direct['routines']:
  if original['address'] in selected:r=dict(original);r.update(module='1ST_READ.BIN',source='source/sa_movie_hardware.c',source_sha256=digest(P/'source/sa_movie_hardware.c'));routines.append(r)
 assert len(routines)==4 and sum(r['instruction_rows'] for r in routines)==400 and sum(len(r['conditional_edges']) for r in routines)==78
 files.append(Path(__file__))
 report=dict(routines=routines,coverage_by_routine=coverage,newly_registered_entries=4,reused_entries=8,qualified_rows=400,qualified_conditional_outcomes=78,integrated_rows=560,integrated_conditional_outcomes=92,direct_comparisons=400,independent_direct_checks=2450,backend_sampled_words=5000,control_subword_cases=168,backend_checks=40,boot_prefix_comparisons=4,boot_prefix_independent_checks=80,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependency='Original context initializer 8c10d5a8; current cold startup preserves null 8c5fbb14 and the reached read fault at 8c10d4bc.',dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=direct['limitations']+prefix['limitations']+['Four counter/stack-overlap cases cover reachable repeated-test branches under explicitly overlapping valid RAM; they do not represent normal initialized driver scheduling.','The four continuous startup cases retain their missing-context fault. Direct initialized-table cases do not establish natural initialization or full MOVIE/boot recovery.'])
 (P/'analysis/movie_hardware_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified four new bodies: 400 rows / 78 outcomes; integrated 560 rows / 92 outcomes')
if __name__=='__main__':main()
