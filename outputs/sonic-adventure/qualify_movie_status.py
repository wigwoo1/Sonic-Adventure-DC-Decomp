"""Qualify native sampled-port integration without recounting recovered bodies."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 paths=[P/f'analysis/{name}.json' for name in ('movie_status_verification','movie_status_backend_verification','movie_status_prefix_verification')]
 direct,backend,prefix=[json.loads(p.read_text()) for p in paths]
 assert direct['cases_passed']==118 and direct['status_counts']=={'0':112,'-2':4,'-3':2} and direct['independent_checks']==1568
 assert backend['sampled_words_passed']==65536 and backend['status_values_checked']==4 and backend['backend_checks']==24
 assert prefix['cases_passed']==18 and prefix['independent_checks']==216 and prefix['status_counts']=={'-2':16,'-4':2}
 assert prefix['next_dependencies']==['0x8c602612','0x8c604594'] and prefix['boot_rows_covered']==33
 assert len(direct['coverage_by_routine'])==2 and sum(r['static_rows'] for r in direct['coverage_by_routine'])==15
 for row in direct['coverage_by_routine']:assert row['static_rows']==row['covered_rows'] and not row['missing_rows']
 files=list(paths)
 for proof in (direct,backend,prefix):
  assert proof['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_status.dll')
  for path,h in proof['sha256'].items():assert digest(path)==h,path
  files.extend(Path(path) for path in proof['sha256'])
 for case in prefix['cases']:
  elapsed=case['input']['clock_profile']=='elapsed'
  assert case['stop_pc']==('0x8c604594' if elapsed else '0x8c602612')
  assert case['loaded_movie_sha256']=='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=[]
 for address in ('0x8c10d914','0x8c10d926'):
  old=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address)
  canonical.append({k:old[k] for k in ('address','name','source','verification')})
  files.extend((P/old['source'],P/old['verification']))
 files.append(Path(__file__))
 report=dict(routines=direct['routines'],canonical_registrations=canonical,newly_registered_entries=0,qualified_rows=15,qualified_conditional_outcomes=0,direct_comparisons=118,independent_direct_checks=1568,exhaustive_sampled_words=65536,backend_checks=24,boot_prefix_comparisons=18,boot_prefix_independent_checks=216,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,movie_startup_recovery_credit=0,next_dependencies=prefix['next_dependencies'],dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=direct['limitations']+prefix['limitations']+['Both original query bodies were already registered. This milestone adds complete-state native integration and an explicit sampled-port host interface, with zero new routine credit.','Sixteen elapsed-clock profiles reach the original PVR transition wait at 8c604594; two finite-clock controls retain input exhaustion at 8c602612. No natural boot or complete MOVIE startup is established.'])
 (P/'analysis/movie_status_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified 15 integrated rows, zero new registrations; 118 direct / 18 continuous boot cases')

if __name__=='__main__':main()
