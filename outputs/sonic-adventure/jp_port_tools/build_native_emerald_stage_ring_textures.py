"""Relink the qualified ring draw build with stage-owned texture lists."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, base, out = (p.resolve() for p in (args.workspace, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    native = root/'outputs/sonic-adventure/native'
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    prior = manifest['emerald_stage_rings']
    source = base/'stage_rings.c'
    assert digest(source) == prior['source_sha256']
    assert digest(native/'jp_native_emerald_stage_rings.inc') == prior['adapter_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    out.mkdir()
    unit, obj, dll = (out/name for name in ('stage_ring_textures.c', 'stage_ring_textures.obj', 'jp_native_frames.dll'))
    raw = source.read_bytes()
    anchor = b'static int close_stage_textures(void);'
    close = b'if (close_stage_land() || close_stage_textures())'
    assert raw.count(anchor) == raw.count(close) == 1
    raw = raw.replace(anchor, anchor + b'\nstatic int close_stage_ring_textures(void);')
    raw = raw.replace(close, b'if (close_stage_land() || close_stage_textures() || close_stage_ring_textures())')
    unit.write_bytes(raw + b'\n#include "jp_native_emerald_stage_ring_textures.inc"\n')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'stage_rings.obj')
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
                    emerald_stage_ring_textures=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_emerald_stage_ring_textures.inc'),
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast stage ring textures compiled and linked')


if __name__ == '__main__':
    main()
