"""Verify multi-frame native land ownership and direct GPU submission."""
import inspect
from pathlib import Path

import verify_native_emerald_direct_land as direct


def dispatch_models(context,image,listing,render_model):
    source = inspect.getsource(direct.dispatch.scheduled_land)
    anchor = '        observed.append(i)'
    assert source.count(anchor)==1
    source = source.replace(anchor,anchor+'''
        try:
            assert close()==-1 and frame(camera,0,0,-1,1,0)==-1
            if emit:
                render_model(i)
                get(after)
                assert bytes(after)==matrices[i]
        except Exception as error:
            errors.append((i,repr(error)))
''')
    start = "    call = bind(f,'jp_land_dispatch_probe'"
    end = '    get(after)\n    assert observed==expected'
    assert source.count(start)==source.count(end)==1
    source = source[:source.index(start)]+'''    open_task = bind(f,'jp_land_task_open',I,P,I,P,I,F,Consume)
    frame = bind(f,'jp_land_task_frame',I,P,I,I,I,I,I)
    close = bind(f,'jp_land_task_close',I)
    camera = (F*3)(-9,16,4)
    emit = False
    assert frame(camera,0,0,-1,1,0)==-1
    assert open_task(entries,count,C.byref(listing),flags,clip,consume)==0
    try:
        assert open_task(entries,count,C.byref(listing),flags,clip,consume)==-1
        bind(f,'task_draw_all',None)()
        assert not observed
        assert frame(camera,0,0,-1,0,0)==0 and not observed
        assert frame(camera,0,0,-1,1,1)==0 and not observed
        for _ in range(2):
            assert frame(camera,0,0,-1,1,0)==len(expected)
            assert observed==expected
            observed.clear()
        emit = True
        assert frame(camera,0,0,-1,1,0)==len(expected)
    finally:
        assert close()==0
    assert close()==0 and frame(camera,0,0,-1,1,0)==-1
    bind(f,'task_draw_all',None)()
    assert open_task(entries,count,C.byref(listing),flags,clip,consume)==0
    assert close()==0
'''+source[source.index(end):]
    namespace = dict(vars(direct.dispatch),render_model=render_model)
    exec(compile(source,direct.dispatch.__file__,'exec'),namespace)
    driver = inspect.getsource(direct.dispatch_models)
    start = '    source = inspect.getsource'
    end = "    lib = context['f']"
    driver = driver[:driver.index(start)]+'    namespace = owner_namespace\n'+driver[driver.index(end):]
    driver_globals = dict(vars(direct),owner_namespace=namespace)
    exec(compile(driver,direct.__file__,'exec'),driver_globals)
    return driver_globals['dispatch_models'](context,image,listing,render_model)


def draw_land(context,track):
    source = inspect.getsource(direct.draw_land)
    namespace = dict(vars(direct),dispatch_models=dispatch_models)
    exec(compile(source,direct.__file__,'exec'),namespace)
    result,release = namespace['draw_land'](context,track)
    result.update(persistent_land_visible_frames=3,persistent_land_skipped_frames=2,
                  reentrant_frame_and_close_rejected=True,retired_task_can_be_reopened=True)
    return result,release


def scene_caller(raw):
    return direct.scene_caller(raw).replace(
        'from verify_native_emerald_direct_land import draw_land',
        'from verify_native_emerald_land_owner import draw_land').replace(
        'sonic_emerald_direct_land','sonic_emerald_land_owner').replace(
        'original_Emerald_Coast_land_callback_direct_GPU_submission_verified',
        'persistent_Emerald_Coast_land_task_and_GPU_submission_verified')


def exercise(context,inputs,track):
    source = inspect.getsource(direct.exercise)
    namespace = dict(vars(direct),__file__=__file__,scene_caller=scene_caller)
    exec(compile(source,direct.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__=='__main__':
    scene = direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n",1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_land_owner import exercise')
    exec(compile('if True:\n'+runner,scene.__file__,'exec'),dict(vars(scene)))
