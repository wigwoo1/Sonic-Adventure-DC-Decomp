"""Replace only the Simple projection object in the measured Emerald Coast build."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--bit-scan', action='store_true',
                        help='use a leading-zero instruction to normalize the dot product')
    for name in ('workspace', 'baseline', 'output'):
        parser.add_argument('--' + name, required=True, type=Path)
    args = parser.parse_args()
    root, base, out = (p.resolve() for p in (args.workspace, args.baseline, args.output))
    assert out.is_relative_to(root / 'work/sonic-adventure')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    link = shlex.split((base / 'link.rsp').read_text())
    old = next(arg for arg in link if Path(arg).name == 'simple_projection.obj')
    original = json.loads(Path(old).with_name('build_verification.json').read_bytes())
    assert digest(old) == original['simple_projection_recipe']['objects_sha256'][str(Path(old))]
    recipe = next(item['command'] for item in original['commands']
                  if item['command'][-1] == str(Path(old)))
    source = root / 'outputs/sonic-adventure/native/jp_native_simple_projection.c'
    obj = out / Path(old).name
    dll = out / 'jp_native_frames.dll'
    out.mkdir()
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link[link.index(old)] = obj.as_posix()
    link[-1] = dll.as_posix()
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    driver = manifest['emerald_projection_profile']['commands'][1][0]
    flags = recipe[:recipe.index('-c')]
    adaptation = {}
    if args.bit_scan:
        vendor = source.parent.parent / 'third_party/skmp-sh4-fpu/sh4_fpu_approx.h'
        raw = vendor.read_bytes()
        assert digest(vendor) == 'cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
        loop = b'while ((S >> (top + 1)) != 0) top++;'
        assert raw.count(loop) == 1
        header = out / vendor.name
        header.write_bytes(raw.replace(loop, b'top = 63 - __builtin_clzll((unsigned long long)S);'))
        flags += ['-include', str(header)]
        adaptation = dict(vendor_sha256=digest(vendor), adapted_header_sha256=digest(header))
    commands = [flags + ['-c', str(source), '-o', str(obj)],
                [driver, '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest['authored_source_sha256'][source.relative_to(root).as_posix()] = digest(source)
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_projection_xyz=dict(parent_sha256=digest(parent),
                                                object_sha256=digest(obj), commands=commands,
                                                **adaptation))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Emerald Coast XYZ projection compiled and linked')


if __name__ == '__main__':
    main()
