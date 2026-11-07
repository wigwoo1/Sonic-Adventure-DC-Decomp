"""Exercise the native disc-backed ring texture path."""
import inspect
from pathlib import Path
from unittest.mock import patch

import verify_native_emerald_same_tick as same_tick
from native_texture_test_support import List


def exercise(context, inputs, track):
    assets_source = inspect.getsource(same_tick.assets)
    assets_source = assets_source.replace("        compressed = read(disc.files()['OBJ_REGULAR.PRS'])\n", "")
    assets_source = assets_source.replace("return image, decompress(compressed)[0]", "return image")
    assets = dict(vars(inspect.getmodule(same_tick.assets)))
    exec(compile(assets_source, same_tick.__file__, 'exec'), assets)
    source = inspect.getsource(same_tick.overlay)
    start = source.index('    image, regular = ')
    end = source.index('    model, span, uv', start)
    source = source[:start] + '    image = assets(context, track)\n' + source[end:]
    start = source.index('    owned = ')
    end = source.index('    task_at', start)
    source = source[:start] + source[end:]
    start = source.index('    rings = bind(')
    end = source.index('        frames = ', start)
    source = source[:start] + """    open_textures = bind(lib, 'jp_emerald_stage_ring_textures_open_disc', I)
    texture_list = bind(lib, 'jp_emerald_stage_ring_texture_list', P,I)
    draw_rings = bind(lib, 'jp_emerald_stage_draw_owned_rings', I,P,P,P,P,P)
    assert not texture_list(0) and not texture_list(1)
    assert open_textures() == 0 and open_textures() == -1
    ring_list, sparkle_list = texture_list(0), texture_list(1)
    assert pvm.loaded(ring_list) == 1 and pvm.loaded(sparkle_list) == 8
""" + source[end:]
    start = source.index('        sprite = Sprite(')
    end = source.index('        assert count > 0', start)
    source = source[:start] + """        sprite = Sprite(Point(), 1, 1, 0, None, C.addressof(frames))
        models, sprites = I(-2), I(-2)
        args = (C.byref(model), C.byref(span), C.byref(sprite))
        assert draw_rings(*args, C.byref(models), C.byref(sprites)) == 0
        assert not sprite.tlist
        count, sprite_count = models.value, sprites.value
""" + source[end:]
    end = source.index('    finally:')
    start = source.index('        frames = ')
    body = ''.join(line[4:] if line.startswith('    ') else line
                   for line in source[start:end].splitlines(keepends=True))
    source = source[:start] + body
    source = source.replace('runtime.lookup(C.byref(listing), slot)',
                            'runtime.lookup(C.cast(listing, C.POINTER(List)), slot)')
    namespace = dict(vars(same_tick), List=List, assets=assets['assets'])
    exec(compile(source, same_tick.__file__, 'exec'), namespace)

    def scene_caller(raw):
        raw = same_tick.scene_caller(raw)
        return raw.replace('sonic_emerald_same_tick', 'sonic_emerald_stage_ring_archives').replace(
            'native_Emerald_Coast_same_tick_scene_verified',
            'native_Emerald_Coast_stage_ring_archives_verified')

    # The draw happens before stage close, within the terrain view scope.
    original = same_tick.collect

    def collect(*args):
        with patch.object(same_tick, 'overlay', namespace['overlay']):
            result = original(*args)
        lib = args[0]['f']
        lookup = same_tick.bind(lib, 'jp_emerald_stage_ring_texture_list', same_tick.P,same_tick.I)
        assert not lookup(0) and not lookup(1)
        return result

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
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_ring_archives import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
