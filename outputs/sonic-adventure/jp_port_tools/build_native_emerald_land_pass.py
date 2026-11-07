"""Link a native terrain render pass to the active POINT matrix."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, base, out = (p.resolve() for p in (args.workspace, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    recipe, _, driver = manifest['emerald_camera_view']['commands']
    out.mkdir()
    unit, obj, dll = (out/name for name in ('land_pass.c', 'land_pass.obj', 'jp_native_frames.dll'))
    unit.write_bytes(b'#include <ninja.h>\n#include "jp_native_emerald_land_pass.inc"\n')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    link = shlex.split((base/'link.rsp').read_text())
    link.insert(link.index('-o'), str(obj))
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    adapter = root/'outputs/sonic-adventure/native/jp_native_emerald_land_pass.inc'
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_land_pass=dict(parent_sha256=digest(parent),
                                           adapter_sha256=digest(adapter), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast terrain pass compiled and linked')


if __name__ == '__main__':
    main()
