"""Whole response/math bodies, full original/native state and independent finite oracles."""
import ast,collections,hashlib,itertools,json,math,struct
from fractions import Fraction
from pathlib import Path
import movement_response_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def configurations():
    vectors=((0,0,0),(1,0,0),(0,1,0),(0,0,-1),(1,2,3),(.5,-.25,1))
    for entry,left,right,mode in itertools.product((0x8c639b50,0x8c638eac),vectors,vectors,(0,4)):
        yield dict(entry=entry,kind='cross' if entry==0x8c639b50 else 'add',left=left,right=right,mode=mode)
    angles=[((1,0,0),(1,0,0),0),((1,0,0),(-1,0,0),32768),((1,0,0),(0,1,0),16384),((0,1,0),(0,0,1),16384)]
    for (left,right,expected),signed,sign,mode in itertools.product(angles,(False,True),((0,0,1),(0,0,-1)),(0,4)):
        yield dict(entry=0x8c055dac,kind='angle',left=left,right=right,expected=expected,signed=signed,sign=sign,mode=mode)
    for phase,mode in itertools.product((0,16384,32768,49152,65536,-16384,0x12340000),range(8)):
        yield dict(entry=0x8c63a970,kind='cosine',phase=phase,mode=mode)
    for distance,flags,mode in itertools.product((0,.25,.5,2,4),(0,0x800000),(0,4)):
        yield dict(entry=0x8c075416,kind='response_axis',contacts=[dict(point=(distance,0,0))],distance=distance,flags=flags,mode=mode)
    for velocity,limit,flags,mode in itertools.product(((0,0,0),(2,0,0),(8,0,0)),(2,16),(0,0x800000),(0,4)):
        yield dict(entry=0x8c075416,kind='response_limit',motion=(-6,8,0),velocity=velocity,limit=limit,flags=flags,mode=mode)
    for normal,contact_normal,motion,mode in itertools.product(((1,0,0),(-1,0,0),(0,1,0)),((1,0,0),(0,1,0),(0,-1,0)),((0,0,0),(2,0,0),(-2,0,0)),(0,4)):
        yield dict(entry=0x8c075416,kind='response',normal=normal,contacts=[dict(normal=contact_normal)],motion=motion,mode=mode)
    for count,mode in itertools.product((0,2,3,16),(0,4)):
        yield dict(entry=0x8c075416,kind='multiple',contacts=[dict(point=((j+1)*.25,0,0)) for j in range(count)],mode=mode)
    for entry,mode in itertools.product((0x8c075416,0x8c055dac,0x8c639b50,0x8c638eac),range(8)):
        yield dict(entry=entry,kind='raw_mode',mode=mode)
    for entry,word,dn,bank in itertools.product((0x8c055dac,0x8c639b50,0x8c638eac),(0x80000000,1,0x7f800000,0x7fbfffff,0x7fc00001),(0,0x40000),(0,4)):
        yield dict(entry=entry,kind='edge',left_words=[word,q.bits(1),q.bits(2)],fpscr=dn,mode=bank)
    for entry,bad in ((0x8c075416,'stack'),(0x8c075416,'query'),(0x8c055dac,'stack'),(0x8c055dac,'first'),(0x8c639b50,'output'),(0x8c638eac,'second')):
        yield dict(entry=entry,kind='guard',bad=bad)

def main():
    # Extract pure independent rational-root definitions, never execute an old
    # verification script's module-level writers or rewrite historical proofs.
    op=P/'verify_native_sh4_sqrt.py';tree=ast.parse(op.read_text());nodes=[n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name in ('value','oracle')];assert len(nodes)==2
    ns=dict(Fraction=Fraction,math=math,struct=struct);exec(compile(ast.Module(body=nodes,type_ignores=[]),str(op),'exec'),ns);sqrt_oracle=ns['oracle']
    ram=q.v.seed_ram();seen=set();edges=set();records=[];checks=0;oracles=0;counts=collections.Counter()
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for case_id,c in enumerate(configurations()):
        c=dict(c,case_id=case_id);m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
        r,path,a,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status'] in (0,1,-1,-3),'complete or existing numeric/mapped-memory stop;no missing callee')
        require(r['full_state_match'] and r['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/device/queue and ordered boundaries')
        if r['status']==0:
            require(s.pc==s.pr==initial_s.pr and s.r[15]==initial_s.r[15],'ordinary return and stack')
            require(list(s.r[8:15])==list(initial_s.r[8:15]),'preserved integer registers')
        bank=(initial_f.fpscr>>21)&1
        if c['kind'] in ('cross','add'):
            require(r['status']==0,'independent finite vector case completes')
            left=list(map(Fraction,c['left']));right=list(map(Fraction,c['right']))
            expected=[left[j]+right[j] for j in range(3)] if c['kind']=='add' else [left[1]*right[2]-left[2]*right[1],left[2]*right[0]-left[0]*right[2],left[0]*right[1]-left[1]*right[0]]
            out=q.A if c['kind']=='add' else q.OUT
            require([Fraction.from_float(q.number(q.get(a,out+4*j))) for j in range(3)]==expected,'independent exact small-dyadic vector arithmetic')
            wanted=bytearray(before);start=out&0xffffff;wanted[start:start+12]=a[start:start+12]
            require(a==wanted,'only original output footprint changes')
            if c['kind']=='cross':
                square=sum((x*x for x in expected),Fraction());word=q.bits(float(square));require(Fraction.from_float(q.number(word))==square,'chosen squared cross norm exactly representable')
                expected_root,_=sqrt_oracle(word,initial_f.fpscr&3)
                require(f.banks[bank][0]==expected_root,'independent rational squared-bracket cross-length oracle')
            oracles+=1
        elif c['kind']=='angle':
            left,right=c['left'],c['right'];cross=(left[1]*right[2]-left[2]*right[1],left[2]*right[0]-left[0]*right[2],left[0]*right[1]-left[1]*right[0]);expected=c['expected']
            if c['signed'] and sum(x*y for x,y in zip(cross,c['sign']))<0:expected=65536-expected
            require(r['status']==0 and s.r[0]==expected,'independent axis angle and optional orientation sign');oracles+=1
        elif c['kind']=='cosine' and not initial_f.fpscr&0x180000:
            phase=c['phase']&65535;expected={0:1,16384:0,32768:-1,49152:0}[phase]
            require(r['status']==0 and q.number(f.banks[bank][0])==expected,'independent quarter-turn cosine and16bit phase wrap');oracles+=1
        elif c['kind']=='response_axis':
            require(r['status']==0 and s.r[0]==int(2>c['distance']),'independent axis response decision')
            require([q.number(q.get(a,q.QUERY+0x10+4*j)) for j in range(3)]==[-min(2,c['distance']),0,0],'independent exact axis movement limit')
            expected=c.get('velocity',(3,4,0)) if c['flags'] else (0,0,0)
            require([q.number(q.get(a,q.B+4+4*j)) for j in range(3)]==list(expected),'independent head-on velocity clear or caller-flag bypass');oracles+=1
        elif c['kind']=='response_limit':
            expected=c['velocity'][0] if c['flags'] else min(c['velocity'][0],c['limit'])
            require(r['status']==0 and [q.number(q.get(a,q.B+4+4*j)) for j in range(3)]==[expected,0,0],'independent power-of-two velocity magnitude limit');oracles+=1
        elif c['kind']=='multiple':
            require(r['status']==0 and s.r[0]==int(bool(c['contacts'])),'empty or repeated contact decision')
            expected=-.25 if c['contacts'] else -2
            require([q.number(q.get(a,q.QUERY+0x10+j*4)) for j in range(3)]==[expected,0,0],'nearest repeated contact or unchanged empty query');oracles+=1
        if c['kind']=='guard':require(r['status']==-3,'mapped-memory guard reached')
        seen.update(path)
        for j,pc in enumerate(path):
            text=q.v.ROWS.get(pc,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+skip<len(path):edges.add((pc,path[j+skip]))
        counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],result=s.r[0],original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('RESPONSE DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in edge) for edge in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(x),hex(y)] for x,y in sorted(outcomes-edges)]))
    files=[Path(__file__),P/'movement_response_fixtures.py',P/'movement_response_reference.py',P/'write_watch_movement_response.py',q.v.MP,P/'analysis/movement_response_connection.json',q.v.effects.DLL,P/'source/sa_movement_response.c',op]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Whole original response/math bodies and actual distance/acos/trig/cross/normalization callees. Full CPU/FPU/RAM/device/queue andordered boundaries. Authored initial contact/query/state/vector data,not callee outputs orframes. Independent finite cross/add/axisangle/cosine/selected response oracles;other numerics rely onoriginalinstructioncomparison. Successfulgeometricownerprofiles PR0/SZ0;rawleaf modes/selectedguards checked. PhysicalFPU/exception/MMU/gameplayunqualified.')
    (P/'analysis/movement_response_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('RESPONSE DIRECT PASS',len(records),checks,coverage,flush=True)

if __name__=='__main__':main()
