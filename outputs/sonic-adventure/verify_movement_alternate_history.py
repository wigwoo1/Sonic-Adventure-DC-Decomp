"""Actual original producer calls exercise the alternate response's zero-length path."""
import hashlib,itertools,json
from pathlib import Path
import movement_alternate_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();records=[];seen=set();edges=set();checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(mode,rm,value,motion,label);checks+=1
    for mode,rm,value,motion in itertools.product((0,4),(0,1),(-1,0),((0,0,0),(-2,0,0))):
        c=dict(entry=q.ENTRY,normal=(1,0,0),motion=motion,contacts=[dict(point=(1001,0,0))],mode=mode,rm=rm)
        m,s,f,cfg=q.prepare(ram,c);slot=q.STACK-0xc4+0x54
        q.vector(m,slot,(0,0,0));q.vector(m,q.OUT,(value,0,0))
        stages=((0x8c638eac,[slot,q.OUT]),(q.ENTRY,[q.A,q.B,q.C,q.QUERY]))
        for entry,args in stages:
            cfg['entry']=entry;s.pc=entry;s.pr=0x8cfed000
            for j,arg in enumerate(args):s.r[4+j]=arg
            sp=s.r[15];r,path,m,s,f=q.fast.compare(m,s,f,cfg)
            require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'actual producer/consumer completes with full state and boundaries')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'ordinary call return/stack')
            require(q.get(m,slot)==q.bits(value),'original vector-add produced old RAM word remains live')
            if entry==q.ENTRY:
                zero=value<0 and motion==(0,0,0)
                require((0x8c075998 in path)==zero,'actual zero-length branch and controls')
                require(s.r[0]==int(value<0 or motion!=(0,0,0)),'independent clamp decision with selected index -1')
                expected=1 if value<0 and motion!= (0,0,0) else 0
                require([q.number(q.get(m,q.QUERY+0x10+j*4)) for j in range(3)]==[expected,0,0],'independent final axis movement')
            seen.update(path)
            for j,pc in enumerate(path):
                text=q.v.ROWS.get(pc,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
                if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+skip<len(path):edges.add((pc,path[j+skip]))
            records.append(dict(mode=mode,rm=rm,old_word=value,motion=motion,entry=hex(entry),status=0,original_rows=len(path),boundaries=r['boundaries']))
    body=q.v.ANALYSIS_ROUTINES[0];wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in edge) for edge in body['conditional_edges']}
    coverage=[dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)])]
    files=[Path(__file__),P/'movement_alternate_fixtures.py',q.v.MP,P/'analysis/movement_alternate_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,original_zero_length_branch_executed=True,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='16actual638eac->075860sequences,negative/zero oldRAMword,zero/nonzeromotion,bothFPbanks/RM0/1. Originalvectoradd producesoldRAM;normal075860entryallocatesitsownframe. Distance1001 leavesselectedindex-1,readingframe+54beforethe distancearray;0>negativeword reacheszero-lengthbranch. No fabricatedframes/returns/calleeresults orinputdisjointness exclusion. AuthoredrawRAMhistory,notnaturalgame-layout proof.')
    (P/'analysis/movement_alternate_history_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ALTERNATE HISTORY PASS',len(records),checks,coverage,flush=True)

if __name__=='__main__':main()
