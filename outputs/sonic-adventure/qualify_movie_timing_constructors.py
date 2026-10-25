"""Combine retained renderer evidence and new timing reachability proofs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    paths=[P/f'analysis/{name}.json' for name in ('movie_render_config_qualification','movie_timing_constructors_verification','movie_timing_static_verification')]
    old,direct,static=[json.loads(p.read_text()) for p in paths];files=list(paths)
    for proof in (old,direct,static):
        for path,h in proof['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
    assert old['dll_sha256']==direct['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_render_config.dll')
    assert direct['cases_passed']==134 and direct['status_counts']=={'0':134} and direct['independent_checks']==670
    additional=set(direct['covered_addresses']);edges=set(map(tuple,direct['covered_conditional_edges']))
    closure={r['address']:r for r in static['routines']};coverage=[]
    for previous in old['partial_constructors']:
        row=dict(previous);proof=closure[row['address']]
        dead=set(proof['proven_unreachable_rows']);dead_edges=set(map(tuple,proof['proven_unreachable_edges']))
        unexecuted=set(row['missing_rows'])-additional
        unexplored=set(map(tuple,row['missing_edges']))-edges
        assert unexecuted==dead,(row['address'],unexecuted,dead)
        assert unexplored==dead_edges,(row['address'],unexplored,dead_edges)
        assert not dead&additional and not dead_edges&edges
        row.update(covered_rows=row['static_rows']-len(unexecuted),missing_rows=[],
                   proven_unreachable_rows=sorted(dead),proven_unreachable_edges=[list(e) for e in sorted(dead_edges)],
                   missing_edges=[],conditional_edges_covered=row['conditional_edges_covered']+len(set(map(tuple,previous['missing_edges']))&edges),
                   conditional_outcomes_accounted=row['conditional_outcomes_accounted']+len(previous['missing_edges']))
        coverage.append(row)
    manifest=json.loads((P/'analysis/movie_render_config_translation.json').read_text())
    routines=[]
    for original in manifest['routines']:
        if original['address'] in closure:
            r=dict(original,module='1ST_READ.BIN',source='source/sa_movie_render_config.c',source_sha256=digest(P/'source/sa_movie_render_config.c'));routines.append(r)
    assert len(routines)==3 and sum(r['instruction_rows'] for r in routines)==879
    assert sum(r['covered_rows'] for r in coverage)==867
    assert sum(r['conditional_edges_covered'] for r in coverage)==146
    assert sum(len(r['proven_unreachable_edges']) for r in coverage)==26
    files.append(Path(__file__))
    report=dict(routines=routines,coverage_by_routine=coverage,newly_registered_entries=3,qualified_rows=879,
                qualified_executed_rows=867,qualified_proven_unreachable_rows=12,
                qualified_static_conditional_outcomes=172,qualified_executed_conditional_outcomes=146,
                qualified_proven_unreachable_outcomes=26,new_direct_comparisons=134,new_independent_direct_checks=670,
                retained_direct_comparisons=753,retained_independent_direct_checks=2950,
                family_static_rows=1814,family_executed_rows=1802,family_proven_unreachable_rows=12,
                family_static_conditional_outcomes=282,family_executed_conditional_outcomes=255,family_proven_unreachable_outcomes=27,
                remaining_partial_constructors=0,next_dependency='Integrate existing helper 8c653386 and continue original renderer setup; startup parent remains partial.',
                dll_sha256=direct['dll_sha256'],sha256={str(p):digest(p) for p in files},
                scope=direct['scope'],preconditions=static['preconditions'],
                limitations=['Unreachable rows and outcomes receive proof accounting, never executed-coverage credit.',
                             'Out-of-range pixel index six uses explicitly seeded adjacent scratch RAM to exercise original unchecked lookup/arithmetic, not a supported display setting.',
                             'No native C, original bytes, historical proof or DLL is changed. Existing six startup profiles are retained evidence, not newly rerun or advanced.',
                             'Software timing/register state is not pixels, physical device timing, a playable port, requested FPS/resolutions/settings, or proof of natural reset-to-main execution.'])
    (P/'analysis/movie_timing_constructors_qualification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Three constructors qualify: 867 executed / 12 unreachable rows; 146 executed / 26 unreachable conditional outcomes')
if __name__=='__main__':main()
