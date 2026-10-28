"""Merge complete collision-angle shards without hiding coverage gaps."""
import collections,json
from pathlib import Path
from verify_collision_angle_direct import configurations,digest,P,v

def main():
 paths=[P/f'analysis/collision_angle_direct_{i}_verification.json' for i in range(3)];reports=[json.loads(p.read_text()) for p in paths];pins={str(Path(__file__)):digest(Path(__file__))}
 for r in reports:
  assert r['dll_sha256']==digest(v.effects.DLL) and r['existing_numeric_probe_mode']
  for path,h in r['sha256'].items():assert digest(path)==h,path;pins[path]=h
 cases=[c for r in reports for c in r['cases']];expected=[dict(c,case_id=i) for i,c in enumerate(configurations())]
 assert len(cases)==len(expected)==1046 and sorted(json.dumps(c['configuration'],sort_keys=True) for c in cases)==sorted(json.dumps(c,sort_keys=True) for c in expected)
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in cases)
 coverage=[]
 for first in reports[0]['coverage_by_routine']:
  cvs=[next(c for c in r['coverage_by_routine'] if c['address']==first['address']) for r in reports];missing=set.intersection(*(set(c['missing_rows']) for c in cvs));missing_edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs));body=next(b for b in v.ANALYSIS_ROUTINES if b['address']==first['address'])
  coverage.append(dict(address=first['address'],static_rows=first['static_rows'],executed_rows=first['static_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 counts=collections.Counter();targets=collections.Counter()
 for r in reports:counts.update(r['status_counts']);targets.update(r['target_invocations'])
 for p in paths:pins[str(p)]=digest(p)
 result=dict(reports[0],original_native_comparisons=len(cases),independent_checks=sum(r['independent_checks'] for r in reports),finite_sanity_cases=sum(r['finite_sanity_cases'] for r in reports),cases=sorted(cases,key=lambda c:c['configuration']['case_id']),status_counts=dict(counts),target_invocations=dict(targets),coverage_by_routine=coverage,sha256=pins,independent_process_shards=3)
 assert result['independent_checks']==sum(c['independent_checks'] for c in cases)
 (P/'analysis/collision_angle_direct_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('COLLISION ANGLE MERGED',len(cases),result['independent_checks'],dict(counts),coverage,flush=True)
if __name__=='__main__':main()
