"""Complete software-state comparisons plus independent semantic geometry checks."""
import collections,hashlib,itertools,json,sys
from pathlib import Path
import movement_special_tail_fixtures as q
P=q.P
NORMALS=((1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,1),(1,1,0),(-1,1,0),(8,1,0),(1,8,0),(1,.01,0),(0,0,0),(.1,0,0))
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(m,a):return tuple(q.get(m,a+4*j) for j in range(3))
def vector(m,a):return tuple(q.number(x) for x in words(m,a))
PLANES=(
 ((0,0,0),(0,0,4),(4,0,0)),
 ((0,0,0),(4,0,0),(0,0,4)),
 ((0,0,0),(0,4,0),(0,0,4)),
 ((0,0,0),(0,0,4),(0,4,0)),
 ((0,0,0),(4,0,0),(0,4,0)),
 ((0,0,0),(0,4,0),(4,0,0)))
def scene(vertices=PLANES[0],count=1,flags=0):
    nodes=[dict(object_index=j,low=[dict(key=.5,point=(.5,0,.5),extent=64)],high=[dict(key=-100)]) for j in range(count)]
    return dict(vertices=vertices,static=[dict(id=0,flags=flags) for _ in range(count)],nodes=nodes)
def configurations():
    for velocity,flags,angles,prior in itertools.product(((0,0,0),(1,0,0),(0,-1,0),(1,2,3)),(0,1,2,3,0x100,0x4000),((0,0,0),(0x1000,0x2000,0x3000)),(0,1)):
        yield dict(kind='empty_scene',velocity=velocity,flags=flags,angles=angles,prior_status=prior)
    for plane,velocity,flags,lift in itertools.product(range(6),((0,0,0),(-1,0,0),(1,0,0),(0,-1,0),(0,0,-1)),(0,1,2,3),(0,1,-1)):
        yield dict(kind='triangle_scene',position=(.5,0,.5),velocity=velocity,flags=flags,lift=lift,**scene(PLANES[plane]))
    for count,owner,selected,velocity in itertools.product((2,3),(0,0x8ca70000),(0,0x8cb60000),((0,0,0),(-8,0,0))):
        yield dict(kind='multiple_objects',position=(.5,0,.5),previous_owner=owner,previous_object=selected,velocity=velocity,**scene(PLANES[2],count))
    for height,velocity in itertools.product((0,2,16),((-16,0,0),(0,-16,0),(0,0,-16))):
        plane=tuple(tuple(x*8 for x in p) for p in PLANES[2])
        yield dict(kind='large_geometry',position=(.5,0,.5),height=height,velocity=velocity,**scene(plane))
    for flags,kind,stage in itertools.product((0x40,0x200000,0x200040),(0,6),((0,0),(0,3))):
        yield dict(kind='contact_flags_stage',position=(.5,0,.5),velocity=(-1,0,0),character_kind=kind,major=stage[0],minor=stage[1],**scene(PLANES[2],flags=flags))
    for mode in (1,2,3,5,6,7):yield dict(kind='raw_numeric_mode',mode=mode)
    for mode in (0,4):yield dict(kind='fpu_disabled',mode=mode,fd=True)
    for bad in ('stack','actor','aux','state','query'):yield dict(kind='mapped_guard',bad=bad)


def main():
    ram=q.v.seed_ram();records=[];checks=oracles=0;seen=set();edges=set();counts=collections.Counter()
    selected=[]
    for n,c in enumerate(configurations()):selected.append(dict(c,entry=q.ENTRY,case_id=n,mode=c.get('mode',4*(n%2)),rm=(n//2)%2))
    diagnostic='--case-id' in sys.argv or '--smoke' in sys.argv
    if '--case-id' in sys.argv:selected=[selected[int(sys.argv[sys.argv.index('--case-id')+1])]]
    if '--smoke' in sys.argv:
        first={}
        for c in selected:first.setdefault(c['kind'],c)
        selected=list(first.values())
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for c in selected:
        m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,1,-1,-2,-3),'complete or reached original numeric/mapped stop')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'fullCPU/FPU/RAM/device/queue state and ordered boundaries')
        require(result['boundaries'][0]['target']==hex(c['entry']),'normal original entry')
        if c['kind'] not in ('raw_numeric_mode','fpu_disabled','mapped_guard'):require(result['status']==0,'ordinary authored scene completes through real dependencies')
        if result['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'return and balanced stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
            if c['mode'] in (0,4) and c['kind'] not in ('raw_geometry','mapped_guard','fpu_disabled'):
                require(any(x['target']=='0x8c028bfe' for x in result['boundaries']),'real world producer executes')
                if c['kind']=='empty_scene':
                    require(s.r[0]==0 and q.get(m,q.world.NEAR_COUNT,2)==0,'independent empty scene produces no contacts')
                    expected=tuple(a+b for a,b in zip(c.get('position',(0,0,0)),c['velocity']))
                    require(vector(m,q.A+0x20)==expected,'independent empty-scene actor advance')
                    require(q.get(m,q.C+0x6c)==q.get(m,q.C+0x74)==0,'empty scene clears previous owner andobject')
                    oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],return_value=s.r[0],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('SPECIAL TAIL DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if diagnostic:print('SPECIAL TAIL DIAGNOSTIC PASS',len(records),checks,[(x['address'],x['executed_rows'],x['executed_outcomes'],len(x['missing_rows']),len(x['missing_outcomes'])) for x in coverage],flush=True);return
    files=[Path(__file__),P/'movement_special_tail_fixtures.py',P/'movement_special_tail_reference.py',P/'write_watch_movement_special_tail.py',q.v.MP,P/'analysis/movement_special_tail_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original0777e0 ordinary-entry full-state software comparisons with actual original length/normalization/cross/angle/plane/matrix dependencies. Authored argument geometry only. Original dropped-draft comparisons retained. Physical hardware/fullgame and uncovered rows/outcomes unqualified.')
    (P/'analysis/movement_special_tail_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('SPECIAL TAIL DIRECT PASS',len(records),checks,oracles,[(x['address'],x['executed_rows'],x['executed_outcomes'],len(x['missing_rows']),len(x['missing_outcomes'])) for x in coverage],flush=True)
if __name__=='__main__':main()
