"""Link the original sphere-pair collision worker alongside the existing list owner."""
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
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (p.resolve() for p in
                                (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert sha(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    prior = manifest['emerald_collision_lists']
    path = 'src/collision/collision.c'
    raw = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{path}'])
    assert raw == (upstream / path).read_bytes()
    assert sha(upstream / path) == prior['pinned_collision_sha256']
    start = raw.index(b'#define SET_HIT')
    end = raw.index(b'/* note that', start)
    sphere = owner_function(raw, 'collide_sphere_sphere')
    debug = sphere.index(b'    if (debug_hidden >= 0)')
    assert b'shape_draw' in sphere[debug:]
    sphere = sphere[:debug] + b'    return distance;\n}\n'
    names = ('collision_hit', 'collision_hurt', 'try_mark_pushed', 'collision_get_shape_center')
    out.mkdir()
    source, obj, dll = (out / name for name in
                        ('collision_spheres.c', 'collision_spheres.obj', 'jp_native_frames.dll'))
    source.write_bytes(b'\n'.join([
        b'#include "collision/collision.h"\n#include "system/util.h"\n'
        b'#include "jp_native_action_numeric.h"\n'
        b'Float njScalor(NJS_VECTOR *v) { return njSqrt(jp_action_fipr_self(v->x,v->y,v->z,0)); }',
        raw[start:end], *(owner_function(raw, name) for name in names), sphere,
        b'#include "jp_collision_spheres_fixture.inc"\n',
    ]))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link = shlex.split((base / 'link.rsp').read_text())
    assert link[-2] == '-o'
    link[-2:] = [obj.as_posix(), '-o', dll.as_posix()]
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    recipe, driver = prior['commands']
    commands = [recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_collision_spheres=dict(upstream_commit=PIN, parent_sha256=sha(parent),
                                                   generated_source_sha256=sha(source),
                                                   object_sha256=sha(obj), commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Original sphere collision worker compiled and linked')


if __name__ == '__main__':
    main()
