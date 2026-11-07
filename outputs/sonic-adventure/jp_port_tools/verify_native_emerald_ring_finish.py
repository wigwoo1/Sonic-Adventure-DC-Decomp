"""Verify a collected ring finishes and stays gone after leaving its range."""
import inspect
from pathlib import Path

import verify_native_emerald_ring_draw as draw


def adapt(source):
    # The older contact fixture expects collected tasks to retire immediately
    # out of range. This fixture verifies their new, longer lifetime below.
    previous = 'walk.score.draw_land(context, track)'
    assert source.count(previous) == 1
    source = source.replace(previous, 'walk.score.contact.rings.tasks.registry.draw_land(context, track)')
    anchor = "        result['native_ring_scheduler_draw']"
    assert source.count(anchor) == 1
    return source.replace(anchor, '''        viewers(None, None, None, 0)
        elapsed = 0
        third = C.c_float(-4)
        while int(third.value) <= 7:
            third.value += C.c_float(0.8).value
            elapsed += 1
            tick(3)
            assert task_at(145) == at and work.scale[2] == third.value
            assert work.header & 255 == (3 if int(third.value) > 7 else 2)
        tick(3)
        assert task_at(145) == at and get(145, entry) == -1
        tick(3)
        assert not task_at(145)
        viewers(C.byref(center), None, None, 0)
        assert step(C.byref(center), None) == visible-1 and not task_at(145)
        assert (rings.value, lives.value) == initial
        result['native_ring_completion'] = dict(
            sparkle_ticks=elapsed, retired_on_scheduler_tick=True,
            respawn_suppressed=True, survives_leaving_view=True)
''' + anchor)


def draw_land(context, track):
    source = inspect.getsource(draw.draw_land)
    anchor = '    namespace = dict(vars(model))'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '    source = adapt(source)\n' + anchor)
    namespace = dict(vars(draw), adapt=adapt)
    exec(compile(source, draw.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    return draw.scene_caller(raw).replace(
        'from verify_native_emerald_ring_draw import draw_land',
        'from verify_native_emerald_ring_finish import draw_land').replace(
        'sonic_emerald_ring_draw', 'sonic_emerald_ring_finish').replace(
        'native_Emerald_Coast_scheduled_ring_draw_verified',
        'native_Emerald_Coast_collected_ring_completion_verified')


def exercise(context, inputs, track):
    direct = draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_finish import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
