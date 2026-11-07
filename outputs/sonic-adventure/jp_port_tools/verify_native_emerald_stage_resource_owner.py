"""Exercise native stage resource startup and teardown with the Emerald Coast scene."""
import ctypes as C
import hashlib
import inspect
from pathlib import Path
from types import FunctionType

import verify_native_emerald_stage_resources as queued
from native_packet_test_support import bind
from verify_native_read_queue import Info


class View(C.Structure):
    _fields_ = [('data', C.c_void_p), ('bytes', C.c_size_t)]


def draw_land(context, track):
    lib = context['f']
    open_stage = bind(lib, 'jp_stage_resources_open', C.c_int, C.c_int)
    close_stage = bind(lib, 'jp_stage_resources_close', C.c_int)
    get = bind(lib, 'jp_stage_resources_get', C.c_int, C.c_uint, C.POINTER(View))
    queue = context['queue']
    snapshot = context['snapshot']

    def completed():
        return snapshot(queue, 'jp_read_queue_snapshot', Info).completed

    view = View()
    before = completed()
    assert get(0, C.byref(view)) == -1 and open_stage(0) == -1
    assert completed() == before
    assert open_stage(1) == 0 and completed() == before + 2

    def load_resources(_context, _track):
        resources = []
        records = []
        for index, name in enumerate(queued.FILES):
            assert get(index, C.byref(view)) == 0 and view.data
            raw = C.string_at(view.data, view.bytes)
            digest = hashlib.sha256(raw).hexdigest()
            assert digest == queued.DECODED_SHA256[name], name
            resources.append(raw)
            records.append(dict(name=name, decoded_bytes=len(raw), decoded_sha256=digest))
        return resources[0], resources[1], records

    try:
        assert open_stage(1) == -1 and completed() == before + 2
        assert get(2, C.byref(view)) == -1 and get(0, None) == -1
        draw = FunctionType(queued.draw_land.__code__, dict(vars(queued), load_resources=load_resources))
        result, release = draw(context, track)
    except BaseException:
        assert close_stage() == 0
        raise

    def retire():
        try:
            release()
        finally:
            assert close_stage() == 0
        assert close_stage() == 0 and get(0, C.byref(view)) == -1
        assert open_stage(1) == 0
        try:
            assert completed() == before + 4
            assert load_resources(context, track)[2] == result['native_stage_resource_reads']
        finally:
            assert close_stage() == 0
        result['native_stage_resource_lifecycle_verified'] = True

    return result, retire


def scene_caller(raw):
    raw = queued.scene_caller(raw).replace(
        'from verify_native_emerald_stage_resources import draw_land',
        'from verify_native_emerald_stage_resource_owner import draw_land').replace(
        'sonic_emerald_stage_resources', 'sonic_emerald_stage_resource_owner').replace(
        'queued_Emerald_Coast_resources_and_GPU_submission_verified',
        'native_Emerald_Coast_resource_lifecycle_and_GPU_submission_verified')
    anchor = '    release_land()\n'
    assert raw.count(anchor) == 1
    return raw.replace(anchor, anchor +
        "    check(land['native_stage_resource_lifecycle_verified'],"
        "'native resource views retire and both archives can be reopened')\n")


def exercise(context, inputs, track):
    direct = queued.owner.direct
    source = inspect.getsource(direct.exercise)
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(source, direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_resource_owner import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
