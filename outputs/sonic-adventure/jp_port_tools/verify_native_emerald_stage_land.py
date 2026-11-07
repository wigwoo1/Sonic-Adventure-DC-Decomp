"""Verify stage-owned terrain relocation and automatic task retirement."""
import ctypes as C
import inspect
from pathlib import Path
import struct

import verify_native_emerald_land_pass as land
from native_packet_test_support import bind
from native_pvm_test_support import PVM, destination, index_archive
from verify_native_emerald_stage_resource_owner import View
from verify_native_model_preflight import Model

I, P, U = C.c_int, C.c_void_p, C.c_uint32
Resolve = C.CFUNCTYPE(P, U, U)
Consume = C.CFUNCTYPE(None, P)


def collect(context, triangles, draw, track):
    lib = context['f']
    get = bind(lib, 'jp_stage_resources_get', I, U, P)
    attach = bind(lib, 'jp_emerald_stage_land_open', I, P, Resolve, Consume)
    render = bind(lib, 'jp_emerald_draw_land', I, I, I, I)
    close_stage = bind(lib, 'jp_emerald_stage_close', I)
    texture = P.in_dll(lib, '_nj_current_texlist')
    pvm = PVM(lib)
    pvm_owner = None
    observations = {}
    assert attach(None, Resolve(), Consume()) == -1

    def inspect_stage():
        nonlocal pvm_owner
        status = draw()
        view = View()
        assert get(0, C.byref(view)) == 0
        image = C.string_at(view.data, view.bytes)
        base = 0x0c900000
        count, _, _, _, _, table = struct.unpack_from('<hhhhfI', image, 0x0c981904-base)
        models = (Model*count)()
        addresses, seen, resolved, close_attempts = {}, [], [], []

        @Resolve
        def resolve(index, model):
            resolved.append((index, model))
            pointer = C.addressof(models[index])
            addresses[pointer] = index
            return pointer

        @Consume
        def consume(pointer):
            seen.append(addresses.get(pointer, -1))
            if len(seen) == 1:
                close_attempts.append(close_stage())

        assert get(7, C.byref(view)) == 0
        archive = C.string_at(view.data, view.bytes)
        _, rows, _ = index_archive(archive)
        names, listing, buffers = destination(len(rows))
        assert pvm.load(C.byref(listing), len(rows), 28, view.data, view.bytes) == 1
        pvm_owner = (names, listing, buffers, models, resolve, consume)
        reject = Resolve(lambda index, model: None)
        assert attach(C.byref(listing), reject, consume) == -1
        assert attach(C.byref(listing), resolve, consume) == 0
        assert attach(C.byref(listing), resolve, consume) == -1
        expected = []
        for index in range(count):
            obj, _, flags = struct.unpack_from('<III', image, table-base+index*36+24)
            if flags & 0x80000000:
                model, = struct.unpack_from('<I', image, obj-base+4)
                expected.append((index, model))
        assert resolved == expected
        matrix = (C.c_float*16)()
        bind(lib, 'njGetMatrix', None, P)(matrix)
        set_matrix = bind(lib, 'njSetMatrix', None, P,P)
        saved = texture.value
        set_matrix(None, context['shared_point_view'])
        try:
            assert render(-1, 0, 0) == 0 and not seen
            assert render(-1, 1, 0) == 77 and len(seen) == 77
            assert seen == sorted(set(seen), reverse=True) and min(seen) >= 0
            assert close_attempts == [-1]
        finally:
            set_matrix(None, matrix)
            texture.value = saved
        observations.update(relocated_drawable_objects=len(resolved), callbacks=len(seen),
                            duplicate_attach_rejected=True, failed_resolver_can_retry=True,
                            close_during_draw_rejected=True)
        return status

    try:
        result = land.live.shared.view.collect(context, triangles, inspect_stage, track)
        assert render(-1, 1, 0) == -1
        observations['task_retired_by_stage_close'] = True
        result['native_stage_land'] = observations
        return result
    finally:
        if pvm_owner:
            assert pvm.release(C.byref(pvm_owner[1])) == 1


def scene_caller(raw):
    return land.scene_caller(raw).replace(
        'from verify_native_emerald_camera_view import collect',
        'from verify_native_emerald_stage_land import collect').replace(
        'sonic_emerald_land_pass', 'sonic_emerald_stage_land').replace(
        'native_Emerald_Coast_native_land_pass_verified',
        'native_Emerald_Coast_stage_land_lifetime_verified')


def exercise(context, inputs, track):
    direct = land.live.shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = land.live.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_land import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
