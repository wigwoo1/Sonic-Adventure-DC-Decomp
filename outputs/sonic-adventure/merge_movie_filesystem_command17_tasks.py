"""Merge independently completed command17 multiple-name-deletion scenario shards and verify all pinned bytes."""
import json
from pathlib import Path
from movie_filesystem_command17_vectors import P,digest,v
from verify_movie_filesystem_command17_tasks import configurations

def main():
    paths=[P/f'analysis/movie_filesystem_command17_tasks_{i}_verification.json' for i in range(3)];reports=[json.loads(p.read_text()) for p in paths];pins={str(Path(__file__)):digest(Path(__file__))}
    for r in reports:
        assert r['dll_sha256']==digest(v.effects.DLL)
        for path,h in r['sha256'].items():assert digest(path)==h,path;pins[path]=h
    cases=[c for r in reports for c in r['cases']];expected=list(configurations());assert len(cases)==len(expected)==18 and sorted(json.dumps(c['configuration'],sort_keys=True) for c in cases)==sorted(json.dumps(c,sort_keys=True) for c in expected)
    coverage=[]
    for first in reports[0]['coverage_by_routine']:
        cvs=[next(c for c in r['coverage_by_routine'] if c['address']==first['address']) for r in reports];rows=set.intersection(*(set(c['missing_rows']) for c in cvs));edges=set.intersection(*(set(map(tuple,c['missing_outcomes'])) for c in cvs))
        coverage.append(dict(address=first['address'],static_rows=first['static_rows'],executed_rows=first['static_rows']-len(rows),executed_outcomes=first['executed_outcomes']+len(first['missing_outcomes'])-len(edges),missing_rows=sorted(rows),missing_outcomes=sorted(edges)))
    for path in paths:pins[str(path)]=digest(path)
    result=dict(reports[0],original_task_sessions=18,original_native_comparisons=sum(r['original_native_comparisons'] for r in reports),independent_checks=sum(r['independent_checks'] for r in reports),cases=sorted(cases,key=lambda c:expected.index(c['configuration'])),coverage_by_routine=coverage,sha256=pins,independent_process_shards=3)
    (P/'analysis/movie_filesystem_command17_tasks_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('COMMAND17 BATCH DELETE TASKS MERGED',result['original_native_comparisons'],result['independent_checks'])
if __name__=='__main__':main()
