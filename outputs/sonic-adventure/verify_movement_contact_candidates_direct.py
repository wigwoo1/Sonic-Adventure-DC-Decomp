"""Complete software-state comparisons plus independent semantic geometry checks."""
import collections,hashlib,itertools,json,sys
from pathlib import Path
import movement_contact_candidates_fixtures as q
P=q.P
NORMALS=((1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,1),(1,1,0),(-1,1,0),(8,1,0),(1,8,0),(1,.01,0),(0,0,0),(.1,0,0))
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(m,a):return tuple(q.get(m,a+4*j) for j in range(3))
def vector(m,a):return tuple(q.number(x) for x in words(m,a))
def configurations():
    for entry,count,normal,motion in itertools.product((q.CLAMP,q.RESPONSE),(0,1,2),NORMALS,((0,0,0),(-3,0,0),(3,0,0))):
        yield dict(kind='candidate_geometry',entry=entry,count=count,normal=normal,motion=motion)
    for entry,count,points in itertools.product((q.CLAMP,q.RESPONSE),(1,2,16),((1,2),(2,1),(1,1),(4,2))):
        yield dict(kind='candidate_selection',entry=entry,count=count,contacts=[dict(point=(points[j%2],0,0)) for j in range(count)])
    for flags,delta,addition,aux in itertools.product((0,0x4000),((0,0,0),(0,0,2)),((0,0,0),(0,0,-2)),((0,0,0),(-1,0,0))):
        yield dict(kind='response_gates',entry=q.RESPONSE,state_flags=flags,delta=delta,add_delta=addition,aux_velocity=aux)
    for entry,motion,normal in itertools.product((q.CLAMP,q.RESPONSE),((-1,0,0),(-100,0,0)),((.0625,-.5,.863),(0,1,0),(0,-1,0))):
        yield dict(kind='candidate_threshold',entry=entry,motion=motion,normal=normal)
    for delta in ((1,0,0),(-1,0,0),(0,0,2),(0,0,-2)):
        yield dict(kind='response_plane_orientation',entry=q.RESPONSE,delta=delta)
    for entry,mode in itertools.product((q.CLAMP,q.RESPONSE),(1,2,3,5,6,7)):
        yield dict(kind='raw_numeric_mode',entry=entry,mode=mode)
    for entry,word in itertools.product((q.CLAMP,q.RESPONSE),(1,0x00800000,0x7f7fffff,0x7f800000,0x7fc00001,0x7f800001)):
        yield dict(kind='raw_geometry',entry=entry,point_words=(word,0,0))
    for entry in (q.CLAMP,q.RESPONSE):yield dict(kind='fpu_disabled',entry=entry,fd=True)
    for entry,bad in ((q.CLAMP,'stack'),(q.CLAMP,'query'),(q.RESPONSE,'stack'),(q.RESPONSE,'query'),(q.RESPONSE,'state'),(q.RESPONSE,'aux')):
        yield dict(kind='mapped_guard',entry=entry,bad=bad)


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
                if c['entry']==q.CLAMP:
                    for address in (q.B,q.C):
                        off=address&0xffffff;require(m[off:off+0x200]==before[off:off+0x200],'clamp preserves auxiliary/state objects')
                    if 0x8c0757dc not in path and 0x8c0757ea not in path:
                        off=q.Q&0xffffff;require(m[off:off+0x200]==before[off:off+0x200],'unselected clamp preserves query')
                    if c['kind']=='candidate_selection':
                        limit=min(abs(x['point'][0]) for x in c['contacts']);speed=3
                        require(s.r[0]==int(speed>limit),'independent unit-axis minimum-distance clamp status')
                        # Original FSRRA(9) rounds1/3 before multiplying by3.
                        # Toward-zero mode yields exactly0x3f7fffff,not ideal1.
                        from fractions import Fraction
                        third_floor=Fraction(q.number(0x3eaaaaaa));third_ceil=Fraction(q.number(0x3eaaaaab))
                        require(third_floor<Fraction(1,3)<third_ceil and 3*third_floor==Fraction(q.number(0x3f7fffff)),'independent rational rounding derivation')
                        unit=q.number(0x3f7fffff) if c['rm'] else 1.0
                        expected=-unit*limit if speed>limit else -speed
                        require(vector(m,q.Q+0x10)==(expected,0,0),'independent correctly rounded unit-axis nearest-candidate motion clamp')
                    oracles+=1
                else:
                    require(s.r[0]==(12 if 0x8c076344 in path else 0),'original response returns final offset12 on redirect,zero on earlier exits')
                    if 0x8c076154 not in path:
                        for address in (q.B,q.C,q.Q):
                            off=address&0xffffff;require(m[off:off+0x200]==before[off:off+0x200],'early response gates preserve argument objects')
                    else:
                        require(words(m,q.C+0x44)==tuple(q.bits(x) for x in c.get('normal',(1,0,0))),'state normal copies original32bit selected record words')
                        if c['kind']=='response_gates' and c['aux_velocity']==(0,0,0):
                            expected=tuple(a+b for a,b in zip(c['delta'],c['add_delta']))
                            require(vector(m,q.Q+0x6c)==expected,'response adds delta before rejecting zero auxiliary speed')
                            require(words(m,q.B+4)==words(before,q.B+4),'zero auxiliary speed remains unchanged')
                    oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],return_value=s.r[0],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('CONTACT CANDIDATES DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if diagnostic:print('CONTACT CANDIDATES DIAGNOSTIC PASS',len(records),checks,coverage,flush=True);return
    files=[Path(__file__),P/'movement_contact_candidates_fixtures.py',P/'movement_contact_candidates_reference.py',P/'write_watch_movement_contact_candidates.py',q.v.MP,P/'analysis/movement_contact_candidates_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original075686/075f8c ordinary-entry full-state software comparisons with actual original length/normalization/cross/angle/plane/matrix dependencies. Authored argument geometry only. Original dropped-draft comparisons retained. Physical hardware/fullgame and uncovered rows/outcomes unqualified.')
    (P/'analysis/movement_contact_candidates_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('CONTACT CANDIDATES DIRECT PASS',len(records),checks,oracles,coverage,flush=True)
if __name__=='__main__':main()
