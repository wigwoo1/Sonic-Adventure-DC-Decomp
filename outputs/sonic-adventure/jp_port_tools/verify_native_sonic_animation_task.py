"""Capture a walking pose produced by the native animation scheduler."""
import inspect

import verify_native_sonic_aura_frames as aura_frames
import verify_native_sonic_walk_frames as walk_frames


def exercise(context, inputs, track):
    source = inspect.getsource(walk_frames.exercise)
    anchor = "    caller = context['d']/'private_walk_caller.py'"
    assert source.count(anchor) == 1
    source = source.replace(anchor, "    raw = use_task_animation(raw)\n"+anchor)
    source = source.replace('fixture_action_frame=8.5','scheduler_ticks=24, drawn_frame_from_native_state=True')
    namespace = dict(vars(walk_frames), __file__=__file__, use_task_animation=use_task_animation)
    exec(compile(source,walk_frames.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


def use_task_animation(raw):
    anchor = "    action=Action.from_address(action_view.data)"
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, """    stand=View()
    check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(7,b'sonic_stand_action',C.byref(stand))==0,
          'original standing action retained for scheduler update')
"""+anchor)
    anchor = "            draw=bind(f,'jp_sonic_scheduled_draw_probe',I,P,P,P,F,I,I)"
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, """            native_draw=bind(f,'jp_sonic_animation_task_probe',I,P,P,P,P,I,I,P)
            def draw(textures,aura,walk,charged,hidden):
                frame=F()
                status=native_draw(textures,aura,stand.data,walk,charged,hidden,C.byref(frame))
                check(status==0 and abs(frame.value-10.2)<0.00001,
                      ('24 scheduler updates reach the walking pose before draw',status,frame.value))
                return status""")
    assert raw.count('action_view.data,8.5,') == 2
    raw = raw.replace('action_view.data,8.5,','action_view.data,')
    anchor = "and bind(motions,'jp_motions_release',I,Q)(action_view.token)==0"
    assert raw.count(anchor) == 1
    return raw.replace(anchor,"and bind(motions,'jp_motions_release',I,Q)(stand.token)==0 "+anchor).replace(
        'sonic_original_walk','sonic_scheduler_walk').replace(
        'original_Sonic_walk_action_scheduled_GPU_presentation_verified',
        'original_Sonic_animation_scheduler_update_and_GPU_draw_verified')


if __name__ == '__main__':
    runner = inspect.getsource(aura_frames.main)
    anchor = 'from verify_native_sonic_aura_frames import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor,'from verify_native_sonic_animation_task import exercise')
    namespace = dict(vars(aura_frames))
    exec(compile(runner,aura_frames.__file__,'exec'),namespace)
    namespace['main']()
