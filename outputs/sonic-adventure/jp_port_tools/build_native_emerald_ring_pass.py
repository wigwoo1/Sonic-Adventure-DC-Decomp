"""Run registered Emerald Coast ring contacts after the placement task level."""
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
    raw = replace_once(raw, b'#include "jp_emerald_ring_sphere.inc"', b'''
#include "jp_emerald_ring_sphere.inc"
void jp_collision_finish_ring_pass(void);
void jp_collision_forget_ring(struct collision_work *);

void jp_emerald_ring_collide(struct collision_work *ring)
{
    ring_test_collector(ring, &collector.center, collector.radius);
}
''')
    # Declare the retirement hook before the original ring destructor.
    raw = replace_once(raw, b'static void ring_destroy(struct task *task)\n{',
                       b'void jp_collision_forget_ring(struct collision_work *);\n'
                       b'static void ring_destroy(struct task *task)\n{\n'
                       b'    jp_collision_forget_ring(task->work->collision);')
    raw = replace_once(raw, b'''    collision_reset(work, collision);
    work->ang.y += NJM_DEG_ANG(2);
    if (!collector.active || work->mode != RING_WAIT) return;
    ring_test_collector(collision, &collector.center, collector.radius);''', b'''    work->ang.y += NJM_DEG_ANG(2);
    if (collector.active && work->mode == RING_WAIT)
        collision_register(work);
    else
        collision_reset(work, collision);''')
    return replace_once(raw, b'        task_step_level(3);',
                        b'        task_step_level(3);\n        jp_collision_finish_ring_pass();')


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
    terrain = manifest['emerald_ring_sphere']
    lists = manifest['emerald_collision_lists']
    link = shlex.split((base / 'link.rsp').read_text())
    out.mkdir()
    dll = out / 'jp_native_frames.dll'
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    commands = []
    sources = {}
    for name, prior, expected in (('terrain_profile', terrain, terrain['source_sha256']),
                                  ('collision_lists', lists, lists['generated_source_sha256'])):
        old = next(arg for arg in link if Path(arg).name == name + '.obj')
        original = Path(old).with_suffix('.c')
        assert sha(original) == expected
        raw = original.read_bytes().replace(b'\r\n', b'\n')
        new = (terrain_source(raw) if name == 'terrain_profile' else
               raw + b'\n#include "jp_emerald_ring_pass.inc"\n')
        source, obj = out / original.name, out / Path(old).name
        source.write_bytes(new)
        sources[source.name] = sha(source)
        link[link.index(old)] = obj.as_posix()
        recipe = prior['commands'][0]
        commands.append(recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)])
    link[-1] = dll.as_posix()
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    commands.append([terrain['commands'][1][0], '@' + str(rsp)])
    for phase, command in zip(('terrain', 'lists', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_ring_pass=dict(parent_sha256=sha(parent), sources_sha256=sources,
                                           commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Emerald Coast post-task ring pass compiled and linked')


if __name__ == '__main__':
    main()
