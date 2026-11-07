"""Verify real ring callbacks initialize scheduler-owned collision records."""
import ctypes as C
import inspect
import math
import os
from pathlib import Path
import struct

import verify_native_emerald_placement_tasks as tasks
from extract_native_emerald_collision import TRIANGLE, surfaces

I, U, P = tasks.I, tasks.U, tasks.P


class Shape(C.Structure):
    _fields_ = [('flags', U), ('levels', C.c_uint8), ('push', C.c_uint8),
                ('center', tasks.Vec), ('radius', C.c_float)]


class Collision(C.Structure):
    _fields_ = [('kind', C.c_uint16), ('flags', C.c_uint16),
                ('count', C.c_uint16), ('current', C.c_uint16),
                ('radius', C.c_float), ('shape', C.POINTER(Shape)), ('owner', P)]


def draw_land(context, track):
    lib = context['f']
    bind = tasks.bind
    ring = bind(lib, 'jp_emerald_ring_bind', I, I)
    floor = bind(lib, 'jp_emerald_ring_floor', None, P, C.c_size_t)
    select = bind(lib, 'jp_emerald_set_select', I, U)
    close = bind(lib, 'jp_emerald_set_close', I)
    get = bind(lib, 'jp_emerald_set_get', I, U, P)
    placement_task = bind(lib, 'jp_emerald_placement_task', P, U)
    step = bind(lib, 'jp_emerald_placement_step', I, P, P)
    viewers = bind(lib, 'jp_emerald_placement_viewers', None, P, P, P, I)
    tick = bind(lib, 'task_step_level', None, I)
    result, release = tasks.registry.draw_land(context, track)
    try:
        assert close() == 0 and ring(1) == 0
        triangles = (Path(os.environ['JP_SONIC_EMERALD_TEST_GEOMETRY'])/'act0_triangles.bin').read_bytes()
        buffer = C.create_string_buffer(triangles)
        floor(buffer, len(triangles) // TRIANGLE.size)
        count = select(0)
        entry = C.create_string_buffer(32)
        candidates = {}
        for index in range(count):
            assert get(index, entry) == 0
            kind, *values = struct.unpack('<H3h6f', entry.raw)
            if kind & 0x7fff == 0:
                candidates[index] = values
        first = next(iter(candidates.values()))
        center = tasks.Work(pos=tasks.Vec(*first[3:6]))
        viewers(C.byref(center), None, None, 0)
        expected = {i for i, pose in candidates.items()
                    if sum((a-b)**2 for a,b in zip(pose[3:6], center.pos)) <= 400**2}
        assert step(C.byref(center), None) == len(expected)
        live = {i: at for i in range(count) if (at := placement_task(i))}
        assert set(live) == expected
        tick(3)
        collisions = set()
        for index, at in live.items():
            task = tasks.Task.from_address(at)
            work = task.work.contents
            assert work.header & 255 == 1 and (work.header >> 16) & 255 == 11
            x, y, z = work.pos
            heights = [h for h, _, _, up in surfaces(triangles, x, z) if up and h <= y]
            assert math.isclose(work.scale[1], max(heights, default=-1e6), abs_tol=0.002)
            assert work.scale[0] == -2 and work.scale[2] == -4
            collision = Collision.from_address(work.collision)
            assert collision.kind == 7 and collision.count == 1 and collision.flags == 0x820
            assert collision.owner == at and math.isclose(collision.radius, 5.05, abs_tol=1e-6)
            shape = collision.shape.contents
            assert shape.flags == 0 and tuple(shape.center) == (0, 0, 0) and shape.radius == 5
            collisions.add(work.collision)
        assert len(collisions) == len(live)
        tick(3)
        assert {tasks.Task.from_address(at).work.contents.collision for at in live.values()} == collisions
        viewers(None, None, None, 0)
        tick(3)
        assert all(placement_task(i) for i in live)
        tick(3)
        assert not any(placement_task(i) for i in live)
        assert close() == 0 and ring(0) == 0 and select(2) == 1
        result['native_ring_initialization'] = dict(initialized=len(live), distinct_colliders=len(collisions),
                                                    ground_matches=True, range_retired=len(live))
    finally:
        floor(None, 0)
        viewers(None, None, None, 0)
    return result, release


def scene_caller(raw):
    return tasks.registry.scene_caller(raw).replace(
        'from verify_native_emerald_set import draw_land',
        'from verify_native_emerald_ring_init import draw_land').replace(
        'sonic_emerald_set', 'sonic_emerald_ring_init').replace(
        'native_Emerald_Coast_Sonic_placement_registry_and_GPU_submission_verified',
        'native_Emerald_Coast_ring_initialization_verified')


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
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_init import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
