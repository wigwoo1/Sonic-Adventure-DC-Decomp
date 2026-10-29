"""Real corner, ordering, transition, material and ending-phase complete-tail scenes."""
import hashlib,itertools,json
from pathlib import Path
import movement_special_tail_context_fixtures as inputs
import verify_movement_special_tail_direct as direct
P=Path(__file__).resolve().parent

def corner(planes=(2,0),objects=1):
    low=[dict(key=.5,point=(.5,0,.5),extent=64,vertices=direct.PLANES[p]) for p in planes]
    return dict(static=[dict(id=0) for _ in range(objects)],nodes=[dict(object_index=j,low=low,low_head=len(low)-1,high=[dict(key=-100)]) for j in range(objects)])
def configurations():
    for planes,velocity,objects,owner,selected,mode in itertools.product(((2,0),(4,0),(2,4,0)),((-8,-16,-1),(-1,-16,-8)),(1,2),(0,0x8ca70000),(0,0x8cb60000),(0,4)):
        yield dict(kind='corner_response',position=(.5,0,.5),velocity=velocity,previous_owner=owner,previous_object=selected,mode=mode,major=9,minor=2,**corner(planes,objects))
    for angles,objects,owner,selected,mode in itertools.product(((0x8000,0,0),(0,0,0x8000)),(1,2,3),(0,0x8ca70000),(0,0x8cb60000),(0,4)):
        yield dict(kind='prepass_clamp_order',position=(.5,0,.5),velocity=(-1,0,0),angles=angles,previous_owner=owner,previous_object=selected,mode=mode,major=9,minor=2,**corner((2,0),objects))
    for angles,plane,prior,flags,lift,mode in itertools.product(((0x8000,0,0),(0,0,0x8000)),(2,4),(0,1),(0,1),(0,1),(0,4)):
        yield dict(kind='ground_transition',position=(.5,0,.5),velocity=(-1,0,-1),angles=angles,prior_status=prior,flags=flags,lift=lift,mode=mode,major=9,minor=2,**direct.scene(direct.PLANES[plane]))
    for flags,character,minor,mode in itertools.product((0x40,0x200000,0x200040),(0,6),(2,3),(0,4)):
        yield dict(kind='floor_material_stage',position=(.5,0,.5),velocity=(0,0,0),character_kind=character,major=9,minor=minor,mode=mode,**direct.scene(direct.PLANES[0],flags=flags))
    for value,lock,state_word,state_flags,flags,mode in itertools.product((0,.5),(0,1),(0,1),(0,0x4000),(1,0x4001),(0,4)):
        yield dict(kind='ending_heading_gate',position=(.5,0,.5),velocity=(0,0,0),table_value=value,character_word=lock,state_word_a=state_word,state_flags=state_flags,flags=flags,mode=mode,major=9,minor=2,**direct.scene())
    controls=(dict(lift=1),dict(state_speed=2),dict(vertical=1),dict(slope=.5),dict(threshold=-1),dict(threshold=0))
    for values,mode in itertools.product(controls,(0,4)):
        yield dict(kind='ending_scalar_gate',position=(.5,0,.5),velocity=(1,0,0),table_value=.5,mode=mode,major=9,minor=2,**values,**direct.scene())
    for height,speed,state_speed,vertical,mode in itertools.product((2,16),(0,1,3),(0,1),(0,1),(0,4)):
        yield dict(kind='ending_displacement',position=(.5,0,.5),velocity=(speed,0,0),height=height,state_speed=state_speed,vertical=vertical,table_value=.5,mode=mode,major=9,minor=2,**direct.scene())

def main():
    parent=P/'verify_movement_special_tail_direct.py';source=parent.read_text();marker='movement_special_tail_direct_verification.json';assert source.count(marker)==1;source=source.replace(marker,'movement_special_tail_phases_verification.json')
    ns=dict(__name__='special_tail_phases_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['q']=inputs;ns['configurations']=configurations;ns['main']()
    path=P/'analysis/movement_special_tail_phases_verification.json';r=json.loads(path.read_text());r['scope']='Complete1273-row routine withgenuineworld/triangle/task inputs,backwardlow-listheads,realproducercontacttables andactualcallees. Corners,ordering,clampselection,air/groundtransitions,materials/stages andendingheading/scalar/displacement gates. No candidate buffer/query outputs,callee return values or executionframes supplied. Broaderinitialization/callback/physical/fullgame remainunqualified.'
    for p in (Path(__file__),P/'movement_special_tail_context_fixtures.py'):r['sha256'][str(p)]=hashlib.sha256(p.read_bytes()).hexdigest()
    path.write_text(json.dumps(r,indent=2)+'\n');print('SPECIAL TAIL PHASES PASS',r['original_native_comparisons'],r['independent_checks'],flush=True)
if __name__=='__main__':main()
