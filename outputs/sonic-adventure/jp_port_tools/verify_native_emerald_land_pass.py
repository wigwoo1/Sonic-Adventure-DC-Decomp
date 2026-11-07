"""Verify the native terrain pass against the independent POINT pose adapter."""
import inspect
from pathlib import Path
from unittest.mock import patch

import verify_native_emerald_live_land as live


def draw_land(context, track):
    def source_for(function):
        source = live.original_source(function)
        if function is not live.owner.dispatch_models:
            return source
        anchor = "    close = bind(f,'jp_land_task_close',I)"
        assert source.count(anchor) == 1
        source = source.replace(anchor, anchor +
            "\n    native_frame = bind(f,'jp_emerald_draw_land',I,I,I,I)")
        for args in ('-1,1,0', '-1,0,0', '-1,1,1'):
            old = f'frame(camera,0,0,{args})'
            assert old in source
            source = source.replace(old, f'native_frame({args})')
        return source

    with patch.object(inspect, 'getsource', source_for):
        result, release = live.draw_land(context, track)
    assert result['direct_callback_draws'] == result['live_land_visibility']['live_count']
    result['native_land_pass'] = dict(camera_derived_in_native_code=True,
                                      matches_independent_pose_selection=True,
                                      draws=result['direct_callback_draws'])
    return result, release


def scene_caller(raw):
    return live.scene_caller(raw).replace(
        'from verify_native_emerald_live_land import draw_land',
        'from verify_native_emerald_land_pass import draw_land').replace(
        'sonic_emerald_live_land', 'sonic_emerald_land_pass').replace(
        'native_Emerald_Coast_live_land_visibility_verified',
        'native_Emerald_Coast_native_land_pass_verified')


def exercise(context, inputs, track):
    direct = live.shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = live.shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_land_pass import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
