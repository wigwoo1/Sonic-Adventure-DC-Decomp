"""Whole original field/table setter with full-state comparisons and finite field oracles."""
import collections,hashlib,itertools,json,math
from fractions import Fraction
from pathlib import Path
import movement_parameters_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def finite(word):return Fraction.from_float(q.number(word))

def rounded(value,rm):
    assert value>=0 and rm in (0,1)
    if not value:return 0
    hint=q.bits(float(value));choices=[b for b in range(max(0,hint-2),min(0x7f800000,hint+3))]
    if rm:result=max(b for b in choices if finite(b)<=value)
    else:result=min(choices,key=lambda b:(abs(finite(b)-value),b&1))
    # Exact dyadic neighbouring values bracket the result, independently of
    # the host's float hint. Products here are positive, finite and normal.
    assert finite(max(0,result-1))<=value<=finite(result+1)
    return result

def expected_scalar(flags):
    return 0x3e199999 if flags&8 else 0x3dcccccc if flags&4 else 0x3f000000 if flags&16 else 0x3fc00000 if flags&128 else 0x3f800000

def expected_fields(before,index,state_flags,rm):
    words=[q.get(before,q.TABLE+index*0x84+off) for off in (0x78,0x58,0x3c)]
    if state_flags&2:factors=(0x3f000000 if index==5 else 0x3ee66666,0x3fc00000,0x3f266666)
    elif state_flags&0x400000:factors=(0x3f199999,0x40000000,0x3ee66666)
    else:return words
    return [rounded(finite(a)*finite(b),rm) for a,b in zip(words,factors)]

def configurations():
    for state_flags,index,flags,mode in itertools.product((0,2,0x400000,0x400002),(0,5,7,255),(0,4,8,16,128,12,144),(0,4)):
        yield dict(kind='finite',state_flags=state_flags,selector=index,flags=flags,mode=mode)
    for state_flags,index,flags,mode in itertools.product((2,0x400000),(0,5),(0,8),(0,4)):
        yield dict(kind='finite',state_flags=state_flags,selector=index,flags=flags,mode=mode,rm=1)
    for state_flags,index,mode in itertools.product((0,2,0x400000),(0,5,7),(0,4)):
        yield dict(kind='retail_table',state_flags=state_flags,selector=index,mode=mode,authored_table=False)
    for mode,state_flags,flags in itertools.product(range(8),(0,2,0x400000),(0,8)):
        yield dict(kind='raw_mode',state_flags=state_flags,flags=flags,mode=mode)
    for state_flags,word,dn,mode in itertools.product((2,0x400000),(0,0x80000000,1,0x7f800000,0xff800000,0x7fbfffff,0x7fc00001,0x7f7fffff),(0,0x40000),(0,4)):
        yield dict(kind='edge',state_flags=state_flags,table_words=[dict(index=0,offset=0x78,word=word)],fpscr=dn,mode=mode)
    for state_flags,mode,enable in itertools.product((2,0x400000),(0,4),(0x80,0xf80)):
        yield dict(kind='trap_profile',state_flags=state_flags,table_words=[dict(index=0,offset=0x78,word=0x7fc00001)],fpscr=0x40000|enable|0x3f07c,mode=mode)
    for view,mode in itertools.product((0x0c000000,0x8c000000,0xac000000),(0,4)):
        yield dict(kind='finite',state_flags=2,selector=5,flags=0x8000008c,view=view,mode=mode)
    for bad in ('stack','selector','state','output'):yield dict(kind='guard',bad=bad)

def main():
    ram=q.v.seed_ram();seen=set();edges=set();records=[];counts=collections.Counter();checks=oracles=0
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for case_id,c in enumerate(configurations()):
        c=dict(c,case_id=case_id);m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s)
        r,path,a,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status'] in (0,1,-1,-3),'complete or reached existing numeric/mapped-memory stop')
        require(r['full_state_match'] and r['ordered_boundaries_match'],'full original/native CPU/FPU/RAM/device/queue and ordered boundaries')
        require([b['target'] for b in r['boundaries']]==[hex(q.ENTRY)],'original leaf entry only;no substituted game callees')
        if r['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'ordinary return and stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
        if c['kind']=='finite':
            require(r['status']==0,'finite ordinary profile completes')
            expected=expected_fields(before,c['selector'],c['state_flags'],c.get('rm',0))
            require([q.get(a,q.STATE+off) for off in (0x114,0xf4,0xd8)]==expected,'independent finite parameter field scaling and mode priority')
            require(q.get(a,q.STATE+0x5c)==(q.get(before,q.STATE+0x5c)|c['flags'])&0xffffffff,'independent flag OR with high bits')
            require(q.get(a,q.OUTPUT+0x3c)==expected_scalar(c['flags']),'independent scalar flag precedence')
            require(s.r[0]==0x3c and s.sr==(initial.sr&~1)|int(not c['flags']&0x9c),'independent final scratch offset and T flag')
            wanted=bytearray(before)
            for address,n in [(q.STATE+off,4) for off in (0x5c,0x114,0xf4,0xd8)]+[(q.OUTPUT+0x3c,4),(q.STACK-8,8)]:
                off=address&0xffffff;wanted[off:off+n]=a[off:off+n]
            require(a==wanted,'only declared parameter/scalar/flag and original stack stores');oracles+=1
        if c['kind']=='guard':require(r['status']==-3,'reached mapped-memory guard')
        seen.update(path)
        for j,pc in enumerate(path):
            text=q.v.ROWS.get(pc,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+skip<len(path):edges.add((pc,path[j+skip]))
        counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],result=s.r[0],original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('PARAMETERS DIRECT',len(records),dict(counts),flush=True)
    body=q.v.ANALYSIS_ROUTINES[0];wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
    coverage=[dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)])]
    files=[Path(__file__),P/'movement_parameters_fixtures.py',P/'movement_parameters_reference.py',P/'write_watch_movement_parameters.py',q.v.MP,P/'analysis/movement_parameters_connection.json',q.v.effects.DLL,P/'source/sa_movement_parameters.c']
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Whole original075214 field/table setter;authored finite tables and selected untouched retail-table inputs. FullCPU/FPU/RAM/device/queue,orderedleafboundary. Independent finite field scaling/flag/scalar-priority/footprint oracles with exactdyadicrounding;othernumericcases compareoriginalinstructions. No calleeoutputs/frames. SuccessfulgeometricfieldprofilesPR0/SZ0;selectedrawmodes/guardschecked. PhysicalFPU/exception/MMU/gameplayunqualified.')
    (P/'analysis/movement_parameters_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('PARAMETERS DIRECT PASS',len(records),checks,coverage,flush=True)

if __name__=='__main__':main()
