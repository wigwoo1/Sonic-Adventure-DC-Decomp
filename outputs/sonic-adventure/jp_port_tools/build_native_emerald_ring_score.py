"""Link the pinned action-stage ring and extra-life accounting."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


def action_counter(hud, name):
    source = owner_function(hud, name)
    begin = source.index(b'    if (stage_group() == STAGE_GROUP_FIELD)')
    assert source.endswith(b'    }\n}\n')
    return source[:begin] + b'}\n'


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
    prior = manifest['emerald_ring_contact']
    source = base/'ring_contact.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    hud = pinned(upstream, 'src/hud/hud.c')
    lives = action_counter(hud, 'lives_add')
    audio = b'    if (count > 0) {\n        sound_play(SE_EXTRA_LIFE, 0, 0, 0);\n    }\n'
    assert lives.count(audio) == 1
    lives = lives.replace(audio, b'')
    raw = source.read_bytes()
    anchor = b'        if (!collector.falling && pickup) pickup(1);'
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, b'''        if (!collector.falling) {
            ring_add(1);
            if (pickup) pickup(1);
        }''')
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_score.c', 'ring_score.obj', 'jp_native_frames.dll'))
    unit.write_bytes(b'#include "stage/stage.h"\n#include "hud/hud.h"\n' +
                     raw + lives + action_counter(hud, 'ring_add'))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'ring_contact.obj')
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
                    emerald_ring_score=dict(parent_sha256=digest(parent),
                                            source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ring scoring compiled and linked')


if __name__ == '__main__':
    main()
