"""Relink the qualified ground modes with the ordinary Sonic jump edge."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('#include "jp_native_sonic_level_stop.inc"',
         '#include "jp_native_sonic_level_stop.inc"\n#include "jp_native_sonic_ground_jump.inc"'),
        ('        pad_enable();\n    }\nfinish:', """        pad_enable();
        if (!fixture.result) {
            Uint32 saved = pad_records[0].press;
            pad_records[0].press = 0;
            player->jump_steps = 9;
            if (jp_sonic_try_ground_jump(task) != 0 || work->mode != JP_SONIC_STAND ||
                player->jump_steps != 9) fixture.result = 17;
            pad_records[0].press = pad_jump_buttons;
            if (!fixture.result &&
                (jp_sonic_try_ground_jump(task) != 1 || work->mode != JP_SONIC_JUMP ||
                 player->speed.y != player->consts.jump_speed || player->jump_steps ||
                 player->anim.action != ACTION_SPIN || player->action_speed != 5.0F ||
                 (work->flags & (PLAYER_ATTACKING | PLAYER_ON_TASK)) != PLAYER_ATTACKING ||
                 jp_sonic_try_ground_jump(task) != -1)) fixture.result = 18;
            pad_records[0].press = saved;
        }
    }
finish:"""),
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
    prior = manifest['emerald_level_stop']
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
    assert digest(native/'jp_native_sonic_level_stop.inc') == prior['adapter_sha256']
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
                    emerald_ground_jump=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_sonic_ground_jump.inc'),
                        source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ground jump compiled and linked')


if __name__ == '__main__':
    main()
