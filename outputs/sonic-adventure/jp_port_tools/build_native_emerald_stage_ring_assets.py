"""Relink the qualified disc build with locally generated ring and sparkle geometry."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest
from generate_native_emerald_ring_data import generate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'output', 'generated'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, base, out = (p.resolve() for p in (args.workspace, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    native = root/'outputs/sonic-adventure/native'
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    prior = manifest['emerald_stage_ring_archives']
    source = base/'stage_ring_archives.c'
    assert digest(source) == prior['source_sha256']
    assert digest(native/'jp_native_emerald_stage_ring_archives.inc') == prior['adapter_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    out.mkdir()
    unit, obj, dll = (out/name for name in ('stage_ring_assets.c', 'stage_ring_assets.obj', 'jp_native_frames.dll'))
    generated = out/'stage_ring_data.inc'
    generated.write_text(generate(args.generated), encoding='utf-8')
    unit.write_bytes(source.read_bytes() + b'\n#include "jp_native_emerald_stage_ring_assets.inc"\n')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    _, recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'stage_ring_archives.obj')
    link[link.index(old)] = str(obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-I'+str(out), '-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_stage_ring_assets=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_emerald_stage_ring_assets.inc'),
                        private_data_sha256=digest(generated),
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast stage ring assets compiled and linked')


if __name__ == '__main__':
    main()
