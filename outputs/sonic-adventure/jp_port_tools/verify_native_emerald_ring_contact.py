"""Check scheduler-driven sphere contact and one-shot ring pickup."""
import inspect
from pathlib import Path

import verify_native_emerald_ring_init as rings


def draw_land(context, track):
    source = inspect.getsource(rings.draw_land)
    anchor = '        viewers(None, None, None, 0)\n        tick(3)'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '''        Notify = C.CFUNCTYPE(None, I)
        collector = bind(lib, 'jp_emerald_ring_collector', None, P, C.c_float, I, Notify)
        awards = []
        notify = Notify(awards.append)
        positions = {i: tuple(tasks.Task.from_address(at).work.contents.pos) for i, at in live.items()}
        isolated = [i for i, pos in positions.items() if all(
            i == j or sum((a-b)**2 for a,b in zip(pos, other)) > 12**2
            for j, other in positions.items())]
        assert len(isolated) >= 2
        target, falling = isolated[:2]

        def work(i):
            return tasks.Task.from_address(live[i]).work.contents

        x, y, z = positions[target]
        tangent = tasks.Vec(x+6, y, z)
        collector(tangent, 1, 0, notify)
        tick(3)
        assert not Collision.from_address(work(target).collision).flags & 2
        collector(tasks.Vec(x,y,z), 1, 0, notify)
        tick(3)
        assert Collision.from_address(work(target).collision).flags & 2
        assert work(target).header & 255 == 1 and awards == []
        tick(3)
        assert work(target).header & 255 == 2 and awards == [1]
        tick(3)
        assert awards == [1]
        collector(tasks.Vec(*positions[falling]), 1, 1, notify)
        tick(3)
        tick(3)
        assert work(falling).header & 255 == 2 and awards == [1]
        collector(None, 0, 0, Notify())
        result['native_ring_contact'] = dict(awards=awards, tangent_rejected=True,
                                             falling_award_suppressed=True,
                                             consumed_on_following_tick=True)
''' + anchor)
    namespace = dict(vars(rings))
    exec(compile(source, rings.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    return rings.scene_caller(raw).replace(
        'from verify_native_emerald_ring_init import draw_land',
        'from verify_native_emerald_ring_contact import draw_land').replace(
        'sonic_emerald_ring_init', 'sonic_emerald_ring_contact').replace(
        'native_Emerald_Coast_ring_initialization_verified',
        'native_Emerald_Coast_stationary_ring_contact_verified')


def exercise(context, inputs, track):
    direct = rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_contact import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
