"""Check all retail kind rows and spawn with the original ring metadata."""
from collections import Counter
import ctypes as C
import hashlib
import inspect
from pathlib import Path
import struct
import sys

import verify_native_emerald_placement_tasks as tasks
from verify_native_read_queue import Disc


def compare_table(context, track, read):
    sys.path.insert(0, str(context['r']/'outputs/sonic-adventure'))
    from decompress_prs import decompress

    disc = Disc(track)
    try:
        fad, size, directory = disc.files()['STG01.PRS']
        assert not directory
        image, _ = decompress(disc.sectors(fad, (size + 2047) // 2048)[:size])
    finally:
        disc.f.close()
    count, table = struct.unpack_from('<H2xI', image, 0x1a92d8)
    assert count == 84 and table == 0x0caa8c48
    offset = table - 0x0c900000
    row = C.create_string_buffer(20)
    for i in range(count):
        assert read(i, row) == 0 and row.raw == image[offset + i*20:offset + (i+1)*20]
    assert read(count, row) == -1
    first = image[offset:offset+20]
    assert struct.unpack_from('<BBhfI', first) == (2, 3, 0, 0, 0)
    return dict(rows=count, sha256=hashlib.sha256(image[offset:offset+count*20]).hexdigest(),
                kind_zero_task_level=3, kind_zero_uses_default_range=True)


def draw_land(context, track):
    source = inspect.getsource(tasks.draw_land)
    changes = (
        ("register = bind(lib, 'jp_emerald_kind_register', I, U, I, I, C.c_float, Callback)",
         "register = bind(lib, 'jp_emerald_kind_bind', I, U, Callback)\n"
         "    read = bind(lib, 'jp_emerald_kind_read', I, U, P)\n"
         "    close = bind(lib, 'jp_emerald_set_close', I)"),
        ('assert register(0, 6, 1, 0, update) == 0',
         'assert read(0, C.create_string_buffer(20)) == -1'),
        ('        count = select(0)',
         '        assert close() == 0\n'
         '        result["native_kind_table"] = compare_table(context, track, read)\n'
         '        assert register(0, update) == 0\n'
         '        count = select(0)'),
        ('if pose[3:6] == first[3:6]',
         'if sum((a-b)**2 for a,b in zip(pose[3:6], first[3:6])) <= 400**2'),
        ('assert [(a, p, s) for _, a, p, s, _ in seen] == expected',
         'assert Counter((a, p, s) for _, a, p, s, _ in seen) == Counter(expected)'),
        ('assert retired == [at for at, *_ in seen]',
         'assert Counter(retired) == Counter(at for at, *_ in seen)'),
        ('register(0, 6, 0, 0, Callback())', 'register(0, Callback())'),
    )
    for old, new in changes:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    assert source.count('run(6)') == 2
    source = source.replace('run(6)', 'run(3)')
    namespace = dict(vars(tasks), compare_table=compare_table, Counter=Counter)
    exec(compile(source, tasks.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    return tasks.scene_caller(raw).replace(
        'from verify_native_emerald_placement_tasks import draw_land',
        'from verify_native_emerald_kind_table import draw_land').replace(
        'sonic_emerald_placement_tasks', 'sonic_emerald_kind_table').replace(
        'native_Emerald_Coast_placement_tasks_and_GPU_submission_verified',
        'native_Emerald_Coast_retail_kind_table_and_GPU_submission_verified')


def exercise(context, inputs, track):
    direct = tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_kind_table import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
