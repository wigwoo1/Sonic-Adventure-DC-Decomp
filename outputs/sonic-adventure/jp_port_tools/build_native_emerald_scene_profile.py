"""Relink the qualified recording profile with native draw component timings."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def instrument(source, native, manifest):
    phases = (
        ('stage_scene', 'emerald_stage_scene', (
            ('    result.terrain = jp_emerald_stage_terrain_draw(blocks, TRUE, step);',
             '    uint64_t start = jp_clock_wall_counts();\n'
             '    result.terrain = jp_emerald_stage_terrain_draw(blocks, TRUE, step);\n'
             '    scene_ticks[1] = jp_clock_wall_counts()-start;'),
            ('    int failed = result.terrain < 0 ||',
             '    start = jp_clock_wall_counts();\n    int failed = result.terrain < 0 ||'),
            ('    njSetMatrix(0, &saved);',
             '    scene_ticks[2] = jp_clock_wall_counts()-start;\n    njSetMatrix(0, &saved);'),
        )),
        ('stage_frame', 'emerald_stage_frame', (
            ('    if (jp_emerald_stage_draw_scene(view, blocks, step, &result) ||\n'
             '        jp_frames_submit(&id)) return -1;',
             '    if (jp_emerald_stage_draw_scene(view, blocks, step, &result)) return -1;\n'
             '    uint64_t start = jp_clock_wall_counts();\n'
             '    if (jp_frames_submit(&id)) return -1;\n'
             '    scene_ticks[3] = jp_clock_wall_counts()-start;'),
        )),
        ('player_frame', 'emerald_player_frame', (
            ('    int failed = jp_sonic_draw_with_assets(player, body, effect, aura);',
             '    uint64_t start = jp_clock_wall_counts();\n'
             '    int failed = jp_sonic_draw_with_assets(player, body, effect, aura);\n'
             '    scene_ticks[0] = jp_clock_wall_counts()-start;'),
        )),
    )
    for name, manifest_key, replacements in phases:
        filename = f'jp_native_emerald_{name}.inc'
        path = native/filename
        assert digest(path) == manifest[manifest_key]['adapter_sha256']
        unit = path.read_text()
        for old, new in replacements:
            assert unit.count(old) == 1, old
            unit = unit.replace(old, new)
        anchor = f'#include "{filename}"'
        assert source.count(anchor) == 1
        source = source.replace(anchor, unit)
    return """#include "jp_native_clock.h"
static uint64_t scene_ticks[4];
__declspec(dllexport) void jp_emerald_profile_scene(uint64_t *out)
{
    for (unsigned i = 0; i < 4; ++i) out[i] = scene_ticks[i];
}
""" + source


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
    prior = manifest['emerald_update_frame']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(Path(p) for p in link if Path(p).name == 'update_frame.obj')
    source = old.with_suffix('.c')
    assert digest(source) == prior['source_sha256']
    assert digest(native/'jp_native_emerald_update_frame.inc') == prior['adapter_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    out.mkdir()
    unit, obj, dll = (out/name for name in ('scene_profile.c', 'scene_profile.obj', 'jp_native_frames.dll'))
    generated = out/'stage_ring_data.inc'
    assert digest(old.parent/generated.name) == prior['private_data_sha256']
    shutil.copyfile(old.parent/generated.name, generated)
    unit.write_text(instrument(source.read_text(), native, manifest), encoding='utf-8')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    _, recipe, driver = prior['commands']
    link[link.index(old.as_posix())] = str(obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')-1] + ['-I'+str(out), '-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_scene_profile=dict(parent_sha256=digest(parent),
                        
                        private_data_sha256=digest(generated),
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast scene profile compiled and linked')


if __name__ == '__main__':
    main()
