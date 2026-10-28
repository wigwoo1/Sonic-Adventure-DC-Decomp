"""Ordinary-input cap rejection and genuine finite-underflow branch witnesses."""
import hashlib,itertools,json,sys
from pathlib import Path
import movement_surface_resolution_fixtures as q
P=q.P

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
    for mode,rm in itertools.product((0,4),(0,1)):
        yield dict(kind='grounded_cap_rejection',entry=q.OWNER,grounded=1,normal=(8,1,0),state_velocity=(1,0,0),cap=0,mode=mode,rm=rm)
    for power,scale,mode,rm in itertools.product((70,80),(16,20,24),(0,4),(0,1)):
        yield dict(kind='finite_direction_underflow',entry=q.REDIRECT,direction=(2.0**-power,0,0),normal=(0,2.0**scale,0),secondary_normal=(0,0,1),mode=mode,rm=rm)

def main():
    ram=q.v.seed_ram();records=[];checks=0;seen=set();edges=set();zero_cases=0
    selected=list(configurations());diagnostic='--case-id' in sys.argv
    if diagnostic:selected=[selected[int(sys.argv[sys.argv.index('--case-id')+1])]]
    for c in selected:
        m,s,f,cfg=q.prepare(ram,c);initial=type(s).from_buffer_copy(s)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        def require(ok,label):
            nonlocal checks
            assert ok,(c,label);checks+=1
        require(result['status'] in (0,1,-1,-3),'complete or documented reached numeric/mapped stop')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'complete original/native state and ordered boundaries')
        if result['status']==0:require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'normal return and stack')
        if c['kind']=='grounded_cap_rejection':
            require(result['status']==0 and s.r[0]==2,'independent cap rejection returns status2')
            require(0x8c0748c4 in path and not any(b['target']==hex(q.REDIRECT) for b in result['boundaries']),'real cap branch executes without redirection')
        case_edges=set()
        for i,a in enumerate(path):
            text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):case_edges.add((a,path[i+skip]))
        hit=(0x8c0764d2,0x8c0764da) in case_edges
        zero_cases+=hit;seen.update(path);edges.update(case_edges)
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],zero_direction_branch_reached=hit,original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if diagnostic:print('SURFACE EDGE DIAGNOSTIC',records[0]['status'],zero_cases,coverage,flush=True);return
    assert (0x8c0748c2,0x8c0748c4) in edges and zero_cases>0
    direct=json.loads((P/'analysis/movement_surface_resolution_direct_verification.json').read_text())
    for row in coverage:
        old=next(x for x in direct['coverage_by_routine'] if x['address']==row['address'])
        assert not set(row['missing_rows'])&set(old['missing_rows'])
        assert not set(map(tuple,row['missing_outcomes']))&set(map(tuple,old['missing_outcomes']))
    files=[Path(__file__),P/'movement_surface_resolution_fixtures.py',P/'movement_surface_resolution_reference.py',P/'write_watch_movement_surface_resolution.py',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,zero_direction_branch_cases=zero_cases,cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All inputs finite and authored at ordinary entry. Small normal-valued direction and large normal-valued plane normal produce nonzero cross product but an original direction norm rounded/flushed to zero. Actual original helpers execute;no supplied math result or fabricated frame. Together with direct suite all647rows/64outcomes execute;no infeasibility shortcut.')
    (P/'analysis/movement_surface_resolution_edges_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('SURFACE EDGES PASS',len(records),checks,'zero-branch witnesses',zero_cases,flush=True)
if __name__=='__main__':main()
