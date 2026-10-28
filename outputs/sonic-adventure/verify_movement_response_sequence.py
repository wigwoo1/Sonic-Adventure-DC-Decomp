"""Original world selection -> collision -> response, with actual produced contact records."""
import hashlib,itertools,json
from pathlib import Path
import movement_response_fixtures as q
import movement_world_fixtures as world
P=q.P;VELOCITY,STATE=0x8cbb0000,0x8cbc0000
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(side,point,motion,mode,label);checks+=1
    for side,point,motion,mode in itertools.product(('low','high'),((.5,.25,.5),(4,.25,4)),((0,-2,0),(0,0,0)),(0,4)):
        node=dict(low=[dict(key=0)],high=[dict(key=-100)]) if side=='low' else dict(low=[dict(key=-2)],high=[dict(key=0),dict(key=-100)])
        m,s,f,cfg=world.prepare(ram,dict(entry=0x8c028bfe,point=point,motion=motion,mode=mode,nodes=[node]))
        face=world.LOW if side=='low' else world.HIGH
        for j,p in enumerate(((0,0,0),(0,0,2),(2,0,0))):q.vector(m,face+0x18+j*12,p)
        q.put(m,face+0x3c,0);q.vector(m,world.QUERY+0x1c,(0,-1,0));q.vector(m,q.UP,(0,1,0))
        q.vector(m,VELOCITY+4,(3,4,0));q.put(m,STATE+0xc4,q.bits(2));q.put(m,q.STACK,0)
        depth=q.get(m,0x8c88f69c);stack=q.get(m,0x8c88f5f8)
        stages=((0x8c028bfe,[world.QUERY]),(0x8c029b00,[world.QUERY,world.OBJECTS]),(0x8c075416,[world.OBJECTS,VELOCITY,STATE,world.QUERY]))
        for entry,args in stages:
            cfg['entry']=entry;s.pc=entry;s.pr=0x8cfed000
            for j,value in enumerate(args):s.r[4+j]=value
            initial_sp=s.r[15];r,path,m,s,f=q.fast.compare(m,s,f,cfg)
            require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'actual producer/consumer stage completes with full state and ordered boundaries')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==initial_sp,'ordinary return and stack')
            require(q.get(m,0x8c88f69c)==depth and q.get(m,0x8c88f5f8)==stack,'matrix stack restored')
            hit=point==(.5,.25,.5)
            if entry==0x8c028bfe:require(q.get(m,world.COUNT)==int(hit),'independent selected polygon count')
            elif entry==0x8c029b00:
                require(s.r[0]==int(hit) and q.get(m,world.QUERY+0x28,2)==int(hit),'original collision producer sets query contact count')
                if hit:
                    require([q.number(q.get(m,q.CONTACTS+0x14+j*4)) for j in range(3)]==[0,1,0],'actual horizontal contact normal')
                    require([q.number(q.get(m,q.CONTACTS+0x2c+j*4)) for j in range(3)]==[.5,1,.5],'actual radius-offset contact point from original producer')
            else:
                responds=hit and motion[1]<0
                require(s.r[0]==int(responds),'independent response decision')
                expected=(0,-.75,0) if responds else motion
                require([q.number(q.get(m,world.QUERY+0x10+j*4)) for j in range(3)]==list(expected),'independent exact axis distance-limited motion')
                require([q.number(q.get(m,VELOCITY+4+j*4)) for j in range(3)]==([0,0,0] if responds else [3,4,0]),'independent head-on velocity clear or untouched velocity')
            records.append(dict(side=side,point=point,motion=motion,mode=mode,entry=hex(entry),status=0,original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    files=[Path(__file__),P/'movement_response_fixtures.py',P/'movement_world_fixtures.py',q.v.MP,P/'analysis/movement_response_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},
      scope='Sixteen actual028bfe->029b00->075416sequences,48comparisons:lower/upperlists,horizontaltrianglehit/miss,stationary/downwardmotion,bothFPbanks. Originalworldproducer suppliesselection;originalcollisionproducer suppliescontactcountand88byterecords6bacf0;neworiginalresponseconsumesthem. Initialmesh/query/velocity/state/normal/sentinelinputs only;hoststartsordinarycalls,neverinjectscontactresults orframes. Existing028bfe/029b00gapsremainunqualified;notfull0730d8/naturalworldstartup/gameplay/frame timing.')
    (P/'analysis/movement_response_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('RESPONSE SEQUENCE PASS',len(records),checks,flush=True)

if __name__=='__main__':main()
