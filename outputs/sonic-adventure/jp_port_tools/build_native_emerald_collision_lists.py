"""Link the pinned collision registration and list owner into the native frame."""
import argparse
import json
import re
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

    def pinned(path):
        raw = subprocess.check_output(['git', '-C', str(upstream), 'show', f'{PIN}:{path}'])
        assert raw == (upstream / path).read_bytes()
        return raw

    path = 'src/collision/collision.c'
    raw = pinned(path)
    declarations = re.findall(
        rb'^(?:static )?(?:Uint16 collision_\w+_count|struct collision_work \*collision_\w+\[\d+\]|Sint8 dat_8c1b010[cd] = 0);',
        raw, re.M)
    assert len(declarations) == 18
    stage = pinned('src/stage/stage.c')
    counts = re.findall(rb'^Uint16 collision_(?:item|enemy)_count;', stage, re.M)
    assert len(counts) == 2
    room = re.search(rb'^#define ROOM\(list\) [^\r\n]+', raw, re.M).group()
    edit = re.search(rb'^Sint16 edit_mode = 0;', pinned('src/debug/editor.c'), re.M).group()
    names = ('collision_register', 'collision_clear_lists')
    out.mkdir()
    source, obj, dll = (out / name for name in
                        ('collision_lists.c', 'collision_lists.obj', 'jp_native_frames.dll'))
    source.write_bytes(b'\n'.join([
        b'#include "collision/collision.h"\n#include "debug/editor.h"\nextern int frame_redrawing;',
        edit, *declarations, *counts, room, owner_function(raw, 'collision_reset'),
        b'#define GCC 1', owner_function(raw, names[0]), b'#undef GCC',
        owner_function(raw, names[1]), b'#include "jp_collision_lists_fixture.inc"\n',
    ]))
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out / dependency.name)
    link = shlex.split((base / 'link.rsp').read_text())
    assert link[-2] == '-o'
    link[-2:] = [obj.as_posix(), *('-Wl,/export:' + name for name in names), '-o', dll.as_posix()]
    rsp = out / 'link.rsp'
    rsp.write_text(''.join(f'"{arg}"\n' for arg in link), encoding='utf-8')
    recipe = manifest['emerald_stage_tick']['commands'][0]
    driver = manifest['emerald_gameplay_clock']['commands'][1][0]
    commands = [recipe[:recipe.index('-c')] + ['-c', str(source), '-o', str(obj)],
                [driver, '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out / (phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=sha(dll),
                    emerald_collision_lists=dict(upstream_commit=PIN, parent_sha256=sha(parent),
                                                 pinned_collision_sha256=sha(upstream / path),
                                                 generated_source_sha256=sha(source),
                                                 object_sha256=sha(obj), commands=commands))
    (out / 'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Original collision registration and list clear compiled and linked')


if __name__ == '__main__':
    main()
