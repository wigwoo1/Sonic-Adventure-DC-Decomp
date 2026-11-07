"""Present the original Sonic walk from the extended runtime motion owner."""
import inspect
import json

import verify_native_sonic_aura_frames as aura_frames
import verify_native_sonic_draw_frames as player_frames


def exercise(context, inputs, track):
    raw = player_frames.derive()
    anchor = "(0,b'sonic_updraft_action',"
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, "(7,b'sonic_walk_action',").replace(
        'motion.frames==33', 'motion.frames==32').replace(
        'all2793 canonical model records', 'all retained model and motion records').replace(
        "'jp_sonic_draw_probe'", "'jp_sonic_scheduled_draw_probe'").replace(
        'sonic_original_player_callback', 'sonic_original_walk').replace(
        'original_Sonic_player_callback_and_retained_GPU_presentation_verified',
        'original_Sonic_walk_action_scheduled_GPU_presentation_verified')
    anchor = "    check(bind(f,'jp_sonic_toes_bind',I)"
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, "    bind(f,'heap_init',None)()\n    bind(f,'task_init',None)()\n"+anchor)
    caller = context['d']/'private_walk_caller.py'
    caller.write_text(raw, encoding='utf-8')
    namespace = dict(vars(aura_frames.tree_suite), __file__=__file__)
    exec(compile(raw,str(caller),'exec'),namespace)
    namespace['exercise'](context,inputs,track)
    result_path = context['d']/'result.json'
    result = aura_frames.read(result_path)
    result.update(original_action='sonic_walk_action',action_bank=7,fixture_action_frame=8.5,
                  derived_caller_sha256=aura_frames.sha(caller))
    result_path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')


if __name__ == '__main__':
    runner = inspect.getsource(aura_frames.main)
    anchor = 'from verify_native_sonic_aura_frames import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor,'from verify_native_sonic_walk_frames import exercise')
    namespace = dict(vars(aura_frames))
    exec(compile(runner,aura_frames.__file__,'exec'),namespace)
    namespace['main']()
