"""Complete software-state comparisons plus independent semantic geometry checks."""
import collections,hashlib,itertools,json,sys
from pathlib import Path
import movement_surface_resolution_fixtures as q
P=q.P
NORMALS=((1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,1),(1,1,0),(-1,1,0),(8,1,0),(1,8,0),(1,.01,0),(0,0,0),(.1,0,0))
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(m,a):return tuple(q.get(m,a+4*j) for j in range(3))
def vector(m,a):return tuple(q.number(x) for x in words(m,a))
def configurations():
    for grounded,normal,speed,flags in itertools.product((0,1),NORMALS,(1,16),(0,0x200000)):
        yield dict(kind='owner_geometry',entry=q.OWNER,grounded=grounded,normal=normal,state_velocity=(speed,0,0),flags=flags)
    for normal,kind,cap,flags,rotate in itertools.product(((8,1,0),(1,1,0),(0,1,0),(-1,1,0)),(0,3),(0,20),(0,0x1000),(0,1)):
        yield dict(kind='owner_air_policy',entry=q.OWNER,grounded=0,normal=normal,character_kind=kind,cap=cap,flags=flags,rotate_global=rotate)
    for grounded in (0,1):
        yield dict(kind='owner_zero',entry=q.OWNER,grounded=grounded,direction=(0,0,0),aux_velocity=(0,0,0))
    for direction,normal,motion in itertools.product(((1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,0)),NORMALS[:10],((0,0,0),(3,0,0),(-3,0,0))):
        yield dict(kind='redirect_geometry',entry=q.REDIRECT,direction=direction,normal=normal,motion=motion)
    for status,low in itertools.product((0,1),(0,20)):
        yield dict(kind='helper_gate',entry=q.REDIRECT,status=status,low=low,state_velocity=(3,4,0))
    for direction in ((1,0,0),(0,1,0)):
        yield dict(kind='helper_parallel',entry=q.REDIRECT,direction=direction,normal=direction)
    for status,flags,index,heading,motion in itertools.product((0,1),(0,0x800000),(0,7),(0,0x7000),((.1,0,0),(3,0,0))):
        yield dict(kind='helper_character_heading',entry=q.REDIRECT,status=status,state_flags=flags,index=index,character_heading=heading,motion=motion)
    yield dict(kind='helper_cardinal',entry=q.REDIRECT)
    for state_present,query_present,view in itertools.product((False,True),(False,True),(0x8c000000,0xac000000,0x0c000000)):
        yield dict(kind='clear_nullable',entry=q.CLEAR,state_present=state_present,query_present=query_present,view=view)
    for entry,mode in itertools.product((q.OWNER,q.REDIRECT,q.CLEAR),(1,2,3,5,6,7)):
        yield dict(kind='raw_numeric_mode',entry=entry,mode=mode)
    for entry,word in itertools.product((q.OWNER,q.REDIRECT),(1,0x00800000,0x7f7fffff,0x7f800000,0x7fc00001,0x7f800001)):
        yield dict(kind='raw_geometry',entry=entry,normal_words=(word,0,0))
    for entry in (q.OWNER,q.REDIRECT,q.CLEAR):yield dict(kind='fpu_disabled',entry=entry,fd=True)
    for entry,bad in ((q.OWNER,'stack'),(q.OWNER,'actor'),(q.OWNER,'state'),(q.OWNER,'query'),(q.OWNER,'target'),(q.OWNER,'offset'),(q.REDIRECT,'actor'),(q.REDIRECT,'state'),(q.REDIRECT,'query'),(q.REDIRECT,'direction'),(q.REDIRECT,'character'),(q.CLEAR,'state'),(q.CLEAR,'query')):
        yield dict(kind='mapped_guard',entry=entry,bad=bad,state_flags=0x800000)

def main():
    ram=q.v.seed_ram();records=[];checks=oracles=0;seen=set();edges=set();counts=collections.Counter()
    selected=[]
    for n,c in enumerate(configurations()):selected.append(dict(c,case_id=n,mode=c.get('mode',4*(n%2)),rm=(n//2)%2))
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
        if result['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'return and balanced stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
            if c['mode'] in (0,4) and c['kind'] not in ('raw_geometry','mapped_guard','fpu_disabled'):
                if c['entry']==q.CLEAR:
                    for present,a,offset in ((c['query_present'],q.Q,4),(c['state_present'],q.C,0x38)):
                        require(words(m,a+offset)==((0,0,0) if present else words(before,a+offset)),'independent nullable velocity clear')
                    require(s.sr==((initial.sr&~1)|int(not c['query_present'])),'clear preserves status except final null-query T flag')
                    oracles+=1
                elif c['entry']==q.OWNER:
                    require(words(m,q.C+0x44)==words(before,q.Q+0x48),'independent normal copy into state')
                    if 0x8c074a94 in path:
                        require(words(m,q.Q+4)==words(before,q.Q+0x60),'query position copied from final anchor')
                        require(vector(m,q.A+0x20)==tuple(a-b for a,b in zip(vector(before,q.Q+0x60),vector(before,q.W))),'independent actor anchor minus caller offset')
                        require(words(m,q.Q+0x10)==words(m,q.Q+0x6c),'output velocity uses actual response delta')
                    else:
                        require(s.r[0]==c.get('prior_return',9),'early return preserves supplied status argument')
                        require(words(m,q.A+0x20)==words(before,q.A+0x20),'early return leaves actor position')
                    oracles+=1
                elif c['kind']=='helper_gate' and c['status']==0 and c['low']==20 or c['kind']=='helper_parallel':
                    for a in (q.A,q.C,q.Q,q.D):
                        off=a&0xffffff;require(m[off:off+0x200]==before[off:off+0x200],'early gate/parallel normals preserve argument objects')
                    oracles+=1
                elif c['kind']=='helper_cardinal':
                    require(vector(m,q.Q+0x6c)==(-3,0,0),'independent perpendicular-plane response opposes incoming direction at preserved speed')
                    oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],return_value=s.r[0],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('SURFACE DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if diagnostic:print('SURFACE DIAGNOSTIC PASS',len(records),checks,coverage,flush=True);return
    files=[Path(__file__),P/'movement_surface_resolution_fixtures.py',P/'movement_surface_resolution_reference.py',P/'write_watch_movement_surface_resolution.py',q.v.MP,P/'analysis/movement_surface_resolution_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original0747d2/0763a0/07256a ordinary-entry full-state software comparisons with actual original length/normalization/cross/angle/plane/matrix dependencies. Authored argument geometry only. Original dropped-draft comparisons retained. Physical hardware/fullgame and uncovered rows/outcomes unqualified.')
    (P/'analysis/movement_surface_resolution_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('SURFACE DIRECT PASS',len(records),checks,oracles,coverage,flush=True)
if __name__=='__main__':main()
