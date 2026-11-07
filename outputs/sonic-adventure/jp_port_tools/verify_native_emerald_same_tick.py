"""Present live Sonic, terrain, rings and sparkles from the same stage tick."""
import ctypes as C
import inspect
from pathlib import Path

import verify_native_emerald_single_terrain as terrain
from native_packet_test_support import bind
from native_pvm_test_support import PVM, archive, destination, index_archive, texture
from native_texture_test_support import Record, Runtime
from verify_native_emerald_ring_model import assets, model_data
from verify_native_emerald_placement_tasks import Task
from verify_native_model_preflight import Vec
from verify_native_read_queue import Disc
from verify_native_sprites import Frame, Point, Sprite

I, P, U = C.c_int, C.c_void_p, C.c_uint32


def overlay(context, track):
    lib = context['f']
    image, regular = assets(context, track)
    from decompress_prs import decompress
    disc = Disc(track)
    try:
        fad, size, directory = disc.files()['EFF_REGULAR.PRS']
        assert not directory
        effects = decompress(disc.sectors(fad, (size+2047)//2048)[:size])[0]
    finally:
        disc.f.close()
    model, span, uv = model_data(image)
    model.materials[0].texture = 0
    pvm, runtime = PVM(lib), Runtime(context['b']['frames_DLL'])
    owned = []

    def load(data, slots):
        _, rows, pictures = index_archive(data)
        chosen = {row['number']:(row,pic) for row,pic in zip(rows,pictures)}
        pairs = [chosen[i] for i in slots]
        packed = archive([texture(p['pixels'], p['kind'], p['w'], p['h']) for _,p in pairs],
                         gids=[r['gid'] for r,_ in pairs], names=[r['name'] for r,_ in pairs])
        names, listing, buffers = destination(len(pairs))
        storage = C.create_string_buffer(packed)
        assert pvm.load(C.byref(listing), len(pairs), 28, storage, len(packed)) == 1
        owned.append((names, listing, buffers))
        return listing

    task_at = bind(lib, 'jp_emerald_placement_task', P,U)
    target = task_at(145)
    assert target
    work = Task.from_address(target).work.contents
    assert work.header & 255 == 2
    state = bytes(work)
    rings = bind(lib, 'jp_emerald_ring_draw_level', I,P,P,P,P)
    sparkles = bind(lib, 'jp_emerald_sparkle_draw_level', I,P,P,I,I)
    rebase = Vec()
    try:
        ring_list = load(regular, [50])
        sparkle_list = load(effects, range(8))
        frames = (Frame*8).from_buffer_copy(image, 0x8c1d5380-0x8c010000)
        assert [frame.texid for frame in frames] == list(range(8))
        sprite = Sprite(Point(), 1, 1, 0, C.addressof(sparkle_list), C.addressof(frames))
        count = rings(C.byref(model), C.byref(span), C.byref(ring_list), C.byref(rebase))
        sprite_count = sparkles(C.byref(sprite), C.byref(rebase), 0, 0)
        assert count > 0 and sprite_count > 0 and bytes(work) == state

        def word(listing, slot):
            return Record.from_address(runtime.lookup(C.byref(listing), slot)).texparambuffer & 0x1fffff

        context['same_tick_rings'] = dict(texture_word=word(ring_list, 0), models=count,
                                          collected_target_omitted=True)
        context['same_tick_sparkles'] = dict(texture_words=[word(sparkle_list, i) for i in range(8)],
                                             sprites=sprite_count, simulation_unchanged=True,
                                             third_timer=work.scale[2])
    finally:
        for _, listing, _ in reversed(owned):
            assert pvm.release(C.byref(listing)) == 1


def collect(context, triangles, draw, track):
    source = inspect.getsource(terrain.collect)
    anchor = "    namespace = dict(vars(terrain), Runtime=Runtime, Record=Record)"
    assert source.count(anchor) == 1
    source = source.replace(anchor, '''    point = "            info.update(loaded_textures=count"
    assert source.count(point) == 1
    source = source.replace(point, "            overlay(context, track)\\n" + point)
    namespace = dict(vars(terrain), Runtime=Runtime, Record=Record, overlay=overlay)''')
    namespace = dict(vars(terrain), overlay=overlay)
    exec(compile(source, terrain.__file__, 'exec'), namespace)
    return namespace['collect'](context, triangles, draw, track)


def draw_land(context, track):
    return dict(texture_words=context['terrain_texture_words'],
                vq_texture_words=context['terrain_vq_words'],
                opaque_bytes=context['terrain_opaque_bytes'],
                native_ring_model=context['same_tick_rings'],
                native_ring_sparkles=context['same_tick_sparkles'],
                same_stage_tick=True), lambda: None


def scene_caller(raw):
    raw = terrain.scene_caller(raw)
    for old, new in (
        ('from verify_native_emerald_single_terrain import collect',
         'from verify_native_emerald_same_tick import collect'),
        ('from verify_native_emerald_single_terrain import draw_land',
         'from verify_native_emerald_same_tick import draw_land'),
        ('sonic_emerald_single_terrain', 'sonic_emerald_same_tick'),
        ('native_Emerald_Coast_single_native_terrain_pass_verified',
         'native_Emerald_Coast_same_tick_scene_verified'),
    ):
        assert old in raw
        raw = raw.replace(old, new)
    return raw


def exercise(context, inputs, track):
    direct = terrain.shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = terrain.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_same_tick import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
