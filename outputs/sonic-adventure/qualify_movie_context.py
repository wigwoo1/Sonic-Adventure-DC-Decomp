"""Qualify original initialization integration without duplicate routine credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_context_verification','movie_context_backend_verification','movie_context_sequence_verification','movie_context_prefix_verification','movie_context_frontier')];direct,backend,sequence,prefix,frontier=[json.loads(p.read_text()) for p in paths]
 assert direct['cases_passed']==70 and direct['status_counts']=={'0':60,'-2':6,'-3':4} and direct['independent_checks']==514
 assert backend['register_word_cases']==144 and backend['register_alias_reads']==432 and backend['backend_checks']==28
 assert sequence['cases_passed']==3 and sequence['independent_checks']==12 and sequence['status_counts']=={'-2':2,'-3':1}
 assert prefix['cases_passed']==6 and prefix['independent_checks']==120 and prefix['status_counts']=={'-2':4,'-3':2} and prefix['next_dependencies']==['0x8c10d4bc','0x8c6526bc']
 for case in prefix['cases']:
  initialized=case['input']['initialize_context'];assert case['stop_pc']==('0x8c6526bc' if initialized else '0x8c10d4bc') and case['initialized_context_root_at_boot']==('0x8c7ac7a0' if initialized else '0x0')
 assert sum(r['static_rows'] for r in direct['coverage_by_routine'])==312 and sum(r['conditional_edges_covered'] for r in direct['coverage_by_routine'])==2
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in direct['coverage_by_routine'])
 files=list(paths)
 for proof in (direct,backend,sequence,prefix,frontier):
  if 'dll_sha256' in proof:assert proof['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_context.dll')
  for path,h in proof['sha256'].items():assert digest(path)==h,path
  files.extend(Path(path) for path in proof['sha256'])
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];addresses={'0x8c10d5a8','0x8c10ed54','0x8c10ee64'};canonical=[]
 for row in registry:
  if row.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and row['address'] in addresses:
   canonical.append({k:row[k] for k in ('address','name','source','verification')});files.extend((P/row['source'],P/row['verification']))
 assert len(canonical)==4 and len({r['address'] for r in canonical})==3;files.append(Path(__file__))
 report=dict(routines=direct['routines'],canonical_registrations=canonical,newly_registered_entries=0,integrated_unique_bodies=3,integrated_canonical_entries=4,integrated_rows=312,integrated_conditional_outcomes=2,direct_comparisons=70,independent_direct_checks=514,backend_register_cases=144,backend_alias_reads=432,backend_checks=28,failed_initialization_cases=3,failed_initialization_checks=12,startup_profiles=6,startup_independent_checks=120,initialized_startup_profiles=4,cold_controls=2,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependency='8c6526bc renderer configuration; cold controls retain missing-context read 8c10d4bc.',native_initialized_entry='sa_movie_context_initialized_connected',dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=direct['limitations']+prefix['limitations']+['Three unique original bodies already have four canonical registrations, including two historical names at 8c10ee64. All are preserved; this integration adds zero routine credit.','The exported native entry calls original initialization and then the requested next entry with state retained. It establishes a declared native two-stage sequence, not the original reset/startup invocation order.'])
 (P/'analysis/movie_context_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified 312 integrated rows / 2 outcomes; four initialized startup profiles; zero new routine credit')
if __name__=='__main__':main()
