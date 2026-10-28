"""Merge both complete normalization-route process shards."""
import collections,json
from pathlib import Path
from verify_collision_normalize_direct import configurations,digest,P,v

def main():
 paths=[P/f'analysis/collision_normalize_direct_{i}_verification.json' for i in range(2)];reports=[json.loads(p.read_text()) for p in paths];pins={str(Path(__file__)):digest(Path(__file__))}
 for r in reports:
  assert r['dll_sha256']==digest(v.effects.DLL)
  for p,h in r['sha256'].items():assert digest(p)==h;pins[p]=h
 cases=[c for r in reports for c in r['cases']];expected=[dict(c,case_id=i) for i,c in enumerate(configurations())]
 assert len(cases)==len(expected)==514 and sorted(json.dumps(c['configuration'],sort_keys=True) for c in cases)==sorted(json.dumps(c,sort_keys=True) for c in expected)
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in cases)
 counts=collections.Counter()
 for r in reports:counts.update(r['status_counts']);assert r['canonical_routine_coverage']['executed_rows']==16 and not r['canonical_routine_coverage']['missing_rows']
 pins.update({str(p):digest(p) for p in paths})
 out=dict(reports[0],original_native_comparisons=len(cases),independent_checks=sum(r['independent_checks'] for r in reports),finite_unit_vector_oracle_cases=sum(r['finite_unit_vector_oracle_cases'] for r in reports),cases=sorted(cases,key=lambda c:c['configuration']['case_id']),status_counts=dict(counts),sha256=pins,independent_process_shards=2)
 assert out['independent_checks']==sum(c['independent_checks'] for c in cases)==4638
 (P/'analysis/collision_normalize_direct_verification.json').write_text(json.dumps(out,indent=2)+'\n');print('NORMALIZER MERGED',len(cases),out['independent_checks'],out['finite_unit_vector_oracle_cases'],dict(counts))
if __name__=='__main__':main()
