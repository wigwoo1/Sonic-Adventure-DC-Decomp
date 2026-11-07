"""Exercise the original Sonic spin and landing action playback in the scheduled jump."""
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
        raw = raw.replace("(8<<20,16)", "(8<<20,32)")
        original = "native_draw=bind(f,'jp_sonic_emerald_task_probe',I,P,P,P,P,I,I,P,P,S,P,P)"
        assert raw.count(original) == 1
        raw = raw.replace(original, original + "\n            submit_player=bind(f,'jp_sonic_emerald_jump_draw_probe',I,P,P,P,P,I,I,P,P,S,P,P,P,P,P,P,P,P,P,U)")
        start = raw.index('                status=native_draw(')
        end = raw.index('                check(status==0', start)
        raw = raw[:start] + """                args = (textures,aura,stand.data,walk,charged,hidden,C.byref(frame),
                        triangle_buffer,len(triangles)//TRIANGLE.size,position,C.byref(entry))
                if c.get('capture_native_player'):
                    result, frame_id = Draw(), Q()
                    spin, land, air = View(), View(), (Q*3)()
                    acquire = bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)
                    release = bind(motions,'jp_motions_release',I,Q)
                    spin_status = acquire(7,b'sonic_spin_action',C.byref(spin))
                    check(spin_status==0,('original Sonic spin action leased',spin_status))
                    try:
                        check(acquire(7,b'sonic_jump_land_action',C.byref(land))==0,'original Sonic landing action leased')
                        try:
                            status=submit_player(*args, C.byref(result), C.byref(frame_id), spin.data, land.data, C.byref(air), C.byref(air_bound), words, len(words))
                            c['air_frame_ids'] = list(air)
                        finally:
                            check(release(land.token)==0,'landing action released')
                    finally:
                        check(release(spin.token)==0,'spin action released')
                    c['native_player_draw'], c['native_stage_frame_id'] = result, frame_id.value
                else:
                    status=native_draw(*args)
""" + raw[end:]
        raw = raw.replace('from verify_native_emerald_same_tick import collect',
                          'from verify_native_emerald_jump_sequence import collect')
        raw = raw.replace('            def draw(textures,aura,walk,charged,hidden):',
                          '            from verify_native_emerald_jump_sequence import Draw\n'
                          '            def draw(textures,aura,walk,charged,hidden):')
        anchor = "frame=Q();check(bind(f,'jp_frames_submit',I,P)(C.byref(frame))==0,'actual original model packets captured in immutable retained frame');decoder="
        assert raw.count(anchor) == 1
        raw = raw.replace(anchor,
            "frame=Q(c['native_stage_frame_id']);check(frame.value!=0,'native stage submitted the retained frame before close');decoder=")
        marker = '    registrations=(ModelSpan*len(models))()'
        raw = raw.replace(marker, """    spin_view = View()
    check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(7,b'sonic_spin_action',C.byref(spin_view))==0,
          'spin model available for scope registration')
    prior_count = len(models)
    seen.clear()
    visit(Action.from_address(spin_view.data).object)
    spin_models = models[prior_count:]
    del models[prior_count:]
    air_bound = Bound()
    for address,model,spans in spin_models:
        item=Bound()
        check(preflight(address,extent(C.cast(model.points,P).value,12),
              extent(C.cast(model.normals,P).value,12),extent(C.cast(model.meshes,P).value,48),
              extent(C.cast(model.materials,P).value,20),spans,C.byref(item))==0,
              'airborne model packet bounds verified')
        air_bound.opaque+=item.opaque
        air_bound.translucent+=item.translucent
        air_bound.vertices=max(air_bound.vertices,item.vertices)
        air_bound.meshes+=item.meshes
    check(bind(motions,'jp_motions_release',I,Q)(spin_view.token)==0,'spin registration lease retired')
    scope_models = list({entry[0]:entry for entry in models + spin_models}.values())
    registrations=(ModelSpan*len(scope_models))()""")
        raw = raw.replace('enumerate(models):\n        registrations', 'enumerate(scope_models):\n        registrations')
        raw = raw.replace('(registrations,len(models))', '(registrations,len(scope_models))')
        marker = "    check(jobs.release(handle,off_job.value"
        start = raw.index(marker)
        raw = raw[:start] + """    ids = c['air_frame_ids']
    check(len(set(ids+[frame.value]))==4 and all(ids),'three distinct retained airborne frames')
    seen_air = set()
    for label, air_id in zip(('rise','apex','descent'),ids):
        air_job = Q()
        check(jobs.submit(handle,air_id,C.byref(cfg),C.byref(air_job),C.byref(Report()),
              C.byref(error))==0,('airborne frame reaches native GPU',label))
        jobs.wait(air_job.value)
        check(jobs.copy(window,air_job.value,C.byref(error))==0 and
              bind(gl,'jp_window_readback',I,P,P,S,P)(window,output,len(output),C.byref(error))==0,
              ('airborne frame copied to the window',label))
        pixels = output.raw
        check(pixels != rgba and pixels not in seen_air,('distinct airborne pixels',label))
        seen_air.add(pixels)
        png(d/('sonic_emerald_airborne_'+label+'.png'),pixels,512,512)
        check(bind(gl,'jp_window_present',I,P,U,P)(window,1,C.byref(error))==0,
              ('airborne frame presented',label))
        check(jobs.release(handle,air_job.value,C.byref(error))==0 and
              bind(f,'jp_frame_release',I,Q)(air_id)==0,('airborne frame retired',label))
""" + raw[start:]
        return raw.replace('actual_window_presents=2','actual_window_presents=5').replace('sonic_emerald_same_tick', 'sonic_emerald_jump_sequence').replace(
            'native_Emerald_Coast_same_tick_scene_verified',
            'native_Emerald_Coast_jump_sequence_verified')

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
    runner = runner.replace(anchor, 'from verify_native_emerald_jump_sequence import exercise')
    sample = "b'Sample(1,0,-100,-12,0,0,0,0)'"
    replacement = "b'(lambda phase: Sample(1,4 if phase==2 else 0,0,0,0,0,0,0) if phase else Sample(1,0,-100,-12,0,0,0,0))(bind(f,\"jp_emerald_probe_input_phase\",I)())'"
    assert runner.count(sample) == 1
    runner = runner.replace(sample, replacement)
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
