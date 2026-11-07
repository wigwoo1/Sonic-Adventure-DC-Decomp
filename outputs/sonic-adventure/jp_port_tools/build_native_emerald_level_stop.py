"""Relink the qualified ground modes with Sonic walking to a stop."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('#include "jp_native_sonic_level_modes.inc"',
         '#include "jp_native_sonic_level_modes.inc"\n#include "jp_native_sonic_level_stop.inc"'),
        ('struct animation_fixture {', 'struct animation_fixture {\n    int neutral_tail;'),
        ('if (jp_sonic_step_level_modes(task, fixture->triangles,',
         'if (jp_sonic_step_level_stop(task, fixture->triangles,'),
        ('work->mode != expected) fixture->result = 8;',
         '(!fixture->neutral_tail && work->mode != expected)) fixture->result = 8;'),
        ('finish:\n    if (player->anim.blend)', """    if (frame && !fixture.result) {
        fixture.neutral_tail = 1;
        pad_disable();
        Float starting_speed = player->speed.x;
        if (!(starting_speed > 0) || work->mode != JP_SONIC_WALK)
            fixture.result = 13;
        unsigned ticks = 0;
        while (!fixture.result && work->mode == JP_SONIC_WALK && ticks++ < 120) {
            Float before = player->speed.x;
            task_step_level(1);
            if (player->speed.x > before) fixture.result = 14;
        }
        if (!fixture.result && (work->mode != JP_SONIC_STAND || player->speed.x ||
            player->anim.action != ACTION_STAND || ticks < 2)) fixture.result = 15;
        NJS_POINT3 stopped = work->pos;
        for (unsigned i = 0; !fixture.result && i < 3; ++i) task_step_level(1);
        if (!fixture.result && (work->mode != JP_SONIC_STAND ||
            stopped.x != work->pos.x || stopped.y != work->pos.y || stopped.z != work->pos.z))
            fixture.result = 16;
        pad_enable();
    }
finish:
    if (player->anim.blend)"""),
    )
    for old, new in changes:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    return source


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
    prior = manifest['emerald_level_modes']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    out.mkdir()
    dll = out/'jp_native_frames.dll'
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    fixture_recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    fixture_source = Path(fixture_recipe[-3])
    assert digest(fixture_source) == prior['source_sha256']
    assert digest(native/'jp_native_sonic_level_modes.inc') == prior['adapter_sha256']
    old_fixture = next(p for p in link if Path(p).name == 'player_frame_probe.obj')
    assert Path(old_fixture) == Path(fixture_recipe[-1])
    fixture, fixture_obj = out/'player_frame_probe.c', out/'player_frame_probe.obj'
    fixture.write_text(probe(fixture_source.read_text()), encoding='utf-8')
    link[link.index(old_fixture)] = str(fixture_obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands = [fixture_recipe[:-4] + ['-c', str(fixture), '-o', str(fixture_obj)],
                [driver[0], '@' + str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_level_stop=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_sonic_level_stop.inc'),
                        source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast level stop compiled and linked')


if __name__ == '__main__':
    main()
