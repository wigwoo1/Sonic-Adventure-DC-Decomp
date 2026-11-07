"""Keep collected Emerald Coast placements retired after their sparkle timer."""
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


def generate(raw, ring):
    sparkle = owner_function(ring, 'ring_draw_sparkle')
    advance = sparkle[sparkle.index(b'    RING(work)->spin +='):sparkle.index(b'    draw_set_material')]
    finish = sparkle[sparkle.index(b'    if (frame[2] > SPARKLE_END)'):]
    finish = finish.replace(b'frame[2]', b'(int)work->scl.z')
    limit = re.search(rb'^#define SPARKLE_END .+$', ring, re.M).group()
    anchor = b'static void native_ring_step'
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, limit + b'\nstatic void native_ring_sparkle_step(struct task_work *work)\n{\n'
                      + advance + finish + b'\n' + anchor)
    anchor = b'    if (placement_try_leave(task)) {'
    assert raw.count(anchor) == 1
    return raw.replace(anchor, br'''    if (work->mode == RING_SPARKLE) {
        native_ring_sparkle_step(work);
        return;
    }
    if (work->mode == RING_DONE) {
        /* Keep the live bit until task_destroy detaches the task. An act
         * transition between these ticks can still retire it safely. */
        if (task->placement) task->placement->flags &= ~PLACEMENT_ACTIVE;
        task_kill(task);
        return;
    }
''' + anchor)


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
    prior = manifest['emerald_ring_draw']
    source = base/'ring_draw.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    assert digest(root/'outputs/sonic-adventure/native/jp_native_ring_draw.inc') == prior['adapter_sha256']
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_finish.c', 'ring_finish.obj', 'jp_native_frames.dll'))
    unit.write_bytes(generate(source.read_bytes(), pinned(upstream, 'src/object/ring.c')))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'ring_draw.obj')
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
                    emerald_ring_finish=dict(parent_sha256=digest(parent),
                                             source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ring completion compiled and linked')


if __name__ == '__main__':
    main()
