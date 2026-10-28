"""Merge complete configuration shards and retain all outstanding original outcomes."""
import collections,json
from pathlib import Path
from verify_collision_contact_direct import configurations,digest,P,v
def main():
 paths=[P/f'analysis/collision_contact_direct_{i}_verification.json' for i in range(3)];reports=[json.loads(p.read_text()) for p in paths];pins={str(Path(__file__)):digest(Path(__file__))}
 for r in reports:
  assert r['dll_sha256']==digest(v.effects.DLL)
  for p,h in r['sha256'].items():assert digest(p)==h,p;pins[p]=h
 cases=[c for r in reports for c in r['cases']];expected=[dict(c,case_id=i) for i,c in enumerate(configurations())];assert len(cases)==len(expected)==199
 assert sorted(json.dumps(c['configuration'],sort_keys=True) for c in cases)==sorted(json.dumps(c,sort_keys=True) for c in expected)
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in cases)
 counts=collections.Counter()
 for r in reports:counts.update(r['status_counts'])
 pins.update({str(p):digest(p) for p in paths})
 sums={k:sum(r[k] for r in reports) for k in ('independent_checks','independent_contact_oracle_cases')}
 coverage=[]
 for body in v.ANALYSIS_ROUTINES:
  cs=[next(c for c in r['coverage_by_routine'] if c['address']==body['address']) for r in reports]
  missing=set.intersection(*(set(c['missing_rows']) for c in cs));outcomes=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cs))
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
 out=dict(reports[0],original_native_comparisons=len(cases),cases=sorted(cases,key=lambda c:c['configuration']['case_id']),status_counts=dict(counts),sha256=pins,independent_process_shards=3,coverage_by_routine=coverage,**sums)
 assert out['independent_checks']==sum(c['independent_checks'] for c in cases)==956
 assert coverage[0]['executed_rows']==1121 and coverage[0]['executed_outcomes']==71 and not coverage[0]['missing_rows']
 (P/'analysis/collision_contact_direct_verification.json').write_text(json.dumps(out,indent=2)+'\n');print('CONTACT MERGED',len(cases),sums,dict(counts),coverage)
if __name__=='__main__':main()
