"""Exercise real scheduler tasks from the Emerald Coast SET registry."""
import ctypes as C
import inspect
import struct
from pathlib import Path

import verify_native_emerald_set as registry
from native_packet_test_support import bind

P, I, U = C.c_void_p, C.c_int, C.c_uint
Vec = C.c_float * 3
Callback = C.CFUNCTYPE(None, P)


class Work(C.Structure):
    _fields_ = [('header', C.c_uint64), ('vars', P * 3), ('angles', I * 3),
                ('pos', Vec), ('scale', Vec), ('collision', P), ('event', P)]


class Task(C.Structure):
    _fields_ = [('links', P * 4), ('step', P), ('draw', P), ('destructor', P),
                ('placement', P), ('work', C.POINTER(Work))]


def draw_land(context, track):
    lib = context['f']
    register = bind(lib, 'jp_emerald_kind_register', I, U, I, I, C.c_float, Callback)
    select = bind(lib, 'jp_emerald_set_select', I, U)
    step = bind(lib, 'jp_emerald_placement_step', I, P, P)
    get = bind(lib, 'jp_emerald_set_get', I, U, P)
    run = bind(lib, 'task_step_level', None, I)
    seen, retired = [], []

    @Callback
    def destroy(at):
        retired.append(at)

    @Callback
    def update(at):
        task = Task.from_address(at)
        work = task.work.contents
        seen.append((at, tuple(work.angles), tuple(work.pos), tuple(work.scale), task.placement))
        task.destructor = C.cast(destroy, P).value

    assert register(0, 6, 1, 0, update) == 0
    result, release = registry.draw_land(context, track)
    try:
        count = select(0)
        assert count > 0 and step(None, None) == 0
        raw = C.create_string_buffer(32)
        candidates = []
        for i in range(count):
            assert get(i, raw) == 0
            kind, *pose = struct.unpack('<H3h6f', raw.raw)
            if kind & 0x7fff == 0:
                candidates.append(pose)
        assert candidates
        first = candidates[0]
        expected = [(tuple(pose[:3]), tuple(pose[3:6]), tuple(pose[6:]))
                    for pose in candidates if pose[3:6] == first[3:6]]
        distant = Work(pos=Vec(1e6, 1e6, 1e6))
        center = Work(pos=Vec(*first[3:6]))
        assert step(C.byref(distant), C.byref(center)) == 0
        assert step(None, C.byref(center)) == len(expected)
        assert step(C.byref(center), None) == len(expected)
        run(6)
        assert [(a, p, s) for _, a, p, s, _ in seen] == expected
        assert all(placement for *_, placement in seen)
        assert select(2) == 1
        assert retired == [at for at, *_ in seen]
        run(6)
        assert len(seen) == len(expected)
        result['native_placement_task_dispatch'] = dict(
            supported_kind=0, callbacks=len(seen), destructors=len(retired),
            duplicated=False, diagnostic_callback=True)
    except BaseException:
        release()
        raise

    def retire():
        release()
        assert step(C.byref(center), None) == -1
        assert register(0, 6, 0, 0, Callback()) == 0
        assert update and destroy  # Keep both bindings alive through retirement.

    return result, retire


def scene_caller(raw):
    return registry.scene_caller(raw).replace(
        'from verify_native_emerald_set import draw_land',
        'from verify_native_emerald_placement_tasks import draw_land').replace(
        'sonic_emerald_set', 'sonic_emerald_placement_tasks').replace(
        'native_Emerald_Coast_Sonic_placement_registry_and_GPU_submission_verified',
        'native_Emerald_Coast_placement_tasks_and_GPU_submission_verified')


def exercise(context, inputs, track):
    direct = registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_placement_tasks import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
