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
    if 'emerald_ring_sphere' in context['b']:
        assert bind(lib, 'jp_emerald_ring_sphere_pairs', U)() > 0
    if 'emerald_ring_pass' in context['b']:
        assert bind(lib, 'jp_collision_pending_rings', U)() == 0
    if 'emerald_live_ring_contact' in context['b']:
        assert bind(lib, 'jp_emerald_live_ring_pairs', U)() == bind(lib, 'jp_emerald_ring_sphere_pairs', U)() > 0
    if context['b'].get('emerald_ring_reach', {}).get('retain_near_rings'):
        assert bind(lib, 'jp_emerald_near_ring_count', U)() > 0
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
    if context['b'].get('emerald_ring_reach', {}).get('retain_near_rings'):
        assert bind(context['f'], 'jp_emerald_near_ring_count', U)() == 0
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
                    spin, land = View(), View()
                    acquire = bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)
                    release = bind(motions,'jp_motions_release',I,Q)
                    if c['b'].get('emerald_light_dash_target',{}).get('actions_bound'):
                        dash, skid = View(), View()
                        check(acquire(0,b'sonic_light_dash_action',C.byref(dash))==0,'original Light Dash action leased')
                        try:
                            check(acquire(7,b'sonic_skid_action',C.byref(skid))==0,'original skid action leased')
                            try:
                                check(bind(f,'jp_emerald_light_dash_actions_check',I,P,P)(dash.data,skid.data)==0,
                                      'original dash and skid actions bind with their native playback settings')
                            finally:
                                check(release(skid.token)==0,'skid action released')
                        finally:
                            check(release(dash.token)==0,'Light Dash action released')
                    spin_status = acquire(7,b'sonic_spin_action',C.byref(spin))
                    check(spin_status==0,('original Sonic spin action leased',spin_status))
                    try:
                        check(acquire(7,b'sonic_jump_land_action',C.byref(land))==0,'original Sonic landing action leased')
                        try:
                            status=stream_jump(args, result, frame_id, spin, land)
                        finally:
                            check(release(land.token)==0,'landing action released')
                    finally:
                        check(release(spin.token)==0,'spin action released')
                    c['native_player_draw'], c['native_stage_frame_id'] = result, frame_id.value
                else:
                    status=native_draw(*args)
""" + raw[end:]
        raw = raw.replace('from verify_native_emerald_same_tick import collect',
                          'from verify_native_emerald_projection_profile import collect')
        raw = raw.replace('            def draw(textures,aura,walk,charged,hidden):',
                          '            from verify_native_emerald_projection_profile import Draw\n'
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
    air_bound.opaque+=bound.opaque
    air_bound.translucent+=bound.translucent
    air_bound.vertices=max(air_bound.vertices,bound.vertices)
    air_bound.meshes+=bound.meshes
    check(bind(motions,'jp_motions_release',I,Q)(spin_view.token)==0,'spin registration lease retired')
    scope_models = list({entry[0]:entry for entry in models + spin_models}.values())
    registrations=(ModelSpan*len(scope_models))()""")
        raw = raw.replace('enumerate(models):\n        registrations', 'enumerate(scope_models):\n        registrations')
        anchor = '            ring_walk = collect(c,triangles,lambda: draw(listing,aura_view.data,action_view.data,1,0),track)\n'
        assert raw.count(anchor) == 1
        # The full charge fixture queues effects that outlive its capture.
        # Run it after the ordinary scene has completed its audio checks.
        charge_capture = context['b'].get('emerald_light_dash_target',{}).get('full_charge_capture')
        before = '' if charge_capture else anchor
        after = anchor if charge_capture else ''
        raw = raw.replace(anchor, after + """            if b.get('emerald_light_dash_target',{}).get('actions_bound'):
                from verify_native_emerald_projection_profile import capture_light_dash_poses
                def record_pose(action):
                    check(draw(listing,aura_view.data,action,0,0)==0,'original Sonic renderer records the leased pose')
                    pose_frame=Q()
                    check(bind(f,'jp_frames_submit',I,P)(C.byref(pose_frame))==0,'pose frame retained')
                    return pose_frame.value
                def reserve_pose():
                    check(bind(f,'jp_model_reserve',I,P)(C.byref(bound))==0 and
                          bind(f,'jp_frames_prepare_textures',I,P,U)(words,len(words))==0,
                          'next recording reserves buffers and textures after pose capture')
                if b['emerald_light_dash_target'].get('full_charge_capture'):
                    reserve_pose()
                def record_scheduled_pose(task):
                    check(bind(f,'jp_emerald_light_dash_draw',I,P,P,P)(
                        task,listing,aura_view.data)==0,'scheduled Sonic task draws its current animation')
                    pose_frame=Q()
                    check(bind(f,'jp_frames_submit',I,P)(C.byref(pose_frame))==0,'scheduled pose retained')
                    return pose_frame.value
                capture_light_dash_poses(d,b,f,gl,motions,stand.data,record_pose,
                                         record_scheduled_pose,reserve_pose,check,png,
                                         triangle_buffer,len(triangles)//TRIANGLE.size)
""" + before)
        raw = raw.replace('(registrations,len(models))', '(registrations,len(scope_models))')
        marker = '            def draw(textures,aura,walk,charged,hidden):'
        assert raw.count(marker) == 1
        raw = raw.replace(marker, """            def stream_jump(args, result, frame_id, spin, land):
                device=C.CDLL(b['device_DLL'])
                handle=bind(device,'jp_device_gpu',P)()
                jobs=Jobs(types.SimpleNamespace(lib=gl,handle=P(handle)))
                cfg=Config.make(512,512)
                cfg.logical_width=cfg.logical_height=512
                window, error = P(), Failure()
                close=bind(gl,'jp_window_close',None,P)
                check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(handle,512,512,1,
                      C.byref(window),C.byref(error))==0,'airborne stream window opens')
                output=C.create_string_buffer(512*512*4)
                samples = {}
                heights = []
                readback_counts = []
                ground_count = approach_count = 0
                ids = set()
                failures = []
                sound=c['sound']
                queue_effects=bind(sound,'jp_effects_queue',I)
                peer=bind(sound,'peer_effects_slot',I,U,P)
                from verify_native_audio import Info as AudioInfo
                from verify_native_sound_clock import Options, Info as ClockInfo, WaitInfo, ClockSource
                output_audio=C.CDLL(str(Path(b['frames_DLL']).with_name('jp_native_audio.dll')))
                drain_audio=bind(output_audio,'jp_audio_drain',I,U)
                audio_snapshot=bind(output_audio,'jp_audio_snapshot',I,P)
                start_clock=bind(sound,'jp_sound_clock_start',I,P)
                stop_clock=bind(sound,'jp_sound_clock_stop',I,U)
                clock_snapshot=bind(sound,'jp_sound_clock_snapshot',I,P)
                from native_gpu_jobs_test_support import Timing
                import time
                wait_owner=c['present']
                run_service=bind(wait_owner,'jp_wait_service_run',I)
                service_snapshot=bind(wait_owner,'jp_wait_service_snapshot',I,P)
                idle_window=bind(wait_owner,'jp_wait_window_idle',I,P,U)
                wall=bind(f,'jp_clock_wall_counts',Q)
                wall_status=bind(f,'jp_clock_status',I)
                end_wall=None
                @ClockSource
                def scene_wall(_, output):
                    output[0]=wall() if end_wall is None else end_wall
                    return int(wall_status()==0)
                options=Options(C.sizeof(Options),441,8192,5,C.cast(scene_wall,P),None,781250,None,None)

                timings={}
                prior_callback_end=None
                def measure(label, fn, *args):
                    before=wall()
                    value=fn(*args)
                    timings.setdefault(label,[]).append(wall()-before)
                    return value

                def await_gpu(job):
                    deadline=time.monotonic()+10
                    timing=Timing()
                    while True:
                        assert measure('audio_service',run_service)==0,'native wait service advances wall-clock audio'
                        status=jobs.poll(handle,job,C.byref(timing),C.byref(error))
                        if status==0:
                            assert timing.id==job
                            return
                        assert status==100 and time.monotonic()<deadline
                        assert idle_window(window,1)>0
                baseline=AudioInfo()
                clock_started=False
                audio_events=[]
                audio_ticks=0
                def mix_tick(phase):
                    nonlocal audio_ticks
                    for slot in range(6):
                        row=(C.c_int64*13)()
                        assert peer(slot,row)==0
                        if row[3]==0x11:
                            audio_events.append((row[4],audio_ticks,phase))
                    assert queue_effects()==0
                    assert measure('audio_service',run_service)==0
                    audio_ticks+=1

                readback=bind(gl,'jp_window_readback',I,P,P,S,P)
                scene_profile=bind(f,'jp_emerald_profile_scene',None,P)
                bounded_profile=bind(f,'jp_emerald_profile_bounded',None,P)
                bounded=(Q*4)()
                bounded_profile(bounded)
                projection_profile=bind(f,'jp_emerald_profile_projection',None,P)
                projection_times=(Q*2)()
                projection_profile(projection_times)
                callback_type = C.CFUNCTYPE(I,Q,F,I,Q,Q)
                @callback_type
                def present_air(air_id, height, phase, prepare_counts, record_counts):
                    nonlocal ground_count, approach_count, prior_callback_end
                    timings.setdefault('native_packet_prepare',[]).append(prepare_counts)
                    timings.setdefault('native_draw_and_submit',[]).append(record_counts)
                    entered=wall()
                    if prior_callback_end is not None:
                        timings.setdefault('between_frame_callbacks',[]).append(entered-prior_callback_end)
                    job=Q()
                    try:
                        components=(Q*6)()
                        scene_profile(components)
                        bounded_profile(bounded)
                        projection_profile(projection_times)
                        assert 0 <= projection_times[0] <= projection_times[1]
                        timings.setdefault('projection_loop',[]).append(projection_times[0])
                        timings.setdefault('projection_original_total',[]).append(projection_times[1])
                        for label,value in zip(('bounded_validation','bounded_projection',
                                                'bounded_state_and_queue','bounded_emit'),bounded):
                            timings.setdefault(label,[]).append(value)
                        assert 0 < sum(components[:4]) <= record_counts
                        assert 0 < sum(components[4:]) <= components[1]
                        for label,value in zip(('native_sonic','native_terrain','native_rings','native_retention',
                                                'terrain_conversion','terrain_bounded_draw'),components):
                            timings.setdefault(label,[]).append(value)
                        assert air_id and air_id not in ids and air_id != frame_id.value
                        mix_tick(phase)
                        assert measure('gpu_submit',jobs.submit,handle,air_id,C.byref(cfg),C.byref(job),
                                           C.byref(Report()),C.byref(error))==0
                        measure('gpu_wait_including_audio',await_gpu,job.value)
                        assert measure('window_copy',jobs.copy,window,job.value,C.byref(error))==0
                        if phase == 2:
                            approach_count += 1
                            label='approach' if approach_count==6 else None
                        elif phase == 1:
                            heights.append(height)
                            label={1:'rise',19:'mid_jump',37:'descent'}.get(len(heights))
                        else:
                            ground_count += 1
                            label={1:'coast',5:'stand'}.get(ground_count)
                        if label:
                            before=wall()
                            assert measure('readback',readback,window,output,len(output),C.byref(error))==0
                            readback_counts.append(wall()-before)
                            samples[label]=output.raw
                        assert measure('present',bind(gl,'jp_window_present',I,P,U,P),window,1,C.byref(error))==0
                        ids.add(air_id)
                    except Exception as exc:
                        failures.append(repr(exc))
                    finally:
                        if job.value and jobs.release(handle,job.value,C.byref(error)) != 0:
                            failures.append('airborne GPU job release failed')
                        prior_callback_end=wall()
                    return -1 if failures else 0

                try:
                    check(drain_audio(8000)==0 and audio_snapshot(C.byref(baseline))==0 and
                          baseline.state==2 and baseline.session_muted and baseline.rate==44100,
                          'borrow the drained muted WASAPI session from the sound owner')
                    check(start_clock(C.byref(options))==0,'native sound clock owns scene PCM production')
                    clock_started=True
                    status=submit_player(*args,C.byref(result),C.byref(frame_id),spin.data,
                                         land.data,C.cast(present_air,P),C.byref(air_bound),words,len(words))
                    if status == 0 and not failures:
                        mix_tick(3)
                        clock=ClockInfo()
                        service=WaitInfo()
                        check(clock_snapshot(C.byref(clock))==0 and clock.frequency==781250 and
                              clock.due_frames==(clock.last_tick-clock.anchor)*44100//clock.frequency and
                              clock.due_frames==clock.rendered_frames+clock.backlog and
                              clock.rendered_frames>36051 and clock.render_calls>audio_ticks,
                              'wall-clock elapsed time accounts for every rendered or owed sample')
                        check(service_snapshot(C.byref(service))==0 and service.bound and
                              service.calls>=audio_ticks and not service.failures,
                              'the native wait service owns audio progression')
                        end_wall=wall()
                        deadline=time.monotonic()+8
                        peak_debt=clock.backlog
                        while True:
                            check(run_service()==0 and clock_snapshot(C.byref(clock))==0,
                                  'native wait service flushes the fixed scene audio tail')
                            peak_debt=max(peak_debt,clock.backlog)
                            if not clock.backlog:
                                break
                            assert time.monotonic()<deadline and idle_window(window,5)>0
                        expected=(end_wall-clock.anchor)*44100//clock.frequency
                        check(clock.last_tick==end_wall and clock.due_frames==clock.rendered_frames==expected,
                              'every sample owed at the scene boundary is rendered')
                        check(stop_clock(0)==0,'stop real-time accumulation with no cancelled samples')
                        clock_started=False
                        final_clock=ClockInfo()
                        check(clock_snapshot(C.byref(final_clock))==0 and
                              final_clock.due_frames==expected and final_clock.cancelled_frames==0 and
                              final_clock.backlog==0,'final clock has no cancelled or outstanding samples')
                        check(drain_audio(8000)==0,'scene PCM drains past the Windows audio clock')
                        audio_state=AudioInfo()
                        check(audio_snapshot(C.byref(audio_state))==0 and
                              audio_state.submitted-baseline.submitted==expected and
                              audio_state.consumed-baseline.consumed==expected and
                              audio_state.queued_frames==0 and audio_state.discarded==baseline.discarded and
                              audio_state.worker_thread==baseline.worker_thread and audio_state.session_muted,
                              'WASAPI consumes the complete native-clock scene mix')
                        c['native_clock_audio']=dict(ticks=audio_ticks,frames=expected,
                            events=audio_events,render_calls=clock.render_calls,
                            queue_full=clock.queue_full,source_hash=audio_state.source_hash,
                            wall_counts=clock.last_tick-clock.anchor,wait_service_calls=service.calls,
                            cancelled_frames=final_clock.cancelled_frames,tail_debt_frames=peak_debt)
                finally:
                    stopped=stop_clock(1) if clock_started else 0
                    close(window)
                    check(stopped==0,'scene clock retires before inherited shutdown')
                check(not failures,('airborne frame stream succeeds',failures))
                check(len(ids)==len(heights)+ground_count+approach_count and approach_count==6 and ground_count>=5 and len(heights)>8 and heights[0]<max(heights)>heights[-1],
                      'every airborne tick presented through ascent and descent')
                check(len(set(samples.values()))==6,'ground and airborne sample screenshots differ')
                for label,pixels in samples.items():
                    png(d/('sonic_emerald_stream_'+label+'.png'),pixels,512,512)
                dash_options = b.get('emerald_light_dash_target',{})
                dash_audio = dash_options.get('scheduled_pose_capture') and not dash_options.get('full_charge_capture')
                expected_events = [725,1118,7,17,33] if dash_audio else [7,17,33]
                check([event[0] for event in audio_events]==expected_events and
                      [event[2] for event in audio_events][-2:]==[1,3],
                      ('pickup, jump and landing reach mixer in order',audio_events))
                check(audio_ticks==len(ids)+1,'audio clock advances once per streamed update and landing')
                check(len(readback_counts)==6 and len(ids)==48,
                      'diagnostic capture reads six of 48 presented stream frames')
                check(all(len(timings[name])==48 for name in
                          ('gpu_submit','gpu_wait_including_audio','window_copy','present',
                           'native_packet_prepare','native_draw_and_submit')) and
                      len(timings['readback'])==6 and len(timings['audio_service'])>=49,
                      'wall-clock timings cover every streamed frame and audio service')
                c['stage_timings']={name:dict(calls=len(values),total=sum(values),maximum=max(values))
                                    for name,values in timings.items()}
                c['readback_profile']=dict(readbacks=len(readback_counts),frames=len(ids),
                                          wall_counts=sum(readback_counts))
                c['airborne_stream_frames']=len(heights)
                c['ground_stream_frames']=ground_count
                c['approach_stream_frames']=approach_count
                return status

""" + marker)
        return raw.replace('actual_window_presents=2',"actual_window_presents=2+c['airborne_stream_frames']+c['ground_stream_frames']+c['approach_stream_frames'],stage_timings=c['stage_timings'],readback_profile=c['readback_profile'],native_clock_audio=c['native_clock_audio'],approach_stream_frames=c['approach_stream_frames'],airborne_stream_frames=c['airborne_stream_frames'],ground_stream_frames=c['ground_stream_frames']").replace('sonic_emerald_same_tick', 'sonic_emerald_projection_profile').replace(
            'native_Emerald_Coast_same_tick_scene_verified',
            'native_Emerald_Coast_projection_profile_verified')

    direct = same_tick.terrain.shared.registry.loader.owner.queued.owner.direct
    harness = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    import verify_native_emerald_ring_audio as ring_audio
    getsource = inspect.getsource

    def audio_source(function):
        source = getsource(function)
        if function is not ring_audio.collect:
            return source
        anchor = 'after.requests == state.requests + 1'
        assert source.count(anchor) == 1
        source = source.replace(anchor, 'after.requests == state.requests + 3')
        start = source.index('    slots = []')
        return source[:start] + "    slots = []\n    return record\n"

    with patch.object(inspect, 'getsource', audio_source), patch.object(same_tick, 'collect', collect):
        exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), harness)
        harness['exercise'](context, inputs, track)


def capture_light_dash_poses(out, manifest, frames, gpu, motions, stand, record_pose,
                            record_scheduled_pose, reserve_pose, check, png, triangles, count):
    import types
    from verify_native_motion_store import View
    from native_gpu_jobs_test_support import Jobs
    from native_gpu_test_support import Config, Failure, Report

    Q, S = C.c_uint64, C.c_size_t
    device = C.CDLL(manifest['device_DLL'])
    handle = bind(device, 'jp_device_gpu', P)()
    jobs = Jobs(types.SimpleNamespace(lib=gpu, handle=P(handle)))
    config = Config.make(512, 512)
    config.logical_width = config.logical_height = 512
    window, error = P(), Failure()
    pixels = C.create_string_buffer(512 * 512 * 4)
    acquire = bind(motions, 'jp_motions_acquire', I, U, C.c_char_p, P)
    release = bind(motions, 'jp_motions_release', I, Q)
    close = bind(gpu, 'jp_window_close', None, P)
    check(bind(gpu, 'jp_window_open', I, P, U, U, I, P, P)(
        handle, 512, 512, 1, C.byref(window), C.byref(error)) == 0, 'pose capture window opens')
    images = []
    actions = ((0, b'sonic_light_dash_action', 'light_dash'),
               (7, b'sonic_skid_action', 'skid'))

    def capture(source, record, label):
        frame, job = 0, Q()
        try:
            frame = record(source)
            check(jobs.submit(handle, frame, C.byref(config), C.byref(job),
                              C.byref(Report()), C.byref(error)) == 0, f'{label} GPU job submits')
            jobs.wait(job.value)
            check(jobs.copy(window, job.value, C.byref(error)) == 0, f'{label} reaches the window')
            check(bind(gpu, 'jp_window_readback', I, P, P, S, P)(
                window, pixels, len(pixels), C.byref(error)) == 0, f'{label} pixels read back')
            rgba = pixels.raw
            check(len(set(zip(rgba[0::4], rgba[1::4], rgba[2::4]))) > 16, f'{label} pose is visible')
            png(out / f'sonic_emerald_pose_{label}.png', rgba, 512, 512)
            images.append(rgba)
            check(bind(gpu, 'jp_window_present', I, P, U, P)(
                window, 1, C.byref(error)) == 0, f'{label} presented')
        finally:
            if job.value:
                check(jobs.release(handle, job.value, C.byref(error)) == 0, f'{label} GPU result retired')
            if frame:
                check(bind(frames, 'jp_frame_release', I, Q)(frame) == 0, f'{label} frame retired')
        check(bind(frames, 'jp_queue_reset_frame', I)() == 0, f'{label} queue reset')
        reserve_pose()

    views = []
    try:
        for bank, action, label in actions:
            view = View()
            check(acquire(bank, action, C.byref(view)) == 0, f'{label} capture lease acquired')
            views.append(view)
        if manifest['emerald_light_dash_target'].get('scheduled_pose_capture'):
            callback = C.CFUNCTYPE(I, P, I)
            errors = []

            @callback
            def observe(task, phase):
                try:
                    capture(task, record_scheduled_pose, actions[phase][2])
                    return 0
                except Exception as exc:
                    errors.append(repr(exc))
                    return -1

            args = (stand, views[0].data, views[1].data, observe)
            if manifest['emerald_light_dash_target'].get('retail_floor_capture'):
                entry = ('jp_emerald_light_dash_observe_geometry_input'
                         if manifest['emerald_light_dash_target'].get('native_capture_input')
                         else 'jp_emerald_light_dash_observe_geometry')
                status = bind(frames, entry,
                              I, P, P, P, callback, P, S)(*args, triangles, count)
            else:
                status = bind(frames, 'jp_emerald_light_dash_observe', I, P, P, P, callback)(*args)
            check(status == 0 and not errors, ('scheduled dash and skid captured', status, errors))
        else:
            for view, (_, _, label) in zip(views, actions):
                capture(view.data, record_pose, label)
        check(images[0] != images[1], 'Light Dash and skid produce distinct poses')
    finally:
        for view in reversed(views):
            check(release(view.token) == 0, 'pose capture lease retired')
        close(window)


if __name__ == '__main__':
    scene = same_tick.terrain.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_projection_profile import exercise')
    sample = "b'Sample(1,0,-100,-12,0,0,0,0)'"
    replacement = (
        "b'(lambda dash, phase: Sample(1,2 if dash==1 else 0,0,0,0,0,0,0) if dash "
        "else Sample(1,4 if phase==2 else 0,0,0,0,0,0,0) if phase "
        "else Sample(1,0,-100,-12,0,0,0,0))"
        "(bind(f,\"jp_emerald_dash_capture_input_phase\",I)() if hasattr(f,\"jp_emerald_dash_capture_input_phase\") else 0,"
        "bind(f,\"jp_emerald_probe_input_phase\",I)())'"
    )
    assert runner.count(sample) == 1
    runner = runner.replace(sample, replacement)
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
