"""Original nearby/world selection feeds the existing collision caller on retained RAM."""
import hashlib,itertools,json
from pathlib import Path
import movement_world_fixtures as q
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(side,point,mode,label);checks+=1
    for side,point,mode in itertools.product(('low','high'),((.5,.25,.5),(4,.25,4)),(0,4)):
        node=dict(low=[dict(key=0)],high=[dict(key=-100)]) if side=='low' else dict(low=[dict(key=-2)],high=[dict(key=0),dict(key=-100)])
        m,s,f,cfg=q.prepare(ram,dict(entry=0x8c028bfe,point=point,mode=mode,nodes=[node]))
        face=q.LOW if side=='low' else q.HIGH
        # Mesh vertices and flags are scene INPUT, authored before either call.
        for j,p in enumerate(((0,0,0),(0,0,2),(2,0,0))):q.vector(m,face+0x18+j*12,p)
        q.put(m,face+0x3c,0)
        depth=q.get(m,0x8c88f69c);stack=q.get(m,0x8c88f5f8)
        for entry,args in ((0x8c028bfe,[q.QUERY]),(0x8c029b00,[q.QUERY,q.OBJECTS])):
            cfg['entry']=entry;s.pc=entry;s.pr=0x8cfed000
            for j,value in enumerate(args):s.r[4+j]=value
            initial_sp=s.r[15];r,path,m,s,f=q.fast.compare(m,s,f,cfg)
            require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'actual sequence complete with full state and boundaries')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==initial_sp,'ordinary call return/stack')
            require(q.get(m,0x8c88f69c)==depth and q.get(m,0x8c88f5f8)==stack,'matrix stack restored')
            hit=point==(.5,.25,.5)
            if entry==0x8c028bfe:
                require(q.get(m,q.COUNT)==int(hit) and q.get(m,q.NODES+0x24)==int(hit),'independent finite polygon candidate count')
                if hit:require(q.get(m,q.NODES+0x1c)==q.CONTACTS and q.get(m,q.CONTACTS)==face,'actual original producer selects authored polygon')
            else:require(s.r[0]==int(hit) and q.get(m,0x8c73e400)==int(hit),'independent stationary horizontal-triangle contact')
            records.append(dict(side=side,point=point,mode=mode,entry=hex(entry),status=0,original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    files=[Path(__file__),P/'movement_world_fixtures.py',P/'movement_world_reference.py',P/'write_watch_movement_world.py',q.v.MP,P/'analysis/movement_world_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Eight actual028bfe->029b00 call sequences,lower/upper polygon lists,finite hit/miss andbothFPbanks. Original0522c0andmathcalleesproduce nearbymatchingrecords;original028bfeproduces nodepointer/countandpolygonarray consumedby029b00. Authoredinitialquery/scene/mesh/sentinels,not naturalworldconstruction orfull0730d8movement. Host startsordinarycalls,neverprovidescalleeoutputs/frames. Bothlargecallers retainexplicitcoveragegaps;no routinecreditforthem.')
    (P/'analysis/movement_world_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('WORLD SEQUENCE PASS',len(records),checks,flush=True)

if __name__=='__main__':main()
