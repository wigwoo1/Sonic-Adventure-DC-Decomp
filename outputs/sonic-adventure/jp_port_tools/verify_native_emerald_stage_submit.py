"""Exercise bounded GPU submission from stage-owned mesh views."""
import inspect
from pathlib import Path

import verify_native_emerald_stage_mesh as mesh


def adapt(source):
    anchor = '        seen = []'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '''        seen = []
        submissions = []
        submit = bind(lib, 'jp_emerald_stage_mesh_submit', I,P)
        used = bind(lib, 'jp_queue_committed_bytes', C.c_size_t,U)
''')
    anchor = '            seen.append(pointer)'
    assert source.count(anchor) == 1
    source = source.replace(anchor, anchor + '''
            before = used(0)+used(1)
            code = submit(pointer)
            submissions.append((code, used(0)+used(1)-before))
''')
    anchor = '        order = context'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '''        assert len(submissions) == 77
        assert all(code == 0 for code, _ in submissions), submissions
        packet_bytes = sum(size for _, size in submissions)
        assert packet_bytes > 0
        observations['native_submission'] = dict(models=len(submissions),
            models_with_packets=sum(size>0 for _, size in submissions),
            packet_bytes=packet_bytes)
''' + anchor)
    return source


def collect(context, triangles, draw, track):
    source = inspect.getsource(mesh.collect)
    anchor = '    namespace = dict(vars(stage), check_mesh=check_mesh)'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '    source = adapt(source)\n' + anchor)
    namespace = dict(vars(mesh), adapt=adapt)
    exec(compile(source, mesh.__file__, 'exec'), namespace)
    return namespace['collect'](context, triangles, draw, track)


def scene_caller(raw):
    return mesh.scene_caller(raw).replace(
        'from verify_native_emerald_stage_mesh import collect',
        'from verify_native_emerald_stage_submit import collect').replace(
        'sonic_emerald_stage_mesh', 'sonic_emerald_stage_submit').replace(
        'native_Emerald_Coast_stage_mesh_relocation_verified',
        'native_Emerald_Coast_stage_mesh_submission_verified')


def exercise(context, inputs, track):
    direct = mesh.stage.land.live.shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = mesh.stage.land.live.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_submit import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
