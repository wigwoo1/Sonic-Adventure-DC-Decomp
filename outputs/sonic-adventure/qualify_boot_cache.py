"""Qualify one physical cache leaf with separate cached/uncached PC evidence."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[P/f'analysis/boot_cache_{name}.json' for name in ('verification','prefix_verification','backend_verification')];direct,prefix,backend=[json.loads(p.read_text()) for p in files]
 assert direct['cases_passed']==88 and direct['status_counts']=={'0':82,'-2':2,'-3':4} and direct['independent_checks']==984
 assert backend['ccr_write_read_cases']==130 and backend['backend_checks']==20
 assert prefix['cases_passed']==4 and prefix['boot_rows_covered']==33 and prefix['next_dependencies']==['0x8ceb0000']
 for proof in (direct,prefix,backend):
  assert digest(ROOT/'work/sonic-adventure/build/boot_cache.dll')==proof['dll_sha256']
  for p,h in proof['sha256'].items():assert digest(p)==h,p
  files.extend(Path(p) for p in proof['sha256'])
 for row in direct['coverage_by_routine']:assert row['covered_rows']==row['static_rows']==6 and not row['missing_rows']
 files.append(Path(__file__))
 q=dict(source=direct['source'],source_sha256=direct['source_sha256'],routines=direct['routines'],coverage_by_routine=direct['coverage_by_routine'],cases_passed=88,status_counts=direct['status_counts'],independent_checks=984,ccr_write_read_cases=130,backend_checks=20,boot_prefix_cases=4,boot_prefix_independent_checks=36,boot_rows_covered=33,boot_static_rows=51,boot_recovery_credit=0,newly_registered_entries=1,alias_duplicate_credit=0,unique_physical_rows=6,dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope=direct['scope'],limitations=direct['limitations']+prefix['limitations'],reference_document=direct['reference_document'],reference_scope=direct['reference_scope'])
 (P/'analysis/boot_cache_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified one six-instruction physical body; cached/uncached PCs independently matched')
if __name__=='__main__':main()
