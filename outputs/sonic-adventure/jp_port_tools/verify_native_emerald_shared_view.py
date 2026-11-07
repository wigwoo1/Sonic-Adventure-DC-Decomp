"""Submit Sonic, land, rings and sparkles through the same live POINT view."""
import ctypes as C
import inspect
from pathlib import Path

import verify_native_emerald_camera_view as view
from native_packet_test_support import bind

sparkle = view.mode.contacts.stage.session.audio.sparkle
registry = sparkle.finish.draw.model.walk.score.contact.rings.tasks.registry
P, F = C.c_void_p, C.c_float
Matrix = F*16


def shared_land(context, track):
    lib = context['f']
    matrix = context['shared_point_view']
    set_matrix = bind(lib, 'njSetMatrix', None, P,P)
    display_drop = registry.loader.owner.queued.owner.direct.textured.scene.DISPLAY_DROP
    # Cancel the old land fixture's diagnostic stage translation.
    bind(lib, 'njTranslate', None, P,F,F,F)(None, -9, 4+display_drop, 4)
    try:
        return registry.draw_land(context, track)
    finally:
        set_matrix(None, matrix)


def draw_land(context, track):
    source = inspect.getsource(sparkle.draw_land)
    anchor = '    namespace = dict(vars(previous), adapt=adapt, draw_sparkles=draw_sparkles)'
    assert source.count(anchor) == 1

    def adapt(recipe):
        recipe = sparkle.adapt(recipe)
        old = 'walk.score.contact.rings.tasks.registry.draw_land(context, track)'
        assert recipe.count(old) == 1
        recipe = recipe.replace(old, 'shared_land(context, track)')
        old = 'rebase = Vec(-x-10, -work.scale[1], -z)'
        assert recipe.count(old) == 1
        return recipe.replace(old, 'rebase = Vec(0,0,0)')

    source = source.replace(anchor,
        "    source = source.replace('draw_sparkles=draw_sparkles)', "
        "'draw_sparkles=draw_sparkles, shared_land=shared_land)', 1)\n"
        "    namespace = dict(vars(previous), adapt=adapt, draw_sparkles=draw_sparkles, shared_land=shared_land)")
    namespace = dict(vars(sparkle), adapt=adapt, shared_land=shared_land)
    exec(compile(source, sparkle.__file__, 'exec'), namespace)
    lib = context['f']
    saved = Matrix()
    bind(lib, 'njGetMatrix', None, P)(saved)
    set_matrix = bind(lib, 'njSetMatrix', None, P,P)
    set_matrix(None, context['shared_point_view'])
    try:
        result, release = namespace['draw_land'](context, track)
        result['shared_POINT_view'] = True
        return result, release
    finally:
        set_matrix(None, saved)


def scene_caller(raw):
    raw = view.scene_caller(raw).replace(
        'from verify_native_emerald_ring_sparkle import draw_land',
        'from verify_native_emerald_shared_view import draw_land').replace(
        'sonic_emerald_camera_view', 'sonic_emerald_shared_view').replace(
        'native_Emerald_Coast_POINT_camera_view_verified',
        'native_Emerald_Coast_shared_POINT_view_verified')
    anchor = '                floors = [h for h in surfaces'
    assert raw.count(anchor) == 1
    return raw.replace(anchor, """                point_matrix = (F*16)()
                if bind(f,'jp_emerald_point_view',I,P,P)(position,point_matrix)==0:
                    c['shared_point_view'] = point_matrix
""" + anchor)


def exercise(context, inputs, track):
    direct = registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_shared_view import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
