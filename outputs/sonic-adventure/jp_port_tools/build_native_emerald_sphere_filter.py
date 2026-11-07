"""Apply the pinned pair-dispatch ignore policy to the live sphere worker."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function, sha


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, required=True, type=Path)
    args = parser.parse_args()
    root, upstream, base, out = (p.resolve() for p in
                                (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    worker = manifest['emerald_collision_spheres']
    link = shlex.split((base / 'link.rsp').read_text())
    old = next(arg for arg in link if Path(arg).name == 'collision_spheres.obj')
    prior = Path(old).with_suffix('.c')
    assert sha(prior) == worker['generated_source_sha256']
    path = 'src/collision/collision.c'
    upstream_source = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{path}'])
    assert upstream_source == (upstream / path).read_bytes()
    original_filter = owner_function(upstream_source, 'collision_ignores')
    anchor = b'#include "jp_collision_spheres_fixture.inc"'
    raw = prior.read_bytes()
    assert raw.count(anchor) == 1
    fixture = (root / 'outputs/sonic-adventure/native/jp_collision_spheres_fixture.inc').read_bytes()
    call = b'    return collide_sphere_sphere(a, b);'
    assert fixture.count(call) == 1
    fixture = fixture.replace(call, b'    if (!sphere_pair_enabled(a, b)) return 1000000.0F;\n' + call)
    raw = raw.replace(anchor, original_filter + b'\n#include "jp_collision_sphere_filter.inc"\n' + fixture)
    out.mkdir()
    source, obj, dll = (out / name for name in
                        ('collision_spheres.c', 'collision_spheres.obj', 'jp_native_frames.dll'))
    source.write_bytes(raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link[link.index(old)] = obj.as_posix()
    link[-1] = dll.as_posix()
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    recipe, driver = worker['commands']
    commands = [recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_sphere_filter=dict(upstream_commit=PIN, parent_sha256=sha(parent),
                                               source_sha256=sha(source), commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Original sphere pair filter compiled and linked')


if __name__ == '__main__':
    main()
