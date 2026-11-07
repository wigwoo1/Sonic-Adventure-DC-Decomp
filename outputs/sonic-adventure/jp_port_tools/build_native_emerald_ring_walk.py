"""Connect the native Sonic movement task to placed-ring contact."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


def generate(source, sonic):
    shape = re.search(rb'#define SONIC_CENTER_Y [^\n]+', sonic).group()
    radius = re.search(rb'PUSH_ALL \| PUSH_FIRM\(PUSH_OTHERS\), \{0.0F, 1.0F, 0.0F\}, ([\d.]+F)', sonic)[1]
    adapter = br'''
void jp_emerald_ring_collector(const NJS_POINT3 *, Float, int, void (*)(int));
void jp_emerald_placement_viewers(struct task_work *, struct task_work *, struct task_work *, int);
int jp_emerald_placement_step(struct task_work *, struct task_work *);

static NJS_POINT3 walk_start;
static int ring_walk_enabled;

__declspec(dllexport) void jp_emerald_ring_walk_start(const NJS_POINT3 *position)
{
    ring_walk_enabled = position != NULL;
    if (position) walk_start = *position;
}

static void ring_player_phase(struct task *task)
{
    struct task_work *work = task->work;
    struct collision_shape *shape = work->collision->shapes;
    struct player *player = task->motion->record;
    jp_emerald_placement_viewers(work, NULL, NULL, 0);
    jp_emerald_ring_collector(&shape->center, shape->radius,
                             player->effects & PLAYER_FALLING, NULL);
    jp_emerald_placement_step(work, NULL);
    task_step_level(3);
}
'''
    move = owner_function(sonic, 'sonic_move_shape')
    source = source.replace(b'struct animation_fixture {',
                            shape + b'\n' + adapter + move + b'\nstruct animation_fixture {')
    source = source.replace(b'    work->collision->shapes->center = work->pos;',
                            b'    if (ring_walk_enabled) sonic_move_shape(work, motion, player);\n'
                            b'    else work->collision->shapes->center = work->pos;')
    source = source.replace(b'    work->pos.y = fixture.floor.height;',
                            b'''    if (ring_walk_enabled) {
        work->pos = walk_start;
        if (!jp_stage_floor_below(triangles, triangle_count, work->pos.x, work->pos.y,
                                  work->pos.z, &fixture.floor)) {
            fixture.result = 10;
            goto finish;
        }
        shape.flags = SHAPE_SPHERE | SHAPE_IN_WORLD;
        shape.radius = ''' + radius + b''';
        player->consts.center_y = SONIC_CENTER_Y;
    }
    work->pos.y = fixture.floor.height;''')
    source = source.replace(b'    for (int tick = 0; tick < 24; tick++) task_step_level(1);',
                            b'''    for (int tick = 0; tick < 24; tick++) {
        task_step_level(1);
        if (ring_walk_enabled && !fixture.result) ring_player_phase(task);
    }
    if (ring_walk_enabled) {
        jp_emerald_ring_collector(NULL, 0, 0, NULL);
        jp_emerald_placement_viewers(NULL, NULL, NULL, 0);
    }''')
    return source


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'movement', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, movement, out = (p.resolve() for p in
        (args.workspace, args.upstream, args.baseline, args.movement, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    move = json.loads((movement/'build_verification.json').read_bytes())['sonic_emerald_task']
    source = movement/'sonic_emerald_task.c'
    assert digest(source) == move['generated_source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_walk.c', 'ring_walk.obj', 'jp_native_frames.dll'))
    unit.write_bytes(generate(source.read_bytes(), pinned(upstream, 'src/player/sonic.c')))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = manifest['emerald_ring_score']['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'sonic_emerald_task.obj')
    link[link.index(old)] = str(obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_ring_walk=dict(parent_sha256=digest(parent),
                                           source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ring walk compiled and linked')


if __name__ == '__main__':
    main()
