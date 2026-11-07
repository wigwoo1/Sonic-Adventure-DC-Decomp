"""Verify an actual walking pickup queues and renders its retail sound."""
import ctypes as C
import hashlib
import inspect
from pathlib import Path

import verify_native_emerald_ring_sparkle as sparkle
from native_packet_test_support import bind
from verify_native_effects import Info

I, P, U, Q = C.c_int, C.c_void_p, C.c_uint32, C.c_uint64


def collect(context, triangles, draw):
    sound, fs = context['sound'], context['fs']
    load = bind(sound, 'jp_effects_load_file', I, C.c_char_p, U, I)
    queue = bind(sound, 'jp_effects_queue', I)
    render = bind(sound, 'jp_sound_render', I, U, P, Q)
    service = bind(sound, 'jp_sound_service', I)
    snapshot = bind(sound, 'jp_effects_snapshot', I, P)
    peer = bind(sound, 'peer_effects_slot', I, U, P)
    state, after = Info(), Info()
    assert bind(fs, 'gdFsChangeDir', I, C.c_char_p)(b'/SONICADV') == 0
    assert load(b'EMERALDCOAST.MLT', 0, 0) == 0
    # The scene clock owns firmware progression until explicitly stopped.
    assert bind(sound, 'jp_sound_clock_stop', I, U)(1) == 0
    block = (C.c_int16*256)()
    for _ in range(32):
        assert service() == 0 and render(128, block, C.sizeof(block)) == 0
        assert not any(block)
    assert snapshot(C.byref(state)) == 0
    record = sparkle.finish.draw.model.walk.collect(context['f'], triangles, draw)
    assert snapshot(C.byref(after)) == 0 and after.requests == state.requests + 1
    slots = []
    for index in range(6):
        row = (C.c_int64*13)()
        assert peer(index, row) == 0
        if list(row[:5]) == [-1, 120, 0, 0x11, 7]:
            slots.append(index)
    assert len(slots) == 1, slots
    assert queue() == 0
    row = (C.c_int64*13)()
    assert peer(slots[0], row) == 0 and row[3] == 0x10
    pcm = bytearray()
    for _ in range(690):
        assert service() == 0
        assert render(128, block, C.sizeof(block)) == 0
        pcm.extend(bytes(block))
    assert any(pcm)
    record['native_ring_audio'] = dict(effect=7, original_slot=slots[0], frames=len(pcm)//4,
                                       silent_before_pickup=True,
                                       pcm_sha256=hashlib.sha256(pcm).hexdigest())
    return record


def scene_caller(raw):
    raw = sparkle.scene_caller(raw).replace(
        'from verify_native_emerald_ring_walk import collect',
        'from verify_native_emerald_ring_audio import collect').replace(
        'ring_walk = collect(f,triangles,', 'ring_walk = collect(c,triangles,').replace(
        'sonic_emerald_ring_sparkle', 'sonic_emerald_ring_audio').replace(
        'native_Emerald_Coast_original_ring_sparkles_verified',
        'native_Emerald_Coast_ring_pickup_audio_verified')
    return raw


def exercise(context, inputs, track):
    direct = sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_audio import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
