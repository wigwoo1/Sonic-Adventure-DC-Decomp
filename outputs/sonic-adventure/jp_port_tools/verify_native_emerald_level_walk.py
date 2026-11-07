"""Exercise the the native level-ground Sonic step in Emerald Coast."""
import ctypes as C
import inspect
from pathlib import Path
from unittest.mock import patch

import verify_native_emerald_same_tick as same_tick
from native_texture_test_support import List
from native_packet_test_support import bind

I, P, U = same_tick.I, same_tick.P, same_tick.U


class Draw(C.Structure):
    _fields_ = [('terrain_bytes', C.c_size_t), ('opaque_bytes', C.c_size_t),
                ('terrain', I), ('rings', I), ('sparkles', I)]


def overlay(context):
    lib = context['f']
    target = bind(lib, 'jp_emerald_placement_task', P,U)(145)
    assert target
    work = same_tick.Task.from_address(target).work.contents
    assert work.header & 255 == 2

    result = context.pop('native_player_draw')
    assert (result.terrain, result.rings, result.sparkles) == (77, 8, 2)
    committed = bind(lib, 'jp_queue_committed_bytes', C.c_size_t,U)
    assert all(committed(i) == 0 for i in range(5))
    assert context['native_stage_frame_id']
    lookup = bind(lib, 'jp_emerald_stage_ring_texture_list', P,I)
    ring, sparkle = lookup(0), lookup(1)
    assert same_tick.PVM(lib).loaded(ring) == 1 and same_tick.PVM(lib).loaded(sparkle) == 8
    runtime = same_tick.Runtime(context['b']['frames_DLL'])

    def word(listing, slot):
        record = runtime.lookup(C.cast(listing, C.POINTER(List)), slot)
        return same_tick.Record.from_address(record).texparambuffer & 0x1fffff

    context['same_tick_rings'] = dict(texture_word=word(ring, 0), models=result.rings,
                                      collected_target_omitted=True)
    context['same_tick_sparkles'] = dict(texture_words=[word(sparkle, i) for i in range(8)],
                                         sprites=result.sparkles, simulation_unchanged=True,
                                         third_timer=work.scale[2])

    return result


def collect(context, triangles, draw, track):
    source = inspect.getsource(same_tick.terrain.collect)
    a = source.index("    namespace = ")
    source = source[:a] + """    assert source.count('        status = draw()\\n') == 1
    source = source.replace('        status = draw()\\n', '')
    start = source.index('        matrix = ')
    end = source.index('        return status', start)
    source = source[:start] + scene_draw + source[end:]
    namespace = dict(vars(terrain), Runtime=Runtime, Record=Record, overlay=overlay)
""" + source[a:].partition('\n')[2]
    scene_draw = """        saved = selected.value
        pins_before = pinned()
        context['capture_native_player'] = True
        try:
            status = draw()
        finally:
            context.pop('capture_native_player')
        assert status == 0
        result = overlay(context)
        assert selected.value == saved and pinned() > pins_before
        assert result.terrain_bytes == 172416
        context['terrain_opaque_bytes'] = result.opaque_bytes
        info.update(loaded_textures=count, callbacks=result.terrain,
                    committed_bytes=result.terrain_bytes,
                    pinned_bytes=pinned(), current_texture_restored=True)
"""
    namespace = dict(vars(same_tick.terrain), scene_draw=scene_draw, overlay=overlay)
    exec(compile(source, same_tick.terrain.__file__, 'exec'), namespace)
    result = namespace['collect'](context, triangles, draw, track)
    lookup = bind(context['f'], 'jp_emerald_stage_ring_texture_list', P,I)
    assert not lookup(0) and not lookup(1)
    return result


def exercise(context, inputs, track):
    def scene_caller(raw):
        raw = same_tick.scene_caller(raw)
        original = "native_draw=bind(f,'jp_sonic_emerald_task_probe',I,P,P,P,P,I,I,P,P,S,P,P)"
        assert raw.count(original) == 1
        raw = raw.replace(original, original + "\n            submit_player=bind(f,'jp_sonic_emerald_task_frame_probe',I,P,P,P,P,I,I,P,P,S,P,P,P,P)")
        start = raw.index('                status=native_draw(')
        end = raw.index('                check(status==0', start)
        raw = raw[:start] + """                args = (textures,aura,stand.data,walk,charged,hidden,C.byref(frame),
                        triangle_buffer,len(triangles)//TRIANGLE.size,position,C.byref(entry))
                if c.get('capture_native_player'):
                    result, frame_id = Draw(), Q()
                    status=submit_player(*args, C.byref(result), C.byref(frame_id))
                    c['native_player_draw'], c['native_stage_frame_id'] = result, frame_id.value
                else:
                    status=native_draw(*args)
""" + raw[end:]
        raw = raw.replace('from verify_native_emerald_same_tick import collect',
                          'from verify_native_emerald_level_walk import collect')
        raw = raw.replace('            def draw(textures,aura,walk,charged,hidden):',
                          '            from verify_native_emerald_level_walk import Draw\n'
                          '            def draw(textures,aura,walk,charged,hidden):')
        anchor = "frame=Q();check(bind(f,'jp_frames_submit',I,P)(C.byref(frame))==0,'actual original model packets captured in immutable retained frame');decoder="
        assert raw.count(anchor) == 1
        raw = raw.replace(anchor,
            "frame=Q(c['native_stage_frame_id']);check(frame.value!=0,'native stage submitted the retained frame before close');decoder=")
        return raw.replace('sonic_emerald_same_tick', 'sonic_emerald_level_walk').replace(
            'native_Emerald_Coast_same_tick_scene_verified',
            'native_Emerald_Coast_level_walk_verified')

    direct = same_tick.terrain.shared.registry.loader.owner.queued.owner.direct
    harness = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    with patch.object(same_tick, 'collect', collect):
        exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), harness)
        harness['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = same_tick.terrain.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_level_walk import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
