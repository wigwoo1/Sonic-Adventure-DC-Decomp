"""Complete original collision-response entry, exact output and rejection contracts."""
import collections,hashlib,itertools,json
from pathlib import Path
import player_collision_response_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def configurations():
    rejects=[dict(null_collision_info=True),dict(collision_index=1,collision_count=0),dict(null_provider=True),dict(null_shapes=True),dict(shape_type=6)]
    for c,mode,rm in itertools.product(rejects,(0,4),(0,1)):
        yield dict(c,mode=mode,rm=rm,expected_reject=True)
    for n,(shape,pos,debug) in enumerate(itertools.product(range(6),((0,0,0),(1,0,0),(0,0,1),(1,2,3)),(False,True))):
        yield dict(shape_type=shape,shape_position=pos,debug_enabled=debug,mode=4*(n%2),rm=(n//2)%2)
    for n,(pos,flags,debug) in enumerate(itertools.product(((1,0,0),(-1,0,0),(0,0,1),(0,0,-1)),(0,0x20,0x2000),(False,True))):
        yield dict(shape_type=4|flags,shape_position=pos,debug_enabled=debug,mode=4*(n%2),rm=(n//2)%2)
    for shape,heading in itertools.product((0,1,2,3,4,5),(0x1000,0x4000)):
        yield dict(shape_type=shape,shape_position=(1,2,3),shape_heading=heading,other_angles=(0,heading,0),angles=(0,-heading,0),mode=0,rm=0)
    yield dict(shape_type=1,collision_index=3,collision_count=3,shape_position=(1,0,0),mode=0,rm=0)
    for shape in (0,1,2,3,4,5):
        yield dict(shape_type=shape,shape_position=(1,2,3),output=q.A+0x20,declared_output_alias=True)

def main():
    target=P/'analysis/player_collision_response_verification.json';assert not target.exists()
    ram=q.v.seed_ram();records=[];seen=set();edges=set();counts=collections.Counter();checks=oracles=bounded=0
    def require(ok,message):
        nonlocal checks
        assert ok,message;checks+=1
    for n,c in enumerate(configurations()):
        m,s,f,cfg=q.prepare(ram,c);out=s.r[5]
        if not c.get('declared_output_alias'):q.vector(m,out,(7,11,13))
        before=type(s).from_buffer_copy(s);old=bytes(m[out&0xffffff:(out&0xffffff)+12])
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['full_state_match'] and result['ordered_boundaries_match'],'complete software state and ordered boundaries')
        require(result['status'] in (0,-1),(c,result['status'],result['stop_pc']))
        if result['status']==0:
            require(s.pc==s.pr==before.pr and s.r[15]==before.r[15],'normal return and restored stack')
            require(list(s.r[8:15])==list(before.r[8:15]),'callee saved integer registers')
            require(s.r[0]==(0 if c.get('expected_reject') else 1),'independent success/rejection result');oracles+=1
        else:require(result['stop_pc']=='0x8c63d2d8','retained debug-renderer numeric software frontier')
        actual=tuple(q.number(q.get(m,out+4*j)) for j in range(3))
        if c.get('expected_reject'):
            require(result['status']==0 and bytes(m[out&0xffffff:(out&0xffffff)+12])==old,'rejected input leaves all output bytes unchanged');oracles+=1
        elif not c.get('declared_output_alias') and not c.get('other_angles'):
            pos=c.get('shape_position',(0,0,0));kind=c['shape_type']&15
            if pos==(0,0,0):expected=(0,0,-1) if kind==4 else (0,0,1)
            elif pos in ((1,0,0),(-1,0,0),(0,0,1),(0,0,-1)):
                expected=tuple(-x for x in pos)
            else:expected=None
            if expected is not None:
                if kind==4:
                    # Original matrix rotations may round a cardinal unit value
                    # by a few float ULPs. This mathematical oracle has a declared
                    # absolute bound; original/native state comparison stays exact.
                    require(all(abs(a-b)<=2**-20 for a,b in zip(actual,expected)),('bounded cardinal direction',c,actual,expected));bounded+=1
                else:require(actual==expected,('independent exact axis/fallback direction',c,actual,expected))
                oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        counts[result['status']]+=1
        records.append(dict(input=c,status=result['status'],stop_pc=result['stop_pc'],return_value=s.r[0],output=actual,steps=len(path),full_state_match=True,ordered_boundaries_match=True))
        if (n+1)%16==0:print('COLLISION RESPONSE',n+1,dict(counts),flush=True)
    manifest=P/'analysis/player_movement_dependencies_translation.json';t=json.loads(manifest.read_text());body=next(x for x in t['routines'] if x['address']=='0x8c0344dc')
    wanted=set(body['reachable_addresses']);outcomes={tuple(x) for x in body['conditional_edges']};missing=wanted-{hex(a) for a in seen};missed=outcomes-edges
    require(not missing and not missed,('complete476-row/52-outcome coverage',sorted(missing),sorted(missed)))
    files=[Path(__file__),P/'player_collision_response_fixtures.py',P/'player_response_clip_fixtures.py',P/'player_response_clip_reference.py',P/'write_watch_player_response_clip.py',manifest,P/'analysis/player_response_clip_connection.json',q.v.effects.DLL]
    report=dict(status='complete_original_collision_response_matches_in_declared_software_domain',original_native_comparisons=len(records),checks=checks,independent_semantic_oracles=oracles,independent_exact_oracles=oracles-bounded,independent_bounded_direction_oracles=bounded,cardinal_direction_absolute_bound=2**-20,status_counts=dict(counts),cases=records,
        coverage=dict(address='0x8c0344dc',static_rows=len(wanted),executed_rows=len(wanted),static_outcomes=len(outcomes),executed_outcomes=len(outcomes),missing_rows=[],missing_outcomes=[]),
        new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Complete original476-row/52-outcome child entry with all six shape types,zero fallback/nonzero directions,four type4 direction choices,absolute/rotation-independent flags,rotated shapes,debug on/off,four retained mode/rounding combinations,inclusive index=count,and six explicit output/actor-position aliases. Rejection leaves the caller-owned output unchanged;no initialized result is invented. Type4 cardinal mathematical checks allow absolute2^-20 rotation error;all original/native software-state comparisons remain exact. Actual debug text,matrices and line rendering execute. Matched63d2d8 numerical stops are downstream software limitations. This does not prove arbitrary input safety,physical hardware,caller validity invariants or retail stack provenance.',
        sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('COLLISION COMPLETE',len(records),checks,oracles,dict(counts),flush=True)
if __name__=='__main__':main()
