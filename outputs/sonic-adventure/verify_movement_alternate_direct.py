"""Original alternate response with full-state comparison and independent finite cases."""
import collections,hashlib,itertools,json
from pathlib import Path
import movement_alternate_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def configurations():
    for distance,motion,mode in itertools.product((0,.25,.5,2,4),(-2,0,2),(0,4)):
        yield dict(entry=q.ENTRY,kind='axis',normal=(1,0,0),contacts=[dict(point=(distance,0,0))],motion=(motion,0,0),distance=distance,mode=mode)
    for normal,contact_normal,motion,mode in itertools.product(((1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,1)),((1,0,0),(0,1,0),(0,-1,0)),((-2,0,0),(0,0,0),(.25,2,0),(-.125,1,0)),(0,4)):
        yield dict(entry=q.ENTRY,kind='gate',normal=normal,contacts=[dict(normal=contact_normal)],motion=motion,mode=mode)
    for count,mode in itertools.product((0,2,3,16),(0,4)):
        yield dict(entry=q.ENTRY,kind='multiple',normal=(1,0,0),contacts=[dict(point=((j+1)*.25,0,0)) for j in range(count)],mode=mode)
    for mode in range(8):
        for empty in (False,True):yield dict(entry=q.ENTRY,kind='raw_mode',normal=(1,0,0),contacts=[] if empty else [dict()],mode=mode)
    for view,rm,dn,mode in itertools.product((0x0c000000,0x8c000000,0xac000000),(0,1),(0,0x40000),(0,4)):
        yield dict(entry=q.ENTRY,kind='view',normal=(1,0,0),view=view,rm=rm,fpscr=dn,mode=mode)
    for word,dn,mode in itertools.product((0x80000000,1,0x80000001,0x7f800000,0x7fbfffff,0x7fc00001),(0,0x40000),(0,4)):
        yield dict(entry=q.ENTRY,kind='edge',normal=(1,0,0),motion_words=[word,0,0],fpscr=dn,mode=mode)
    for mode in (0,4):yield dict(entry=q.ENTRY,kind='far',normal=(1,0,0),contacts=[dict(point=(1001,0,0))],mode=mode)
    for bad in ('stack','query','query_end'):yield dict(entry=q.ENTRY,kind='guard',bad=bad)

def main():
    ram=q.v.seed_ram();seen=set();edges=set();records=[];counts=collections.Counter();checks=oracles=0
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    for i,c in enumerate(configurations()):
        c=dict(c,case_id=i);m,s,f,cfg=q.prepare(ram,c)
        if c.get('bad')=='query_end':s.r[7]=0x8cffffe0
        before=bytes(m);initial=type(s).from_buffer_copy(s)
        r,path,a,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status'] in (0,1,-1,-3),'completed or reached existing numeric/mapped-memory stop;no missing dependency')
        require(r['full_state_match'] and r['ordered_boundaries_match'],'complete original/native CPU/FPU/RAM/device/queue and ordered boundaries')
        if r['status']==0:
            require(s.pc==s.pr==initial.pr and s.r[15]==initial.r[15],'ordinary return and stack')
            require(list(s.r[8:15])==list(initial.r[8:15]),'callee-saved integer registers')
        # This owner has no velocity/state stores; test these declared disjoint
        # inputs independently, rather than assigning behavior from 075416.
        require(a[q.B&0xffffff:(q.B&0xffffff)+0x200]==before[q.B&0xffffff:(q.B&0xffffff)+0x200],'velocity input record remains unchanged')
        require(a[q.C&0xffffff:(q.C&0xffffff)+0x200]==before[q.C&0xffffff:(q.C&0xffffff)+0x200],'state input record remains unchanged')
        if c['kind']=='axis':
            motion=c['motion'][0];limit=min(abs(motion),c['distance']);expected=-limit if motion<0 else limit
            require(r['status']==0 and s.r[0]==int(abs(motion)>c['distance']),'independent finite axis clamp decision')
            require([q.number(q.get(a,q.QUERY+0x10+j*4)) for j in range(3)]==[expected,0,0],'independent exact axis movement clamp');oracles+=1
        elif c['kind']=='multiple':
            require(r['status']==0 and s.r[0]==int(bool(c['contacts'])),'empty/repeated contact response')
            require([q.number(q.get(a,q.QUERY+0x10+j*4)) for j in range(3)]==[-.25 if c['contacts'] else -2,0,0],'independent nearest repeated contact distance');oracles+=1
        if c['kind']=='guard':require(r['status']==-3,'reached original mapped-memory guard')
        seen.update(path)
        for j,pc in enumerate(path):
            text=q.v.ROWS.get(pc,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+skip<len(path):edges.add((pc,path[j+skip]))
        counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],result=s.r[0],original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
        if len(records)%24==0:print('ALTERNATE DIRECT',len(records),dict(counts),flush=True)
    body=q.v.ANALYSIS_ROUTINES[0];wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
    coverage=[dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)])]
    files=[Path(__file__),P/'movement_alternate_fixtures.py',P/'movement_response_fixtures.py',P/'movement_alternate_reference.py',P/'write_watch_movement_alternate.py',q.v.MP,P/'analysis/movement_alternate_connection.json',q.v.effects.DLL,P/'source/sa_movement_alternate.c']
    report=dict(original_native_comparisons=len(records),independent_checks=checks,independent_oracle_cases=oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Whole202row original075860 alternate response with actual distance/angle/dot/length/normalization callees. FullCPU/FPU/RAM/device/queue andorderedboundaries. Authored initialquery/contact/state/vectorinputs only;no calleeoutputs/frames. Independent finiteaxis/nearestcontact cases;othernumeric/geometryusesoriginalinstructioncomparison. SuccessfulgeometricprofilesPR0/SZ0;selectedrawprofiles/guardschecked. Physicaldelivery/gameplay/frame timingunqualified.')
    (P/'analysis/movement_alternate_direct_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ALTERNATE DIRECT PASS',len(records),checks,coverage,flush=True)

if __name__=='__main__':main()
