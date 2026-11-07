"""Feed the live POINT camera pose to the original land scheduler."""
import ctypes as C
import inspect
import math
from pathlib import Path
from unittest.mock import patch

import verify_native_emerald_shared_view as shared
from native_packet_test_support import bind

owner = shared.registry.loader.owner.queued.owner
visible = owner.direct.dispatch.visible
F = C.c_float
original_source = inspect.getsource


def land_source(function):
    source = original_source(function)
    if function is not owner.direct.textured.draw_textured_land:
        return source
    for old, new in (
        ('            assert attr&0x200000 and uv',
         '            textured_mesh = bool(attr&0x200000)\n'
         '            assert textured_mesh == bool(uv)'),
        ("            if by_slot[slot]['kind']&0xff00==0x400:",
         "            if textured_mesh and by_slot[slot]['kind']&0xff00==0x400:"),
        ("                tex = read_at(f'<{count*2}h',uv)",
         "                tex = read_at(f'<{count*2}h',uv) if textured_mesh else [0]*(count*2)"),
        ('                uv += count*4', '                if textured_mesh: uv += count*4'),
        ('None,None,None,C.addressof(uv_array))',
         'None,None,None,C.addressof(uv_array) if textured_mesh else None)'),
        ('Span(len(order),len(order))', 'Span(len(order),len(order) if textured_mesh else 0)'),
        ('                emitted_slots.add(slot)', '                if textured_mesh: emitted_slots.add(slot)'),
    ):
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    return source


def pose(matrix):
    # The matrix is world-to-view, so its transposed rotation maps the
    # negative translation back to the world-space camera origin.
    eye = [-sum(matrix[4*i+j]*matrix[12+j] for j in range(3)) for i in range(3)]
    x, y, z = matrix[2], matrix[6], matrix[10]
    scale = 65536 / math.tau
    return eye, int(math.atan2(-x, -z)*scale), int(math.atan2(y, math.hypot(x,z))*scale)


def draw_land(context, track):
    eye, yaw, pitch = pose(context['shared_point_view'])
    probe = inspect.getsource(visible.select_land)
    probe = probe.replace('def select_land(context,image):',
                          'def select_land(context,image,eye,yaw,pitch):')
    old = '    camera = (F*3)(-9,16,4)'
    assert probe.count(old) == 1
    probe = probe.replace(old, '    camera = (F*3)(*eye)')
    old = 'clip,2400,1,camera,0,0,output'
    assert probe.count(old) == 1
    probe = probe.replace(old, "clip,F.in_dll(context['f'],'_nj_far_clip_').value,"
                          "F.in_dll(context['f'],'port_draw_aspect').value,camera,yaw,pitch,output")
    probe_globals = dict(vars(visible))
    exec(compile(probe, visible.__file__, 'exec'), probe_globals)
    selector = probe_globals['select_land']
    orders = {}

    def compare(context, image):
        orders['fixed'] = selector(context, image, (-9,16,4), 0, 0)
        orders['live'] = selector(context, image, eye, yaw, pitch)
        return orders['live']

    source = inspect.getsource(owner.dispatch_models)
    old = '    camera = (F*3)(-9,16,4)'
    assert source.count(old) == 1
    source = source.replace(old, '    camera = (F*3)(*eye)')
    source = source.replace('frame(camera,0,0,', 'frame(camera,yaw,pitch,')
    old = 'dict(vars(direct.dispatch),render_model=render_model)'
    assert source.count(old) == 1
    source = source.replace(old, 'dict(vars(direct.dispatch),render_model=render_model,eye=eye,yaw=yaw,pitch=pitch)')
    namespace = dict(vars(owner), eye=eye, yaw=yaw, pitch=pitch)
    exec(compile(source, owner.__file__, 'exec'), namespace)
    with patch.object(inspect, 'getsource', land_source), \
         patch.object(visible, 'select_land', compare), \
         patch.object(owner, 'dispatch_models', namespace['dispatch_models']):
        result, release = shared.draw_land(context, track)
    assert result['visibility_draw_order'] == orders['live'] != orders['fixed']
    result['live_land_visibility'] = dict(eye=eye, yaw=yaw, pitch=pitch,
        fixed_count=len(orders['fixed']), live_count=len(orders['live']),
        added=sorted(set(orders['live'])-set(orders['fixed'])),
        removed=sorted(set(orders['fixed'])-set(orders['live'])))
    return result, release


def scene_caller(raw):
    raw = shared.scene_caller(raw).replace(
        'from verify_native_emerald_shared_view import draw_land',
        'from verify_native_emerald_live_land import draw_land').replace(
        'sonic_emerald_shared_view', 'sonic_emerald_live_land').replace(
        'native_Emerald_Coast_shared_POINT_view_verified',
        'native_Emerald_Coast_live_land_visibility_verified')
    anchor = "land['direct_callback_draws']==13"
    assert raw.count(anchor) == 1
    return raw.replace(anchor, "land['direct_callback_draws']==land['live_land_visibility']['live_count']").replace(
        'the original land callback invokes bounded model drawing 13 times',
        'the original land callback draws the live camera selection')


def exercise(context, inputs, track):
    direct = shared.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = shared.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_live_land import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
