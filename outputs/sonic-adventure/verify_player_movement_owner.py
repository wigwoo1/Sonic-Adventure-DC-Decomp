"""Normal full072b44 entry comparisons;retain genuine software frontiers and gaps."""
import collections,hashlib,itertools,json
from pathlib import Path
import player_movement_owner_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
    for n,(flags,coll,velocity) in enumerate(itertools.product(range(8),(0,2,0x88,0x8a),((0,0,0),(2,0,0),(1,2,3)))):
        yield dict(flags=flags,collision_flags=coll,velocity=velocity,state_speed=(0,.5,1,2,20)[n%5],lift=0 if n%2 else -1,
                   mode=4*(n%2),rm=(n//2)%2,major=9 if n%3==0 else 0,minor=2 if n%3==0 else 0,
                   other_kind=12 if n%2 else 5,previous_surface_flags=n,fourth_argument=n%2,
                   angles=(0,0x1000,0) if flags&2 else (0,0,0))
def main():
    ram=q.v.seed_ram();records=[];seen=set();edges=set();checks=0;counts=collections.Counter()
    for n,c in enumerate(configurations()):
        m,s,f,cfg=q.prepare(ram,c);before=type(s).from_buffer_copy(s)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        assert result['full_state_match'] and result['ordered_boundaries_match'];checks+=2
        assert result['status'] in (0,-1,-2,-3,-4);checks+=1
        if result['status']==0:
            assert s.pc==s.pr==before.pr and s.r[15]==before.r[15];checks+=1
            assert list(s.r[8:15])==list(before.r[8:15]);checks+=1
        calls=[b for b in result['boundaries'] if b['target']=='0x8c0730d8']
        for b in calls:
            assert b['arguments'][:3]==[q.A,q.B,q.C];checks+=1
            assert int(b['stack'],16)==before.r[15]-0x4c;checks+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        counts[result['status']]+=1
        records.append(dict(input=c,status=result['status'],stop_pc=result['stop_pc'],return_value=s.r[0],steps=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if (n+1)%16==0:print('WHOLE MOVEMENT OWNER',n+1,dict(counts),flush=True)
    coverage=[]
    for prefix in ('player_movement_owner','player_movement_dependencies','player_movement_route'):
        t=json.loads((P/f'analysis/{prefix}_translation.json').read_text())
        for body in t['routines']:
            wanted=set(body['reachable_addresses']);outcomes={tuple(x) for x in body['conditional_edges']}
            missing=wanted-{hex(a) for a in seen};missed=outcomes-edges
            coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted)-len(missing),static_outcomes=len(outcomes),executed_outcomes=len(outcomes)-len(missed),missing_rows=sorted(missing),missing_outcomes=[list(x) for x in sorted(missed)]))
    files=[Path(__file__),P/'player_movement_owner_fixtures.py',P/'player_movement_path_reference.py',P/'write_watch_player_movement_path.py',P/'analysis/player_movement_owner_translation.json',P/'analysis/player_movement_dependencies_translation.json',P/'analysis/player_movement_route_translation.json',P/'analysis/player_movement_path_connection.json',q.v.effects.DLL]
    report=dict(status='partial_whole_movement_owner_software_verification;not_registered',original_native_comparisons=len(records),checks=checks,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,
                new_original_routine_credit=0,owner_registered=False,routing_body_registered=False,caller_allocation_alias_contract_globally_proven=False,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
                scope='Full original072b44 entry with actual0730d8 and its reached real callees;authored actor/state/collision/world inputs. No supplied callee outputs or repaired frames. Boundary checks confirm originalR4/R5/R6 forwarding and actual0x4c caller frame for declared narrow transfer modes. Real numeric/mapping/missing-target frontiers are retained and are not normal completions. Incomplete body/branch coverage gives no owner or routing recovery credit. This does not prove retail stack provenance or universal callback/object alias contracts.',
                sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_movement_owner_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
    print('WHOLE OWNER PARTIAL PASS',len(records),checks,dict(counts),[(x['address'],x['executed_rows'],x['executed_outcomes']) for x in coverage],flush=True)
if __name__=='__main__':main()
