"""Verify live stage mode selection and its authored camera view."""
import ctypes as C
import inspect
from pathlib import Path
import struct

import verify_native_emerald_camera_contacts as contacts
from native_packet_test_support import bind
from verify_native_read_queue import Disc


class Mode(C.Structure):
    _fields_ = [(name, C.c_int32) for name in ('row', 'mode', 'level', 'type', 'adjust', 'cut')]
    _fields_ += [('position', contacts.Vec), ('target', contacts.Vec),
                 ('pitch', C.c_int32), ('yaw', C.c_int32), ('distance', C.c_float)]


def collect(context, triangles, draw, track):
    mode = bind(context['f'], 'jp_emerald_camera_mode', C.c_int, C.POINTER(Mode))
    disc = Disc(track)
    try:
        fad, size, directory = disc.files()['CAM0100S.BIN']
        assert not directory
        data = disc.sectors(fad, (size+2047)//2048)[:size]
    finally:
        disc.f.close()
    observed = {}
    state = Mode()
    assert mode(C.byref(state)) == -1

    def camera_draw():
        assert mode(C.byref(state)) == 0 and state.row == 0
        assert state.mode == data[64] and state.level == 0 and state.adjust == 0
        observed['default_mode'] = state.mode
        result = draw()
        assert mode(C.byref(state)) == 0
        assert (state.row, state.mode, state.level) == (90, 39, 0)
        off = 64*(state.row+1)
        pitch, yaw, *view = struct.unpack_from('<HH7f', data, off+32)
        assert (state.adjust, state.pitch, state.yaw) == (data[off+2], pitch, yaw)
        assert tuple(state.target) == tuple(view[:3])
        assert tuple(state.position) == tuple(view[3:6])
        assert state.distance == view[6]
        observed.update(row=state.row, mode=state.mode, adjust=state.adjust,
                        view_matches_retail=True, position=list(state.position), target=list(state.target))
        return result

    result = contacts.collect(context, triangles, camera_draw, track)
    assert mode(C.byref(state)) == -1
    result['native_camera_mode'] = observed
    return result


def scene_caller(raw):
    return contacts.scene_caller(raw).replace(
        'from verify_native_emerald_camera_contacts import collect',
        'from verify_native_emerald_camera_mode import collect').replace(
        'sonic_emerald_camera_contacts', 'sonic_emerald_camera_mode').replace(
        'native_Emerald_Coast_camera_contacts_verified',
        'native_Emerald_Coast_live_camera_mode_verified')


def exercise(context, inputs, track):
    direct = contacts.stage.session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = contacts.stage.session.audio.sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_camera_mode import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
