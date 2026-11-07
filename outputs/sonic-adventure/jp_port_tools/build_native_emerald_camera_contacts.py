"""Link camera volume contacts to the native Emerald Coast stage tick."""
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
    prior = manifest['emerald_stage_tick']
    stage = base/'stage_session.c'
    native = root/'outputs/sonic-adventure/native'
    assert digest(stage) == prior['source_sha256'][stage.name]
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    camera = pinned(upstream, 'src/camera/camera.c')
    pinned(upstream, 'src/camera/camera.h')
    height = re.search(rb'^#define TEST_HEIGHT [^\r\n]+', camera, re.M).group()
    raw = stage.read_bytes()
    tick = (native/'jp_native_emerald_stage_tick.inc').read_bytes()
    anchor = b'    if (result >= 0) task_step_level(3);'
    assert tick.count(anchor) == 1
    tick = tick.replace(anchor, b'    if (result >= 0) {\n'
                        b'        emerald_camera_sample(&work->pos);\n        task_step_level(3);\n    }')
    for old, new in (
        (b'static int emerald_stage_stepping;', b'int jp_stage_resources_get(unsigned, struct resource_view *);\n'
         + height + b'\n#include "jp_native_emerald_camera.inc"\n\nstatic int emerald_stage_stepping;'),
        (b'    stage_session |= STAGE_FLOOR;', b'    stage_session |= STAGE_FLOOR;\n'
         b'    emerald_camera_act = (int)act;\n    emerald_camera_count = 0;'),
        (b'        stage_session &= ~STAGE_FLOOR;', b'        stage_session &= ~STAGE_FLOOR;\n'
         b'        emerald_camera_act = -1;\n        emerald_camera_count = 0;'),
        (b'#include "jp_native_emerald_stage_tick.inc"', tick),
    ):
        assert raw.count(old) == 1
        raw = raw.replace(old, new)
    out.mkdir()
    unit, obj, dll = (out/name for name in ('camera_contacts.c', 'camera_contacts.obj', 'jp_native_frames.dll'))
    unit.write_bytes(raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, _, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'stage_session.obj')
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
                    emerald_camera_contacts=dict(parent_sha256=digest(parent),
                                                  source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast camera contacts compiled and linked')


if __name__ == '__main__':
    main()
