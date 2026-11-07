"""Relink the qualified terrain profile with bounded drawer timings."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def instrument(source):
    for old,new in (
        ('    loop_error=jp_simple_project(', '    uint64_t started=jp_clock_wall_counts();\n    loop_error=jp_simple_project('),
        ('&queues,kind);', '&queues,kind);\n    projection_ticks[0]+=jp_clock_wall_counts()-started;'),
        ('    Sint32 value=_njSimpleCalcModel(model);',
         '    uint64_t started=jp_clock_wall_counts();\n'
         '    Sint32 value=_njSimpleCalcModel(model);\n'
         '    projection_ticks[1]+=jp_clock_wall_counts()-started;'),
    ):
        assert source.count(old)==1,old
        source=source.replace(old,new)
    return """#include "jp_native_clock.h"
static uint64_t projection_ticks[2];
__declspec(dllexport) void jp_emerald_profile_projection(uint64_t *out)
{
    for (unsigned i=0; i<2; ++i) {
        out[i]=projection_ticks[i];
        projection_ticks[i]=0;
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
    old = next(Path(p) for p in link if Path(p).name == 'simple_calc_with_records.obj')
    source = old.with_suffix('.c')
    original = json.loads((old.parent/'build_verification.json').read_bytes())['simple_lease']
    assert digest(source) == original['generated_sha256']
    assert digest(old) == original['object_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']

    out.mkdir()
    unit,obj,dll=(out/name for name in ('projection_profile.c','projection_profile.obj','jp_native_frames.dll'))
    unit.write_text(instrument(source.read_text()), encoding='utf-8')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe=original['compile']
    _,driver=manifest['emerald_terrain_profile']['commands']
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
                    emerald_projection_profile=dict(parent_sha256=digest(parent),
                        
                        source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast projection profile compiled and linked')


if __name__ == '__main__':
    main()
