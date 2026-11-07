"""Link the original light-dash target selection into the Windows stage build."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function, sha
from rewrite_sprite_float_operations import rewrite


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, required=True, type=Path)
    args = parser.parse_args()
    root, upstream, base, out = (p.resolve() for p in
                                (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    path = 'src/player/sonic.c'
    raw = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{path}'])
    assert raw == (upstream / path).read_bytes()
    player_path = 'src/player/player.c'
    player = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{player_path}'])
    assert player == (upstream / player_path).read_bytes()
    cone = re.search(rb'^#define RING_CONE [^\r\n]+', raw, re.M).group()
    original = owner_function(raw, 'sonic_light_dash').replace(b'\r\n', b'\n')
    declarations = original[original.index(b'    struct near_task'):original.index(b'    if (PLAYER_VARS')]
    selection = original[original.index(b'    if (PLAYER_VARS'):original.index(b'    if (best == NULL) {')]
    start = selection.index(b'        player_accel_roll')
    end = selection.index(b'    }', start)
    selection = selection[:start] + b'        return NULL;\n' + selection[end:]
    selection = selection.replace(b'njSubVector(', b'light_dash_subtract(')
    selection = selection.replace(b'vector_angle(&dir, &to, NULL)', b'light_dash_angle(&dir, &to)')
    constants = [re.search(rb'^#define ' + name + rb' [^\r\n]+', raw, re.M).group()
                 for name in (b'RING_STEPS', b'LIGHT_DASH_TURN', b'DASH_STEP_LEAST', b'DASH_STEP_MOST')]
    steering = original[original.index(b'    if (record->own.sonic.dash_steps'):]
    steering = steering.replace(b'njSubVector(', b'light_dash_subtract(')
    steering = steering.replace(b'atan2f(to.z, to.x) * ANGLE_PER_RADIAN',
                                b'jp_sprite_multiply(jp_pad_atan2(to.z, to.x), 0x1.45f30cp+13F)')
    steering = steering[:steering.rindex(b'}')] + b'    return 1;\n}\n'
    release = owner_function(raw, 'sonic_try_light_dash').replace(b'\r\n', b'\n')
    start = release.index(b'    /* @hack redundant: target')
    condition = release.index(b'    if (!(pad_records', start)
    attack = release.index(b'        if ((record->upgrades', condition)
    ring = release.index(b'        work->flags &= ~SONIC_CHARGED;', release.index(b'            return TRUE;', attack))
    release = (release[:start] + release[condition:attack] +
               b'        if (record->upgrades & PLAYER_SONIC_ANCIENT_LIGHT) return FALSE;\n' + release[ring:])
    release = release.replace(b'static Bool sonic_try_light_dash',
                              b'__declspec(dllexport) Bool jp_emerald_try_light_dash', 1)
    release_constants = [re.search(rb'^#define ' + name + rb' [^\r\n]+', raw, re.M).group()
                         for name in (b'LIGHT_DASH_STEPS', b'LIGHT_DASH_SPEED', b'SE_LIGHT_DASH',
                                      b'SE_CHARGED', b'SE_LIGHT_DASH_START')]
    charge_constants = [re.search(rb'^#define ' + name + rb' [^\r\n]+', raw, re.M).group()
                        for name in (b'CHARGE_STEPS', b'RING_CHARGE_STEPS', b'CURL_STEPS',
                                     b'SPIN_PACE_LEAST', b'SPIN_PACE_MOST', b'SPIN_PACE_GAIN',
                                     b'SE_SPIN_DASH', b'SE_LIGHT_CHARGE', b'CHARGED_TOP_SPEED',
                                     rb'CHARGED\(task\)')]
    charge_struct = re.search(rb'^struct sonic_charged \{.*?^\};', raw, re.M | re.S).group()
    charge_bodies = [owner_function(raw, name).replace(b'(int)work', b'(intptr_t)work')
                     for name in ('sonic_charged_step', 'sonic_charged_destroy',
                                  'sonic_charged_create', 'sonic_try_spin', 'sonic_spin_charge')]
    charge_bodies[0] = charge_bodies[0].replace(b'player_work[0] != NULL', b'player_work[0] == work')
    charge_bodies[1] = charge_bodies[1].replace(b'player_work[0] != NULL',
                                                b'player_work[0] == CHARGED(task)->work')
    allocation = b'    task = task_create(TASK_EXTRA, 0, sonic_charged_step);'
    assert charge_bodies[2].count(allocation) == 1
    charge_bodies[2] = charge_bodies[2].replace(allocation,
        b'    task = task_create(0, 0, sonic_charged_step);\n'
        b'    if (task != NULL) {\n'
        b'        task->extra = heap_calloc(1, sizeof(struct sonic_charged));\n'
        b'        if (!task->extra) { task_destroy(task); return; }\n'
        b'    }')
    enums = [re.search(rb'^enum ' + name + rb' \{.*?^\};', raw, re.M | re.S).group()
             for name in (b'sonic_mode', b'sonic_action')]
    mode = re.search(rb'    case SONIC_LIGHT_DASH: \{\r?\n.*?^    }', raw, re.M | re.S).group()
    begin = mode.index(b'        /* @hack spelling: an empty arm')
    finish = mode.rindex(b'        break;')
    duration = mode[begin:finish].replace(b'            break;', b'            return 0;')
    duration = (b'__declspec(dllexport) int jp_emerald_light_dash_duration(\n'
                b'    struct task_work *work, struct player *record)\n{\n'
                b'    NJS_POINT3 pos;\n'
                b'    unsigned slot = PLAYER_VARS(work)->player;\n'
                b'    if (work->mode != SONIC_LIGHT_DASH || slot >= PLAYER_COUNT ||\n'
                b'        player_work[slot] != work) return -1;\n' +
                duration + b'    return 1;\n}\n')
    duration_constants = [re.search(rb'^#define ' + name + rb' [^\r\n]+', raw, re.M).group()
                          for name in (b'LIGHT_DASH_STOP', b'LIGHT_DASH_END_SPEED')]
    distance_path = 'sdk/ninja/njDistanceP2P.c'
    distance = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{distance_path}'])
    assert distance == (upstream / distance_path).read_bytes()
    nodes = re.search(rb'^#define SONIC_NODES [^\r\n]+', raw, re.M).group()
    rows = []
    for name, mode, parameter in (
        (b'sonic_light_dash_action', b'PLAYER_ANIM_LOOP', b'dash'),
        (b'sonic_skid_action', b'PLAYER_ANIM_ONCE', b'skid'),
    ):
        row = re.search(rb'\{&' + name + rb', SONIC_NODES, ' + mode + rb',\s*[^}]+\}', raw).group()
        rows.append(row.replace(b'&' + name, parameter, 1))
    binding = (b'__declspec(dllexport) int jp_emerald_bind_light_dash_actions(\n'
               b'    struct player_action *actions, size_t count, NJS_ACTION *dash, NJS_ACTION *skid)\n{\n'
               b'    if (!actions || count <= ACTION_LIGHT_DASH || !dash || !skid ||\n'
               b'        !dash->object || !dash->motion || !skid->object || !skid->motion) return -1;\n'
               b'    actions[ACTION_LIGHT_DASH] = (struct player_action)' + rows[0] + b';\n'
               b'    actions[ACTION_SKID] = (struct player_action)' + rows[1] + b';\n'
               b'    return 0;\n}\n')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    out.mkdir()
    source, obj, dll = (out / name for name in
                        ('light_dash_target.c', 'light_dash_target.obj', 'jp_native_frames.dll'))
    source.write_bytes(b'\n'.join([
        b'#include "collision/collision.h"\n#include "collision/polygon.h"\n#include "collision/world.h"\n'
        b'#include "player/player.h"\n#include "stage/stage.h"\n#include "system/util.h"',
        b'#include "player/sonic.h"\n#define sound_play_for original_sound_play_for_declaration\n'
        b'#include "sound/sound.h"\n#undef sound_play_for\n'
        b'#include "system/pad.h"\n#include <stdint.h>\n'
        b'int sound_play_for(int, intptr_t, int, int, int);',
        b'#include "event/event.h"',
        b'#include "jp_native_sprite_numeric.h"',
        b'#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"',
        b'#include "jp_light_dash_target_numeric.inc"', cone,
        b'static struct task_work *light_dash_target(struct task_work *work, Float *distance)\n{',
        declarations, selection, b'    if (best) *distance = best_distance;\n    return best;\n}\n',
        b'__declspec(dllexport) struct task_work *jp_emerald_light_dash_target(struct task_work *work)\n'
        b'{ Float distance; return light_dash_target(work, &distance); }',
        *constants, b'Float jp_pad_atan2(Float, Float);',
        b'__declspec(dllexport) int jp_emerald_light_dash_steer(struct task_work *work,\n'
        b'    struct task_motion *motion, struct player *record)\n{\n'
        b'    Float best_distance;\n'
        b'    struct task_work *best = light_dash_target(work, &best_distance);\n'
        b'    if (!best) return 0;\n'
        b'    struct collision_shape *shape;\n    NJS_VECTOR to;',
        steering,
        owner_function(player, 'player_accel_roll'),
        b'__declspec(dllexport) int jp_emerald_light_dash_step(struct task_work *work,\n'
        b'    struct task_motion *motion, struct player *record)\n{\n'
        b'    struct near_task *near = PLAYER_VARS(work)->player == 0 ? near_rings : near_rings_player1;\n'
        b'    if (near->work) return jp_emerald_light_dash_steer(work, motion, record);\n'
        b'    if ((work->flags & PLAYER_ON_GROUND) && !(work->flags & PLAYER_EVENT_DRIVEN) &&\n'
        b'        !record->contact) return -1;\n'
        b'    player_accel_roll(work, motion, record);\n'
        b'    player_apply_accel(work, motion, record);\n'
        b'    return 2;\n}\n',
        *enums, *release_constants, release,
        *duration_constants, owner_function(distance, 'njDistanceP2P'), duration,
        nodes, binding,
        charge_struct, *charge_constants, *charge_bodies,
        b'#include "jp_emerald_light_dash_charge.inc"',
        b'#include "jp_light_dash_action_fixture.inc"',
        owner_function(raw, 'sonic_move_shape'), b'#include "jp_emerald_light_dash_mode.inc"',
        b'#include "jp_light_dash_target_fixture.inc"',
    ]))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link = shlex.split((base / 'link.rsp').read_text())
    assert link[-2] == '-o'
    link[-2:] = [obj.as_posix(), '-o', dll.as_posix()]
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    recipe, *_, driver = manifest['emerald_ring_reach']['commands']
    flags = recipe[:recipe.index('-c')]
    ast = subprocess.run(flags + ['-Xclang', '-ast-dump=json', '-fsyntax-only', str(source)],
                         capture_output=True)
    assert ast.returncode == 0 and not ast.stderr, ast.stderr.decode(errors='replace')
    adapted, arithmetic = rewrite(source.read_bytes(), json.loads(ast.stdout),
                                  ['player_accel_roll', 'njDistanceP2P', 'jp_emerald_light_dash_duration',
                                   'sonic_spin_charge'])
    native = out / 'light_dash_target_rz.c'
    native.write_bytes(adapted)
    commands = [flags + ['-c', str(native), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_light_dash_target=dict(upstream_commit=PIN, parent_sha256=sha(parent),
                                                   source_sha256=sha(source), adapted_sha256=sha(native),
                                                   roll_arithmetic=arithmetic, actions_bound=True,
                                                   scheduled_pose_capture=True,
                                                   retail_floor_capture=True,
                                                   full_charge_capture=True,
                                                   native_capture_input=True,
                                                   commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Original light-dash target selection compiled and linked')


if __name__ == '__main__':
    main()
