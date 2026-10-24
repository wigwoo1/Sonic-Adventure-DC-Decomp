"""Qualify original display-wait integration and historical mask correction."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_transition_verification','movie_transition_backend_verification','movie_transition_prefix_verification')];direct,backend,prefix=[json.loads(p.read_text()) for p in paths]
 assert direct['cases_passed']==103 and direct['status_counts']=={'0':86,'-4':14,'-2':2,'-3':1} and direct['independent_checks']==945
 assert backend['sampled_words_passed']==5000 and backend['backend_checks']==23 and backend['legacy_outside_field_cases']==23 and backend['legacy_mask_counterexamples']==12
 assert prefix['cases_passed']==10 and prefix['independent_checks']==160 and prefix['status_counts']=={'-2':4,'-4':6}
 assert prefix['next_dependencies']==['0x8c602612','0x8c60459e','0x8c6046c0'] and prefix['boot_rows_covered']==33
 row=direct['coverage_by_routine'][0];assert row['covered_rows']==row['static_rows']==10 and row['conditional_edges_covered']==4 and not row['missing_rows'] and not row['missing_edges']
 files=list(paths)
 for proof in (direct,backend,prefix):
  assert proof['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_transition.dll')
  for path,h in proof['sha256'].items():assert digest(path)==h,path
  files.extend(Path(path) for path in proof['sha256'])
 for case in prefix['cases']:
  profile=case['input']['clock_profile'];assert case['stop_pc']=={'elapsed':'0x8c6046c0','exhausted':'0x8c60459e','finite':'0x8c602612'}[profile]
  assert case['loaded_movie_sha256']=='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c604594');canonical={k:old[k] for k in ('address','name','source','verification')}
 files.extend((Path(__file__),P/old['source'],P/old['verification']))
 report=dict(routines=direct['routines'],canonical_registration=canonical,newly_registered_entries=0,qualified_rows=10,qualified_conditional_outcomes=4,direct_comparisons=103,independent_direct_checks=945,backend_sampled_words=5000,backend_checks=23,legacy_mask_counterexamples=12,original_mask='0x000001ff',legacy_mask='0xff0f01ff',boot_prefix_comparisons=10,boot_prefix_independent_checks=160,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependencies=prefix['next_dependencies'],dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=direct['limitations']+prefix['limitations']+['This original body was already registered. Canonical historical source and proof remain immutable, with the identified mask limitation recorded in new metadata. This milestone adds zero routine credit.','Four advancing-input boot profiles reach 8c6046c0; four exhausted-input cases stop at the original read 8c60459e; two finite-clock controls stop at 8c602612. None establishes full startup.'])
 (P/'analysis/movie_transition_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified original 01ff display wait: 10 rows / 4 outcomes; zero new routine credit')
if __name__=='__main__':main()
