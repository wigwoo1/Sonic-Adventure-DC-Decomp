"""Verify gameplay clock and event control against the shared native owner."""
import argparse
import ctypes as C
import json
import os
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
    field = bind(dll, 'jp_boot_reset_field', C.c_void_p, C.c_uint)
    width = bind(dll, 'jp_boot_reset_field_bytes', C.c_uint, C.c_uint)
    indices = {row['name']: row['id'] for row in manifest['boot_reset_fields']}

    def view(name, kind):
        index = indices[name]
        assert width(index) == C.sizeof(kind)
        return kind.from_address(field(index))

    character = view('character_current', C.c_int16)
    steps = view('time_steps', C.c_int)
    minutes, seconds, frames = (view('time_' + part, C.c_int8)
                                for part in ('minutes', 'seconds', 'frames'))
    event = view('event_controlling', C.c_void_p)
    step = bind(dll, 'time_step', None)
    start = bind(dll, 'time_start', None)
    stop = bind(dll, 'time_stop', None)
    reset = bind(dll, 'time_reset', None)
    in_control = bind(dll, 'event_in_control', C.c_int)
    combo = bind(dll, 'jp_gameplay_clock_combo', C.c_int)
    beep = bind(dll, 'jp_gameplay_clock_beep', C.c_int)
    checks = []

    def check(name, actual, expected):
        assert actual == expected, (name, actual, expected)
        checks.append(name)

    def time():
        return minutes.value, seconds.value, frames.value

    character.value = 0
    reset()
    start()
    check('Sonic reset', time(), (0, 0, 0))
    for _ in range(60):
        step()
    check('60 simulation steps make one game second', time(), (0, 1, 0))
    check('shared step counter', steps.value, 60)
    check('combo stays at zero', combo(), 0)
    stop()
    step()
    check('stopped time does not advance', time(), (0, 1, 0))
    check('stopped clock still counts calls', steps.value, 61)
    start()
    minutes.value, seconds.value, frames.value = 99, 59, 59
    step()
    check('Sonic timer saturates', time(), (99, 59, 59))

    character.value = 6
    reset()
    check('Gamma starts at three minutes', time(), (3, 0, 0))
    step()
    check('Gamma borrows from minutes', time(), (2, 59, 59))
    minutes.value, seconds.value, frames.value = 0, 8, 10
    step()
    check('Gamma two-second beep interval', beep(), 120)
    for _ in range(120):
        step()
    check('Gamma repeats the two-second interval', beep(), 120)
    for _ in range(120):
        step()
    check('Gamma one-second beep interval', beep(), 60)
    minutes.value, seconds.value, frames.value = 0, 0, 0
    step()
    check('Gamma timer stays at zero', time(), (0, 0, 0))
    check('no controlling event initially', in_control(), 0)
    marker = C.c_byte()
    event.value = C.addressof(marker)
    try:
        check('query reads the shared controlling event', in_control(), 1)
    finally:
        event.value = None
    check('clearing shared event clears query', in_control(), 0)
    with args.output.open('x', encoding='utf-8') as out:
        json.dump(dict(checks=checks, build_manifest_sha256=sha(report),
                       playable=False, overall_completion_percent=10), out, indent=2)
        out.write('\n')
    print(f'{len(checks)} gameplay clock and event checks passed')


if __name__ == '__main__':
    main()
