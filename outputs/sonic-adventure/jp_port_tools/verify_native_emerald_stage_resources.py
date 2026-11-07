"""Verify queued retail stage reads and native PRS decoding in the land fixture."""
import ctypes as C
import hashlib
import inspect
from pathlib import Path

import verify_native_emerald_land_owner as owner
from native_packet_test_support import bind
from verify_native_read_queue import Disc, Info

I, P, S = C.c_int32, C.c_void_p, C.c_size_t
FILES = ('STG01.PRS', 'BEACH01.PRS')
DECODED_SHA256 = {
    'STG01.PRS': '83ade159f74b4f433cb0e264f55c19e3571ee22c7c95bc940d335bfafcdf17c6',
    'BEACH01.PRS': '4ada1d216b3aca7d461224ba9986bce798cbf49e6ddeaefc9341e6f006881b0c',
}


def load_resources(context, track):
    queue = context['queue']
    snapshot = context['snapshot']
    add = bind(queue, 'jp_read_queue_add', I, C.c_char_p, P, S)
    serve = bind(queue, 'jp_read_queue_serve', I)
    decode = bind(context['f'], 'prskit_decompress', I, P, S, P, S, P, P)

    def state():
        return snapshot(queue, 'jp_read_queue_snapshot', Info)

    disc = Disc(track)
    records = []
    buffers = []
    try:
        files = disc.files()
        for name in FILES:
            fad, size, directory = files[name]
            assert not directory and 0 < size <= 8 << 20
            rounded = (size + 2047) & ~2047
            expected = disc.sectors(fad, rounded // 2048)
            storage = C.create_string_buffer(rounded + 96)
            address = (C.addressof(storage) + 63) & ~31
            C.memset(storage, 0xa5, len(storage))
            buffers.append((name, storage, address, size, rounded, expected))
    finally:
        disc.f.close()

    before = state()
    assert before.queued == 0
    for name, _, address, _, rounded, _ in buffers:
        assert add(name.encode('ascii'), address, rounded) == 0, name
    assert state().queued == len(buffers)
    for _ in range(100000):
        assert serve() == 0
        after = state()
        if not after.queued:
            break
    else:
        raise AssertionError('Stage reads did not finish')
    assert after.completed == before.completed + len(buffers)

    outputs = []
    for name, storage, address, size, rounded, expected in buffers:
        assert C.string_at(address, rounded) == expected, name
        assert C.string_at(address - 32, 32) == C.string_at(address + rounded, 32) == b'\xa5' * 32
        count, stream_end = S(), S()
        status = decode(address, size, None, 0, C.byref(count), C.byref(stream_end))
        assert status in (0, 4) and 0 < count.value <= 32 << 20, (name, status)
        assert 0 < stream_end.value <= size
        output = C.create_string_buffer(count.value)
        written, consumed = S(), S()
        assert decode(address, size, output, len(output), C.byref(written), C.byref(consumed)) == status
        assert (written.value, consumed.value) == (count.value, stream_end.value)
        raw = output.raw
        decoded_sha256 = hashlib.sha256(raw).hexdigest()
        assert decoded_sha256 == DECODED_SHA256[name], name
        outputs.append(raw)
        records.append(dict(name=name, sector_bytes=rounded, file_bytes=size,
                            compressed_sha256=hashlib.sha256(expected[:size]).hexdigest(),
                            decoded_bytes=len(raw), decoded_sha256=decoded_sha256))
    image, archive = outputs
    return image, archive, records


def draw_land(context, track):
    direct = owner.direct
    source = inspect.getsource(direct.draw_land)
    anchor = '    source = inspect.getsource(textured.draw_textured_land)'
    assert source.count(anchor) == 1
    source = source.replace(anchor, anchor + '\n    source = use_native_resources(source)')

    def use_native_resources(recipe):
        begin = "    sys.path.insert(0,"
        end = "    _,rows,pictures = index_archive(archive)"
        assert recipe.count(begin) == recipe.count(end) == 1
        return (recipe[:recipe.index(begin)] +
                '    image, archive, resources = load_resources(context, track)\n'
                "    context['stage_resource_reads'] = resources\n" + recipe[recipe.index(end):])

    # Pass only the resource loader through the retained fixture's globals.
    anchor = "    exec(compile(source,textured.__file__,'exec'),namespace)"
    assert source.count(anchor) == 1
    source = source.replace(anchor, "    namespace['load_resources'] = load_resources\n" + anchor)
    namespace = dict(vars(direct), dispatch_models=owner.dispatch_models,
                     use_native_resources=use_native_resources, load_resources=load_resources)
    exec(compile(source, direct.__file__, 'exec'), namespace)
    result, release = namespace['draw_land'](context, track)
    result['native_stage_resource_reads'] = context.pop('stage_resource_reads')
    return result, release


def scene_caller(raw):
    return owner.scene_caller(raw).replace(
        'from verify_native_emerald_land_owner import draw_land',
        'from verify_native_emerald_stage_resources import draw_land').replace(
        'sonic_emerald_land_owner', 'sonic_emerald_stage_resources').replace(
        'persistent_Emerald_Coast_land_task_and_GPU_submission_verified',
        'queued_Emerald_Coast_resources_and_GPU_submission_verified')


def exercise(context, inputs, track):
    direct = owner.direct
    source = inspect.getsource(direct.exercise)
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(source, direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_stage_resources import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
