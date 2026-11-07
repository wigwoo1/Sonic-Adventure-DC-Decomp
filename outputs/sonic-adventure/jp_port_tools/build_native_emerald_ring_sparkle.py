"""Add the pinned sparkle draw loop to the placed-ring scheduler."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


SCOPE = br'''
extern NJS_TEXLIST *_nj_current_texlist;
extern NJS_ARGB _nj_constant_material_;
static NJS_SPRITE *sparkle_sprite;
static NJS_POINT3 sparkle_rebase;
static Angle sparkle_yaw, sparkle_pitch;
static int sparkle_draws;

__declspec(dllexport) int jp_emerald_sparkle_draw_level(NJS_SPRITE *sprite,
    const NJS_POINT3 *rebase, Angle yaw, Angle pitch)
{
    if (sparkle_sprite || !sprite || !sprite->tanim || !sprite->tlist || !rebase)
        return -1;
    sparkle_sprite = sprite;
    sparkle_rebase = *rebase;
    sparkle_yaw = yaw;
    sparkle_pitch = pitch;
    sparkle_draws = 0;
    task_draw_level(3);
    sparkle_sprite = NULL;
    return sparkle_draws;
}
'''


def generate(source, adapter, ring):
    sparkle = owner_function(ring, 'ring_draw_sparkle')
    body = sparkle[sparkle.index(b'    frame[0] ='):sparkle.index(b'    if (frame[2] >')]
    for old, new in (
        (b'            njPushMatrix(NULL);', b'            if (!njPushMatrix(NULL)) continue;\n'
                                           b'            njTranslateV(NULL, &sparkle_rebase);'),
        (b'TURN(njRotateY, camera_work->ang.y);', b'njRotateY(NULL, sparkle_yaw);'),
        (b'TURN(njRotateX, camera_work->ang.x);', b'njRotateX(NULL, sparkle_pitch);'),
        (b'_builtin_fabsf(frame[i] * 0.05F)', b'size'),
        (b'&ring_sprite', b'sparkle_sprite'),
        (b'            njPopMatrix(1);', b'            njPopMatrix(1);\n            ++sparkle_draws;'),
    ):
        assert body.count(old) == 1
        body = body.replace(old, new)
    draw = SCOPE + br'''
static void native_sparkle_draw(struct task_work *work)
{
    if (!sparkle_sprite || work->mode != RING_SPARKLE) return;
    Float size, scale;
    int frame[3], i;
    NJS_TEXLIST *saved_list = _nj_current_texlist;
    NJS_ARGB saved_material = _nj_constant_material_;
    _nj_constant_material_ = (NJS_ARGB){0};
''' + body + br'''
    _nj_current_texlist = saved_list;
    _nj_constant_material_ = saved_material;
}
'''
    anchor = b'    if (!ring_draw_model || ring_draw_error'
    assert adapter.count(anchor) == 1
    adapter = adapter.replace(anchor, b'    native_sparkle_draw(work);\n' + anchor)
    include = b'#include "jp_native_ring_draw.inc"'
    assert source.count(include) == 1
    return source.replace(include, b'#define SPARKLE_END 7.0F\n' + draw + adapter).replace(
        b'\n#define SPARKLE_END 7.0F\nstatic void native_ring_sparkle_step',
        b'\nstatic void native_ring_sparkle_step')


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
    prior = manifest['emerald_ring_finish']
    source = base/'ring_finish.c'
    adapter = root/'outputs/sonic-adventure/native/jp_native_ring_draw.inc'
    assert digest(source) == prior['source_sha256']
    assert digest(adapter) == manifest['emerald_ring_draw']['adapter_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_sparkle.c', 'ring_sparkle.obj', 'jp_native_frames.dll'))
    unit.write_bytes(generate(source.read_bytes(), adapter.read_bytes(), pinned(upstream, 'src/object/ring.c')))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'ring_finish.obj')
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
                    emerald_ring_sparkle=dict(parent_sha256=digest(parent),
                                              source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ring sparkle draw compiled and linked')


if __name__ == '__main__':
    main()
