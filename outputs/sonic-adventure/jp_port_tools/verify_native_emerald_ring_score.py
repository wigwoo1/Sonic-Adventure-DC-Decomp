"""Verify a real scheduled pickup updates the original HUD globals."""
import inspect
from pathlib import Path

import verify_native_emerald_ring_contact as contact


def draw_land(context, track):
    source = inspect.getsource(contact.draw_land)
    anchor = '        awards = []'
    assert source.count(anchor) == 1
    source = source.replace(anchor, """        rings = C.c_int16.in_dll(lib, 'rings')
        lives = C.c_int8.in_dll(lib, 'lives')
        initial = (rings.value, lives.value)
        rings.value, lives.value = 99, 3
        awards = []""")
    anchor = '        collector(None, 0, 0, Notify())'
    assert source.count(anchor) == 1
    source = source.replace(anchor, """        assert (rings.value, lives.value) == (100, 4)
        result['native_ring_score'] = dict(before=[99, 3], after=[rings.value, lives.value],
                                           hud_globals=True)
        rings.value, lives.value = initial
""" + anchor)
    namespace = dict(vars(contact))
    exec(compile(source, contact.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    return contact.scene_caller(raw).replace(
        'from verify_native_emerald_ring_contact import draw_land',
        'from verify_native_emerald_ring_score import draw_land').replace(
        'sonic_emerald_ring_contact', 'sonic_emerald_ring_score').replace(
        'native_Emerald_Coast_stationary_ring_contact_verified',
        'native_Emerald_Coast_ring_and_life_accounting_verified')


def exercise(context, inputs, track):
    direct = contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_score import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
