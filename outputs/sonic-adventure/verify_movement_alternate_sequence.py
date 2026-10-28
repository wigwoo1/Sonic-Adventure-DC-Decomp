"""Actual world selection and collision produce the records consumed by 075860."""
import hashlib,itertools,json
from pathlib import Path
import movement_alternate_fixtures as q
import movement_world_fixtures as world
P=q.P;VELOCITY,STATE=0x8cbb0000,0x8cbc0000
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(side,point,motion,normal,mode,label);checks+=1
    for side,point,motion,normal,mode in itertools.product(('low','high'),((.5,.25,.5),(4,.25,4)),((0,-2,0),(0,0,0)),((0,1,0),(0,-1,0)),(0,4)):
        node=dict(low=[dict(key=0)],high=[dict(key=-100)]) if side=='low' else dict(low=[dict(key=-100)],high=[dict(key=0),dict(key=-100)])
        m,s,f,cfg=world.prepare(ram,dict(entry=0x8c028bfe,point=point,motion=motion,mode=mode,nodes=[node]))
        face=world.LOW if side=='low' else world.HIGH
        for j,p in enumerate(((0,0,0),(0,0,2),(2,0,0))):q.vector(m,face+0x18+j*12,p)
        q.put(m,face+0x3c,0);q.vector(m,world.QUERY+0x1c,normal);q.vector(m,q.UP,(0,1,0))
        q.vector(m,VELOCITY+4,(3,4,0));q.put(m,STATE+0xc4,q.bits(2))
        depth=q.get(m,0x8c88f69c);stack=q.get(m,0x8c88f5f8)
        for entry,args in ((0x8c028bfe,[world.QUERY]),(0x8c029b00,[world.QUERY,world.OBJECTS]),(q.ENTRY,[world.OBJECTS,VELOCITY,STATE,world.QUERY])):
            cfg['entry']=entry;s.pc=entry;s.pr=0x8cfed000
            for j,a in enumerate(args):s.r[4+j]=a
            sp=s.r[15];r,path,m,s,f=q.fast.compare(m,s,f,cfg)
            require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'actual producer/consumer stage matches full state and boundaries')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'ordinary return and stack')
            require(q.get(m,0x8c88f69c)==depth and q.get(m,0x8c88f5f8)==stack,'matrix stack restored')
            hit=point==(.5,.25,.5)
            if entry==0x8c028bfe:require(q.get(m,world.COUNT)==int(hit),'independent polygon selection count')
            elif entry==0x8c029b00:
                require(s.r[0]==int(hit) and q.get(m,world.QUERY+0x28,2)==int(hit),'original collision produces query contact count')
                if hit:
                    require([q.number(q.get(m,q.CONTACTS+0x14+j*4)) for j in range(3)]==[0,1,0],'original producer horizontal contact normal')
                    require([q.number(q.get(m,q.CONTACTS+0x2c+j*4)) for j in range(3)]==[.5,1,.5],'original producer radius-offset contact point')
            else:
                responds=hit and motion[1]<0 and normal[1]>0
                require(s.r[0]==int(responds),'independent alternate response gate')
                require([q.number(q.get(m,world.QUERY+0x10+j*4)) for j in range(3)]==([0,-.75,0] if responds else list(motion)),'independent exact axis movement result')
                require([q.number(q.get(m,VELOCITY+4+j*4)) for j in range(3)]==[3,4,0],'alternate owner preserves separate velocity record')
            records.append(dict(side=side,point=point,motion=motion,normal=normal,mode=mode,entry=hex(entry),status=0,original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    files=[Path(__file__),P/'movement_alternate_fixtures.py',P/'movement_world_fixtures.py',q.v.MP,P/'analysis/movement_alternate_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,supplied_game_callee_outputs=0,supplied_execution_frames=0,
      dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='32actual028bfe->029b00->075860sequences,96comparisons. Lower/upperlists,hit/miss,stationary/downwardmotion,up/downquerynormals,bothFPbanks. Realcollisionproducesquerycountand88byterecords6bacf0;realalternateownerconsumesthem. Initialscene/query/mesh/velocity/state/normal/sentinelinputs only,no intermediateoutputs/frames. Explicitfar sentinels are authoredsceneinputs,notnaturalinitializationproof. Olderworld/collisionownercoveragegapsremain;notcomplete0730d8movement/gameplay/frame timing.')
    (P/'analysis/movement_alternate_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ALTERNATE SEQUENCE PASS',len(records),checks,flush=True)

if __name__=='__main__':main()
