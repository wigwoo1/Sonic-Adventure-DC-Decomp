"""Verify the live POINT view and the Windows scene using it."""
import ctypes as C
import inspect
import math
from pathlib import Path

import verify_native_emerald_camera_mode as mode
from native_packet_test_support import bind


def verify_view(lib, selected):
    I, P = C.c_int, C.c_void_p
    point_view = bind(lib, 'jp_emerald_point_view', I, P, P)
    current = bind(lib, 'njGetMatrix', None, P)
    matrix_type = C.c_float*16
    matrix, before, after = matrix_type(), matrix_type(), matrix_type()
    player = mode.contacts.Vec(-71.45, 3.34, 36.71)
    view = P.in_dll(lib, '_nj_view_')
    view_matrix = matrix_type.in_dll(lib, '_nj_view_matrix_')
    depth = I.in_dll(lib, '_stack_size_')
    saved = (view.value, bytes(view_matrix), depth.value)
    current(before)
    assert point_view(player, matrix) == 0
    current(after)
    assert bytes(before) == bytes(after)
    assert (view.value, bytes(view_matrix), depth.value) == saved

    target = (player[0], player[1]+7.5, player[2])
    vector = [a-b for a,b in zip(player, selected.target)]
    length = math.sqrt(sum(v*v for v in vector))
    eye = [p + selected.distance*v/length for p,v in zip(target, vector)]

    def project(point):
        return [sum(point[i]*matrix[4*i+j] for i in range(3)) + matrix[12+j]
                for j in range(3)]

    assert max(map(abs, project(eye))) < 0.002
    mapped = project(target)
    assert max(abs(mapped[0]), abs(mapped[1]), abs(mapped[2]-selected.distance)) < 0.03
    return dict(eye_maps_to_origin=True, target_depth=mapped[2], matrix_state_restored=True)


def collect(context, triangles, draw, track):
    source = inspect.getsource(mode.collect)
    anchor = '        observed.update(row=state.row'
    assert source.count(anchor) == 1
    source = source.replace(anchor, "        observed['point_view'] = verify_view(context['f'], state)\n" + anchor)
    namespace = dict(vars(mode), verify_view=verify_view)
    exec(compile(source, mode.__file__, 'exec'), namespace)
    return namespace['collect'](context, triangles, draw, track)


def scene_caller(raw):
    return mode.scene_caller(raw).replace(
        'from verify_native_emerald_camera_mode import collect',
        'from verify_native_emerald_camera_view import collect').replace(
        'sonic_emerald_camera_mode', 'sonic_emerald_camera_view').replace(
        'native_Emerald_Coast_live_camera_mode_verified',
        'native_Emerald_Coast_POINT_camera_view_verified')


def exercise(context, inputs, track):
    direct = mode.contacts.stage.session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = mode.contacts.stage.session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_camera_view import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
