"""Verify authored camera regions and the native stage's contact sampling."""
from collections import Counter
import ctypes as C
import inspect
import math
from pathlib import Path
import struct

import verify_native_emerald_stage_tick as stage
from native_packet_test_support import bind
from verify_native_read_queue import Disc

I, U, P, S = C.c_int, C.c_uint, C.c_void_p, C.c_size_t
Vec = C.c_float * 3


def check_layouts(lib, track):
    query = bind(lib, 'jp_emerald_camera_contacts', I, U,P,P,P,P,S)
    results = (C.c_uint16*511)()
    disc = Disc(track)
    checked = Counter()
    try:
        files = disc.files()
        for act in range(3):
            fad, size, directory = files[f'CAM010{act}S.BIN']
            assert not directory
            data = disc.sectors(fad, (size+2047)//2048)[:size]
            count, = struct.unpack_from('<I', data)

            def contacts(now, previous, older):
                args = (act, Vec(*now), Vec(*previous), Vec(*older))
                n = query(*args, results, len(results))
                assert 0 <= n <= count and list(results[:n]) == sorted(set(results[:n]))
                return set(results[:n])

            for index in range(count):
                _, _, _, kind, ax, ay, x,y,z, sx,sy,sz = struct.unpack_from(
                    '<bbBbHH6f', data, 64*(index+1))
                origin = (x, y-4, z)
                if kind == 0:
                    assert (index in contacts(origin, origin, origin)) == (sz > 0)
                elif kind == 2:
                    assert (index in contacts(origin, origin, origin)) == (
                        min(sx,sy,sz) >= 0 and 64 <= sx*sx+sy*sy+sz*sz)
                else:
                    assert kind == 1
                    rx, ry = ax*math.tau/65536, ay*math.tau/65536
                    normal = (math.sin(ry), -math.sin(rx)*math.cos(ry), math.cos(rx)*math.cos(ry))
                    previous = tuple(a+b for a,b in zip(origin, normal))
                    older = tuple(a-b for a,b in zip(origin, normal))
                    assert index not in contacts(origin, previous, previous)
                    assert (index in contacts(origin, previous, older)) == (
                        min(sx,sy) > 0 and 64 <= sx*sx+sy*sy)
                checked[kind] += 1
            assert not contacts((1e7,)*3, (1e7,)*3, (1e7,)*3)
    finally:
        disc.f.close()
    return dict(spheres=checked[0], planes=checked[1], blocks=checked[2])


def collect(context, triangles, draw, track):
    lib = context['f']
    last = bind(lib, 'jp_emerald_camera_last', I,P,S)
    observations = {}

    def camera_draw():
        assert last(None, 0) == 0
        observations['rows_checked'] = check_layouts(lib, track)
        status = draw()
        indices = (C.c_uint16*511)()
        count = last(indices, len(indices))
        assert 0 < count <= 93 and list(indices[:count]) == sorted(set(indices[:count]))
        observations['walking_contacts'] = list(indices[:count])
        assert last(None, 0) == count
        return status

    assert last(None, 0) == -1
    result = stage.collect(context, triangles, camera_draw)
    assert last(None, 0) == -1
    result['native_camera_contacts'] = observations
    return result


def scene_caller(raw):
    raw = stage.scene_caller(raw).replace(
        'from verify_native_emerald_stage_tick import collect',
        'from verify_native_emerald_camera_contacts import collect').replace(
        'sonic_emerald_stage_tick', 'sonic_emerald_camera_contacts').replace(
        'native_Emerald_Coast_stage_tick_pickup_audio_verified',
        'native_Emerald_Coast_camera_contacts_verified')
    anchor = 'ring_walk = collect(c,triangles,lambda: draw(listing,aura_view.data,action_view.data,1,0))'
    assert raw.count(anchor) == 1
    return raw.replace(anchor, anchor[:-1] + ',track)')


def exercise(context, inputs, track):
    direct = stage.session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = stage.session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_camera_contacts import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
