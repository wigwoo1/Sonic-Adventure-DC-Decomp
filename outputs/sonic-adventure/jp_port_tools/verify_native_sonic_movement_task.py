"""Verify a GPU pose selected by real pad scan and level movement."""
import inspect

import verify_native_sonic_animation_task as animation
import verify_native_sonic_aura_frames as aura


def movement_caller(raw):
    raw = animation.use_task_animation(raw)
    for old, new in (
        ('jp_sonic_animation_task_probe', 'jp_sonic_movement_task_probe'),
        ('abs(frame.value-10.2)<0.00001', '0<frame.value<4'),
        ('sonic_scheduler_walk', 'sonic_controller_walk'),
        ('original_Sonic_animation_scheduler_update_and_GPU_draw_verified',
         'original_Sonic_controller_movement_animation_and_GPU_draw_verified'),
    ):
        assert old in raw
        raw = raw.replace(old,new)
    return raw


def exercise(context, inputs, track):
    source = inspect.getsource(animation.exercise)
    namespace = dict(vars(animation),__file__=__file__,use_task_animation=movement_caller)
    exec(compile(source,animation.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__ == '__main__':
    runner = inspect.getsource(aura.main)
    anchor = 'from verify_native_sonic_aura_frames import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_sonic_movement_task import exercise')
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
