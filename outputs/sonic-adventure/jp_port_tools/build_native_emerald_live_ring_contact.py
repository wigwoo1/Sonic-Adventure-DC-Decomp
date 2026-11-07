"""Borrow the live Sonic collision record during the registered ring pass."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import sha


def replace_once(raw, old, new):
    assert raw.count(old) == 1, old
    return raw.replace(old, new)


def terrain_source(raw):
    raw = replace_once(raw, b'#include "jp_emerald_ring_sphere.inc"',
                       b'#include "jp_emerald_ring_sphere.inc"\n'
                       b'#include "jp_emerald_live_ring_contact.inc"')
    raw = replace_once(raw, b'    ring_test_collector(ring, &collector.center, collector.radius);',
                       b'    if (ring_live_player) ring_collide_live(ring);\n'
                       b'    else ring_test_collector(ring, &collector.center, collector.radius);')
    raw = replace_once(raw, b'    if (!collision || !collision->count || !collision->shapes)',
                       b'    if (!collision || collision->owner != sonic ||\n'
                       b'        collision->kind != COLLISION_PLAYER || !collision->count || !collision->shapes)')
    start = raw.index(b'    struct collision_shape *shape = collision->shapes;',
                      raw.index(b'int jp_emerald_stage_tick('))
    end = raw.index(b'    emerald_stage_stepping = 1;', start)
    assert b'SHAPE_IN_WORLD' in raw[start:end]
    raw = raw[:start] + raw[end:]
    raw = replace_once(raw, b'    jp_emerald_ring_collector(&shape->center, shape->radius,\n'
                       b'                             sonic->motion->record->effects & PLAYER_FALLING, NULL);',
                       b'    collector.active = 1;\n'
                       b'    collector.falling = sonic->motion->record->effects & PLAYER_FALLING;\n'
                       b'    pickup = NULL;')
    return replace_once(raw, b'        jp_collision_finish_ring_pass();',
                        b'        ring_live_player = collision;\n'
                        b'        jp_collision_finish_ring_pass();\n'
                        b'        ring_live_player = NULL;')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, base, out = (p.resolve() for p in (args.workspace, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    terrain = manifest['emerald_ring_pass']
    probe = manifest['emerald_record_profile']
    link = shlex.split((base / 'link.rsp').read_text())
    out.mkdir()
    dll = out / 'jp_native_frames.dll'
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    commands = []
    sources = {}
    for name, prior, expected in (('terrain_profile', terrain, terrain['sources_sha256']['terrain_profile.c']),
                                  ('player_frame_probe', probe, probe['source_sha256'])):
        old = next(arg for arg in link if Path(arg).name == name + '.obj')
        original = Path(old).with_suffix('.c')
        assert sha(original) == expected
        raw = original.read_bytes().replace(b'\r\n', b'\n')
        new = (terrain_source(raw) if name == 'terrain_profile' else
               replace_once(raw, b'    collision.shapes = &shape;',
                            b'    collision.owner = task;\n    collision.shapes = &shape;'))
        source, obj = out / original.name, out / Path(old).name
        source.write_bytes(new)
        sources[source.name] = sha(source)
        link[link.index(old)] = obj.as_posix()
        recipe = prior['commands'][0]
        commands.append(recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)])
    link[-1] = dll.as_posix()
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    commands.append([terrain['commands'][-1][0], '@' + str(rsp)])
    for phase, command in zip(('terrain', 'player', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_live_ring_contact=dict(parent_sha256=sha(parent), sources_sha256=sources,
                                           commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Emerald Coast live ring contact compiled and linked')


if __name__ == '__main__':
    main()
