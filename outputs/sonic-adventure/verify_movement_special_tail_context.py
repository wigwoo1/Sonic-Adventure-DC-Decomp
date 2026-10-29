"""Complete-tail valid task/entity and moving-object scene inputs."""
import hashlib,itertools,json
from pathlib import Path
import movement_special_tail_context_fixtures as inputs
import verify_movement_special_tail_direct as direct
P=Path(__file__).resolve().parent

def configurations():
    for entity,count,mode,rm in itertools.product((0,0x100),(1,2),(0,4),(0,1)):
        scene=direct.scene(direct.PLANES[0],count)
        for record in scene['static']:record['id']=inputs.TASK
        yield dict(kind='context_disabled' if entity==0 else 'context_enabled',entity_flags=entity,position=(.5,0,.5),velocity=(1,0,0),mode=mode,rm=rm,major=9,minor=2,**scene)
    for moving,index,offset,angles,mode in itertools.product((False,True),(0,7),((0,0,0),(.5,0,-.5)),((0,0,0),(0x1000,0x2000,0x3000)),(0,4)):
        records=[dict(key=.5,point=(.5,0,.5),extent=64,vertices=direct.PLANES[p]) for p in (2,0)]
        yield dict(kind='moving_context',entity_flags=0x100,moving_data=moving,index=index,moving_offset=offset,moving_angles=angles,position=(.5,0,.5),velocity=(-8,-16,-1),static=[dict(id=inputs.TASK)],nodes=[dict(low=records,low_head=1,high=[dict(key=-100)])],mode=mode,major=9,minor=2)

def main():
    parent=P/'verify_movement_special_tail_direct.py';source=parent.read_text();marker='movement_special_tail_direct_verification.json';assert source.count(marker)==1;source=source.replace(marker,'movement_special_tail_context_verification.json')
    marker='        seen.update(path)';assert source.count(marker)==1
    extra="        if c['kind']=='context_disabled':\n            require(result['status']==0 and q.get(m,q.world.NEAR_COUNT,2)==len(c['static']),'actual world producer keeps context-tagged nearby objects')\n            require(not any(b['target']=='0x8c029b00' for b in result['boundaries']) and s.r[0]==0,'disabled entity flag excludes collision using actual task/entity reads')\n            oracles+=1\n        elif c['kind']=='context_enabled':\n            require(any(b['target']=='0x8c029b00' for b in result['boundaries']),'enabled context reaches actual collision producer')\n            oracles+=1\n"
    source=source.replace(marker,extra+marker)
    ns=dict(__name__='special_tail_context_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['q']=inputs;ns['configurations']=configurations;ns['main']()
    path=P/'analysis/movement_special_tail_context_verification.json';r=json.loads(path.read_text());r['scope']='Whole0777e0 withauthoredvalidtask+20entity/+4flag andoptional+28movingdata records. Actualworld/collision/movementproducers execute;callbackfield0 isordinaryinput,no suppliedcalleeoutputs/frames. Entity0disablescollision,100enablesit;movingdata inputsincludeangles,offsets,indices0/7 andbanks. Naturalinitialization/callbackassignment/fullgame remainunqualified.'
    for p in (Path(__file__),P/'movement_special_tail_context_fixtures.py'):r['sha256'][str(p)]=hashlib.sha256(p.read_bytes()).hexdigest()
    path.write_text(json.dumps(r,indent=2)+'\n');print('SPECIAL TAIL CONTEXT PASS',r['original_native_comparisons'],r['independent_checks'],flush=True)
if __name__=='__main__':main()
