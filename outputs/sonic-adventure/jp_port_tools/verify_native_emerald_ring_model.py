"""Submit the original ring mesh and texture using a native placement task."""
import ctypes as C
import hashlib
import inspect
import os
from pathlib import Path
import struct
import sys

import verify_native_emerald_ring_walk as walk
from extract_native_emerald_collision import TRIANGLE
from native_packet_test_support import bind
from native_pvm_test_support import PVM, archive as pack_pvm, destination, index_archive, texture
from native_texture_test_support import Record, Runtime
from verify_native_model_preflight import Material, Mesh, Model, Span, Vec
from verify_native_read_queue import Disc

I, P, S, F = C.c_int, C.c_void_p, C.c_size_t, C.c_float


def assets(context, track):
    sys.path.insert(0, str(context['r']/'outputs/sonic-adventure'))
    from decompress_prs import decompress
    disc = Disc(track)
    try:
        def read(row):
            fad, size, directory = row
            assert not directory
            return disc.sectors(fad, (size+2047)//2048)[:size]
        pvd = disc.sectors(45166, 1)
        root = disc.directory(struct.unpack_from('<I', pvd, 158)[0]+150,
                              struct.unpack_from('<I', pvd, 166)[0])
        image = read(root['1ST_READ.BIN'])
        compressed = read(disc.files()['OBJ_REGULAR.PRS'])
    finally:
        disc.f.close()
    assert hashlib.sha256(image).hexdigest() == 'e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    return image, decompress(compressed)[0]


def model_data(image):
    def get(fmt, address):
        offset = address - 0x8c010000
        assert 0 <= offset <= len(image)-struct.calcsize(fmt)
        return struct.unpack_from(fmt, image, offset)

    points, normals, n, meshes, materials, nm, nt, x, y, z, radius = get('<5I2H4f', 0x8c1b8b44)
    assert (n, nm, nt) == (48, 1, 1)
    kind, strips, indices, attrs, mesh_normals, colors, uv = get('<HH5I', meshes)
    assert kind == 0xc000 and not (attrs or mesh_normals or colors)
    material = Material(*get('<IIfII', materials))
    assert material.texture == 50
    order, coords = [], []
    for _ in range(strips):
        header, = get('<H', indices)
        count, flipped = header & 0x7fff, header >> 15
        assert 3 <= count <= 1024
        strip = get(f'<{count}H', indices+2)
        tex = get(f'<{count*2}h', uv)
        assert all(index < n for index in strip)
        for j in range(count-2):
            triangle = (j, j+2, j+1) if (flipped+j)&1 else (j, j+1, j+2)
            order.extend(strip[k] for k in triangle)
            coords.extend(tex[2*k+l] for k in triangle for l in (0, 1))
        indices += 2+count*2
        uv += count*4
    vertices = (Vec*n)(*(Vec(*get('<3f', points+12*i)) for i in range(n)))
    directions = (Vec*n)(*(Vec(*get('<3f', normals+12*i)) for i in range(n)))
    indices = (C.c_int16*len(order))(*order)
    uv_array = (C.c_int16*len(coords))(*coords)
    mesh = Mesh(0, len(order)//3, indices, None, None, None, C.addressof(uv_array))
    model = Model(vertices, directions, n, C.pointer(mesh), C.pointer(material), 1, 1, Vec(x,y,z), radius)
    return model, Span(len(order), len(order)), uv_array


def draw_land(context, track):
    result, release_land = walk.score.draw_land(context, track)
    lib = context['f']
    bind_ring = bind(lib, 'jp_emerald_ring_bind', I, I)
    floor = bind(lib, 'jp_emerald_ring_floor', None, P, S)
    select = bind(lib, 'jp_emerald_set_select', I, C.c_uint)
    close = bind(lib, 'jp_emerald_set_close', I)
    get = bind(lib, 'jp_emerald_set_get', I, C.c_uint, P)
    task_at = bind(lib, 'jp_emerald_placement_task', P, C.c_uint)
    step = bind(lib, 'jp_emerald_placement_step', I, P, P)
    viewers = bind(lib, 'jp_emerald_placement_viewers', None, P, P, P, I)
    tick = bind(lib, 'task_step_level', None, I)
    types = walk.score.contact.rings.tasks
    image, archive = assets(context, track)
    model, span, uvs = model_data(image)
    _, rows, pictures = index_archive(archive)
    row, picture = next((row, picture) for row, picture in zip(rows, pictures) if row['number'] == 50)
    assert row['name'].split(b'\0')[0] == b'ring_ref_a'
    encoded = texture(picture['pixels'], picture['kind'], picture['w'], picture['h'])
    one_texture = pack_pvm([encoded], gids=[row['gid']], names=[row['name']])
    names, listing, buffers = destination(1)
    source = C.create_string_buffer(one_texture)
    model.materials[0].texture = 0
    pvm, runtime = PVM(lib), Runtime(context['b']['frames_DLL'])
    assert pvm.load(C.byref(listing), 1, 28, source, len(one_texture)) == 1
    try:
        assert close() == 0 and bind_ring(1) == 0 and select(0) == 786
        entry = C.create_string_buffer(32)
        assert get(145, entry) == 0
        _, _, _, _, x,y,z, *_ = struct.unpack('<H3h6f', entry.raw)
        center = types.Work(pos=types.Vec(x,y,z))
        triangles = (Path(os.environ['JP_SONIC_EMERALD_TEST_GEOMETRY'])/'act0_triangles.bin').read_bytes()
        buffer = C.create_string_buffer(triangles)
        floor(buffer, len(triangles)//TRIANGLE.size)
        viewers(C.byref(center), None, None, 0)
        assert step(C.byref(center), None) > 0
        tick(3)
        at = task_at(145)
        assert at
        work = types.Task.from_address(at).work.contents
        assert work.header & 255 == 1
        assert runtime.select(C.byref(listing)) == 1
        record = Record.from_address(runtime.lookup(C.byref(listing), 0))
        word = record.texparambuffer & 0x1fffff
        matrix = (F*16)()
        bind(lib, 'njGetMatrix', None, P)(matrix)
        translate = bind(lib, 'njTranslate', None, P,F,F,F)
        try:
            # Rebase the ring to the same diagnostic origin as the walk.
            translate(None, -x-10, -work.scale[1], -z)
            translate(None, *work.pos)
            bind(lib, 'njRotateY', None, P,I)(None, work.angles[1])
            used = bind(lib, 'jp_queue_committed_bytes', S, C.c_uint32)
            before = used(0)+used(1)
            draw = bind(lib, 'jp_simple_draw_model_bounded', I,P,S,S,S,P,S,P)
            projection = I()
            assert draw(C.byref(model), 48, 48, 1, C.byref(span), 1, C.byref(projection)) == 0
            assert used(0)+used(1) > before, projection.value
        finally:
            bind(lib, 'njSetMatrix', None, P,P)(None, matrix)
        result['native_ring_model'] = dict(set_index=145, vertices=48, triangles=span.indices//3,
                                           texture_slot=50, texture_word=word)
    finally:
        viewers(None, None, None, 0)
        floor(None, 0)
        assert close() == 0 and bind_ring(0) == 0 and select(2) == 1

    def release():
        try:
            assert pvm.release(C.byref(listing)) == 1
            assert len(names) == len(buffers)
        finally:
            release_land()

    return result, release


def scene_caller(raw):
    raw = walk.scene_caller(raw).replace(
        'from verify_native_emerald_ring_score import draw_land',
        'from verify_native_emerald_ring_model import draw_land').replace(
        'sonic_emerald_ring_walk', 'sonic_emerald_ring_model').replace(
        'native_Emerald_Coast_live_Sonic_ring_pickup_verified',
        'native_Emerald_Coast_original_ring_model_verified')
    anchor = "    land_words = set(land['texture_words'])"
    assert raw.count(anchor) == 1
    return raw.replace(anchor, anchor + """
    ring_word = land['native_ring_model']['texture_word']
    ring_batches = [batch for batch in decoded.batches[:decoded.batch_count]
                    if batch.triangle_count and batch.header[3]&0x1fffff == ring_word]
    check(bool(ring_batches),'original ring texture appears in submitted GPU packets')
""")


def exercise(context, inputs, track):
    direct = walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_model import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
