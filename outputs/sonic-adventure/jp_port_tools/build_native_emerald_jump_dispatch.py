"""Relink the qualified vertical jump with native scheduled input dispatch."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('#include "jp_native_sonic_vertical_jump.inc"',
         '#include "jp_native_sonic_vertical_jump.inc"\n#include "jp_native_sonic_level_dispatch.inc"'),
        ('    int neutral_tail;', '    int neutral_tail;\n    int jump_edge;'),
        ('        if (jp_sonic_step_level_stop(task, fixture->triangles,',
         '        if (fixture->jump_edge) {\n'
         '            pad_records[0].press |= pad_jump_buttons;\n'
         '            fixture->jump_edge = 0;\n'
         '        }\n'
         '        if (jp_sonic_step_level_dispatch(task, fixture->triangles,'),
    )
    for old, new in changes:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    start = source.index('        pad_enable();\n        if (!fixture.result) {')
    end = source.index('    }\nfinish:', start)
    return source[:start] + """        if (!fixture.result) {
            NJS_POINT3 start = work->pos;
            fixture.jump_edge = 1;
            task_step_level(1);
            if (fixture.result || fixture.jump_edge || work->mode != JP_SONIC_JUMP ||
                player->speed.y != player->consts.jump_speed || player->anim.action != ACTION_SPIN)
                fixture.result = 17;
            int rose = 0, descended = 0;
            for (unsigned i = 0; i < 120 && !fixture.result &&
                 work->mode == JP_SONIC_JUMP; ++i) {
                Float prior = work->pos.y;
                task_step_level(1);
                int airborne = work->mode == JP_SONIC_JUMP;
                if (work->pos.x != start.x || work->pos.z != start.z ||
                    (airborne && (work->flags & PLAYER_ON_GROUND))) fixture.result = 19;
                rose |= work->pos.y > prior;
                descended |= airborne && work->pos.y < prior;
            }
            if (!fixture.result && (!rose || !descended || work->mode != JP_SONIC_STAND ||
                work->pos.y != start.y || !(work->flags & PLAYER_ON_LAND) ||
                (work->flags & PLAYER_ATTACKING) || player->speed.y ||
                player->anim.action != ACTION_LANDED)) fixture.result = 20;
        }
        pad_enable();
""" + source[end:]


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
    prior = manifest['emerald_vertical_jump']
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
    assert digest(native/'jp_native_sonic_vertical_jump.inc') == prior['adapter_sha256']
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
                    emerald_jump_dispatch=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_sonic_level_dispatch.inc'),
                        source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast jump dispatch compiled and linked')


if __name__ == '__main__':
    main()
