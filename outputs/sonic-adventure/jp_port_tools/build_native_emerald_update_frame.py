"""Relink the qualified stage frame with player scheduler integration."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    assert source.count('jp_emerald_step_and_submit') == 2
    source = source.replace('jp_emerald_step_and_submit', 'jp_emerald_update_and_submit')
    changes = (
        ('        task_step_level(1);\n        if (ring_walk_enabled && !fixture.result && (!frame || tick != 23) &&',
         '        if (frame && tick == 23) {\n'
         '            if (!fixture.result) fixture_draw(task);\n'
         '            break;\n        }\n'
         '        task_step_level(1);\n        if (ring_walk_enabled && !fixture.result &&'),
        ('        task_draw_level(1);', '        if (!frame) task_draw_level(1);'),
    )
    for old, new in changes:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    return source


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
    prior = manifest['emerald_step_frame']
    source = base/'step_frame.c'
    assert digest(source) == prior['source_sha256']
    assert digest(native/'jp_native_emerald_step_frame.inc') == prior['adapter_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    out.mkdir()
    unit, obj, dll = (out/name for name in ('update_frame.c', 'update_frame.obj', 'jp_native_frames.dll'))
    generated = out/'stage_ring_data.inc'
    assert digest(base/generated.name) == prior['private_data_sha256']
    shutil.copyfile(base/generated.name, generated)
    unit.write_bytes(source.read_bytes() + b'\n#include "jp_native_emerald_update_frame.inc"\n')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    fixture_recipe, recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'step_frame.obj')
    link[link.index(old)] = str(obj)
    fixture_source = Path(fixture_recipe[-3])
    old_fixture = next(p for p in link if Path(p).name == 'player_frame_probe.obj')
    assert Path(old_fixture) == Path(fixture_recipe[-1])
    fixture, fixture_obj = out/'player_frame_probe.c', out/'player_frame_probe.obj'
    fixture.write_text(probe(fixture_source.read_text()), encoding='utf-8')
    link[link.index(old_fixture)] = str(fixture_obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [fixture_recipe[:-4] + ['-c', str(fixture), '-o', str(fixture_obj)],
                recipe[:recipe.index('-c')-1] + ['-I'+str(out), '-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile_probe', 'compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_update_frame=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_emerald_update_frame.inc'),
                        private_data_sha256=digest(generated),
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast update and frame compiled and linked')


if __name__ == '__main__':
    main()
