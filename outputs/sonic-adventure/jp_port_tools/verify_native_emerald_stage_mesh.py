"""Check native mesh headers and spans against the loaded retail image."""
import ctypes as C
import inspect
from pathlib import Path
import struct

import verify_native_emerald_stage_land as stage
from native_packet_test_support import bind
from verify_native_model_preflight import Model, Span


def check_mesh(lib, pointer, image, image_pointer, address):
    native = Model.from_address(pointer)
    def guest(offset, layout):
        return struct.unpack_from(layout, image, offset-0x0c900000)
    points, normals, n, meshes, materials, nm, nt, *bounds = guest(address, '<5I2H4f')
    def host(offset):
        return image_pointer + offset - 0x0c900000 if offset else None
    assert C.cast(native.points, C.c_void_p).value == host(points)
    assert C.cast(native.normals, C.c_void_p).value == host(normals)
    assert C.cast(native.materials, C.c_void_p).value == host(materials)
    assert (native.point_count, native.mesh_count, native.material_count) == (n,nm,nt)
    assert (native.center.x,native.center.y,native.center.z,native.radius) == tuple(bounds)
    spans = C.POINTER(Span)()
    extent = bind(lib, 'jp_emerald_stage_mesh_bounds', C.c_int, C.c_void_p, C.c_void_p)
    assert extent(pointer, C.byref(spans)) == nm
    for i in range(nm):
        kind, count, indices, _, _, _, uv = guest(meshes+24*i, '<HH5I')
        mesh = native.meshes[i]
        assert (mesh.kind, mesh.count, C.cast(mesh.indices, C.c_void_p).value, mesh.uvs) == (
            kind, count, host(indices), host(uv))
        cursor = indices
        corners = 0
        for _ in range(count):
            mode = kind >> 14
            if mode == 3:
                length, = guest(cursor, '<H')
                length &= 0x7fff
                cursor += 2
            else:
                length = mode+3
            corners += length
            cursor += length*2
        assert (spans[i].indices, spans[i].uvs) == ((cursor-indices)//2, corners if uv else 0)
    return nm


def collect(context, triangles, draw, track):
    source = inspect.getsource(stage.collect)
    source = source.replace("bind(lib, 'jp_emerald_stage_land_open', I, P, Resolve, Consume)",
                            "bind(lib, 'jp_emerald_stage_land_open_native', I, P, Consume)")
    source = source.replace('attach(None, Resolve(), Consume())', 'attach(None, Consume())')
    source = source.replace('    observations = {}', '    observations = {}\n    saved_models = []')
    begin = '        models = (Model*count)()'
    end = '        assert get(7, C.byref(view)) == 0'
    assert source.count(begin) == source.count(end) == 1
    source = source[:source.index(begin)] + '''        image_pointer = view.data
        seen = []

        @Consume
        def consume(pointer):
            seen.append(pointer)

''' + source[source.index(end):]
    begin = '        pvm_owner = (names, listing, buffers, models, resolve, consume)'
    end = '        matrix = (C.c_float*16)()'
    assert source.count(begin) == source.count(end) == 1
    source = source[:source.index(begin)] + '''        pvm_owner = (names, listing, buffers, consume)
        assert attach(C.byref(listing), consume) == 0
        assert attach(C.byref(listing), consume) == -1
''' + source[source.index(end):]
    begin = '            assert seen == sorted(set(seen), reverse=True)'
    end = '        finally:'
    assert source.count(begin) == source.count(end) == 1
    source = source[:source.index(begin)] + source[source.index(end):]
    begin = '        observations.update(relocated_drawable_objects'
    end = '        return status'
    assert source.count(begin) == source.count(end) == 1
    source = source[:source.index(begin)] + '''        order = context['native_mesh_expected_order']
        assert len(order) == len(seen)
        seen_models = {}
        meshsets = 0
        for index, pointer in zip(order, seen):
            obj, = struct.unpack_from('<I', image, table-base+index*36+24)
            address, = struct.unpack_from('<I', image, obj-base+4)
            if address in seen_models:
                assert seen_models[address] == pointer
            else:
                seen_models[address] = pointer
                meshsets += check_mesh(lib, pointer, image, image_pointer, address)
        assert len(seen_models) == len(set(seen))
        saved_models.extend(seen_models.values())
        observations.update(callbacks=len(seen), unique_visible_models=len(seen_models),
                            visible_meshsets=meshsets, duplicate_attach_rejected=True)
''' + source[source.index(end):]
    anchor = "        observations['task_retired_by_stage_close'] = True"
    assert source.count(anchor) == 1
    source = source.replace(anchor, anchor + '''
        extent = bind(lib, 'jp_emerald_stage_mesh_bounds', I,P,P)
        assert all(extent(model, C.byref(P())) == -1 for model in saved_models)
        observations['mesh_bounds_retired_by_stage_close'] = True
''')
    namespace = dict(vars(stage), check_mesh=check_mesh)
    exec(compile(source, stage.__file__, 'exec'), namespace)
    return namespace['collect'](context, triangles, draw, track)


def scene_caller(raw):
    raw = stage.scene_caller(raw).replace(
        'from verify_native_emerald_stage_land import collect',
        'from verify_native_emerald_stage_mesh import collect').replace(
        'sonic_emerald_stage_land', 'sonic_emerald_stage_mesh').replace(
        'native_Emerald_Coast_stage_land_lifetime_verified',
        'native_Emerald_Coast_stage_mesh_relocation_verified')
    # Compare with the independent ordered selection from the qualified
    # live-land run using this same camera route and original disc.
    anchor = '            ring_walk = collect(c,triangles,'
    assert raw.count(anchor) == 1
    return raw.replace(anchor, """            from verify_native_emerald_stage_mesh import expected_order
            c['native_mesh_expected_order'] = expected_order(c['r'])
""" + anchor)


def expected_order(root):
    import json
    proof = root/'work/sonic-adventure/jp-native-emerald-land-pass-gpu-v1/verification.json'
    return json.loads(proof.read_bytes())['stage_geometry']['visibility_draw_order']


def exercise(context, inputs, track):
    direct = stage.land.live.shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = stage.land.live.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_mesh import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
