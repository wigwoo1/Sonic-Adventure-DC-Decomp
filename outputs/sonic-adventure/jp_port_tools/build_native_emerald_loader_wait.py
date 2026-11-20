"""Give asynchronous Emerald Coast reads a wall-clock completion deadline."""
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
    assert out.is_relative_to(root / 'work/sonic-adventure')
    parent = base / 'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(base / 'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    recipe = manifest['emerald_stage_ring_archives']['commands'][0]
    original = Path(recipe[recipe.index('-c') + 1])
    link = shlex.split((base / 'link.rsp').read_text())
    old_object = str(Path(recipe[recipe.index('-o') + 1])).replace('\\', '/')
    old = next(arg for arg in link if arg.replace('\\', '/') == old_object)
    raw = original.read_text()
    loop = '    for (unsigned poll = 0; poll < 100000; ++poll) {'
    ending = '            return 0;\n        }\n    }\nfailed:'
    assert raw.count(loop) == raw.count(ending) == 1
    raw = raw.replace(loop,
        '    ULONGLONG started = GetTickCount64();\n'
        '    for (;;) {')
    raw = raw.replace(ending,
        '            return 0;\n        }\n'
        '        if (GetTickCount64() - started >= 30000) goto failed;\n'
        '        Sleep(1);\n    }\nfailed:')
    out.mkdir()
    unit, obj, dll = (out / name for name in
                      ('stage_disc_loader.c', 'stage_disc_loader.obj', 'jp_native_frames.dll'))
    unit.write_text(raw, encoding='utf-8')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link[link.index(old)] = obj.as_posix()
    link[-1] = dll.as_posix()
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    driver = manifest['emerald_light_dash_target']['commands'][-1][0]
    commands = [recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)],
                [driver, '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        result = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(result.stderr)
        assert result.returncode == 0 and not result.stderr, result.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_loader_wait=dict(parent_sha256=digest(parent),
                        previous_source_sha256=digest(original), source_sha256=digest(unit),
                        timeout_ms=30000, commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n',
                                                encoding='utf-8')
    print('Emerald Coast asynchronous loader deadline compiled and linked')


if __name__ == '__main__':
    main()
