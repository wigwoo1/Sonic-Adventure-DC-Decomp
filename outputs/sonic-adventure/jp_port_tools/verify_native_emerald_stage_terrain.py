"""Verify the terrain texture and draw lifetime inside the active stage."""
import ctypes as C
import inspect
from pathlib import Path

import verify_native_emerald_stage_submit as submit
from native_packet_test_support import bind
from native_texture_test_support import List

I, P, S, U = C.c_int, C.c_void_p, C.c_size_t, C.c_uint32
camera = submit.mesh.stage.land.live.shared.view


def collect(context, triangles, draw, track):
    lib = context['f']
    open_terrain = bind(lib, 'jp_emerald_stage_terrain_open', I)
    draw_terrain = bind(lib, 'jp_emerald_stage_terrain_draw', I,I,I,I)
    textures = bind(lib, 'jp_emerald_stage_terrain_textures', C.POINTER(List))
    loaded = bind(lib, 'jp_pvm_loaded_count', U,P)
    pinned = bind(lib, 'jp_frames_pinned_bytes', S)
    used = bind(lib, 'jp_queue_committed_bytes', S,U)
    selected = P.in_dll(lib, '_nj_current_texlist')
    info = {}
    assert open_terrain() == -1 and not textures()

    def draw_live():
        status = draw()
        assert open_terrain() == 0 and open_terrain() == -1
        listing = textures()
        count = listing.contents.nbTexture
        assert count == loaded(listing) and count > 50
        matrix = (C.c_float*16)()
        bind(lib, 'njGetMatrix', None, P)(matrix)
        set_matrix = bind(lib, 'njSetMatrix', None, P,P)
        saved = selected.value
        set_matrix(None, context['shared_point_view'])
        try:
            before = used(0)+used(1)
            pins_before = pinned()
            assert draw_terrain(-1, 0, 0) == 0 and used(0)+used(1) == before
            assert draw_terrain(-1, 1, 0) == 77
            assert selected.value == saved
            committed = used(0)+used(1)-before
            assert committed == 172416 and pinned() > pins_before
            info.update(loaded_textures=count, callbacks=77, committed_bytes=committed,
                        pinned_bytes=pinned(), current_texture_restored=True)
        finally:
            set_matrix(None, matrix)
        return status

    result = camera.collect(context, triangles, draw_live, track)
    assert not textures() and draw_terrain(-1, 1, 0) == -1
    assert pinned() == info['pinned_bytes']
    info['recording_keeps_texture_pins_after_stage_close'] = True
    result['native_stage_terrain'] = info
    return result


def scene_caller(raw):
    raw = submit.mesh.stage.land.scene_caller(raw)
    for old, new in (
        ('from verify_native_emerald_camera_view import collect',
         'from verify_native_emerald_stage_terrain import collect'),
        ('sonic_emerald_land_pass', 'sonic_emerald_stage_terrain'),
        ('native_Emerald_Coast_native_land_pass_verified',
         'native_Emerald_Coast_stage_texture_and_draw_ownership_verified'),
    ):
        assert old in raw
        raw = raw.replace(old, new)
    return raw


def exercise(context, inputs, track):
    direct = submit.mesh.stage.land.live.shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = submit.mesh.stage.land.live.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_terrain import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
