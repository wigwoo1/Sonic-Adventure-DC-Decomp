"""Relink the qualified jump animation with airborne frame capture."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('    struct emerald_scene_draw *scene, uint64_t *frame, NJS_ACTION *spin, NJS_ACTION *land)',
         '    struct emerald_scene_draw *scene, uint64_t *frame, NJS_ACTION *spin, NJS_ACTION *land, uint64_t *air,\n'
         '    const struct jp_model_bound *air_bound, const uint32_t *words, uint32_t word_count)'),
        ('int jp_sonic_emerald_jump_animation_probe(', 'int jp_sonic_emerald_jump_draw_probe('),
        ('return jp_sonic_emerald_jump_animation_probe(', 'return jp_sonic_emerald_jump_draw_probe('),
        ('position, entry, 0, 0, 0, 0);', 'position, entry, 0, 0, 0, 0, 0, 0, 0, 0);'),
        ('static int input_phase;', """#include "jp_native_frames.h"
#include "jp_model_preflight.h"
struct emerald_scene_draw {
    size_t terrain_bytes, terrain_opaque_bytes;
    int terrain, rings, sparkles;
};
int jp_emerald_submit_player_frame(struct task *, NJS_TEXLIST *, NJS_TEXLIST *,
    NJS_OBJECT *, struct emerald_scene_draw *, uint64_t *);

static int input_phase;"""),
        ('                rose |= work->pos.y > prior;', """                if (air && !*air && airborne && work->pos.y < prior &&
                    player->anim.current == ACTION_SPIN) {
                    struct emerald_scene_draw air_scene;
                    Uint32 flags = work->flags;
                    work->flags &= ~SONIC_CHARGED;
                    player->own.sonic.curled_steps = 0;
                    if (jp_model_reserve(air_bound) || jp_frames_prepare_textures(words, word_count) ||
                        jp_emerald_submit_player_frame(task, textures, textures, aura,
                                                       &air_scene, air)) fixture.result = 23;
                    work->flags = flags;
                }
                rose |= work->pos.y > prior;"""),
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
    prior = manifest['emerald_jump_animation']
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
    assert digest(native/'jp_native_sonic_jump_animation.inc') == prior['adapter_sha256']
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
                    emerald_jump_draw=dict(parent_sha256=digest(parent),
                        
                        source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast jump draw compiled and linked')


if __name__ == '__main__':
    main()
