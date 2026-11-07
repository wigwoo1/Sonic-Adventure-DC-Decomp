"""Exercise the original player callback on the native GPU backend."""
import inspect
import json

import verify_native_sonic_aura_frames as aura_frames


def derive():
    raw = aura_frames.derive()

    def swap(old, new):
        nonlocal raw
        assert raw.count(old) == 1, old
        raw = raw.replace(old, new)

    swap("    try:\n        control_flags=U.in_dll",
         """    check(bind(f,'jp_sonic_toes_bind',I)()==0,'actual Sonic toe views bound')
    try:
        control_flags=U.in_dll""")
    swap("            bind(f,'jp_sonic_aura_probe',None,P,P,F,I)(aura_view.data,action_view.data,8.5,0)",
         """            draw=bind(f,'jp_sonic_draw_probe',I,P,P,P,F,I,I)
            def count_batches(charged, hidden):
                check(draw(listing,aura_view.data,action_view.data,8.5,charged,hidden)==0,
                      ('original player callback succeeds',charged,hidden))
                frame=Q()
                check(bind(f,'jp_frames_submit',I,P)(C.byref(frame))==0,'comparison frame retained')
                decoder=Decoder(b['GPU_DLL']);plan=P();failure=PacketFailure()
                check(decoder.frame(frame.value,64<<20,C.byref(plan),C.byref(failure))==0,
                      'comparison packets decode')
                batches=C.cast(plan,C.POINTER(Plan)).contents.batch_count
                decoder.destroy(plan)
                check(bind(f,'jp_frame_release',I,Q)(frame.value)==0 and
                      bind(f,'jp_queue_reset_frame',I)()==0,'comparison frame retired')
                check(bind(f,'jp_model_reserve',I,P)(C.byref(bound))==0 and
                      bind(f,'jp_frames_prepare_textures',I,P,U)(words,len(words))==0,
                      'next recording reserves original model buffers and retains textures')
                return batches

            hidden_batches=count_batches(1,1)
            uncharged_batches=count_batches(0,0)
            check(hidden_batches==0 and uncharged_batches>0,
                  'hidden callback skips geometry and uncharged callback draws the body')
            check(draw(listing,aura_view.data,action_view.data,8.5,1,0)==0,
                  'original charged player draw callback succeeds')""")
    swap("    check(draw_status==0,", """    check(bind(f,'jp_sonic_toes_close',I)()==0,'actual Sonic toe views retired')
    check(draw_status==0,""")
    swap('decoded.batch_count==3*len(models)', 'decoded.batch_count==uncharged_batches+3*len(models)')
    swap('packet_counts=counts,retail_palette',
         'packet_counts=counts,hidden_batches=hidden_batches,uncharged_batches=uncharged_batches,retail_palette')
    swap('off_rgba==rgba and report.texture_uploads==0', 'off_rgba!=rgba and report.texture_uploads>0')
    return raw.replace('sonic_original_aura', 'sonic_original_player_callback').replace(
        'original_Sonic_aura_callback_and_retained_GPU_presentation_verified',
        'original_Sonic_player_callback_and_retained_GPU_presentation_verified').replace(
        "'original_untextured_aura'", "'original_textures'")


def exercise(context, inputs, track):
    caller = context['d'] / 'private_player_draw_caller.py'
    caller.write_text(derive(), encoding='utf-8')
    namespace = dict(vars(aura_frames.tree_suite), __file__=__file__)
    exec(compile(caller.read_bytes(), str(caller), 'exec'), namespace)
    namespace['exercise'](context, inputs, track)
    result_path = context['d'] / 'result.json'
    result = aura_frames.read(result_path)
    result.update(derived_caller_sha256=aura_frames.sha(caller),
                  fixture_action_frame=8.5, fixture_player_charged=True)
    result_path.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')


if __name__ == '__main__':
    # Preserve the original aura verifier and its proof hashes.
    runner = inspect.getsource(aura_frames.main)
    anchor = 'from verify_native_sonic_aura_frames import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_sonic_draw_frames import exercise')
    runner_globals = dict(vars(aura_frames))
    exec(compile(runner, aura_frames.__file__, 'exec'), runner_globals)
    runner_globals['main']()
