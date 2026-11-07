"""Relink the qualified ring texture build with native disc archive reads."""
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
    prior = manifest['emerald_stage_ring_textures']
    loader = manifest['emerald_stage_loader']
    source = base/'stage_ring_textures.c'
    assert digest(source) == prior['source_sha256']
    assert digest(native/'jp_native_emerald_stage_ring_textures.inc') == prior['adapter_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    loader_source = Path(loader['commands'][0][-3])
    assert digest(loader_source) == loader['source_sha256']
    raw = loader_source.read_text()
    for old, new in (
        ('"BEACH01.PRS"}', '"BEACH01.PRS", "OBJ_REGULAR.PRS", "EFF_REGULAR.PRS"}'),
        ('i == RESOURCE_COUNT - 1', 'i >= 7'),
        ('decode(RESOURCE_COUNT - 1, bytes[RESOURCE_COUNT - 1])',
         'decode(7, bytes[7]) || decode(8, bytes[8]) || decode(9, bytes[9])'),
        ('i < RESOURCE_COUNT - 1', 'i < 7'),
    ):
        assert raw.count(old) == 1, old
        raw = raw.replace(old, new)

    out.mkdir()
    unit, obj, dll = (out/name for name in ('stage_ring_archives.c', 'stage_ring_archives.obj', 'jp_native_frames.dll'))
    load_unit, load_obj = out/'stage_disc_loader.c', out/'stage_disc_loader.obj'
    load_unit.write_text(raw, encoding='utf-8')
    unit.write_bytes(source.read_bytes() + b'\n#include "jp_native_emerald_stage_ring_archives.inc"\n')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'stage_ring_textures.obj')
    link[link.index(old)] = str(obj)
    old_loader = next(p for p in link if Path(p).name == 'stage_loader.obj')
    link[link.index(old_loader)] = str(load_obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    loader_recipe = loader['commands'][0]
    commands = [loader_recipe[:-4] + ['-c', str(load_unit), '-o', str(load_obj)],
                recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile_loader', 'compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_stage_ring_archives=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_emerald_stage_ring_archives.inc'),
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast stage ring disc archives compiled and linked')


if __name__ == '__main__':
    main()
