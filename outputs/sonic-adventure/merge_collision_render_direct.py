"""Check complete selected configurations and merge renderer comparison shards."""
import collections,json
from pathlib import Path
from verify_collision_render_direct import configurations,digest,P,v
def main():
 paths=[P/f'analysis/collision_render_direct_{i}_verification.json' for i in range(3)];reports=[json.loads(p.read_text()) for p in paths];pins={str(Path(__file__)):digest(Path(__file__))}
 for r in reports:
  assert r['dll_sha256']==digest(v.effects.DLL)
  for p,h in r['sha256'].items():assert digest(p)==h,p;pins[p]=h
 cases=[c for r in reports for c in r['cases']];expected=[dict(c,case_id=i) for i,c in enumerate(configurations())];assert len(cases)==len(expected)==382
 assert sorted(json.dumps(c['configuration'],sort_keys=True) for c in cases)==sorted(json.dumps(c,sort_keys=True) for c in expected)
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in cases)
 proof=P/'analysis/collision_render_predicate_proof.json';p=json.loads(proof.read_text());assert p['infeasible_outcomes']==[['0x8c63d6e0','0x8c63d6f2']] and not p['domain_excluded_outcomes'];pins[str(proof)]=digest(proof)
 counts=collections.Counter()
 for r in reports:
  counts.update(r['status_counts'])
  assert len(r['coverage_by_routine'])==1
  c=r['coverage_by_routine'][0];assert c['executed_rows']==332 and c['executed_outcomes']==31 and not c['missing_rows'] and c['missing_outcomes']==p['infeasible_outcomes']
 pins.update({str(p):digest(p) for p in paths})
 sums={k:sum(r[k] for r in reports) for k in ('independent_checks','independent_geometry_oracle_cases')}
 out=dict(reports[0],original_native_comparisons=len(cases),cases=sorted(cases,key=lambda c:c['configuration']['case_id']),status_counts=dict(counts),sha256=pins,independent_process_shards=3,**sums)
 assert out['independent_checks']==sum(c['independent_checks'] for c in cases)==4352
 assert out['independent_geometry_oracle_cases']==332 and dict(counts)=={'0':354,'-1':22,'-3':6}
 out['coverage_by_routine']=[dict(out['coverage_by_routine'][0],proven_infeasible_outcomes=p['infeasible_outcomes'],unaccounted_outcomes=[],domain_excluded_rows=[],domain_excluded_outcomes=[])]
 (P/'analysis/collision_render_direct_verification.json').write_text(json.dumps(out,indent=2)+'\n');print('RENDER MERGED382comparisons/4352checks;332rows,31executed+1infeasible outcome',dict(counts))
if __name__=='__main__':main()
