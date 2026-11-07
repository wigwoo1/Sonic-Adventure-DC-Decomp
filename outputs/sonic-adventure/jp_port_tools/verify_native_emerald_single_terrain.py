"""Present terrain from a single stage-owned draw with retained textures."""
import ctypes as C
import inspect
from pathlib import Path
from unittest.mock import patch

import verify_native_emerald_stage_terrain as terrain
from native_packet_test_support import bind
from native_texture_test_support import Record, Runtime

shared = terrain.submit.mesh.stage.land.live.shared
I, P, U = C.c_int, C.c_void_p, C.c_uint32


def collect(context, triangles, draw, track):
    source = inspect.getsource(terrain.collect)
    anchor = "        assert count == loaded(listing) and count > 50"
    assert source.count(anchor) == 1
    source = source.replace(anchor, anchor + '''
        runtime = Runtime(context['b']['frames_DLL'])
        records = [Record.from_address(runtime.lookup(listing, i)) for i in range(count)]
        context['terrain_texture_words'] = sorted({r.texparambuffer&0x1fffff for r in records})
        context['terrain_vq_words'] = sorted({r.texparambuffer&0x1fffff for r in records
                                              if r.texparambuffer&0xc0000000 == 0xc0000000})
''')
    anchor = '            before = used(0)+used(1)'
    assert source.count(anchor) == 1
    source = source.replace(anchor, anchor + '\n            opaque_before = used(0)')
    anchor = '            committed = used(0)+used(1)-before'
    assert source.count(anchor) == 1
    source = source.replace(anchor, anchor + "\n            context['terrain_opaque_bytes'] = used(0)-opaque_before")
    namespace = dict(vars(terrain), Runtime=Runtime, Record=Record)
    exec(compile(source, terrain.__file__, 'exec'), namespace)
    return namespace['collect'](context, triangles, draw, track)


def open_ring_overlay(context, track):
    # The inherited ring diagnostic needs its SET entries. Its setup
    # deliberately leaves terrain to the stage-owned pass already recorded.
    lib = context['f']
    open_stage = bind(lib, 'jp_stage_resources_open', I,I)
    close_stage = bind(lib, 'jp_stage_resources_close', I)
    select = bind(lib, 'jp_emerald_set_select', I,U)
    close_set = bind(lib, 'jp_emerald_set_close', I)
    assert open_stage(1) == 0
    try:
        assert select(2) == 1
    except BaseException:
        close_set()
        close_stage()
        raise

    def release():
        assert close_set() == 0 and close_stage() == 0

    return dict(texture_words=context['terrain_texture_words'],
                vq_texture_words=context['terrain_vq_words'],
                opaque_bytes=context['terrain_opaque_bytes']), release


def draw_land(context, track):
    with patch.object(shared, 'shared_land', open_ring_overlay):
        return shared.draw_land(context, track)


def scene_caller(raw):
    raw = terrain.scene_caller(raw)
    for old, new in (
        ('from verify_native_emerald_stage_terrain import collect',
         'from verify_native_emerald_single_terrain import collect'),
        ('from verify_native_emerald_land_pass import draw_land',
         'from verify_native_emerald_single_terrain import draw_land'),
        ('sonic_emerald_stage_terrain', 'sonic_emerald_single_terrain'),
        ('native_Emerald_Coast_stage_texture_and_draw_ownership_verified',
         'native_Emerald_Coast_single_native_terrain_pass_verified'),
        ("    check(land['direct_callback_draws']==land['live_land_visibility']['live_count'],"
         "'the original land callback draws the live camera selection')",
         """    terrain_pass = land['native_ring_walk']['native_stage_terrain']
    check(terrain_pass['callbacks']==77 and terrain_pass['committed_bytes']==172416,
          'one stage-owned terrain pass contributed the selected geometry')
    retained = bind(f,'jp_frame_surface',P,Q,U,P)
    for batch in land_batches:
        size = S()
        check(bool(retained(frame.value,batch.header[3]&0x1fffff,C.byref(size))) and size.value>0,
              'stage terrain texture remains available after stage close')
    land['single_native_terrain_pass'] = True"""),
        ("    check(land['native_placement_lifecycle_verified'],"
         "'native placement entries survive resource teardown and retire independently')\n", ''),
        ("    check(land['native_stage_resource_lifecycle_verified'],"
         "'native resource views retire and all eight resources can be reopened')\n", ''),
    ):
        assert old in raw, old
        raw = raw.replace(old, new)
    return raw


def exercise(context, inputs, track):
    direct = shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_single_terrain import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
