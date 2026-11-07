"""Verify native placement initialization for all three loaded Sonic acts."""
import ctypes as C
import hashlib
import inspect
import struct
from pathlib import Path

import verify_native_emerald_stage_loader as loader
from native_packet_test_support import bind
from verify_native_read_queue import Disc


def draw_land(context, track):
    lib = context['f']
    select = bind(lib, 'jp_emerald_set_select', C.c_int, C.c_uint)
    get = bind(lib, 'jp_emerald_set_get', C.c_int, C.c_uint, C.c_void_p)
    close = bind(lib, 'jp_emerald_set_close', C.c_int)
    entry = C.create_string_buffer(32)
    assert select(0) == -1 and get(0, entry) == -1
    result, release = loader.draw_land(context, track)
    records = []
    disc = Disc(track)
    try:
        files = disc.files()
        for act, name in enumerate(loader.RAW_FILES[:3]):
            fad, size, directory = files[name]
            assert not directory
            data = disc.sectors(fad, (size + 2047) // 2048)[:size]
            count, = struct.unpack_from('<I', data)
            assert select(act) == count
            for index in range(count):
                assert get(index, entry) == 0
                assert entry.raw == data[32 + index * 32:64 + index * 32], (act, index)
            assert get(count, entry) == -1 and select(3) == -1
            assert get(0, entry) == 0 and entry.raw == data[32:64]
            records.append(dict(act=act, entries=count,
                                entries_sha256=hashlib.sha256(data[32:32 + count * 32]).hexdigest()))
        last_first = entry.raw
    except BaseException:
        close()
        release()
        raise
    finally:
        disc.f.close()
    result['native_placement_registry'] = records

    def retire():
        try:
            release()
            assert select(0) == -1
            assert get(0, entry) == 0 and entry.raw == last_first
        finally:
            assert close() == 0
        assert close() == 0 and get(0, entry) == -1
        result['native_placement_lifecycle_verified'] = True

    return result, retire


def scene_caller(raw):
    raw = loader.scene_caller(raw).replace(
        'from verify_native_emerald_stage_loader import draw_land',
        'from verify_native_emerald_set import draw_land').replace(
        'sonic_emerald_stage_loader', 'sonic_emerald_set').replace(
        'native_Emerald_Coast_Sonic_loader_schedule_and_GPU_submission_verified',
        'native_Emerald_Coast_Sonic_placement_registry_and_GPU_submission_verified')
    anchor = '    release_land()\n'
    assert raw.count(anchor) == 1
    return raw.replace(anchor, anchor +
        "    check(land['native_placement_lifecycle_verified'],"
        "'native placement entries survive resource teardown and retire independently')\n")


def exercise(context, inputs, track):
    direct = loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_set import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
