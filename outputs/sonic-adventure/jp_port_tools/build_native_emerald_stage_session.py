"""Link the Emerald Coast startup and teardown around qualified stage owners."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest
from build_native_stage_start import pinned


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
    prior = manifest['emerald_ring_audio']
    source = base/'ring_audio.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    sound = pinned(upstream, 'src/sound/sound.c')
    bank = re.search(rb'case STAGE_BEACH:\s+sound_load_file\(("[^"]+")\);', sound)[1].upper()
    out.mkdir()
    unit, obj, dll = (out/name for name in ('stage_session.c', 'stage_session.obj', 'jp_native_frames.dll'))
    unit.write_bytes(source.read_bytes() + b'\n#define EMERALD_BANK ' + bank +
                     b'\nstruct resource_view { const void *data; size_t bytes; };\n'
                     b'#include "jp_native_emerald_stage.inc"\n')
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'ring_audio.obj')
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
    adapter = root/'outputs/sonic-adventure/native/jp_native_emerald_stage.inc'
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_stage_session=dict(parent_sha256=digest(parent), adapter_sha256=digest(adapter),
                                               source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast stage session compiled and linked')


if __name__ == '__main__':
    main()
