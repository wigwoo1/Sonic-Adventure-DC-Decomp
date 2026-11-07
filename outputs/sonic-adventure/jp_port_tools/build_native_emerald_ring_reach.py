"""Run the original reach and proximity checks in the Emerald Coast ring pass."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function, sha


def replace_once(raw, old, new):
    assert raw.count(old) == 1, old
    return raw.replace(old, new)


def terrain_source(raw, collision):
    fields = re.findall(rb'^(?:struct near_task near_\w+\[NEAR_\w+ \+ 1\]|static Uint16 near_\w+_count(?:_player1)?);',
                        collision, re.M)
    assert len(fields) == 8
    reach = re.search(rb'^#define NEAR_REACH [^\r\n]+', collision, re.M).group()
    bodies = [owner_function(collision, name) for name in
              ('collision_list_near', 'list_near_and_test_reach')]
    block = b'\n'.join([b'#include "player/player.h"', reach, *fields, *bodies,
                       b'#include "jp_emerald_ring_reach.inc"'])
    anchor = b'#include "jp_emerald_live_ring_contact.inc"'
    raw = replace_once(raw, anchor, anchor + b'\n' + block)
    raw = replace_once(raw, b'    if (ring_live_player) ring_collide_live(ring);',
                       b'    if (ring_live_player) {\n'
                       b'        if (list_near_and_test_reach(ring_live_player->owner->work, ring_live_player,\n'
                       b'                                     ring->owner->work, ring)) ring_collide_live(ring);\n'
                       b'    }')
    raw = replace_once(raw, b'void jp_collision_forget_ring(struct collision_work *);\nstatic void ring_destroy',
                       b'void jp_collision_forget_ring(struct collision_work *);\n'
                       b'static void emerald_forget_near_ring(const struct task_work *);\n'
                       b'static void ring_destroy')
    raw = replace_once(raw, b'    jp_collision_forget_ring(task->work->collision);',
                       b'    jp_collision_forget_ring(task->work->collision);\n'
                       b'    emerald_forget_near_ring(task->work);')
    raw = replace_once(raw, b'    if (emerald_stage_stepping) return -1;\n    if (close_stage_land()',
                       b'    if (emerald_stage_stepping) return -1;\n'
                       b'    emerald_clear_near_lists();\n    if (close_stage_land()')
    return replace_once(raw, b'        jp_collision_finish_ring_pass();',
                        b'        emerald_clear_near_lists();\n'
                        b'        jp_collision_finish_ring_pass();')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (p.resolve() for p in
                                (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    terrain = manifest['emerald_live_ring_contact']
    collision_path = 'src/collision/collision.c'
    collision = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{collision_path}'])
    assert collision == (upstream / collision_path).read_bytes()
    link = shlex.split((base / 'link.rsp').read_text())
    out.mkdir()
    dll = out / 'jp_native_frames.dll'
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    commands = []
    sources = {}
    for name, recipe in zip(('terrain_profile', 'player_frame_probe'), terrain['commands']):
        old = next(arg for arg in link if Path(arg).name == name + '.obj')
        original = Path(old).with_suffix('.c')
        assert sha(original) == terrain['sources_sha256'][original.name]
        raw = original.read_bytes().replace(b'\r\n', b'\n')
        new = (terrain_source(raw, collision) if name == 'terrain_profile' else
               replace_once(raw, b'    collision.owner = task;',
                            b'    collision.owner = task;\n    collision.radius = 25.0F;'))
        source, obj = out / original.name, out / Path(old).name
        source.write_bytes(new)
        sources[source.name] = sha(source)
        link[link.index(old)] = obj.as_posix()
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
                    emerald_ring_reach=dict(upstream_commit=PIN, parent_sha256=sha(parent), sources_sha256=sources,
                                           retain_near_rings=True,
                                           commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Emerald Coast ring reach compiled and linked')


if __name__ == '__main__':
    main()
