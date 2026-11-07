"""Relink the original stage timer reads to the native Timer 1 provider."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function, sha


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (
        path.resolve() for path in (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    path = 'src/system/timer.c'
    raw = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{path}'])
    assert raw == (upstream / path).read_bytes()
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    recipe = manifest['emerald_stage_tick']['commands'][0]
    names = ('timer_mark', 'timer_lap', 'timer_elapsed', 'timer_elapsed_and_mark')
    constants = [re.search(rb'^#define ' + name + rb' [^\r\n]+', raw, re.M).group()
                 for name in (b'TICKS_PER_SECOND', b'TICK')]
    preamble = b'''#include <ninja.h>
#include "jp_native_clock.h"
#define TCNT1 jp_clock_read_tcnt1()
static Uint32 timer_marked;
'''
    out.mkdir()
    source, obj, dll = (out / name for name in
                        ('stage_timer.c', 'stage_timer.obj', 'jp_native_frames.dll'))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    source.write_bytes(b'\n'.join([preamble, *constants, *(owner_function(raw, n) for n in names)]))
    link = shlex.split((base / 'link.rsp').read_text())
    assert link[-2] == '-o'
    link[-2:] = [obj.as_posix(), *('-Wl,/export:' + name for name in names), '-o', dll.as_posix()]
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    driver = manifest['emerald_projection_profile']['commands'][1][0]
    commands = [recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)],
                [driver, '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_stage_timer=dict(upstream_commit=PIN, upstream_sha256=sha(upstream / path),
                                             parent_sha256=sha(parent), object_sha256=sha(obj),
                                             generated_source_sha256=sha(source), commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Original stage timer reads compiled and linked')


if __name__ == '__main__':
    main()
