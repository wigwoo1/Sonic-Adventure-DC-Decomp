"""Whole-state original/native comparison and independent finite geometry oracles."""
import collections,ctypes,hashlib,itertools,json,math,sys
from pathlib import Path
import movement_projection_response_fixtures as q
P=q.P;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cross(a,b):return (a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0])
def dot(a,b):return sum(x*y for x,y in zip(a,b))
def read_vector(m,a):return tuple(q.number(q.get(m,a+4*j)) for j in range(3))
def configurations():
    # Nearly parallel initial geometry; no supplied child results or frames.
    # This finite binary32 input reaches original075eb8's negative-dot branch.
    original={'entry': 2349292096, 'normal': [-0.041263483464717865, -0.42914023995399475, -0.9022948741912842], 'motion': [-0.08252696692943573, -0.8582804799079895, -1.8045897483825684], 'velocity': [0.1237904503941536, 1.2874207198619843, 2.7068846225738525], 'delta': [0.04126349464058876, 0.4291402995586395, 0.9022950530052185], 'contacts': [{'normal': [0.041263483464717865, 0.42914023995399475, 0.9022948741912842]}]}
    for mode,rm,view in itertools.product((0,4),(0,1),(0x0c000000,0x8c000000,0xac000000)):
        yield dict(original,kind='rounding_direction',mode=mode,rm=rm,view=view)

def main():
    legacy_source=P/'source/sa_short_absolute_difference_8c055d5a.c';legacy_proof=P/'analysis/short_absolute_difference_8c055d5a_verification.json'
    assert digest(legacy_source)==json.loads(legacy_proof.read_text())['source_sha256']
    legacy_dll=ROOT/'work/sonic-adventure/build/short_absolute_difference_8c055d5a.dll';library=ctypes.CDLL(str(legacy_dll));legacy=library.sa_short_absolute_difference_8c055d5a
    ram=q.v.seed_ram();records=[];checks=old_cases=oracles=0;seen=set();edges=set();counts=collections.Counter()
    selected=list(configurations());case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
    if case_id is not None:selected=[selected[case_id]]
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for n,c in enumerate(selected):
        c=dict(c,case_id=n if case_id is None else case_id);m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s);initial_f=bytes(f)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,1,-1,-3),'complete/existing numeric stop/mapped guard')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'full CPU/FPU/RAM/device/queue state and ordered entries')
        require(result['boundaries'][0]['target']==hex(c['entry']),'ordinary original function entry')
        if result['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'normal return and original stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
            if c['entry']==q.ANGLE:
                d=((initial.r[5]&0xffff)-(initial.r[4]&0xffff))&0xffff
                require(s.r[0]==min(d,0x10000-d),'independent circular short distance, including signed boundary')
                require(bytes(f)==initial_f and m==before,'integer-only leaf preserves RAM and complete FPU')
                old=type(initial).from_buffer_copy(initial);old_ram=bytearray(before);view=(ctypes.c_uint8*len(old_ram)).from_buffer(old_ram)
                legacy.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(type(old))];legacy.restype=None;legacy(view,ctypes.byref(old))
                require(bytes(old)==bytes(s) and old_ram==before,'preserved standalone implementation agrees with current ABI');old_cases+=1;oracles+=1
            elif c['entry']==q.INVERSE:
                value=c['value']
                if value:
                    require(q.number(f.banks[(f.fpscr>>21)&1][0])==math.copysign(1/math.sqrt(abs(value)),value),'independent exact signed reciprocal square root')
                    oracles+=1
                require(m==before,'reciprocal square root does not write memory')
            elif c['entry']==q.PARALLEL and c.get('mode',0) in (0,4):
                normal_a=read_vector(before,q.A+12);normal_b=read_vector(before,q.B+12)
                require(s.r[0]==int(all(x==0 for x in cross(normal_a,normal_b))),'independent cross-normal parallel predicate')
                require(m==before or m[:(q.STACK-8)&0xffffff]==before[:(q.STACK-8)&0xffffff],'no writes below saved FPU stack word');oracles+=1
            elif c['entry']==q.DISTANCE and c.get('mode',0) in (0,4):
                point=read_vector(before,q.A);origin=read_vector(before,q.B);normal=read_vector(before,q.B+12);norm2=dot(normal,normal)
                signed=dot(tuple(x-y for x,y in zip(point,origin)),normal)
                require(q.number(f.banks[(f.fpscr>>21)&1][0])==abs(signed)/math.sqrt(norm2),'independent exact axis-normal point/plane distance')
                if initial.r[6]:require(read_vector(m,q.OUT)==tuple(x-signed*y/norm2 for x,y in zip(point,normal)),'independent orthogonal projection')
                oracles+=1
            elif c['entry']==q.PLANES and c.get('mode',0) in (0,4):
                pa,pb=read_vector(before,q.A),read_vector(before,q.B);a,b=read_vector(before,q.A+12),read_vector(before,q.B+12);direction=cross(a,b)
                if initial.r[6] and any(direction):
                    require(read_vector(m,q.OUT+12)==direction,'independent plane intersection direction')
                    point=read_vector(m,q.OUT)
                    if a==(1,0,0) and b==(0,1,1):
                        # Original6402c8 selects the Y/Z solver even though its
                        # determinant is zero. For the authored input planes,
                        # both numerators are -1 and both denominators +0.
                        require(tuple(q.get(m,q.OUT+4*j) for j in range(3))==(0,0xff800000,0xff800000),'original zero-determinant solver preserves negative infinities')
                    else:require(dot(a,point)==dot(a,pa) and dot(b,point)==dot(b,pb),'independent intersection point satisfies both plane equations')
                    oracles+=1
                elif initial.r[6]:
                    require(read_vector(m,q.OUT)==(0,0,0) and read_vector(m,q.OUT+12)==(0,0,0),'parallel planes clear output record');oracles+=1
            elif c['entry']==q.OWNER:
                angular=[b for b in result['boundaries'] if b['target']==hex(q.ANGLE)]
                if angular:
                    require(len(angular)==1,'one actual final angle-difference call')
                    prior,heading=angular[0]['arguments'][:2]
                    require(prior==q.get(before,q.B+0x20),'angle comparison consumes original prior heading')
                    d=((heading&0xffff)-(prior&0xffff))&0xffff;distance=min(d,0x10000-d)
                    committed=(heading+(0x8000 if distance>0x6000 else 0))&0xffffffff
                    require(q.get(m,q.A+0x18)==q.get(m,q.B+0x20)==committed,'independent circular-distance threshold, half-turn and heading commit')
                    require(s.r[0]==(committed if distance>0x6000 else distance),'original final angle/short-difference return register')
                else:require(s.r[0] in (0,1,2),'original early movement result')
                for address,allowed in ((q.A,{0x18,0x19,0x1a,0x1b}),(q.B,set(range(4,16))|set(range(0x20,0x24))),(q.C,set(range(0x44,0x50))),(q.QUERY,set(range(0x6c,0x78)))):
                    off=address&0xffffff
                    require(all(m[off+j]==before[off+j] for j in range(0x200) if j not in allowed),'independent original input record write footprint')
                off=q.CONTACTS&0xffffff;require(m[off:off+6*0x58]==before[off:off+6*0x58],'contact inputs preserved');oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],result=s.r[0],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('PROJECTION ROUNDING',len(records),dict(counts),flush=True)
    assert 0x8c075ebc in seen,'finite original direction-reversal witness must remain exercised'
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if case_id is not None:print('SINGLE DIAGNOSTIC',records[0]['status'],coverage,flush=True);return
    files=[Path(__file__),P/'movement_projection_response_fixtures.py',P/'movement_projection_response_reference.py',P/'write_watch_movement_projection_response.py',q.v.MP,P/'analysis/movement_projection_response_connection.json',q.v.effects.DLL,legacy_source,legacy_proof,legacy_dll]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,unchanged_standalone_CPU_comparisons=old_cases,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,canonical_duplicate_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original075a40 movement projection and complete64016c/650614/640080plane closure with canonical055d5a ABI reuse. Initial records/geometry/ordinary arguments only;actual game/math callees;full software state and ordered boundary comparisons. Finite exact axis-normal geometry and write-footprint oracles. Existing numeric/guard stops and uncovered branches remain explicit;physical hardware/fullgame unqualified.')
    (P/'analysis/movement_projection_response_rounding_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('PROJECTION ROUNDING PASS',len(records),checks,coverage,flush=True)
if __name__=='__main__':main()
