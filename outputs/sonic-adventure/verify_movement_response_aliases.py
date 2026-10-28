"""Real pointer overlaps and original-call-produced stack history, without fabricated frames."""
import hashlib,itertools,json
from fractions import Fraction
from pathlib import Path
import movement_response_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();seen=set();edges=set();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(c,label);checks+=1
    def compare(m,s,f,cfg,label):
        before_s=type(s).from_buffer_copy(s);r,path,a,s,f=q.fast.compare(m,s,f,cfg)
        require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'complete original/native state and boundaries')
        require(s.pc==s.pr==before_s.pr and s.r[15]==before_s.r[15],'ordinary return/stack')
        seen.update(path)
        for j,pc in enumerate(path):
            text=q.v.ROWS.get(pc,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+skip<len(path):edges.add((pc,path[j+skip]))
        records.append(dict(configuration=dict(c),stage=label,entry=hex(cfg['entry']),status=0,original_rows=len(path),boundaries=r['boundaries']))
        return r,a,s,f,path
    for mode,entry,alias in itertools.product((0,4),(0x8c638eac,0x8c639b50),('same','plus4','second')):
        c=dict(mode=mode,entry=entry,alias=alias,kind='vector_alias');m,s,f,cfg=q.prepare(ram,c)
        # All finite input words, including partial-overlap neighbours, are
        # authored before entering the original leaf.
        q.vector(m,q.A,(1,2,3,4,5,6));q.vector(m,q.B,(-1,.5,2,3,4,5))
        if entry==0x8c638eac:s.r[5]={'same':q.A,'plus4':q.A+4,'second':q.B}[alias]
        else:s.r[6]={'same':q.A,'plus4':q.A+4,'second':q.B}[alias]
        left=[Fraction.from_float(q.number(q.get(m,s.r[4]+j*4))) for j in range(3)];right=[Fraction.from_float(q.number(q.get(m,s.r[5]+j*4))) for j in range(3)]
        expected=[left[j]+right[j] for j in range(3)] if entry==0x8c638eac else [left[1]*right[2]-left[2]*right[1],left[2]*right[0]-left[0]*right[2],left[0]*right[1]-left[1]*right[0]]
        target=s.r[4] if entry==0x8c638eac else s.r[6]
        r,a,s,f,path=compare(m,s,f,cfg,'leaf')
        require([Fraction.from_float(q.number(q.get(a,target+4*j))) for j in range(3)]==expected,'independent exact finite alias arithmetic')
    for mode,sign in itertools.product((0,4),('left','right','computed_cross')):
        c=dict(entry=0x8c055dac,mode=mode,kind='signed_angle_alias',sign_alias=sign);m,s,f,cfg=q.prepare(ram,c)
        s.r[6]={'left':q.A,'right':q.B,'computed_cross':q.STACK-0x14}[sign]
        r,a,s,f,path=compare(m,s,f,cfg,'angle')
        require(s.r[0]==16384,'axis angle with source/computed-cross sign alias')
    for mode,rm,value,alias in itertools.product((0,4),(0,1),(-1,0),(False,True)):
        c=dict(entry=0x8c075416,mode=mode,rm=rm,kind='stack_history_response',old_word=value,velocity_aliases_motion=alias)
        m,s,f,cfg=q.prepare(ram,c|dict(contacts=[dict(point=(1001,0,0))]))
        slot=q.STACK-0xd8+0x64
        q.vector(m,slot,(0,0,0));q.vector(m,q.OUT,(value,0,0))
        # This actual original leaf produces the old RAM word. The response
        # later allocates its own frame and reads this otherwise-uninitialized
        # slot through index -1. No frame/return state/result is installed.
        stages=((0x8c638eac,[slot,q.OUT]),(0x8c075416,[q.A,q.QUERY+0xc if alias else q.B,q.C,q.QUERY]))
        for entry,args in stages:
            cfg['entry']=entry;s.pc=entry;s.pr=0x8cfed000
            for j,a in enumerate(args):s.r[4+j]=a
            r,m,s,f,path=compare(m,s,f,cfg,'producer' if entry==0x8c638eac else 'response')
            if entry==0x8c638eac:require(q.get(m,slot)==q.bits(value),'actual vector-add produces signed old RAM word')
            else:
                zero_branch=alias and value<0
                require((0x8c075628 in path)==zero_branch,'derived original zero-length branch and controls')
                require(s.r[0]==int(not alias or value<0),'independent response decision with original index -1 fallback')
                expected=0 if alias or value==0 else 1
                require([q.number(q.get(m,q.QUERY+0x10+j*4)) for j in range(3)]==[expected,0,0],'independent final motion under alias/history controls')
                require(q.get(m,slot)==q.bits(value),'old RAM word survives actual response frame allocation')
    coverage=[]
    for body in q.v.ANALYSIS_ROUTINES:
        wanted={int(x,16) for x in body['reachable_addresses']};outcomes={tuple(int(x,16) for x in edge) for edge in body['conditional_edges']}
        coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&seen),executed_outcomes=len(outcomes&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(outcomes-edges)]))
    files=[Path(__file__),P/'movement_response_fixtures.py',q.v.MP,P/'analysis/movement_response_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,
      original_zero_length_branch_executed=True,supplied_game_callee_outputs=0,supplied_execution_frames=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},
      scope='Finite vector/source/output overlaps and signed-angle input/computed-cross aliases. Sixteen real638eac->075416sequences:oldRAMword-1/0,velocity/querymotionoverlap/disjoint,bothFPbanks,RM0/1. Originalvectoraddproducesstackhistory;originalresponseallocatesitsownframe. Withcontactdistance1001,minindexstays-1;historyslotatframe+64isreadasdistance. Aliasheadonclearzeroesquerymotion,allowingtruezero-lengthbranchafter0>negativeword. No calleeoutput/frameinjection,no sourcebranch exclusion. This demonstrates original raw input behavior,notnaturalgame scene layout orfullmovement.')
    (P/'analysis/movement_response_aliases_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('RESPONSE ALIASES PASS',len(records),checks,coverage,flush=True)

if __name__=='__main__':main()
