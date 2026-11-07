"""Move ring simulation from the Sonic fixture into the native stage session."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'movement', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, base, movement, out = (p.resolve() for p in
                                 (args.workspace, args.baseline, args.movement, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    session = manifest['emerald_stage_session']
    move = manifest['emerald_ring_walk']
    stage = base/'stage_session.c'
    player = movement/'ring_walk.c'
    native = root/'outputs/sonic-adventure/native'
    assert digest(stage) == session['source_sha256']
    assert digest(native/'jp_native_emerald_stage.inc') == session['adapter_sha256']
    assert digest(player) == move['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    stage_source = stage.read_bytes()
    include = b'#include "jp_native_emerald_stage.inc"'
    assert stage_source.count(include) == 1
    owner = (native/'jp_native_emerald_stage.inc').read_bytes()
    anchor = b'    if (stage_session & STAGE_SET) {'
    assert owner.count(anchor) == 1
    owner = owner.replace(anchor, b'    if (emerald_stage_stepping) return -1;\n' + anchor)
    stage_source = stage_source.replace(include, b'static int emerald_stage_stepping;\n' + owner +
        b'\n#include "jp_native_emerald_stage_tick.inc"\n')
    player_source = player.read_bytes()
    phase = owner_function(player_source, 'ring_player_phase')
    player_source = player_source.replace(phase,
        b'int jp_emerald_stage_tick(struct task *, struct task_work *);\n')
    anchor = b'        if (ring_walk_enabled && !fixture.result) ring_player_phase(task);'
    assert player_source.count(anchor) == 1
    player_source = player_source.replace(anchor,
        b'        if (ring_walk_enabled && !fixture.result && jp_emerald_stage_tick(task, NULL) < 0)\n'
        b'            fixture.result = 11;')
    cleanup = b'''    if (ring_walk_enabled) {
        jp_emerald_ring_collector(NULL, 0, 0, NULL);
        jp_emerald_placement_viewers(NULL, NULL, NULL, 0);
    }
'''
    assert player_source.count(cleanup) == 1
    player_source = player_source.replace(cleanup, b'')
    anchor = b'        shape.flags = SHAPE_SPHERE | SHAPE_IN_WORLD;'
    assert player_source.count(anchor) == 1
    player_source = player_source.replace(anchor, b'        collision.count = 1;\n' + anchor)

    out.mkdir()
    dll = out/'jp_native_frames.dll'
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    link = shlex.split((base/'link.rsp').read_text())
    commands = []
    for name, raw, recipe in (('stage_session', stage_source, session['commands'][0]),
                              ('ring_walk', player_source, move['commands'][0])):
        unit, obj = out/(name + '.c'), out/(name + '.obj')
        unit.write_bytes(raw)
        old = next(p for p in link if Path(p).name == obj.name)
        link[link.index(old)] = str(obj)
        commands.append(recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)])
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands.append([session['commands'][1][0], '@' + str(rsp)])
    for phase, command in zip(('stage', 'movement', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_stage_tick=dict(parent_sha256=digest(parent), commands=commands,
                        source_sha256={p.name: digest(p) for p in out.glob('*.c')}))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast stage tick compiled and linked')


if __name__ == '__main__':
    main()
