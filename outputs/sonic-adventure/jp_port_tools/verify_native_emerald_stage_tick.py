"""Verify the stage-owned task phase using the native Sonic walk."""
import inspect
from pathlib import Path

import verify_native_emerald_stage_session as session


def session_walk(lib, triangles, draw):
    source = inspect.getsource(session.session_walk)
    anchor = '    namespace = dict(vars(walk))'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '''    before = '    assert open_stage(0, buffer, count) == 0'
    assert source.count(before) == 1
    source = source.replace(before, "    tick = bind(lib, 'jp_emerald_stage_tick', I, P, P)\\n"
        "    assert tick(None, None) == -1\\n" + before +
        "\\n    assert tick(None, None) == -1")
    after = '        assert not task_at(145) and get(145, row) == -1'
    assert source.count(after) == 1
    source = source.replace(after, after + '\\n        assert tick(None, None) == -1')
''' + anchor)
    namespace = dict(vars(session))
    exec(compile(source, session.__file__, 'exec'), namespace)
    result = namespace['session_walk'](lib, triangles, draw)
    result['native_stage_tick'] = dict(stage_owned=True, closed_and_null_rejected=True)
    return result


def collect(context, triangles, draw):
    source = inspect.getsource(session.collect)
    namespace = dict(vars(session), session_walk=session_walk)
    exec(compile(source, session.__file__, 'exec'), namespace)
    return namespace['collect'](context, triangles, draw)


def scene_caller(raw):
    return session.scene_caller(raw).replace(
        'from verify_native_emerald_stage_session import collect',
        'from verify_native_emerald_stage_tick import collect').replace(
        'sonic_emerald_stage_session', 'sonic_emerald_stage_tick').replace(
        'native_Emerald_Coast_stage_session_pickup_audio_verified',
        'native_Emerald_Coast_stage_tick_pickup_audio_verified')


def exercise(context, inputs, track):
    direct = session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_tick import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
