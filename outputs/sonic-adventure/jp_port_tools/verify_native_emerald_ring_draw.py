"""Verify placed rings submit through the Windows task draw scheduler."""
import inspect
from pathlib import Path

import verify_native_emerald_ring_model as model


def draw_land(context, track):
    source = inspect.getsource(model.draw_land)
    begin = "        assert runtime.select(C.byref(listing)) == 1"
    end = "        result['native_ring_model']"
    assert source.count(begin) == source.count(end) == 1
    source = source[:source.index(begin)] + '''        record = Record.from_address(runtime.lookup(C.byref(listing), 0))
        word = record.texparambuffer & 0x1fffff
        used = bind(lib, 'jp_queue_committed_bytes', S, C.c_uint32)
        before = used(0)+used(1)
        bind(lib, 'task_draw_level', None, I)(3)
        assert used(0)+used(1) == before
        draw = bind(lib, 'jp_emerald_ring_draw_level', I,P,P,P,P)
        get_matrix = bind(lib, 'njGetMatrix', None, P)
        current = P.in_dll(lib, '_nj_current_texlist')
        saved = current.value
        matrix, after = (F*16)(), (F*16)()
        get_matrix(matrix)
        rebase = Vec(-x-10, -work.scale[1], -z)
        args = (C.byref(model), C.byref(span), C.byref(listing), C.byref(rebase))
        visible = draw(*args)
        assert visible > 0 and used(0)+used(1) > before
        get_matrix(after)
        assert bytes(matrix) == bytes(after) and current.value == saved

        collector = bind(lib, 'jp_emerald_ring_collector', None, P,F,I,P)
        rings = C.c_int16.in_dll(lib, 'rings')
        lives = C.c_int8.in_dll(lib, 'lives')
        initial = rings.value, lives.value
        try:
            collector(C.byref(work.pos), 1, 0, None)
            tick(3)
            tick(3)
            assert work.header & 255 == 2 and rings.value == initial[0]+1
            assert draw(*args) == visible-1
            get_matrix(after)
            assert bytes(matrix) == bytes(after) and current.value == saved
        finally:
            collector(None, 0, 0, None)
            rings.value, lives.value = initial
        result['native_ring_scheduler_draw'] = dict(draws=visible, collected_skipped=True,
                                                   unscoped_skipped=True, draw_state_restored=True)
''' + source[source.index(end):]
    namespace = dict(vars(model))
    exec(compile(source, model.__file__, 'exec'), namespace)
    return namespace['draw_land'](context, track)


def scene_caller(raw):
    return model.scene_caller(raw).replace(
        'from verify_native_emerald_ring_model import draw_land',
        'from verify_native_emerald_ring_draw import draw_land').replace(
        'sonic_emerald_ring_model', 'sonic_emerald_ring_draw').replace(
        'native_Emerald_Coast_original_ring_model_verified',
        'native_Emerald_Coast_scheduled_ring_draw_verified')


def exercise(context, inputs, track):
    direct = model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct
    namespace = dict(vars(direct), __file__=__file__, scene_caller=scene_caller)
    exec(compile(inspect.getsource(direct.exercise), direct.__file__, 'exec'), namespace)
    namespace['exercise'](context, inputs, track)


if __name__ == '__main__':
    scene = model.walk.score.contact.rings.tasks.registry.loader.owner.queued.owner.direct.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n", 1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor) == 1
    runner = runner.replace(anchor, 'from verify_native_emerald_ring_draw import exercise')
    exec(compile('if True:\n' + runner, scene.__file__, 'exec'), dict(vars(scene)))
