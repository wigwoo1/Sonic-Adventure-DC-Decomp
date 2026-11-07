"""Exercise the native stage ring entry point in the same-tick capture."""
import inspect
from pathlib import Path
from unittest.mock import patch

import verify_native_emerald_same_tick as same_tick


def exercise(context, inputs, track):
    source = inspect.getsource(same_tick.overlay)
    start = source.index('    rings = bind(')
    end = source.index('    try:', start)
    source = source[:start] + """    draw_rings = bind(lib, 'jp_emerald_stage_draw_rings', I,P,P,P,P,P,P)
""" + source[end:]
    start = source.index('        count = rings(')
    end = source.index('        assert count > 0', start)
    source = source[:start] + """        models, sprites = I(-2), I(-2)
        args = (C.byref(model), C.byref(span), C.byref(ring_list), C.byref(sprite))
        assert draw_rings(*args, C.byref(models), None) == -1
        assert models.value == -2 and sprites.value == -2
        assert draw_rings(*args, C.byref(models), C.byref(sprites)) == 0
        count, sprite_count = models.value, sprites.value
""" + source[end:]
    namespace = dict(vars(same_tick))
    exec(compile(source, same_tick.__file__, 'exec'), namespace)

    def scene_caller(raw):
        raw = same_tick.scene_caller(raw)
        return raw.replace('sonic_emerald_same_tick', 'sonic_emerald_stage_rings').replace(
            'native_Emerald_Coast_same_tick_scene_verified',
            'native_Emerald_Coast_stage_ring_draw_verified')

    # The draw happens before stage close, within the terrain view scope.
    original = same_tick.collect

    def collect(*args):
        with patch.object(same_tick, 'overlay', namespace['overlay']):
            return original(*args)

    direct = same_tick.terrain.shared.registry.loader.owner.queued.owner.direct
    harness = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    with patch.object(same_tick, 'collect', collect):
        exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), harness)
        harness['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = same_tick.terrain.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_rings import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
