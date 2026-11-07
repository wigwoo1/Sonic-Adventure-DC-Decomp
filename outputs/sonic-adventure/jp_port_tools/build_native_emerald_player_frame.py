"""Relink the qualified frame build with the native player draw and submission."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('struct animation_fixture {',
         'struct emerald_scene_draw;\n'
         'int jp_emerald_submit_player_frame(struct task *, NJS_TEXLIST *, NJS_TEXLIST *, '
         'NJS_OBJECT *, struct emerald_scene_draw *, uint64_t *);\n'
         'struct animation_fixture {\n'
         '    struct emerald_scene_draw *scene;\n    uint64_t *frame;'),
        ('    fixture->draws++;',
         '    fixture->draws++;\n'
         '    if (fixture->frame) {\n'
         '        fixture->result = jp_emerald_submit_player_frame(task, fixture->textures,\n'
         '            fixture->textures, fixture->aura, fixture->scene, fixture->frame);\n'
         '        return;\n    }'),
        ('int jp_sonic_emerald_task_probe(',
         'int jp_sonic_emerald_task_frame_probe('),
        ('size_t triangle_count, NJS_POINT3 *position, Uint32 *entry)',
         'size_t triangle_count, NJS_POINT3 *position, Uint32 *entry,\n'
         '    struct emerald_scene_draw *scene, uint64_t *frame)'),
        ('.textures = textures, .aura = aura,',
         '.scene = scene, .frame = frame, .textures = textures, .aura = aura,'),
    )
    for old, new in changes:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    return source + """
__declspec(dllexport) int jp_sonic_emerald_task_probe(
    NJS_TEXLIST *textures, NJS_OBJECT *aura, NJS_ACTION *stand, NJS_ACTION *walk,
    int charged, int hidden, Float *drawn_frame, const struct jp_stage_triangle *triangles,
    size_t triangle_count, NJS_POINT3 *position, Uint32 *entry)
{
    return jp_sonic_emerald_task_frame_probe(textures, aura, stand, walk, charged, hidden,
        drawn_frame, triangles, triangle_count, position, entry, 0, 0);
}
"""


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
    prior = manifest['emerald_stage_frame']
    source = base/'stage_frame.c'
    assert digest(source) == prior['source_sha256']
    assert digest(native/'jp_native_emerald_stage_frame.inc') == prior['adapter_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    out.mkdir()
    unit, obj, dll = (out/name for name in ('player_frame.c', 'player_frame.obj', 'jp_native_frames.dll'))
    generated = out/'stage_ring_data.inc'
    assert digest(base/generated.name) == prior['private_data_sha256']
    shutil.copyfile(base/generated.name, generated)
    unit.write_bytes(source.read_bytes() + b'\n#include "jp_native_emerald_player_frame.inc"\n')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'stage_frame.obj')
    link[link.index(old)] = str(obj)
    fixture_recipe = manifest['emerald_camera_view']['commands'][1]
    fixture_source = Path(fixture_recipe[-3])
    old_fixture = next(p for p in link if Path(p).name == 'ring_walk.obj')
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
                    emerald_player_frame=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_emerald_player_frame.inc'),
                        private_data_sha256=digest(generated),
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast player frame compiled and linked')


if __name__ == '__main__':
    main()
