"""Exercise the in-lease consumer and prove restoration on success and failure."""
import argparse
import ctypes as c
import json
from pathlib import Path

from build_native_simple_material import digest
from native_packet_test_support import bind
from verify_native_model_preflight import Model, Vec

I, U, P, F, S = c.c_int32, c.c_uint32, c.c_void_p, c.c_float, c.c_size_t
Consumer = c.CFUNCTYPE(I, P, P, S, P)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', type=Path, required=True)
    ap.add_argument('--output', type=Path, required=True)
    a = ap.parse_args()
    b = json.loads((a.build / 'build_verification.json').read_bytes())
    assert digest(b['frames_DLL']) == b['frames_DLL_sha256']
    f = c.CDLL(b['frames_DLL'])
    init = bind(f, 'jp_simple_calc_initialize', I, U, S)
    close = bind(f, 'jp_simple_calc_shutdown', I)
    records = bind(f, 'jp_simple_calc_records', P)
    guards = bind(f, 'jp_simple_calc_guards', I)
    calc = bind(f, 'jp_simple_calc_model', I, P, S, S, P, F, P)
    lease = bind(f, 'jp_simple_calc_with_records', I, P, S, S, P, F, P, Consumer, P)
    matrix = (F * 16)(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
    bind(f, 'njSetMatrix', None, P, P)(None, matrix)
    (F * 5).in_dll(f, '_nj_screen_')[:] = [256, 640, 480, 320, 240]
    (F * 4).in_dll(f, '_nj_2d_clip_')[:] = [0, 0, 640, 480]
    F.in_dll(f, '_nj_near_clip_').value = 1
    F.in_dll(f, '_nj_far_clip_').value = 1000
    F.in_dll(f, 'port_draw_aspect').value = 1
    F.in_dll(f, '_nj_scr_aspect_y_').value = -1
    control = U.in_dll(f, '_nj_control_3d_flag_')
    vertex = P.in_dll(f, '_nj_vertex_buf_')
    count = I.in_dll(f, '_nj_vertex_num_')
    borrowed = c.create_string_buffer(b'\x6d' * 48, 48)
    vertex.value, count.value = c.addressof(borrowed), 3
    assert init(8, 320) == 0
    address = records()
    pts = (Vec * 3)(Vec(-1, 0, 8), Vec(1, 0, 8), Vec(0, 1, 8))
    model = Model()
    model.points, model.point_count, model.center, model.radius = pts, 3, Vec(0, 0, 8), 2
    light, result = Vec(.5, -1, .75), I(123)
    control.value = 256
    assert calc(c.byref(model), 3, 0, c.byref(light), .8, c.byref(result)) == result.value == 0
    expected = c.string_at(address, 96)
    assert any(expected)
    seen = []
    checks = {}

    def restored():
        return vertex.value == c.addressof(borrowed) and count.value == 3 and bytes(borrowed) == b'\x6d' * 48

    for case in ('success', 'consumer_error', 'culled', 'bad_span', 'guard_corruption'):
        result.value = 123
        before = len(seen)
        control.value = 256
        model.center = Vec(0, 0, -10 if case == 'culled' else 8)

        @Consumer
        def consume(model_ptr, data, vertices, context):
            nested = I(321)
            seen.append((model_ptr, data, vertices, context, vertex.value, count.value,
                         c.string_at(data, 96), guards(), close(), init(8, 320),
                         calc(model_ptr, 3, 0, c.byref(light), .8, c.byref(nested)), nested.value))
            if case == 'guard_corruption':
                c.c_ubyte.from_address(address + 256).value = 0
            return 77 if case == 'consumer_error' else 0

        status = lease(c.byref(model), 2 if case == 'bad_span' else 3, 0,
                       c.byref(light), .8, c.byref(result), consume, 1234)
        expected_status, expected_result, called = {
            'success': (0, 0, 1), 'consumer_error': (77, 123, 1), 'culled': (0, -1, 0),
            'bad_span': (1, 123, 0), 'guard_corruption': (5, 123, 1)}[case]
        assert (status, result.value, len(seen) - before) == (expected_status, expected_result, called), case
        assert restored(), case
        if called:
            assert seen[-1] == (c.addressof(model), address, 3, 1234, address, 8, expected,
                                0, 4, 1, 4, 321), case
        checks[case] = dict(status=status, projection_result=result.value, consumer_calls=called)
    assert close() == 5 and not records() and close() == 0
    with a.output.open('x') as out:
        json.dump(dict(cases=checks, nested_projection_and_owner_mutation_rejected=True,
                       shared_scratch_restored=True, frames_DLL_sha256=b['frames_DLL_sha256']), out, indent=2)
        out.write('\n')
    print('Five scratch lease cases passed', flush=True)


if __name__ == '__main__':
    main()
