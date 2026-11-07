"""Use the original sphere worker for the Emerald Coast ring contact phase."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import sha


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
    profile = manifest['emerald_terrain_profile']
    link = shlex.split((base / 'link.rsp').read_text())
    old = next(arg for arg in link if Path(arg).name == 'terrain_profile.obj')
    prior = Path(old).with_suffix('.c')
    assert sha(prior) == profile['source_sha256']
    data = prior.with_name('stage_ring_data.inc')
    assert sha(data) == profile['private_data_sha256']
    raw = prior.read_bytes()
    anchor = b'static void ring_wait(struct task_work *work)'
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, b'#include "jp_emerald_ring_sphere.inc"\n\n' + anchor)
    start = raw.index(b'    Float radius = collector.radius +')
    end = raw.index(b'\n}', start)
    assert b'collision->flags |= COLLISION_HIT;' in raw[start:end]
    raw = raw[:start] + b'    ring_test_collector(collision, &collector.center, collector.radius);' + raw[end:]
    out.mkdir()
    source, obj, dll = (out / name for name in
                        ('terrain_profile.c', 'terrain_profile.obj', 'jp_native_frames.dll'))
    source.write_bytes(raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link[link.index(old)] = obj.as_posix()
    link[-1] = dll.as_posix()
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    recipe, driver = profile['commands']
    commands = [recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_ring_sphere=dict(parent_sha256=sha(parent), source_sha256=sha(source),
                                             object_sha256=sha(obj), commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Emerald Coast ring sphere integration compiled and linked')


if __name__ == '__main__':
    main()
