"""Install the placed ring's bounded native scheduler draw callback."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'score', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, base, score, out = (p.resolve() for p in
                             (args.workspace, args.baseline, args.score, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    prior = manifest['emerald_ring_score']
    source = score/'ring_score.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    raw = source.read_bytes()
    for anchor, replacement in (
        (b'static void native_ring_step',
         b'#include "jp_native_ring_draw.inc"\n\nstatic void native_ring_step'),
        (b'        task->destructor = ring_destroy;',
         b'        task->destructor = ring_destroy;\n        task->draw = native_ring_draw;'),
    ):
        assert raw.count(anchor) == 1
        raw = raw.replace(anchor, replacement)
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_draw.c', 'ring_draw.obj', 'jp_native_frames.dll'))
    unit.write_bytes(raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'ring_score.obj')
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
    adapter = root/'outputs/sonic-adventure/native/jp_native_ring_draw.inc'
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_ring_draw=dict(parent_sha256=digest(parent), adapter_sha256=digest(adapter),
                                           source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast scheduled ring draw compiled and linked')


if __name__ == '__main__':
    main()
