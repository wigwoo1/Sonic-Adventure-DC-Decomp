"""Link the placed ring's stationary sphere contact and pickup transition."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


CONTACT = br'''
static struct { NJS_POINT3 center; Float radius; int active, falling; } collector;
static void (*pickup)(int);

/* The Sonic collision phase supplies its current world-space sphere.
 * Clear the callback before its owner or the stage shuts down. */
__declspec(dllexport) void jp_emerald_ring_collector(const NJS_POINT3 *center,
    Float radius, int falling, void (*notify)(int))
{
    collector.active = center && radius > 0 && radius <= 1000.0F;
    if (collector.active) collector.center = *center;
    collector.radius = radius;
    collector.falling = falling;
    pickup = notify;
}

static void ring_wait(struct task_work *work)
{
    struct collision_work *collision = work->collision;
    if (collision->flags & COLLISION_HIT) {
        work->mode = RING_SPARKLE;
        if (!collector.falling && pickup) pickup(1);
    }
    collision_reset(work, collision);
    work->ang.y += NJM_DEG_ANG(2);
    if (!collector.active || work->mode != RING_WAIT) return;
    Float radius = collector.radius + collision->shapes[0].radius;
    Float x = collector.center.x - work->pos.x;
    Float y = collector.center.y - work->pos.y;
    Float z = collector.center.z - work->pos.z;
    if (radius * radius > x*x + y*y + z*z)
        collision->flags |= COLLISION_HIT;
}
'''


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
    prior = manifest['emerald_ring_init']
    source = base/'ring_init.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    reset = owner_function(pinned(upstream, 'src/collision/collision.c'), 'collision_reset')
    raw = source.read_bytes()
    anchor = b'static void native_ring_step'
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, reset + CONTACT + anchor)
    anchor = b'    if (work->mode != RING_INIT) return;'
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, b'    if (work->mode == RING_WAIT) ring_wait(work);\n' + anchor)
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_contact.c', 'ring_contact.obj', 'jp_native_frames.dll'))
    unit.write_bytes(raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, _, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'ring_init.obj')
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
                    emerald_ring_contact=dict(parent_sha256=digest(parent),
                                              source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ring contact compiled and linked')


if __name__ == '__main__':
    main()
