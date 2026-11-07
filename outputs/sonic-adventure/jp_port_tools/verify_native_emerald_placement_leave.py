"""Check original range retirement and respawn with real placement tasks."""
import inspect
from pathlib import Path

import verify_native_emerald_kind_table as kinds


def adapt(source):
    anchor = '        assert select(2) == 1'
    assert source.count(anchor) == 1
    return source.replace(anchor, '''        viewers = bind(lib, 'jp_emerald_placement_viewers', None, P, P, P, I)
        leave = bind(lib, 'placement_try_leave', I, P)
        leave_by = bind(lib, 'placement_try_leave_by', I, P, C.c_float)
        addresses = [at for at, *_ in seen]
        viewers(None, None, C.byref(center), 0)
        assert all(leave(at) == 0 for at in addresses)
        viewers(C.byref(center), None, None, 0)
        assert all(leave(at) == 0 for at in addresses)
        viewers(C.byref(distant), None, None, 1)
        assert all(leave(at) == 0 for at in addresses)
        viewers(C.byref(distant), None, None, 0)
        assert all(leave_by(at, 0) == 0 for at in addresses)
        assert all(leave(at) == 1 for at in addresses)
        assert not retired
        run(3)
        assert Counter(retired) == Counter(addresses)
        assert step(C.byref(distant), None) == 0
        assert step(C.byref(center), None) == len(expected)
        assert select(2) == 1
        viewers(None, None, None, 0)
        result['native_placement_range_retirement'] = dict(
            destroyed_on_next_tick=len(retired), respawned=len(expected),
            nearby_camera_and_player_protected=True, redraw_protected=True,
            unlimited_range_protected=True)''')


def draw_land(context, track):
    source = inspect.getsource(kinds.draw_land)
    anchor = "    namespace = dict(vars(tasks), compare_table=compare_table, Counter=Counter)"
    assert source.count(anchor) == 1
    source = source.replace(anchor, '    source = adapt(source)\n' + anchor)
    namespace = dict(vars(kinds), adapt=adapt)
    exec(compile(source, kinds.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    return kinds.scene_caller(raw).replace(
        'from verify_native_emerald_kind_table import draw_land',
        'from verify_native_emerald_placement_leave import draw_land').replace(
        'sonic_emerald_kind_table', 'sonic_emerald_placement_leave').replace(
        'native_Emerald_Coast_retail_kind_table_and_GPU_submission_verified',
        'native_Emerald_Coast_placement_range_retirement_verified')


def exercise(context, inputs, track):
    direct = kinds.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = kinds.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_placement_leave import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
