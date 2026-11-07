"""Extend the Windows stage owner with the pinned Sonic beach loader schedule."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


def schedule(upstream):
    path = 'src/system/loader.c'
    loader = pinned(upstream, path).decode()
    source = owner_function(loader.encode(), 'loader_read_stage_files').decode()
    beach = source.split('case STAGE_BEACH:', 1)[1].split('case STAGE_', 1)[0]
    suffix = re.search(r'loader_suffixes\[CHARACTER_COUNT\] = \{"([A-Z]+)"', loader)[1]
    entries = re.findall(r'loader_read_(prs|set|camera)\("([^"]+)"', beach)
    assert beach.count('goto alife_race_camera;') == 1
    assert 'alife_race_camera:\n        loader_read_camera(camera, dest);' in source
    entries.append(('camera', re.search(r'camera = "([^"]+)";', beach)[1]))
    names = [name.upper() if kind == 'prs' else
             ('SET' if kind == 'set' else 'CAM') + name + suffix + '.BIN'
             for kind, name in entries]
    assert len(names) == 7 and names[0] == 'STG01.PRS'
    return names + ['BEACH01.PRS']


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
    native = root/'outputs/sonic-adventure/native'
    owner = native/'jp_native_stage_resources.c'
    prior = manifest['stage_resources']
    assert digest(owner) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    recipe, driver = prior['commands']
    names = schedule(upstream)
    source = owner.read_text()

    def replace(old, new, count=1):
        nonlocal source
        assert source.count(old) == count, old
        source = source.replace(old, new)

    replace('{"STG01.PRS", "BEACH01.PRS"}',
            '{' + ', '.join(json.dumps(name) for name in names) + '}')
    replace('static struct resource_view decoded[2];',
            'enum { RESOURCE_COUNT = sizeof names / sizeof names[0] };\n'
            'static struct resource_view decoded[RESOURCE_COUNT];')
    replace('input[2];', 'input[RESOURCE_COUNT];')
    replace('bytes[2] = {0}', 'bytes[RESOURCE_COUNT] = {0}')
    replace('i < 2;', 'i < RESOURCE_COUNT;', 3)
    replace('index >= 2', 'index >= RESOURCE_COUNT')
    replace('#include "../third_party/prskit/prskit.h"',
            '#include "prskit.h"\n#include <string.h>')
    replace('before.completed + 2 ||\n                decode(0, bytes[0]) || decode(1, bytes[1])) goto failed;',
            '''before.completed + RESOURCE_COUNT) goto failed;
            if (decode(0, bytes[0]) ||
                decode(RESOURCE_COUNT - 1, bytes[RESOURCE_COUNT - 1])) goto failed;
            for (unsigned i = 1; i < RESOURCE_COUNT - 1; ++i) {
                void *copy = malloc(bytes[i]);
                if (!copy) goto failed;
                memcpy(copy, input[i], bytes[i]);
                decoded[i] = (struct resource_view){copy, bytes[i]};
            }''')
    replace('_aligned_malloc(INPUT_CAPACITY, 32)', '_aligned_malloc(capacity, 32)')
    replace('        input[i] = _aligned_malloc', '        size_t capacity = (i == 0 || i == RESOURCE_COUNT - 1)\n'
            '            ? INPUT_CAPACITY : 64u << 10;\n        input[i] = _aligned_malloc')
    replace('names[i], input[i], INPUT_CAPACITY', 'names[i], input[i], capacity')
    replace('entry.file_bytes > INPUT_CAPACITY', 'entry.file_bytes > capacity')
    out.mkdir()
    unit, obj, dll = (out/name for name in ('stage_loader.c', 'stage_loader.obj', 'jp_native_frames.dll'))
    unit.write_text(source, encoding='utf-8')
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out/dep.name)
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'stage_resources.obj')
    assert digest(old) == prior['object_sha256']
    link[link.index(old)] = str(obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')] + ['-I' + str(native.parent/'third_party/prskit'),
                                               '-c', str(unit), '-o', str(obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_stage_loader=dict(files=names, parent_sha256=digest(parent),
                                             source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast loader compiled and linked')


if __name__ == '__main__':
    main()
