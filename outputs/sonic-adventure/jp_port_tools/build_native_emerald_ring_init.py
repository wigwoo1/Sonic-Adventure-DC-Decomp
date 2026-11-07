"""Link the original placed-ring initialization and collision allocation."""
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


PREFIX = br'''
#include <string.h>
#include <ninja.h>
#include "collision/collision.h"
#include "collision/ground.h"
#include "object/placement.h"
#include "object/ring.h"

struct jp_stage_triangle;
struct jp_stage_floor { Float height; Uint32 entry, flags; };
int jp_stage_floor_below(const struct jp_stage_triangle *, size_t,
                         Float, Float, Float, struct jp_stage_floor *);
int jp_emerald_kind_bind(unsigned, void (*)(struct task *));

static const struct jp_stage_triangle *ring_triangles;
static size_t ring_triangle_count;
static Sint32 ring_stagger;

__declspec(dllexport) void jp_emerald_ring_floor(
    const struct jp_stage_triangle *triangles, size_t count)
{
    ring_triangles = triangles;
    ring_triangle_count = count;
}

static Float ring_ground(Float x, Float y, Float z)
{
    struct jp_stage_floor floor;
    return jp_stage_floor_below(ring_triangles, ring_triangle_count, x, y, z, &floor)
        ? floor.height : -GROUND_FAR;
}

static Float ring_vector_length(const NJS_VECTOR *vec)
{
    return __builtin_sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}
'''


def generate(upstream):
    ring = pinned(upstream, 'src/object/ring.c')
    collision = pinned(upstream, 'src/collision/collision.c')
    radius = owner_function(collision, 'collision_set_radius')
    radius = radius.replace(b'collision_set_radius', b'ring_collision_set_radius')
    radius = radius.replace(b'njScalor', b'ring_vector_length').replace(b'njSqrt', b'__builtin_sqrtf')
    init = owner_function(collision, 'collision_init')
    init = init.replace(b'collision_init', b'ring_collision_init')
    start = init.index(b'        /* @hack spelling: njPrintC')
    end = init.index(b'        njMemCopy', start)
    init = (init[:start] + b'''        return;
    }
    copy = heap_calloc(shape_count, sizeof(struct collision_shape));
    if (!copy) {
        heap_free(collision);
        return;
    }
    {
''' + init[end:])
    init = init.replace(b'njMemCopy', b'memcpy').replace(b'collision_set_radius', b'ring_collision_set_radius')
    shape = re.search(rb'static struct collision_shape ring_shape = \{.*?\};', ring, re.S).group()
    step = owner_function(ring, 'ring_step')
    start = step.index(b'        collision_init')
    end = step.index(b'        task->draw = ring_draw;', start)
    body = step[start:end].replace(b'collision_init(', b'ring_collision_init(')
    body = body.replace(b'COLLISION_RING, 1, 1);',
                        b'COLLISION_RING, 1, 1);\n        if (!work->collision) return;')
    body = body.replace(b'ground_below(work->pos.x, work->pos.y, work->pos.z, &work->ang)',
                        b'ring_ground(work->pos.x, work->pos.y, work->pos.z)')
    assert b'ground_below' not in body
    return (PREFIX + b'static ' + radius + b'static ' + init + shape + b'\n' +
            owner_function(ring, 'ring_destroy') + br'''
static void native_ring_step(struct task *task)
{
    struct task_work *work = task->work;
    if (placement_try_leave(task)) {
        RING(work)->gone = 1;
        return;
    }
    if (work->mode != RING_INIT) return;
''' + body + br'''
}

__declspec(dllexport) int jp_emerald_ring_bind(int enabled)
{
    int result = jp_emerald_kind_bind(0, enabled ? native_ring_step : NULL);
    if (!result) ring_stagger = 0;
    return result;
}
''')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (p.resolve() for p in
                                  (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    recipe, driver = manifest['emerald_placement_leave']['commands']
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_init.c', 'ring_init.obj', 'jp_native_frames.dll'))
    unit.write_bytes(generate(upstream))
    placement = base/'placement_leave.c'
    assert digest(placement) == manifest['emerald_placement_leave']['source_sha256']
    placement_unit, placement_obj = out/'placement.c', out/'placement.obj'
    placement_unit.write_bytes(placement.read_bytes() + br'''
/* The returned task is borrowed until the next scheduler or registry call. */
__declspec(dllexport) const struct task *jp_emerald_placement_task(unsigned index)
{
    if (index >= (unsigned)placement_count || !(placements[index].flags & PLACEMENT_LIVE))
        return NULL;
    return placements[index].task;
}
''')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    link = shlex.split((base/'link.rsp').read_text())
    old = next(arg for arg in link if Path(arg).name == 'placement_leave.obj')
    link[link.index(old)] = str(placement_obj)
    link.insert(link.index('-o'), str(obj))
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)],
                recipe[:recipe.index('-c')] + ['-c', str(placement_unit), '-o', str(placement_obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'placement', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_ring_init=dict(parent_sha256=digest(parent),
                                           source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ring initialization compiled and linked')


if __name__ == '__main__':
    main()
