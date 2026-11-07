"""Connect ring pickups to the existing game sound owner."""
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
    for name in ('workspace', 'upstream', 'baseline', 'sound-import', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, sound_import, out = (p.resolve() for p in
        (args.workspace, args.upstream, args.baseline, args.sound_import, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    prior = manifest['emerald_ring_sparkle']
    source = base/'ring_sparkle.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    assert sound_import.name == 'jp_native_game_sound.lib'
    for sound in (sound_import.with_suffix('.dll'), base/'jp_native_game_sound.dll'):
        assert digest(sound) == manifest['game_sound_DLL_sha256']
    header = pinned(upstream, 'src/sound/sound.h')
    hud = pinned(upstream, 'src/hud/hud.c')
    ring = re.search(rb'^#define SE_RING \d+$', header, re.M).group()
    life = re.search(rb'^#define SE_EXTRA_LIFE \d+$', hud, re.M).group()
    prefix = br'''
#define JP_EFFECTS_API __declspec(dllimport)
#include "jp_native_effects.h"
static void play_pickup_effect(int32_t effect)
{
    const struct jp_effects_request request = {.bytes = sizeof request, .se = effect};
    int32_t slot;
    /* Sound availability must not affect ring or life accounting. */
    (void)jp_effects_request(&request, &slot);
}
'''
    raw = source.read_bytes()
    for old, new in (
        (b'            ring_add(1);', b'            ring_add(1);\n            play_pickup_effect(SE_RING);'),
        (b'    lives += (Sint8)count;', b'    if (count > 0) play_pickup_effect(SE_EXTRA_LIFE);\n'
                                       b'    lives += (Sint8)count;'),
    ):
        assert raw.count(old) == 1
        raw = raw.replace(old, new)
    out.mkdir()
    unit, obj, dll = (out/name for name in ('ring_audio.c', 'ring_audio.obj', 'jp_native_frames.dll'))
    unit.write_bytes(ring + b'\n' + life + b'\n' + prefix + raw)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'ring_sparkle.obj')
    link[link.index(old)] = str(obj)
    link.insert(link.index('-o'), str(sound_import))
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
                    emerald_ring_audio=dict(parent_sha256=digest(parent),
                                            source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ring audio compiled and linked')


if __name__ == '__main__':
    main()
