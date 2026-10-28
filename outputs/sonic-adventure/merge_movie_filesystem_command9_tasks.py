"""Merge independently executed original relocation task shards with current pins."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    paths=[P/f'analysis/movie_filesystem_command9_tasks_{i}_verification.json' for i in range(3)];reports=[json.loads(p.read_text()) for p in paths];pins={}
    for report in reports:
        for p,h in report['sha256'].items():assert digest(p)==h,p;pins[p]=h
    cases=[c for r in reports for c in r['cases']];expected=['move_a','move_b','no_data','no_high_free','read_abort','write_abort','dir_abort','fat_abort','has_CC','bad_link'];assert len(cases)==10 and {c['configuration']['kind'] for c in cases}==set(expected)
    coverage=[]
    for first in reports[0]['coverage_by_routine']:
        cvs=[next(c for c in r['coverage_by_routine'] if c['address']==first['address']) for r in reports];missing=set.intersection(*(set(c['missing_rows']) for c in cvs));edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
        coverage.append(dict(address=first['address'],static_rows=first['static_rows'],executed_rows=first['static_rows']-len(missing),executed_outcomes=first['executed_outcomes']+len(first['missing_outcomes'])-len(edges),missing_rows=sorted(missing),missing_outcomes=sorted(edges)))
    for p in paths+[Path(__file__)]:pins[str(p)]=digest(p)
    report=dict(reports[0],original_task_sessions=10,original_native_comparisons=sum(r['original_native_comparisons'] for r in reports),independent_checks=sum(r['independent_checks'] for r in reports),cases=sorted(cases,key=lambda c:expected.index(c['configuration']['kind'])),coverage_by_routine=coverage,newly_observed_nonlocal_parent_routes=sorted({pc for r in reports for pc in r['newly_observed_nonlocal_parent_routes']}),independent_process_shards=3,sha256=pins)
    (P/'analysis/movie_filesystem_command9_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND9 TASK MERGED',report['original_native_comparisons'],report['independent_checks'],flush=True)
if __name__=='__main__':main()
