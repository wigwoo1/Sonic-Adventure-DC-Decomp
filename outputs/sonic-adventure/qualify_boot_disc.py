"""Qualify the original loader body in the explicit native-disc platform domain."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[P/f'analysis/boot_disc_{name}.json' for name in ('verification','prefix_verification','backend_verification')]
 direct,prefix,backend=[json.loads(p.read_text()) for p in files]
 assert direct['cases_passed']==82 and direct['status_counts']=={'0':78,'-2':4}
 assert direct['independent_checks']==406 and backend['backend_and_api_checks']==34
 assert prefix['cases_passed']==4 and prefix['boot_rows_covered']==30 and prefix['next_dependencies']==['0xac5fc0c2']
 for proof in (direct,prefix,backend):
  assert digest(ROOT/'work/sonic-adventure/build/boot_disc.dll')==proof['dll_sha256']
  for p,h in proof['sha256'].items():assert digest(p)==h,p
  files.extend(Path(p) for p in proof['sha256'])
 manifest=json.loads((P/'analysis/boot_file_translation.json').read_text());routine=next(r for r in manifest['routines'] if r['address']=='0x8c09cc80');row=next(r for r in direct['coverage_by_routine'] if r['address']==routine['address'])
 assert row['covered_rows']==row['static_rows']==50 and row['conditional_edges_covered']==4 and not row['missing_rows'] and not row['missing_edges']
 source=P/manifest['source'];files += [Path(__file__),source,P/'analysis/boot_file_translation.json']
 q=dict(source=manifest['source'],source_sha256=digest(source),routines=[routine],coverage_by_routine=[row],cases_passed=82,status_counts=direct['status_counts'],independent_checks=406,backend_and_api_checks=34,boot_prefix_cases=4,boot_prefix_independent_checks=28,boot_rows_covered=30,boot_static_rows=51,boot_recovery_credit=0,newly_registered_entries=1,qualified_rows=50,qualified_conditional_outcomes=4,dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='Complete 50-row original loader body and all four branch outcomes in tested initialized filesystem/driver states with declared native-disc platform semantics. Successful, missing, zero-byte, rounded and failed/partial reads retain actual game behavior; all state/boundaries/disc ownership/transport events match. This is not BIOS equivalence or complete file-subsystem recovery.',limitations=direct['limitations']+prefix['limitations']+['The original loader ignores its read result and returns zero after closing even when transport fails. Failed/unread payload bytes remain unchanged; zero return must not be advertised as successful I/O.','Status formatter error branches outside the host service policies, driver/read/cancellation parents and cache uncached blocks remain partial; no new credit is assigned to them.'])
 (P/'analysis/boot_disc_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified original loader: 50 rows / 4 outcomes; one new entry')
if __name__=='__main__':main()
