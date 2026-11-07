"""Exercise controller-driven movement over original Emerald Coast ground."""
import argparse
import inspect
import os
import sys
from pathlib import Path

import verify_native_sonic_animation_task as animation
import verify_native_sonic_aura_frames as aura
import verify_native_sonic_movement_task as movement


def use_emerald_floor(raw):
    raw = movement.movement_caller(raw)
    anchor = "    d,b,f,system"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,'''    from pathlib import Path
    import hashlib, os
    from extract_native_emerald_collision import TRIANGLE, surfaces
    geometry = Path(os.environ['JP_SONIC_EMERALD_TEST_GEOMETRY'])
    geometry_manifest = json.loads((geometry/'verification.json').read_bytes())
    triangles = (geometry/'act0_triangles.bin').read_bytes()
    assert hashlib.sha256(triangles).hexdigest()==geometry_manifest['acts'][0]['triangle_sha256']
    triangle_buffer = C.create_string_buffer(triangles)
'''+anchor)
    anchor = """            native_draw=bind(f,'jp_sonic_movement_task_probe',I,P,P,P,P,I,I,P)
            def draw(textures,aura,walk,charged,hidden):
                frame=F()
                status=native_draw(textures,aura,stand.data,walk,charged,hidden,C.byref(frame))
                check(status==0 and 0<frame.value<4,
                      ('24 scheduler updates reach the walking pose before draw',status,frame.value))
                return status"""
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,"""            native_draw=bind(f,'jp_sonic_emerald_task_probe',I,P,P,P,P,I,I,P,P,S,P,P)
            def draw(textures,aura,walk,charged,hidden):
                frame, position, entry = F(), (F*3)(), U()
                status=native_draw(textures,aura,stand.data,walk,charged,hidden,C.byref(frame),
                                   triangle_buffer,len(triangles)//TRIANGLE.size,position,C.byref(entry))
                check(status==0 and 0<frame.value<4 and position[0]<-9,
                      ('native Sonic advances from the Emerald Coast start',status,frame.value,list(position),entry.value))
                floors = [h for h in surfaces(triangles,position[0],position[2])
                          if h[0]<=position[1]+0.001 and h[-1]]
                check(bool(floors) and abs(floors[-1][0]-position[1])<0.001 and floors[-1][1]==entry.value,
                      'scheduled pose agrees with the decoded retail floor')
                return status""")
    return raw.replace('sonic_controller_walk','sonic_emerald_floor_walk').replace(
        'original_Sonic_controller_movement_animation_and_GPU_draw_verified',
        'original_Sonic_movement_on_Emerald_Coast_floor_and_GPU_draw_verified')


def exercise(context, inputs, track):
    source = inspect.getsource(animation.exercise)
    namespace = dict(vars(animation),__file__=__file__,use_task_animation=use_emerald_floor)
    exec(compile(source,animation.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--geometry',type=Path,required=True)
    args, remaining = parser.parse_known_args()
    os.environ['JP_SONIC_EMERALD_TEST_GEOMETRY'] = str(args.geometry.resolve())
    sys.argv[1:] = remaining
    runner = inspect.getsource(aura.main)
    anchor = 'from verify_native_sonic_aura_frames import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_sonic_emerald_task import exercise')
    anchor = 'prefix=raw.split(marker)[0]'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,anchor+'''
    sample = b'Sample(1,0,0,0,0,0,0,0)'
    assert prefix.count(sample)==1
    prefix = prefix.replace(sample,b'Sample(1,0,-100,-12,0,0,0,0)')
''')
    namespace = dict(vars(aura))
    exec(compile(runner,aura.__file__,'exec'),namespace)
    namespace['main']()
