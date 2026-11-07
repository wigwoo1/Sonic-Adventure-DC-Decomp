"""Relink the qualified terrain profile with bounded drawer timings."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def instrument(source):
    changes = (
        ('    struct simple_model_context draw={{0,0},0};',
         '    uint64_t started = jp_clock_wall_counts();\n    struct simple_model_context draw={{0,0},0};'),
        ('    int status=jp_simple_calc_with_records(model,',
         '    uint64_t validated = jp_clock_wall_counts();\n'
         '    uint64_t prior_consumer = draw_ticks[2]+draw_ticks[3];\n'
         '    draw_ticks[0] += validated-started;\n'
         '    int status=jp_simple_calc_with_records(model,'),
        ('    _nj_light_material_=saved;',
         '    uint64_t total = jp_clock_wall_counts()-validated;\n'
         '    uint64_t consumed = draw_ticks[2]+draw_ticks[3]-prior_consumer;\n'
         '    if (total >= consumed) draw_ticks[1] += total-consumed;\n'
         '    _nj_light_material_=saved;'),
        ('    (void)records;',
         '    uint64_t started = jp_clock_wall_counts();\n'
         '    uint64_t prior_emit = draw_ticks[3];\n    (void)records;'),
        ('        cursor=emit(mesh,cursor,dispatch[1],projection[0]!=0,scales,draw->ambient);',
         '        uint64_t emit_start = jp_clock_wall_counts();\n'
         '        cursor=emit(mesh,cursor,dispatch[1],projection[0]!=0,scales,draw->ambient);\n'
         '        draw_ticks[3] += jp_clock_wall_counts()-emit_start;'),
        ('    return 0;\n}\n\n__declspec(dllexport) int jp_simple_draw_model_bounded',
         '    uint64_t total = jp_clock_wall_counts()-started;\n'
         '    draw_ticks[2] += total-(draw_ticks[3]-prior_emit);\n'
         '    return 0;\n}\n\n__declspec(dllexport) int jp_simple_draw_model_bounded'),
    )
    for old,new in changes:
        assert source.count(old)==1,old
        source=source.replace(old,new)
    return """#include "jp_native_clock.h"
static uint64_t draw_ticks[4];
__declspec(dllexport) void jp_emerald_profile_bounded(uint64_t *out)
{
    for (unsigned i=0; i<4; ++i) {
        out[i]=draw_ticks[i];
        draw_ticks[i]=0;
    }
}
""" + source


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, base, out = (p.resolve() for p in (args.workspace, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    native = root/'outputs/sonic-adventure/native'
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    link = shlex.split((base/'link.rsp').read_text())
    old = next(Path(p) for p in link if Path(p).name == 'env_polygons_caller.obj')
    source = old.with_suffix('.c')
    original = json.loads((old.parent/'build_verification.json').read_bytes())['simple_env_polygons']
    assert digest(source) == original['generated_sha256'][source.name]
    assert digest(old) == original['generated_sha256'][old.name]
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    out.mkdir()
    unit,obj,dll=(out/name for name in ('bounded_profile.c','bounded_profile.obj','jp_native_frames.dll'))
    unit.write_text(instrument(source.read_text()), encoding='utf-8')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe,driver=manifest['emerald_terrain_profile']['commands']
    link[link.index(old.as_posix())] = str(obj)
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
                    emerald_bounded_profile=dict(parent_sha256=digest(parent),
                        
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast bounded profile compiled and linked')


if __name__ == '__main__':
    main()
