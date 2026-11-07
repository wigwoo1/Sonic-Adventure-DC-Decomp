"""Check the linked stage timer with a controlled clock and exact float results."""
import argparse
import ctypes as C
import json
import os
import struct
from pathlib import Path

from build_native_read_queue import sha
from native_packet_test_support import bind


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('build', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    build = args.build.resolve()
    report = build / 'build_verification.json'
    manifest = json.loads(report.read_bytes())
    path = build / 'jp_native_frames.dll'
    assert sha(path) == manifest['frames_DLL_sha256']
    with os.add_dll_directory(str(build)):
        dll = C.CDLL(str(path))
    source_type = C.CFUNCTYPE(C.c_int, C.c_void_p, C.POINTER(C.c_uint64))
    now = 0

    @source_type
    def source(_context, output):
        output[0] = now
        return 1

    initialize = bind(dll, 'jp_clock_initialize', C.c_int, source_type, C.c_void_p, C.c_uint64)
    write_start = bind(dll, 'jp_clock_write_tstr', None, C.c_uint8)
    reset = bind(dll, 'jp_clock_reset_timer1', C.c_int, C.c_uint32)
    shutdown = bind(dll, 'jp_clock_shutdown', None)
    mark = bind(dll, 'timer_mark', None)
    lap = bind(dll, 'timer_lap', C.c_float)
    elapsed = bind(dll, 'timer_elapsed', C.c_float)
    elapsed_and_mark = bind(dll, 'timer_elapsed_and_mark', C.c_float)

    def f32(value):
        return C.c_float(value).value

    unit = f32(f32(f32(.08) * f32(.001)) * 60)
    checks = {}

    def check(name, actual, ticks):
        expected = f32(f32(ticks) * unit)
        assert struct.pack('<f', actual) == struct.pack('<f', expected), (name, actual, expected)
        checks[name] = actual

    assert initialize(source, None, 12_500_000) == 0
    try:
        write_start(2)
        mark()
        now += 3_125
        check('elapsed_without_moving_mark', elapsed(), 3_125)
        now += 9_375
        check('lap_advances_mark', lap(), 12_500)
        check('second_lap_at_same_tick', lap(), 0)
        now += 2_000
        check('elapsed_and_mark', elapsed_and_mark(), 14_500)
        now += 500
        check('lap_from_elapsed_mark', lap(), 500)
        write_start(0)
        now += 50_000
        check('paused_lap', lap(), 0)
        write_start(2)
        now += 500
        check('resumed_lap', lap(), 500)
        assert reset(12_500_000) == 0
        now += 12_499_999
        mark()
        now += 3
        check('original_unsigned_lap_at_counter_reload', lap(), (1 - 12_499_999) & 0xffffffff)
        check('elapsed_after_reload', elapsed(), 1)
    finally:
        shutdown()
    with args.output.open('x', encoding='utf-8') as out:
        json.dump(dict(checks=checks, build_manifest_sha256=sha(report),
                       overall_completion_percent=10, playable=False), out, indent=2)
        out.write('\n')
    print(f'{len(checks)} stage timer checks passed')


if __name__ == '__main__':
    main()
