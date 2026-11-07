"""Check the Sonic beach loader schedule through the live disc queue and GPU."""
import ctypes as C
import hashlib
import inspect
from pathlib import Path

import verify_native_emerald_stage_resource_owner as owner
from verify_native_read_queue import Disc

RAW_FILES = ('SET0100S.BIN', 'SET0101S.BIN', 'SET0102S.BIN',
             'CAM0100S.BIN', 'CAM0101S.BIN', 'CAM0102S.BIN')


def verify_raw(track, get):
    disc = Disc(track)
    try:
        files = disc.files()
        rows = []
        for index, name in enumerate(RAW_FILES, 1):
            view = owner.View()
            fad, size, directory = files[name]
            assert not directory and get(index, C.byref(view)) == 0
            assert view.data and view.bytes == size
            expected = disc.sectors(fad, (size + 2047) // 2048)[:size]
            assert C.string_at(view.data, size) == expected, name
            rows.append(dict(name=name, bytes=size, sha256=hashlib.sha256(expected).hexdigest()))
        return rows
    finally:
        disc.f.close()


def draw_land(context, track):
    source = inspect.getsource(owner.draw_land)
    changes = (
        ('before + 2', 'before + 8', 2),
        ('before + 4', 'before + 16', 1),
        ('get(2, C.byref(view))', 'get(8, C.byref(view))', 1),
        ('get(index, C.byref(view))', 'get(index * 7, C.byref(view))', 1),
        ("        result, release = draw(context, track)",
         "        result, release = draw(context, track)\n"
         "        result['native_stage_set_and_camera'] = verify_raw(track, get)", 1),
        ("        result['native_stage_resource_lifecycle_verified'] = True",
         "        assert result['native_stage_set_and_camera'] == reopened_raw\n"
         "        result['native_stage_resource_lifecycle_verified'] = True", 1),
        ("            assert completed() == before + 16",
         "            assert completed() == before + 16\n"
         "            reopened_raw = verify_raw(track, get)", 1),
    )
    for old, new, count in changes:
        assert source.count(old) == count, old
        source = source.replace(old, new)
    namespace = dict(vars(owner), verify_raw=verify_raw)
    exec(compile(source, owner.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    return owner.scene_caller(raw).replace(
        'from verify_native_emerald_stage_resource_owner import draw_land',
        'from verify_native_emerald_stage_loader import draw_land').replace(
        'sonic_emerald_stage_resource_owner', 'sonic_emerald_stage_loader').replace(
        'native_Emerald_Coast_resource_lifecycle_and_GPU_submission_verified',
        'native_Emerald_Coast_Sonic_loader_schedule_and_GPU_submission_verified').replace(
        'native resource views retire and both archives can be reopened',
        'native resource views retire and all eight resources can be reopened')


def exercise(context, inputs, track):
    direct = owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_loader import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
