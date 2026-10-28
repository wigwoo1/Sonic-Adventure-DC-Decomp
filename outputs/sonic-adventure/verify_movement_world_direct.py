"""Whole original world-selection bodies/callees with complete state comparisons."""
import collections,hashlib,itertools,json
from pathlib import Path
import movement_world_fixtures as fixture
P=fixture.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def configurations():
    for explicit,mode,view in itertools.product((False,True),range(8),(0x8c000000,0xac000000,0x0c000000)):
        yield dict(entry=0x8c63a8e0,kind='identity',explicit=explicit,mode=mode,view=view)
    for mode,point,flags,angles in itertools.product((0,4),((0,0,0),(100,0,0),(0,100,0),(0,0,100)),(0,0x10000000),((0,0,0),(1000,2000,3000))):
        yield dict(entry=0x8c0522c0,kind='grid',mode=mode,point=point,static=[dict(flags=flags,angles=angles)])
    for mode,enabled,present,count,flags,position,radius in itertools.product((0,4),(0,1,2),(0,1),(-1,0,1),(0,1),((0,0,0),(0,0,10)),(0,2)):
        # Pairwise-reduced combinations retain all gate and geometric outcomes.
        if enabled!=1 and (present,count,flags,position,radius)!=(0,-1,0,(0,0,0),0):continue
        if present==0 and (count,flags,position,radius)!=(-1,0,(0,0,0),0):continue
        if count<=0 and (flags,position,radius)!=(0,(0,0,0),0):continue
        yield dict(entry=0x8c0522c0,kind='dynamic',mode=mode,static=[],dynamic=[dict(flags=flags,position=position,radius=radius)],dynamic_enable=enabled,dynamic_present=present,dynamic_count=count)
    for mode in (0,4):
        yield dict(entry=0x8c0522c0,kind='empty',mode=mode,static=[])
        yield dict(entry=0x8c0522c0,kind='static_limit',mode=mode,static=[dict() for _ in range(1024)])
        yield dict(entry=0x8c0522c0,kind='dynamic_limit',mode=mode,static=[],dynamic=[dict() for _ in range(1024)],dynamic_present=0x3f800000)
        yield dict(entry=0x8c0522c0,kind='mixed_order',mode=mode,static=[dict(id=7),dict(id=8,position=(100,0,0)),dict(id=9)],dynamic=[dict(flags=2)])
        for position in ((33,0,0),(32.5,0,0),(32.75,0,0)):
            yield dict(entry=0x8c0522c0,kind='grid_boundary',mode=mode,static=[dict(position=position,radius=2)])
    for mode in (0,4):
        yield dict(entry=0x8c028bfe,kind='no_world',mode=mode)
        yield dict(entry=0x8c028bfe,kind='unmatched',mode=mode,nodes=[dict(object=0x12345678)])
        yield dict(entry=0x8c028bfe,kind='no_near',mode=mode,static=[],nodes=[dict(object=0x12345678)])
        yield dict(entry=0x8c028bfe,kind='scan_near',mode=mode,static=[dict(),dict(),dict()],nodes=[dict(object_index=2,low=[dict(key=0)],high=[dict(key=-100)]),dict(object_index=0,low=[dict(key=0)],high=[dict(key=-100)]),dict(object=0x12345678)])
    # World record inputs deliberately explore list head position, bounds,
    # each Cartesian distance rejection and both forward/backward scans.
    layouts=[
      dict(low=[dict(key=0)],high=[dict(key=-100)]),
      dict(low=[dict(key=-100)],high=[dict(key=-100)]),
      dict(low=[dict(key=100)],high=[dict(key=-100)]),
      dict(low=[dict(key=0),dict(key=.25),dict(key=.5)],high=[dict(key=-100)]),
      dict(low=[dict(key=-2),dict(key=0),dict(key=2)],low_head=1,high=[dict(key=-100)]),
      dict(low=[dict(key=-2),dict(key=0),dict(key=2)],low_head=2,high=[dict(key=-100)]),
      dict(low=[dict(key=-2),dict(key=-1),dict(key=0)],low_head=2,high=[dict(key=-100)]),
      dict(low=[dict(key=-2)],high=[dict(key=0),dict(key=-.25),dict(key=-100)]),
      dict(low=[dict(key=-2)],high=[dict(key=100),dict(key=0),dict(key=-100)]),
      dict(low=[dict(key=-2)],high=[dict(key=-100)]),
      dict(low=[dict(key=-4),dict(key=-3),dict(key=-2)],low_head=2,high=[dict(key=100),dict(key=0),dict(key=-100)]),
      dict(low=[dict(key=0),dict(key=-3),dict(key=-2)],low_head=2,high=[dict(key=100),dict(key=0),dict(key=-100)]),
    ]
    for node,mode,motion in itertools.product(layouts,(0,4),((0,0,0),(0,0,1))):
        yield dict(entry=0x8c028bfe,kind='world',mode=mode,motion=motion,nodes=[node])
    for side,axis,distance,mode in itertools.product(('low','high'),range(3),(0,2),(0,4)):
        point=[0,0,0];point[axis]=distance
        record=dict(key=point[2],point=point)
        node=dict(low=[dict(key=-2)],high=[dict(key=-100)])
        node[side]=[record]+([dict(key=-100)] if side=='high' else [])
        yield dict(entry=0x8c028bfe,kind='distance',mode=mode,nodes=[node])
    for side,debug,mode in itertools.product(('low','high'),(0x80,0),(0,4)):
        node=dict(low=[dict(key=0) for _ in range(96)],low_head=95,high=[dict(key=-100)]) if side=='low' else dict(low=[dict(key=-2)],high=[dict(key=0) for _ in range(96)]+[dict(key=-100)])
        yield dict(entry=0x8c028bfe,kind='world_limit',side=side,debug=debug,mode=mode,nodes=[node])
    for entry,mode in itertools.product((0x8c028bfe,0x8c0522c0),(1,5)):
        yield dict(entry=entry,kind='numeric_stop',mode=mode)
    for entry in (0x8c028bfe,0x8c0522c0):yield dict(entry=entry,kind='guard',bad='stack')
    yield dict(entry=0x8c028bfe,kind='guard',bad='query')
    yield dict(entry=0x8c0522c0,kind='guard',bad='object')
    yield dict(entry=0x8c63a8e0,kind='guard',explicit=True,bad='output')
    yield dict(entry=0x8c028bfe,kind='world_input_guard',nodes=[dict()])

def main():
    ram=fixture.v.seed_ram();seen=set();edges=set();records=[];checks=0;counts=collections.Counter();oracles=0
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for case_id,c in enumerate(configurations()):
        c=dict(c,case_id=case_id);m,s,f,cfg=fixture.prepare(ram,c)
        before=bytes(m);initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
        result,path,actual,s,f=fixture.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,-1,-3),'complete or reached numeric/mapped-memory stop;no missing dependency')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/device/queue and ordered boundaries')
        if c['kind'] not in ('identity','numeric_stop','guard','world_input_guard'):require(result['status']==0,'ordinary authored scene completes')
        if result['status']==0:
            require(s.pc==s.pr==initial_s.pr and s.r[15]==initial_s.r[15],'return and stack')
            require(list(s.r[8:15])==list(initial_s.r[8:15]),'saved integer registers')
        bank=(initial_f.fpscr>>21)&1
        if c['kind']=='identity':
            expected=-1 if not c['explicit'] and initial_f.fpscr&0x80000 else 0
            require(result['status']==expected and f.fpscr==initial_f.fpscr,'reached FSCHG PR1 stop or unchanged completed FPSCR')
            if result['status']==0 and not c['explicit'] and not initial_f.fpscr&0x100000:
                wanted=[fixture.bits(int(j%5==0)) for j in range(16)]
                require(list(f.banks[bank^1])==wanted,'independent inactive matrix identity')
                require(list(f.banks[bank])==list(initial_f.banks[bank]) and actual==before,'active FP bank and whole RAM unchanged');oracles+=1
            elif result['status']==0 and c['explicit'] and not initial_f.fpscr&0x100000:
                require(actual[fixture.OUTPUT&0xffffff:(fixture.OUTPUT&0xffffff)+64]==before[fixture.IDENTITY&0xffffff:(fixture.IDENTITY&0xffffff)+64],'independent explicit identity RAM copy');oracles+=1
        if c['kind']=='empty':require(fixture.get(actual,fixture.NEAR_COUNT,2)==0,'empty scene produces no nearby records');oracles+=1
        if c['kind']=='grid':
            # All authored model centres are zero, so rotations cannot move
            # them. The four query points are well separated from the boundary.
            expected=int(c['point']==(0,0,0))
            require(fixture.get(actual,fixture.NEAR_COUNT,2)==expected,'independent zero-centre static sphere selection');oracles+=1
        if c['kind']=='dynamic':
            expected=int(c['dynamic_enable']==1 and c['dynamic_present']!=0 and c['dynamic_count']>0 and c['dynamic'][0]['flags']&0x400003!=0 and c['dynamic'][0]['position']==(0,0,0))
            require(fixture.get(actual,fixture.NEAR_COUNT,2)==expected,'independent dynamic gate and separated sphere selection');oracles+=1
        if c['kind'] in ('static_limit','dynamic_limit'):
            require(result['status']==0 and fixture.get(actual,fixture.NEAR_COUNT,2)==1024,'actual 1024 candidate early return');oracles+=1
        if c['kind']=='mixed_order':
            require(fixture.get(actual,fixture.NEAR_COUNT,2)==3,'two static and one dynamic candidates')
            require([fixture.get(actual,fixture.NEAR+j*12+8) for j in range(3)]==[9,7,0],'reverse static order then dynamic zero tag');oracles+=1
        if c['kind']=='world_limit':
            require(result['status']==0 and fixture.get(actual,fixture.COUNT)==96 and s.r[0]==96,'actual 96 polygon candidate early return')
            require(fixture.get(actual,fixture.NODES+0x24)==96 and fixture.get(actual,fixture.NODES+0x1c)==fixture.CONTACTS,'node selection points into produced global array')
            require(any(b['target']=='0x8c640922' for b in result['boundaries'])==(c['debug']==0),'actual enabled/disabled debug text call');oracles+=1
        if c['kind']=='numeric_stop':require(result['status']==-1,'PR1 numeric stop')
        if c['kind']=='guard':require(result['status']==-3,'mapped-memory guard reached')
        if c['kind']=='world_input_guard':require(result['status']==-3,'original dereference of missing polygon head reaches mapped-memory guard')
        seen.update(path)
        for j,pc in enumerate(path):
            t=fixture.v.ROWS.get(pc,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+skip<len(path):edges.add((pc,path[j+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],result=s.r[0],near_count=fixture.get(actual,fixture.NEAR_COUNT,2),polygon_count=fixture.get(actual,fixture.COUNT),original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%24==0:print('WORLD DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in fixture.v.ANALYSIS_ROUTINES:
        wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in edge) for edge in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    files=[Path(__file__),P/'movement_world_fixtures.py',P/'movement_world_reference.py',P/'write_watch_movement_world.py',fixture.v.MP,P/'analysis/movement_world_connection.json',fixture.v.effects.DLL,P/'source/sa_movement_world.c']
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(fixture.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Complete original movement world/nearby query bodies and actual math/matrix/rotation/text callees. Authored scene/query/list/sentinel inputs,not naturally initialized game scenes. No supplied callee output,selection array or execution frame. Raw PR/SZ behavior is compared;successful geometric owners limited to PR0/SZ0. Physical hardware and full movement/gameplay unqualified.')
    (P/'analysis/movement_world_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('WORLD DIRECT PASS',len(records),checks,'coverage',coverage,flush=True)

if __name__=='__main__':main()
