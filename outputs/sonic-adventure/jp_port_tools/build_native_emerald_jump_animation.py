"""Relink the qualified input-driven jump with the original action playback."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('#include "jp_native_sonic_level_dispatch.inc"',
         '#include "jp_native_sonic_level_dispatch.inc"\n#include "jp_native_sonic_jump_animation.inc"'),
        ('if (jp_sonic_step_level_dispatch(task, fixture->triangles,',
         'if (step_level_with_jump_animation(task, fixture->triangles,'),
        ('int jp_sonic_emerald_task_frame_probe(',
         'int jp_sonic_emerald_jump_animation_probe('),
        ('    struct emerald_scene_draw *scene, uint64_t *frame)\n{',
         '    struct emerald_scene_draw *scene, uint64_t *frame, NJS_ACTION *spin, NJS_ACTION *land)\n{'),
        ('    struct player_action actions[10] = {',
         '    if (frame && (!spin || !land)) return 22;\n'
         '    struct player_action actions[15] = {\n'
         '        [ACTION_SPIN] = {spin, 74, PLAYER_ANIM_LOOP_PACE, ACTION_SPIN, 0.25F, 0.2F},\n'
         '        [ACTION_LANDED] = {land, 73, PLAYER_ANIM_LOOP, ACTION_LANDED, 0.25F, 0.25F},'),
        ('return jp_sonic_emerald_task_frame_probe(textures, aura, stand, walk,',
         'return jp_sonic_emerald_jump_animation_probe(textures, aura, stand, walk,'),
        ('drawn_frame, triangles, triangle_count, position, entry, 0, 0);',
         'drawn_frame, triangles, triangle_count, position, entry, 0, 0, 0, 0);'),
        ('            int rose = 0, descended = 0;',
         '            int rose = 0, descended = 0, spin_advanced = 0;\n'
         '            Float initial_spin_frame = player->anim.frame;'),
        ('                rose |= work->pos.y > prior;',
         '                spin_advanced |= airborne && player->anim.current == ACTION_SPIN &&\n'
         '                                 player->anim.frame != initial_spin_frame;\n'
         '                rose |= work->pos.y > prior;'),
        ('player->anim.action != ACTION_LANDED)) fixture.result = 20;',
         'player->anim.action != ACTION_LANDED ||\n'
         '                player->anim.current != ACTION_LANDED || !spin_advanced)) fixture.result = 20;'),
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
    prior = manifest['emerald_jump_input']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    out.mkdir()
    dll = out/'jp_native_frames.dll'
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    fixture_recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    fixture_source = Path(fixture_recipe[-3])
    assert digest(fixture_source) == prior['source_sha256']
    assert digest(native/'jp_native_sonic_level_dispatch.inc') == manifest['emerald_jump_dispatch']['adapter_sha256']
    old_fixture = next(p for p in link if Path(p).name == 'player_frame_probe.obj')
    assert Path(old_fixture) == Path(fixture_recipe[-1])
    fixture, fixture_obj = out/'player_frame_probe.c', out/'player_frame_probe.obj'
    fixture.write_text(probe(fixture_source.read_text()), encoding='utf-8')
    link[link.index(old_fixture)] = str(fixture_obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [fixture_recipe[:-4] + ['-c', str(fixture), '-o', str(fixture_obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_jump_animation=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_sonic_jump_animation.inc'),
                        source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast jump animation compiled and linked')


if __name__ == '__main__':
    main()
