"""Verify native stage startup owns the retail bank and ring resources."""
import inspect
from pathlib import Path

import verify_native_emerald_ring_audio as audio


def session_walk(lib, triangles, draw):
    walk = audio.sparkle.finish.draw.model.walk
    source = inspect.getsource(walk.collect)
    start = source.index('    open_stage =')
    end = source.index('    start =', start)
    source = source[:start] + """    open_stage = bind(lib, 'jp_emerald_stage_open', I, C.c_uint, P, C.c_size_t)
    close_stage = bind(lib, 'jp_emerald_stage_close', I)
""" + source[end:]
    replacements = (
        ('    floor = bind(lib, \'jp_emerald_ring_floor\', None, P, C.c_size_t)\n', ''),
        ('    select = bind(lib, \'jp_emerald_set_select\', I, C.c_uint)\n', ''),
        ('    assert open_stage(1) == 0',
         '''    count = len(triangles)//TRIANGLE.size
    assert close_stage() == 0
    assert open_stage(3, buffer, count) == -1
    assert open_stage(0, None, count) == -1
    assert open_stage(0, buffer, count) == 0'''),
        ('        assert ring(1) == 0 and select(0) == 786',
         '        assert open_stage(0, buffer, count) == -1'),
        ('        floor(buffer, len(triangles)//TRIANGLE.size)\n', ''),
        ('        floor(None, 0)\n', ''),
        ('        assert close_set() == 0 and ring(0) == 0 and close_stage() == 0',
         '''        assert close_stage() == 0 and close_stage() == 0
        assert not task_at(145) and get(145, row) == -1'''),
    )
    for old, new in replacements:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    namespace = dict(vars(walk))
    exec(compile(source, walk.__file__, 'exec'), namespace)
    return namespace['collect'](lib, triangles, draw)


def collect(context, triangles, draw):
    source = inspect.getsource(audio.collect)
    load = "    load = bind(sound, 'jp_effects_load_file', I, C.c_char_p, U, I)\n"
    call = "    assert load(b'EMERALDCOAST.MLT', 0, 0) == 0\n"
    walk = "sparkle.finish.draw.model.walk.collect(context['f'], triangles, draw)"
    for anchor in (load, call, walk):
        assert source.count(anchor) == 1
    source = source.replace(load, '').replace(call, '').replace(
        walk, "session_walk(context['f'], triangles, draw)")
    anchor = '    slots = []'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '''    assert after.loads == state.loads + 1
    assert after.loaded_bytes > state.loaded_bytes
    record['native_stage_session'] = dict(bank_loads=1, duplicate_open_rejected=True,
                                          resources_and_tasks_retired=True)
''' + anchor)
    namespace = dict(vars(audio), session_walk=session_walk)
    exec(compile(source, audio.__file__, 'exec'), namespace)
    return namespace['collect'](context, triangles, draw)


def scene_caller(raw):
    return audio.scene_caller(raw).replace(
        'from verify_native_emerald_ring_audio import collect',
        'from verify_native_emerald_stage_session import collect').replace(
        'sonic_emerald_ring_audio', 'sonic_emerald_stage_session').replace(
        'native_Emerald_Coast_ring_pickup_audio_verified',
        'native_Emerald_Coast_stage_session_pickup_audio_verified')


def exercise(context, inputs, track):
    direct = audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_session import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
