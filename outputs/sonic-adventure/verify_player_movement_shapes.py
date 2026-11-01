"""Whole original point/direction helpers with independent exact input/output checks."""
import hashlib,itertools,json
from pathlib import Path
import player_movement_owner_fixtures as q
P=q.P;OUT=0x8cbf0000
POINTS=(0x8c02fc12,0x8c02fd32,0x8c02fe5c);DIRECTION=0x8c031a84
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ram=q.v.seed_ram();records=[];seen=set();edges=set();checks=oracles=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def compare(entry,c,args):
        m,s,f,cfg=q.prepare(ram,c);cfg['entry']=s.pc=entry
        for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
        before=type(s).from_buffer_copy(s)
        r,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status']==0,(entry,c,r['status'],r['stop_pc']))
        require(r['full_state_match'] and r['ordered_boundaries_match'],'whole software state and ordered boundaries')
        require(s.pc==s.pr==before.pr and s.r[15]==before.r[15],'normal original return and stack')
        require(list(s.r[8:15])==list(before.r[8:15]),'callee saved integer registers')
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        actual=tuple(q.number(q.get(m,OUT+4*j)) for j in range(3))
        records.append(dict(entry=hex(entry),input=c,status=r['status'],steps=len(path),output=actual,boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        return actual
    masks=(0,1,2,4,7)
    for entry,flags,mask,mode,rm in itertools.product(POINTS,(0x20,0x2000,0,0x40),masks,(0,4),(0,1)):
        angles=tuple((0x1000,0x2000,-0x3000)[j] if mask&(1<<j) else 0 for j in range(3))
        c=dict(position=(1,2,3),shape_position=(2,3,4),shape_type=flags,angles=angles,mode=mode,rm=rm)
        actual=compare(entry,c,[q.A,q.SHAPES,OUT])
        if flags&0x20:expected=(2,3,4)
        elif flags&0x2000 or mask==0:expected=(3,5,7)
        else:continue
        require(actual==expected,'independent absolute/translated/zero-angle point result');oracles+=1
        if len(records)%32==0:print('MOVEMENT SHAPES',len(records),'comparisons',flush=True)
    for angle,mode,rm in itertools.product((0,0x4000,-0x2000,0xffff),(0,4),(0,1)):
        actual=compare(DIRECTION,dict(position=(1,2,3),shape_position=(2,3,4),direction_angle=angle,mode=mode,rm=rm),[q.A+0x20,q.SHAPES+8,angle,OUT])
        if angle==0:require(actual==(-1,-1,-1),'independent unrotated point-difference direction');oracles+=1
    t=json.loads((P/'analysis/player_movement_shapes_translation.json').read_text());coverage=[]
    for body in t['routines']:
        if int(body['address'],16) not in (*POINTS,DIRECTION):continue
        wanted=set(body['reachable_addresses']);outcomes={tuple(x) for x in body['conditional_edges']}
        missing=wanted-{hex(a) for a in seen};missed=outcomes-edges
        require(not missing and not missed,(body['address'],'whole body row/outcome coverage',missing,missed))
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted),static_outcomes=len(outcomes),executed_outcomes=len(outcomes),missing_rows=[],missing_outcomes=[]))
    files=[Path(__file__),P/'player_movement_owner_fixtures.py',P/'player_movement_path_reference.py',P/'write_watch_player_movement_path.py',P/'analysis/player_movement_shapes_translation.json',P/'analysis/player_movement_path_translation.json',P/'analysis/player_movement_path_connection.json',q.v.effects.DLL]
    report=dict(status='four_complete_original_shape_helpers_match',original_native_comparisons=len(records),checks=checks,independent_semantic_oracle_cases=oracles,cases=records,coverage_by_routine=coverage,
                supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
                scope='Complete original point conversion and relative direction entries. Four bank/rounding combinations,absolute/translation-only/matrix modes,both rotation orders,and individual/all nonzero Euler axes. Exact independent absolute/translated/zero-angle/difference results. Actual original matrix push/translate/rotate/transform/pop execute. Shared numerical backends are not physical-hardware qualification. Owner/routing/debug wrapper bodies remain separately unqualified.',
                sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_movement_shapes_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
    print('MOVEMENT SHAPES PASS',len(records),checks,oracles,[(x['address'],x['executed_rows'],x['executed_outcomes']) for x in coverage],flush=True)
if __name__=='__main__':main()
