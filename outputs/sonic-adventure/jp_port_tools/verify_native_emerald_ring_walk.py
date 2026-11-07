"""Walk the native Sonic task into an original Emerald Coast ring."""
import ctypes as C
import inspect
from pathlib import Path
import struct

import verify_native_emerald_ring_score as score
from extract_native_emerald_collision import TRIANGLE
from native_packet_test_support import bind


def collect(lib, triangles, draw):
    I, P = C.c_int, C.c_void_p
    open_stage = bind(lib, 'jp_stage_resources_open', I, I)
    close_stage = bind(lib, 'jp_stage_resources_close', I)
    close_set = bind(lib, 'jp_emerald_set_close', I)
    ring = bind(lib, 'jp_emerald_ring_bind', I, I)
    start = bind(lib, 'jp_emerald_ring_walk_start', None, P)
    floor = bind(lib, 'jp_emerald_ring_floor', None, P, C.c_size_t)
    select = bind(lib, 'jp_emerald_set_select', I, C.c_uint)
    get = bind(lib, 'jp_emerald_set_get', I, C.c_uint, P)
    task_at = bind(lib, 'jp_emerald_placement_task', P, C.c_uint)
    rings = C.c_int16.in_dll(lib, 'rings')
    lives = C.c_int8.in_dll(lib, 'lives')
    before = rings.value
    before_lives = lives.value
    buffer = C.create_string_buffer(triangles)
    assert open_stage(1) == 0
    try:
        assert ring(1) == 0 and select(0) == 786
        row = C.create_string_buffer(32)
        assert get(145, row) == 0
        kind, _, _, _, x, y, z, *_ = struct.unpack('<H3h6f', row.raw)
        assert kind & 0x7fff == 0
        floor(buffer, len(triangles)//TRIANGLE.size)
        # Begin just beyond the combined nine-unit radius. The existing
        # seven input ticks move Sonic only a short distance.
        start((C.c_float*3)(x+9.05, y, z))
        assert draw() == 0
        assert rings.value == before + 1, (before, rings.value)
        target = task_at(145)
        task_type = score.contact.rings.tasks.Task
        assert target and task_type.from_address(target).work.contents.header & 255 == 2
        return dict(original_set_index=145, rings_awarded=1, native_sonic_shape=True,
                    diagnostic_start_offset=9.05)
    finally:
        start(None)
        floor(None, 0)
        rings.value = before
        lives.value = before_lives
        assert close_set() == 0 and ring(0) == 0 and close_stage() == 0


def scene_caller(raw):
    raw = score.scene_caller(raw).replace(
        'sonic_emerald_ring_score', 'sonic_emerald_ring_walk').replace(
        'native_Emerald_Coast_ring_and_life_accounting_verified',
        'native_Emerald_Coast_live_Sonic_ring_pickup_verified')
    begin = "            check(draw(listing,aura_view.data,action_view.data,1,0)==0,"
    end = "            from verify_native_emerald_ring_score import draw_land"
    assert raw.count(begin) == raw.count(end) == 1
    raw = raw[:raw.index(begin)] + '''            from verify_native_emerald_ring_walk import collect
            ring_walk = collect(f,triangles,lambda: draw(listing,aura_view.data,action_view.data,1,0))
''' + raw[raw.index(end):]
    anchor = '            land, release_land = draw_textured_land(c,track)'
    assert raw.count(anchor) == 1
    return raw.replace(anchor, anchor + "\n            land['native_ring_walk'] = ring_walk")


def exercise(context, inputs, track):
    direct = score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_walk import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
