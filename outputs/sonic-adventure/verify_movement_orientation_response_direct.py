"""Complete original/native state, circular-angle and cardinal geometry oracles."""
import collections,hashlib,itertools,json,sys
from pathlib import Path
import movement_orientation_response_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def signed(x):return ((x&0xffffffff)^0x80000000)-0x80000000
def approach(left,right,step):
    a,b=left&0xffff,right&0xffff;delta=((b-a+0x8000)&0xffff)-0x8000;limit=signed(step)
    if delta<=limit and delta>=signed(-step):return b
    return (a+(-limit if delta<0 else limit))&0xffff
def read_vector(m,a):return tuple(q.number(q.get(m,a+4*j)) for j in range(3))
def dot(a,b):return sum(x*y for x,y in zip(a,b))
def configurations():
    pairs=((0,0),(0,1),(1,0),(0,0x7fff),(0,0x8000),(0,0x8001),(0xffff,1),(1,0xffff),(0x12340001,0xffff8001))
    for (left,right),step,mode in itertools.product(pairs,(0,1,0x1000,0x7fff,0x8000,0xffffffff,0x80000000),(0,4)):
        yield dict(kind='short',entry=q.APPROACH,left=left,right=right,step=step,mode=mode)
    for mode,t in itertools.product(range(8),(0,1)):
        yield dict(kind='short_raw_mode',entry=q.APPROACH,left=0xffff,right=0x8000,step=1,mode=mode,t=t,fd=True)
    angles=list(itertools.product((0,0x4000),repeat=3))+[(-0x4000,0x8000,0x4000)]
    for a,vector,mode,normalize in itertools.product(angles,((1,2,3),(.5,-.25,2)),(0,4),(0,1)):
        yield dict(kind='local',entry=q.LOCAL,angles=a,vector=vector,mode=mode,normalize=normalize)
    for status,normal,orientation,mode in itertools.product((0,1,0x2001),((0,1,0),(0,0,1)),((0,1,0),(1,0,0),(0,-1,0)),(0,4)):
        yield dict(kind='gates',entry=q.OWNER,status_flags=status,normal=normal,orientation=orientation,mode=mode)
    limits=(dict(cap=0),dict(actor_flags=0x2000),dict(),dict(high=2),dict(high=2,middle=2),dict(high=2,middle=2,low=2))
    for limits,orientation,mode,target in itertools.product(limits,((0,1,0),(1,0,0),(0,-1,0)),(0,4),(0x3000,0xffff8000)):
        yield dict(limits,kind='limits',entry=q.OWNER,orientation=orientation,mode=mode,target_pitch=target)
    for entry,mode in itertools.product((q.LOCAL,q.OWNER),range(8)):
        yield dict(kind='raw_mode',entry=entry,mode=mode)
    for entry,view,rm,mode in itertools.product((q.LOCAL,q.OWNER),(0x0c000000,0xac000000),(0,1),(0,4)):
        yield dict(kind='view_rounding',entry=entry,view=view,rm=rm,mode=mode)
    for entry,bad in ((q.LOCAL,'actor'),(q.LOCAL,'vector'),(q.LOCAL,'stack'),(q.OWNER,'actor'),(q.OWNER,'state'),(q.OWNER,'query'),(q.OWNER,'stack')):
        yield dict(kind='guard',entry=entry,bad=bad)
def main():
    ram=q.v.seed_ram();records=[];checks=oracles=0;seen=set();edges=set();counts=collections.Counter()
    selected=list(configurations());case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
    if case_id is not None:selected=[selected[case_id]]
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for n,c in enumerate(selected):
        c=dict(c,case_id=n if case_id is None else case_id);m,s,f,cfg=q.prepare(ram,c);before=bytes(m);initial=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status'] in (0,1,-1,-3),'complete/existing numeric stop/mapped guard; no missing game callee')
        require(result['full_state_match'] and result['ordered_boundaries_match'],'full CPU/FPU/RAM/device/queue state and ordered entries')
        require(result['boundaries'][0]['target']==hex(c['entry']),'ordinary original entry')
        if result['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'normal return and original stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
            if c['entry']==q.APPROACH:
                require(s.r[0]==approach(c['left'],c['right'],c['step']),'independent circular short approach including wrap, signed limits and half-turn tie')
                require(m==before and bytes(f)==bytes(initial_f),'integer leaf preserves all RAM and FPU state')
                require(all(s.r[j]==initial.r[j] for j in (1,3,6,8,9,10,11,12,13,14,15)),'independent integer-leaf preserved registers');oracles+=1
            elif c.get('mode',0) in (0,4):
                require(q.get(m,0x8c88f69c)==q.get(before,0x8c88f69c) and q.get(m,0x8c88f5f8)==q.get(before,0x8c88f5f8),'independent matrix stack depth and pointer balance')
                bank=((initial_f.fpscr>>21)&1)^1
                require(list(f.banks[bank])==list(initial_f.banks[bank]),'independent complete inactive matrix bank restoration')
                if c['entry']==q.LOCAL:
                    off=q.A&0xffffff;require(m[off:off+0x200]==before[off:off+0x200],'actor input record preserved')
                    off=q.B&0xffffff;require(m[off+12:off+0x200]==before[off+12:off+0x200],'only three vector components written')
                    vector=read_vector(m,q.B);source=read_vector(before,q.B)
                    if c.get('normalize',0):require(abs(dot(vector,vector)-1)<1e-5,'independent unit-length normalized output')
                    else:require(dot(vector,vector)==dot(source,source),'independent cardinal rotation preserves exact squared length')
                    if c.get('angles',(0,0,0))==(0,0,0) and not c.get('normalize',0):require(vector==source,'independent identity orientation')
                    oracles+=1
                else:
                    off=q.A&0xffffff;allowed=set(range(0x14,0x18))|set(range(0x1c,0x20));require(all(m[off+j]==before[off+j] for j in range(0x200) if j not in allowed),'independent actor write footprint')
                    for a in (q.B,q.C):
                        off=a&0xffffff;require(m[off:off+0x200]==before[off:off+0x200],'state/query input records preserved')
                    up=read_vector(before,q.REFERENCE);normal=read_vector(before,q.C+0x1c);orientation=read_vector(before,q.C+0x48);flags=q.get(before,q.B+0x10,2)
                    bypass=dot(up,normal)>dot(up,orientation) or not flags&1 or bool(flags&0x2000)
                    if bypass:
                        require(q.get(m,q.A+0x14)==q.get(before,q.C+0x34) and q.get(m,q.A+0x1c)==q.get(before,q.C+0x38),'independent direct orientation copy gates')
                    elif c.get('angles',(0,0,0))==(0,0,0) and orientation in ((0,1,0),(1,0,0),(0,-1,0)):
                        step={(0,1,0):0,(1,0,0):0x4000,(0,-1,0):0x8000}[orientation]
                        speed=q.number(q.get(before,q.B+0x38));cap=q.number(q.get(before,q.B+0xd0))
                        if not q.get(before,q.A+4,2)&0x2000 and cap>speed:
                            if q.number(q.get(before,q.B+0xc8))<=speed:step=min(step,0x2000)
                            elif q.number(q.get(before,q.B+0xc4))<=speed:step=min(step,0x1500)
                            elif q.number(q.get(before,q.B+0xc0))<=speed:step=min(step,0x1000)
                            else:step=0x400
                        calls=[b for b in result['boundaries'] if b['target']==hex(q.APPROACH)]
                        require(len(calls)==2 and all(b['arguments'][2]==step for b in calls),'independent cardinal tilt and speed-dependent angular limit')
                        require(q.get(m,q.A+0x14)==approach(q.get(before,q.A+0x14),q.get(before,q.C+0x34),step) and q.get(m,q.A+0x1c)==approach(q.get(before,q.A+0x1c),q.get(before,q.C+0x38),step),'independent pitch/roll circular approach')
                    oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((a,path[i+skip]))
        counts[result['status']]+=1
        records.append(dict(configuration=c,status=result['status'],stop_pc=result['stop_pc'],result=s.r[0],original_rows=len(path),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%32==0:print('ORIENTATION DIRECT',len(records),dict(counts),flush=True)
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(a,16) for a in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    if case_id is not None:print('SINGLE DIAGNOSTIC',records[0]['status'],coverage,flush=True);return
    files=[Path(__file__),P/'movement_orientation_response_fixtures.py',P/'movement_orientation_response_reference.py',P/'write_watch_movement_orientation_response.py',q.v.MP,P/'analysis/movement_orientation_response_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original074ee4actororientation response,078e40inverse local-vector transformation and055d20circular angle approach. Actual matrix/rotation/direction/signedsqrt/atan2 dependencies;initialactor/state/query/matrix/vector inputs only,no suppliedresults/frames. Complete software state and ordered boundaries;independent circular angles/cardinal rotation/unitlength/matrixbalance/restoration/actorfootprint/copy/clamp oracles. RawFPUmodes also compared;physicalhardware/wholegame and explicit static gaps remain unqualified.')
    (P/'analysis/movement_orientation_response_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ORIENTATION DIRECT PASS',len(records),checks,coverage,flush=True)
if __name__=='__main__':main()
