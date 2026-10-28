"""Merge complete canonical matrix-adapter verification shards."""
import collections,json
from pathlib import Path
from verify_collision_inverse_direct import configurations,digest,P,v

def main():
 paths=[P/f'analysis/collision_inverse_direct_{i}_verification.json' for i in range(3)];reports=[json.loads(p.read_text()) for p in paths];pins={str(Path(__file__)):digest(Path(__file__))}
 for r in reports:
  assert r['dll_sha256']==digest(v.effects.DLL)
  for p,h in r['sha256'].items():assert digest(p)==h,p;pins[p]=h
 cases=[c for r in reports for c in r['cases']];expected=[dict(c,case_id=i) for i,c in enumerate(configurations())];assert len(cases)==len(expected)==624
 assert sorted(json.dumps(c['configuration'],sort_keys=True) for c in cases)==sorted(json.dumps(c,sort_keys=True) for c in expected)
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in cases)
 counts=collections.Counter()
 for r in reports:
  counts.update(r['status_counts'])
  assert all(not c['missing_rows'] and not c['missing_outcomes'] for c in r['coverage_by_routine'])
 pins.update({str(p):digest(p) for p in paths})
 sums={k:sum(r[k] for r in reports) for k in ('independent_checks','exact_determinant_oracle_cases','exact_inverse_identity_cases','approximate_inverse_identity_cases','singular_sentinel_oracle_cases')}
 out=dict(reports[0],original_native_comparisons=len(cases),cases=sorted(cases,key=lambda c:c['configuration']['case_id']),status_counts=dict(counts),sha256=pins,independent_process_shards=3,**sums)
 assert out['independent_checks']==sum(c['independent_checks'] for c in cases)==5862
 (P/'analysis/collision_inverse_direct_verification.json').write_text(json.dumps(out,indent=2)+'\n');print('INVERSE MERGED624',sums,dict(counts))
if __name__=='__main__':main()
