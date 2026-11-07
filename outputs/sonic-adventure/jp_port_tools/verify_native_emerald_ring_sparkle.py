"""Verify original pickup sprites render without advancing simulation."""
import ctypes as C
import inspect
from pathlib import Path

import verify_native_emerald_ring_finish as finish
from native_packet_test_support import bind
from native_pvm_test_support import PVM, archive, destination, index_archive, texture
from native_texture_test_support import Record, Runtime
from verify_native_emerald_ring_model import assets
from verify_native_read_queue import Disc
from verify_native_sprites import Frame, Point, Sprite


def draw_sparkles(context, track, rebase):
    image, _ = assets(context, track)
    from decompress_prs import decompress

    disc = Disc(track)
    try:
        fad, size, directory = disc.files()['EFF_REGULAR.PRS']
        assert not directory
        raw = disc.sectors(fad, (size+2047)//2048)[:size]
    finally:
        disc.f.close()
    _, rows, pictures = index_archive(decompress(raw)[0])
    rows, pictures = rows[:8], pictures[:8]
    assert [r['name'].split(b'\0')[0] for r in rows] == [f'ring{i:02}'.encode() for i in range(1,9)]
    data = archive([texture(p['pixels'], p['kind'], p['w'], p['h']) for p in pictures],
                   names=[r['name'] for r in rows], gids=[r['gid'] for r in rows])
    lib = context['f']
    pvm, runtime = PVM(lib), Runtime(context['b']['frames_DLL'])
    names, listing, buffers = destination(8)
    storage = C.create_string_buffer(data)
    assert pvm.load(C.byref(listing), 8, 28, storage, len(data)) == 1
    offset = 0x8c1d5380 - 0x8c010000
    frames = (Frame*8).from_buffer_copy(image[offset:offset+8*C.sizeof(Frame)])
    assert [frame.texid for frame in frames] == list(range(8))
    sprite = Sprite(Point(0,0,0), 1, 1, 0, C.addressof(listing), C.addressof(frames))
    draw = bind(lib, 'jp_emerald_sparkle_draw_level', C.c_int, C.c_void_p, C.c_void_p, C.c_int, C.c_int)
    get_matrix = bind(lib, 'njGetMatrix', None, C.c_void_p)
    current = C.c_void_p.in_dll(lib, '_nj_current_texlist')
    saved = current.value
    before, after = (C.c_float*16)(), (C.c_float*16)()
    get_matrix(before)
    count = draw(C.byref(sprite), C.byref(rebase), 0, 0)
    get_matrix(after)
    assert count == 3 and bytes(after) == bytes(before) and current.value == saved
    words = [Record.from_address(runtime.lookup(C.byref(listing), i)).texparambuffer & 0x1fffff
             for i in (0, 2, 4)]

    def release():
        assert pvm.release(C.byref(listing)) == 1
        assert len(names) == len(buffers) == 8

    return dict(sprites=count, texture_words=words, original_frames=[4,2,0]), release


def adapt(source):
    source = finish.adapt(source)
    anchor = '            assert work.header & 255 == (3 if int(third.value) > 7 else 2)'
    assert source.count(anchor) == 1
    source = source.replace(anchor, anchor + '''
            if elapsed == 6:
                state = bytes(work)
                result['native_ring_sparkles'], release_sparkles = draw_sparkles(context, track, rebase)
                assert bytes(work) == state
                result['native_ring_sparkles']['simulation_unchanged'] = True
''')
    anchor = '            release_land()'
    assert source.count(anchor) == 1
    return source.replace(anchor, '''            try:
                release_sparkles()
            finally:
                release_land()''')


def draw_land(context, track):
    previous = finish.draw
    source = inspect.getsource(previous.draw_land)
    anchor = '    namespace = dict(vars(model))'
    assert source.count(anchor) == 1
    source = source.replace(anchor, '    source = adapt(source)\n'
                            '    namespace = dict(vars(model), draw_sparkles=draw_sparkles)')
    namespace = dict(vars(previous), adapt=adapt, draw_sparkles=draw_sparkles)
    exec(compile(source, previous.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    raw = finish.scene_caller(raw).replace(
        'from verify_native_emerald_ring_finish import draw_land',
        'from verify_native_emerald_ring_sparkle import draw_land').replace(
        'sonic_emerald_ring_finish', 'sonic_emerald_ring_sparkle').replace(
        'native_Emerald_Coast_collected_ring_completion_verified',
        'native_Emerald_Coast_original_ring_sparkles_verified')
    anchor = "    ring_word = land['native_ring_model']['texture_word']"
    assert raw.count(anchor) == 1
    return raw.replace(anchor, '''    sparkle_words = set(land['native_ring_sparkles']['texture_words'])
    sparkle_batches = [batch for batch in decoded.batches[:decoded.batch_count]
                       if batch.triangle_count and batch.header[3]&0x1fffff in sparkle_words]
    check(bool(sparkle_batches),'original ring sparkle textures reach GPU packets')
    land['native_ring_sparkles']['packet_batches'] = len(sparkle_batches)
''' + anchor)


def exercise(context, inputs, track):
    direct = finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = finish.draw.model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_sparkle import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
