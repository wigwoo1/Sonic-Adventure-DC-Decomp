"""Extend the existing boot state owner with the original gameplay clock and event query."""
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
    root, upstream, base, out = (p.resolve() for p in
                                (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    link = shlex.split((base / 'link.rsp').read_text())
    old = next(arg for arg in link if Path(arg).name == 'boot_reset_original.obj')
    owner = Path(old).parent
    owner_manifest = json.loads((owner / 'build_verification.json').read_bytes())
    old_source = owner / 'external_boot_reset.c'
    assert sha(old_source) == owner_manifest['boot_reset_generated_source_sha256'][old_source.name]
    recipe = owner_manifest['commands'][0]['command']

    def pinned(path):
        raw = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{path}'])
        assert raw == (upstream / path).read_bytes()
        return raw

    hud = pinned('src/hud/hud.c')
    event = pinned('src/event/play.c')
    fields = re.findall(rb'^static (?:int combo_timer|Sint8 beep_timer);', hud, re.M)
    assert len(fields) == 2
    beep = re.search(rb'^#define SE_TIME_BEEP [^\r\n]+', hud, re.M).group()
    source = out / old_source.name
    obj, dll = out / Path(old).name, out / 'jp_native_frames.dll'
    out.mkdir()
    source.write_bytes(b'\n'.join([
        b'#include "sound/sound.h"', old_source.read_bytes(), beep, *fields,
        owner_function(hud, 'time_step'), owner_function(event, 'event_in_control'),
        b'__declspec(dllexport) int jp_gameplay_clock_combo(void) { return combo_timer; }',
        b'__declspec(dllexport) int jp_gameplay_clock_beep(void) { return beep_timer; }',
    ]))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link[link.index(old)] = obj.as_posix()
    assert link[-2] == '-o'
    link[-2:] = ['-Wl,/export:time_step', '-Wl,/export:event_in_control', '-o', dll.as_posix()]
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    driver = manifest['emerald_stage_timer']['commands'][1][0]
    commands = [recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)],
                [driver, '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_gameplay_clock=dict(upstream_commit=PIN, parent_sha256=sha(parent),
                                                owner_source_sha256=sha(old_source),
                                                generated_source_sha256=sha(source),
                                                object_sha256=sha(obj), commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Original gameplay clock and event query compiled and linked')


if __name__ == '__main__':
    main()
