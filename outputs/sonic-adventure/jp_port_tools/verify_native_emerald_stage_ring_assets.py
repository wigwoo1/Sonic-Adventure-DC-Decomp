"""Exercise the source-generated ring model and sprite animation."""
import ctypes as C
import inspect
from pathlib import Path
from unittest.mock import patch

import verify_native_emerald_same_tick as same_tick
from native_texture_test_support import List
from native_packet_test_support import bind

I, P, U = same_tick.I, same_tick.P, same_tick.U


def overlay(context, _track):
    lib = context['f']
    target = bind(lib, 'jp_emerald_placement_task', P,U)(145)
    assert target
    work = same_tick.Task.from_address(target).work.contents
    state = bytes(work)
    assert work.header & 255 == 2

    draw = bind(lib, 'jp_emerald_stage_draw_disc_rings', I,P,P)
    lookup = bind(lib, 'jp_emerald_stage_ring_texture_list', P,I)
    assert not lookup(0) and not lookup(1)
    models, sprites = I(), I()
    assert draw(C.byref(models), C.byref(sprites)) == 0
    assert models.value == 8 and sprites.value == 2 and bytes(work) == state
    ring, sparkle = lookup(0), lookup(1)
    assert same_tick.PVM(lib).loaded(ring) == 1 and same_tick.PVM(lib).loaded(sparkle) == 8
    runtime = same_tick.Runtime(context['b']['frames_DLL'])

    def word(listing, slot):
        record = runtime.lookup(C.cast(listing, C.POINTER(List)), slot)
        return same_tick.Record.from_address(record).texparambuffer & 0x1fffff

    context['same_tick_rings'] = dict(texture_word=word(ring, 0), models=models.value,
                                      collected_target_omitted=True)
    context['same_tick_sparkles'] = dict(texture_words=[word(sparkle, i) for i in range(8)],
                                         sprites=sprites.value, simulation_unchanged=True,
                                         third_timer=work.scale[2])


def exercise(context, inputs, track):
    def scene_caller(raw):
        raw = same_tick.scene_caller(raw)
        return raw.replace('sonic_emerald_same_tick', 'sonic_emerald_stage_ring_assets').replace(
            'native_Emerald_Coast_same_tick_scene_verified',
            'native_Emerald_Coast_stage_ring_assets_verified')

    # The draw happens before stage close, within the terrain view scope.
    original = same_tick.collect

    def collect(*args):
        with patch.object(same_tick, 'overlay', overlay):
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
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_ring_assets import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
