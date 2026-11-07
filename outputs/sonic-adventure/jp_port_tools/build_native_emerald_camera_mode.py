"""Link pinned camera mode metadata and region transitions into the stage."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest
from build_native_stage_start import pinned


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (p.resolve() for p in
                                  (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    prior = manifest['emerald_camera_contacts']
    source = base/'camera_contacts.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    modes = pinned(upstream, 'src/camera/modes.c').split(b'camera_mode_list[CAMERA_MODES] = {', 1)[1]
    rows = re.findall(rb'\{\w+,\s*(CAMERA_LEVEL_\w+),\s*(CAMERA_\w+),', modes.split(b'\n};',1)[0])
    assert len(rows) == 72
    metadata = b'static const unsigned char camera_mode_metadata[CAMERA_MODES][2] = {\n'
    metadata += b''.join(b'    {' + level + b', ' + kind + b'},\n' for level, kind in rows) + b'};\n'
    camera = pinned(upstream, 'src/camera/camera.c')
    priority = re.search(rb'^#define PRIORITY_HIGH [^\r\n]+', camera, re.M).group()
    raw = source.read_bytes()
    for old, new in (
        (b'#include "jp_native_emerald_camera.inc"', b'#include "jp_native_emerald_camera.inc"\n'
         + priority + b'\n' + metadata + b'#include "jp_native_emerald_camera_mode.inc"'),
        (b'        emerald_camera_act = -1;', b'        emerald_camera_act = -1;\n        emerald_camera_reset();'),
        (b'    emerald_camera_act = (int)act;', b'    emerald_camera_act = (int)act;\n    emerald_camera_reset();'),
        (b'        emerald_camera_sample(&work->pos);', b'        emerald_camera_sample(&work->pos);\n'
         b'        emerald_camera_choose();'),
    ):
        assert raw.count(old) == 1
        raw = raw.replace(old, new)
    out.mkdir()
    unit, obj, dll = (out/name for name in ('camera_mode.c', 'camera_mode.obj', 'jp_native_frames.dll'))
    unit.write_bytes(raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'camera_contacts.obj')
    link[link.index(old)] = str(obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_camera_mode=dict(parent_sha256=digest(parent), mode_rows=len(rows),
                                              source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast camera mode selection compiled and linked')


if __name__ == '__main__':
    main()
