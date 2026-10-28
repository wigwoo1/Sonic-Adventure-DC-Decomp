"""Real movement/math dependencies, full original/native state and independent finite oracles."""
import collections,hashlib,itertools,json,math
from fractions import Fraction
from pathlib import Path
import movement_vectors_fixtures as fixture
P=fixture.P


def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def configurations():
    for entry in (0x8c079000,0x8c078eb0):
        if entry==0x8c079000:
            for orientation,roll,bank in itertools.product(((0,1,0),(1,0,0),(0,0,1),(0,0,-1),(.5,.5,.5)),(0,16384,-16384),(0,4)):
                yield dict(entry=entry,kind='rotation',orientation=orientation,roll=roll,mode=bank)
        else:
            for angles,bank in itertools.product(((0,0,0),(16384,0,0),(0,16384,0),(0,0,16384),(-16384,32768,16384)),(0,4)):
                yield dict(entry=entry,kind='rotation',angles=angles,mode=bank)
    vectors=((0,0,0),(1,0,0),(0,-1,0),(.5,-.25,2))
    for left,right,bank in itertools.product(vectors,vectors,(0,4)):
        yield dict(entry=0x8c639090,kind='dot',left=left,right=right,mode=bank)
    for explicit,normalize,bank,matrix,in_place in itertools.product((False,True),(0,1),(0,4),(0,1),(False,True)):
        yield dict(entry=0x8c638f28,kind='direction',explicit=explicit,normalize=normalize,mode=bank,matrix=matrix,in_place=in_place)
    for mode,full in itertools.product(range(8),(False,True)):
        yield dict(entry=0x8c057b40,kind='matrix_init',mode=mode,depth=64 if full else 1,matrix=1)
    # Expected finite response coefficients are independently authored examples,
    # not installed callee values or an instruction-by-instruction second model.
    examples=[
      dict(label='flag_down',flags=0x4000,velocity=(0,-1,0),factor_word=0x3f199999),
      dict(label='flag_up',flags=0x4000,velocity=(0,1,0),factor_word=0x3e4ccccc),
      dict(label='flag_zero',flags=0x4000,velocity=(0,0,0),factor_word=0x3f199999),
      dict(label='override',flags=0x20000,factor=1),
      dict(label='height_unchanged',height=1,unchanged=True),
      dict(label='height_subtract',height=1,status_flags=0x2000,factor=1),
      dict(label='flag_eight',flags=8,factor_word=0x3e999999),
      dict(label='special_low',status_flags=0x2000,normal=(0,1,0),factor_word=0x3fe66666),
      dict(label='special_middle',status_flags=0x2000,normal=(0,0,1),factor_word=0x3fd33333),
      dict(label='special_high',status_flags=0x2000,normal=(0,-.5,1),factor_word=0x3fb33333),
      dict(label='negative_slope',normal=(0,1,0),factor=1),
      dict(label='zero_velocity',normal=(0,0,1),factor=Fraction(3,2)),
      dict(label='horizontal_velocity',normal=(0,0,1),velocity=(1,0,0),factor=Fraction(3,2)),
      dict(label='vertical_velocity',normal=(0,0,1),velocity=(0,1,0),factor_word=0x3e999999),
      dict(label='steep_slow',normal=(0,-1,0),threshold=2,speed=1,factor=Fraction(1,2)),
      dict(label='steep_fast',normal=(0,-1,0),threshold=.5,speed=1,factor=1),
      dict(label='steep_ratio',normal=(0,-1,0),threshold=.5,speed=1,status_flags=1,cap=2,factor=Fraction(1,2)),
      dict(label='cap_bypass',normal=(0,-1,0),threshold=.5,speed=1,status_flags=1,cap=.5,factor=1),
      dict(label='cap_fallthrough',normal=(0,1,0),speed=1,status_flags=0,cap=.5,factor=1),
    ]
    for example,bank,acceleration in itertools.product(examples,(0,4),(1,.25)):
        yield dict(entry=0x8c074cf0,kind='response_oracle',mode=bank,acceleration=acceleration,**example)
    for slope,velocity,status,bank in itertools.product((-1,-.125,-.05,0,.125,.25,.5,1),((0,0,0),(1,0,0),(0,1,0)),(0,0x2000),(0,4)):
        yield dict(entry=0x8c074cf0,kind='response',normal=(0,-slope,1),velocity=velocity,status_flags=status,mode=bank)
    for status,cap,threshold,bank in itertools.product((1,2),(.5,2),(.5,2),(0,4)):
        yield dict(entry=0x8c074cf0,kind='response',normal=(0,-1,0),status_flags=status,cap=cap,threshold=threshold,mode=bank)
    for entry,mode in itertools.product((0x8c079000,0x8c074cf0,0x8c078eb0,0x8c639090,0x8c638f28),(1,5)):
        yield dict(entry=entry,kind='numeric_stop',mode=mode,orientation=(0,1,0))
    for entry in (0x8c079000,0x8c074cf0,0x8c078eb0):
        yield dict(entry=entry,kind='guard',bad='stack')
    for entry in (0x8c079000,0x8c639090,0x8c638f28):
        for mode in (0,2,4,6):yield dict(entry=entry,kind='guard',bad='second',mode=mode)


def finite(word):return Fraction.from_float(fixture.number(word))


def rounded(value):
    """Exact rational nearest binary32, with a host hint checked against neighbours."""
    if not value:return 0
    sign=0x80000000 if value<0 else 0;value=abs(value)
    hint=fixture.bits(float(value))
    choices=[b for b in (hint-1,hint,hint+1) if 0<=b<0x7f800000]
    result=min(choices,key=lambda b:(abs(finite(b)-value),b&1))
    return sign|result


def main():
    ram=fixture.v.seed_ram();seen=set();edges=set();records=[];checks=0;counts=collections.Counter();oracles=0
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for case_id,c in enumerate(configurations()):
        c=dict(c,case_id=case_id)
        # Fraction expectations belong only to the oracle, not fixture setup or execution.
        m,s,f,cfg=fixture.prepare(ram,c);before=bytes(m);initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
        result,path,actual,s,f=fixture.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,-1,-3),'complete or reached existing numeric/mapped-memory stop;no missing dependency')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/device/queue and boundaries')
        bank=(initial_f.fpscr>>21)&1
        if result['status']==0:
            require(s.pc==s.pr==initial_s.pr and s.r[15]==initial_s.r[15],'return/stack')
            require(list(s.r[8:15])==list(initial_s.r[8:15]),'saved integer registers')
            if not initial_f.fpscr&0x180000 and c['entry'] not in (0x8c057b40,0x8c638f28):
                require(list(f.banks[bank][12:])==list(initial_f.banks[bank][12:]),'saved physical FP registers')
        if c['kind']=='dot':
            expected=sum((Fraction(x)*Fraction(y) for x,y in zip(c['left'],c['right'])),Fraction())
            require(result['status']==0 and finite(f.banks[bank][0])==expected,'independent exact finite dot product')
            require(actual==before,'dot product preserves whole RAM');oracles+=1
        elif c['kind']=='response_oracle':
            factor=finite(c['factor_word']) if 'factor_word' in c else Fraction(c.get('factor',0))
            amount=finite(rounded(finite(fixture.bits(c['acceleration']))*factor))
            expected=finite(fixture.get(before,fixture.OUT+4)) if c.get('unchanged') else finite(rounded(finite(fixture.get(before,fixture.OUT+4))-amount))
            require(result['status']==0 and finite(fixture.get(actual,fixture.OUT+4))==expected,'independent finite response coefficient and separately rounded subtraction')
            require(fixture.get(actual,fixture.OUT)==fixture.get(actual,fixture.OUT+8)==0,'response zeroes horizontal outputs');oracles+=1
        elif c['kind']=='direction' and not c['normalize']:
            target=fixture.B if c['in_place'] else fixture.OUT
            scale=(2,3,4) if c['matrix'] else (1,1,1)
            require([finite(fixture.get(actual,target+4*j)) for j in range(3)]==[Fraction(x)*y for x,y in zip((1,2,3),scale)],'independent identity/diagonal direction transform, including in-place output');oracles+=1
        elif c['kind']=='rotation' and (c['entry']==0x8c079000 and c['orientation']==(0,1,0) and c['roll']==0 or c['entry']==0x8c078eb0 and c['angles']==(0,0,0)):
            require(actual[fixture.B&0xffffff:(fixture.B&0xffffff)+12]==before[fixture.B&0xffffff:(fixture.B&0xffffff)+12],'independent zero-angle vector preservation')
            require(fixture.get(actual,0x8c88f69c)==fixture.get(before,0x8c88f69c) and fixture.get(actual,0x8c88f5f8)==fixture.get(before,0x8c88f5f8),'matrix push/pop depth and pointer restoration');oracles+=1
        elif c['kind']=='matrix_init' and c['depth']==64:
            require(result['status']==0 and s.r[0]==0 and actual==before and bytes(f)==bytes(initial_f),'matrix limit path has no RAM/FP effects');oracles+=1
        elif c['kind']=='matrix_init' and not initial_f.fpscr&0x180000:
            require(result['status']==0 and s.r[0]==1 and fixture.get(actual,0x8c88f69c)==2,'matrix initializer pushes one level')
            require(list(f.banks[bank^1])==[fixture.bits(int(j%5==0)) for j in range(16)],'independent initialized identity matrix');oracles+=1
        if c['kind']=='numeric_stop':require(result['status']==-1,'reached PR1 numeric stop')
        if c['kind']=='guard':require(result['status']==-3,'reached mapped-memory guard')
        seen.update(path)
        for i,pc in enumerate(path):
            text=fixture.v.ROWS.get(pc,'')
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')):
                j=i+(2 if '/s ' in text else 1)
                if j<len(path):edges.add((pc,path[j]))
        counts[result['status']]+=1
        saved={k:(str(value) if isinstance(value,Fraction) else value) for k,value in c.items()}
        records.append(dict(configuration=saved,status=result['status'],stop_pc=result['stop_pc'],original_rows=len(path),result=s.r[0],boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%24==0:print('MOVEMENT DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in fixture.v.ANALYSIS_ROUTINES:
        wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    files=[Path(__file__),P/'movement_vectors_fixtures.py',P/'movement_vectors_reference.py',P/'write_watch_movement_vectors.py',fixture.v.MP,P/'analysis/movement_vectors_connection.json',fixture.v.effects.DLL,P/'source/sa_movement_vectors.c']
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(fixture.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Complete original movement helper bodies plus actual scalar/matrix/normalization/vector callees. Authored vectors/records/reference/globalmatrix INPUT state;no provided callee results/frames. Independent finite dot/response coefficient/direction/zero-angle/matrix-limit examples;other rotations/nonfinite/precision cases use original instruction comparison. Ordinary successful vector owners PR0/SZ0;selected PR1 numeric and PR0/SZ1 memory guards retain original reached state. Fixed code,SRFDclear,mappedsoftwareABI;physicaldelivery andgameplay unqualified. All static gaps retained pending explicit proof;no registration credit from tests alone.')
    (P/'analysis/movement_vectors_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('MOVEMENT DIRECT PASS',len(records),checks,dict(counts),coverage,flush=True)


if __name__=='__main__':main()
