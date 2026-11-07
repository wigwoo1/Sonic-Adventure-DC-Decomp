"""Verify scheduler dispatch using the qualified Sonic GPU fixture."""
import inspect
import json

import verify_native_sonic_aura_frames as aura_frames
import verify_native_sonic_draw_frames as player_frames


def exercise(context, inputs, track):
    raw = player_frames.derive()
    anchor = "'jp_sonic_draw_probe'"
    assert raw.count(anchor) == 1
    owner_start = "    check(bind(f,'jp_sonic_toes_bind',I)"
    assert raw.count(owner_start) == 1
    raw = raw.replace(owner_start, """    # This fixture skips the original stage boot that initializes the pool.
    bind(f,'heap_init',None)()
    bind(f,'task_init',None)()
"""+owner_start)
    raw = raw.replace(anchor, "'jp_sonic_scheduled_draw_probe'").replace(
        'sonic_original_player_callback', 'sonic_scheduled_player_callback').replace(
        'original_Sonic_player_callback_and_retained_GPU_presentation_verified',
        'scheduled_Sonic_player_callback_and_retained_GPU_presentation_verified')
    caller = context['d'] / 'private_scheduled_player_caller.py'
    caller.write_text(raw, encoding='utf-8')
    namespace = dict(vars(aura_frames.tree_suite), __file__=__file__)
    exec(compile(raw, str(caller), 'exec'), namespace)
    namespace['exercise'](context, inputs, track)
    result_path = context['d'] / 'result.json'
    result = aura_frames.read(result_path)
    result.update(scheduler_entry='task_draw_level(0)', expected_callbacks_per_task=1,
                  synthetic_player_task=True, derived_caller_sha256=aura_frames.sha(caller))
    result_path.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')


if __name__ == '__main__':
    runner = inspect.getsource(aura_frames.main)
    anchor = 'from verify_native_sonic_aura_frames import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_sonic_scheduled_draw import exercise')
    namespace = dict(vars(aura_frames))
    exec(compile(runner, aura_frames.__file__, 'exec'), namespace)
    namespace['main']()
