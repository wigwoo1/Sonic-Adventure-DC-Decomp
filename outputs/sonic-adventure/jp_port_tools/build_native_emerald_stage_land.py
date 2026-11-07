"""Link the Emerald Coast session's terrain relocation and lifetime."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest
from build_native_stage_start import pinned
from extract_native_emerald_collision import LOAD


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'camera', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, camera, out = (p.resolve() for p in
        (args.workspace, args.upstream, args.baseline, args.camera, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    source = camera/'camera_mode.c'
    symbols = pinned(upstream, 'config/stg01/symbols.txt')
    addresses = dict((int(act), int(addr,16)) for addr, act in
        re.findall(rb'^(0x[0-9a-f]+)\s+beach_act([012])_land\s*$', symbols, re.M))
    assert set(addresses) == {0,1,2}
    raw = source.read_bytes()
    anchor = '__declspec(dllexport) int jp_emerald_stage_close(void)\n{\n    if (emerald_stage_stepping) return -1;'.encode()
    assert raw.count(anchor) == 1
    raw = raw.replace(anchor, b'static int close_stage_land(void);\n\n' + anchor +
                      b'\n    if (close_stage_land()) return -1;')
    raw += (f'\n#define STAGE_MODULE_BASE {LOAD}u\n'
            'static const uint32_t land_headers[] = {' +
            ', '.join(str(addresses[i]) for i in range(3)) + '};\n'
            '#include "jp_native_emerald_stage_land.inc"\n').encode()
    out.mkdir()
    unit, obj, dll = (out/name for name in ('stage_land.c', 'stage_land.obj', 'jp_native_frames.dll'))
    unit.write_bytes(raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe = manifest['emerald_camera_view']['commands'][0]
    driver = manifest['emerald_land_pass']['commands'][1]
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'camera_mode.obj')
    assert Path(old).parent == camera
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
    adapter = root/'outputs/sonic-adventure/native/jp_native_emerald_stage_land.inc'
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_stage_land=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(adapter), source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast stage terrain owner compiled and linked')


if __name__ == '__main__':
    main()
