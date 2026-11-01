"""Real response/renderer integration; retain numeric frontiers and coverage gaps."""
import collections,hashlib,itertools,json
from pathlib import Path
import player_response_clip_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_response_clip_path_verification.json';assert not target.exists()
    ram=q.v.seed_ram();seen=set();edges=set();records=[];counts=collections.Counter();checks=0
    configs=[dict(flags=0),dict(flags=1,velocity=(.5,0,0)),dict(flags=1,collision_flags=0),
             dict(flags=5,collision_flags=0x88),dict(flags=1,collision_flags=0x80)]
    for position,normal,velocity,heading in itertools.product(((0,0,0),(1,0,0),(0,0,1)),((0,1,0),(0,0,1)),((2,0,0),(-2,0,0),(0,2,0)),(0,0x8000)):
        configs.append(dict(flags=1,collision_flags=0x88,shape_position=position,old_normal=normal,velocity=velocity,previous_heading=heading))
    configs.extend([dict(flags=1,collision_flags=0x88,velocity=(2,0,0),null_collision_info=True),
                    dict(flags=1,collision_flags=0x88,velocity=(0,2,0),state_alias_aux=True)])
    for position,mode,rm in itertools.product(((0,0,0),(1,0,0)),(0,4),(0,1)):
        configs.append(dict(flags=1,collision_flags=0x88,velocity=(2,0,0),shape_position=position,mode=mode,rm=rm,debug_enabled=True))
    for i,c in enumerate(configs):
        m,s,f,cfg=q.prepare(ram,c);before=type(s).from_buffer_copy(s)
        q.put(m,0x8c752bdc,0 if c.get('debug_enabled') else 0x80,1)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        assert result['full_state_match'] and result['ordered_boundaries_match'];checks+=2
        assert result['status'] in (0,-1);checks+=1
        if c.get('debug_enabled'):assert result['status']==0;checks+=1
        if result['status']==0:
            assert s.pc==s.pr==before.pr and s.r[15]==before.r[15];checks+=1
            assert list(s.r[8:15])==list(before.r[8:15]);checks+=1
        else:
            assert result['stop_pc']=='0x8c639b68';checks+=1
        seen.update(path)
        for j,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+skip<len(path):edges.add((hex(a),hex(path[j+skip])))
        counts[result['status']]+=1
        records.append(dict(input=c,status=result['status'],stop_pc=result['stop_pc'],steps=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if (i+1)%16==0:print('RESPONSE PATH',i+1,dict(counts),flush=True)
    coverage=[]
    for prefix,selected in (('player_movement_dependencies',{'0x8c072946','0x8c0344dc'}),('player_movement_shapes',{'0x8c043a40'}),('player_response_draw',{'0x8c63ce24'}),('player_response_clip',{'0x8c64f5e8'})):
        t=json.loads((P/f'analysis/{prefix}_translation.json').read_text())
        for body in t['routines']:
            if body['address'] not in selected:continue
            wanted=set(body['reachable_addresses']);outcomes={tuple(x) for x in body['conditional_edges']}
            missing=wanted-{hex(a) for a in seen};missed=outcomes-edges
            coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted)-len(missing),static_outcomes=len(outcomes),executed_outcomes=len(outcomes)-len(missed),missing_rows=sorted(missing),missing_outcomes=[list(x) for x in sorted(missed)]))
    files=[Path(__file__),P/'player_response_clip_fixtures.py',P/'player_response_fixtures.py',P/'player_response_clip_reference.py',P/'write_watch_player_response_clip.py',q.v.effects.DLL]
    report=dict(status='response_and_renderer_integration_matches;qualification_separate',original_native_comparisons=len(records),checks=checks,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,
        supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Real072946 response entry with full original callees,including real043a40/63ce24/64f5e8 for eight debug-enabled normal returns. Collision/world/heading inputs are declared. The R6=aux-3c case declares an input alias and lets original stores overwrite velocity components;no private frame is seeded. Null collision information lets the original child return without writing its caller output;that unwritten-local case is observed,not a normal gameplay reachability or initialized-output claim. Matched639b68 numeric stops remain software limitations. Larger child/renderer coverage gaps remain open;no game or hardware qualification.',
        sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('RESPONSE PATH',len(records),checks,dict(counts),[(x['address'],x['executed_rows'],x['executed_outcomes']) for x in coverage],flush=True)
if __name__=='__main__':main()
