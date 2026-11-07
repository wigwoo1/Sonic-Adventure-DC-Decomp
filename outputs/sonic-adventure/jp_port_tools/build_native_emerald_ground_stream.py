"""Relink the qualified jump stream with ground stop and stand presentation."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('static int input_phase;', '#include "jp_native_emerald_stream_frame.inc"\n\nstatic int input_phase;'),
        ('int (*present_air)(uint64_t, Float)', 'int (*present_air)(uint64_t, Float, int)'),
        ('        fixture.neutral_tail = 1;', """        Uint32 charged_flag = work->flags & SONIC_CHARGED;
        work->flags &= ~SONIC_CHARGED;
        fixture.neutral_tail = 1;"""),
        ('            if (player->speed.x > before) fixture.result = 14;',
         """            if (player->speed.x > before) fixture.result = 14;
            if (!fixture.result && present_air && stream_player_frame(task, textures, aura,
                    air_bound, words, word_count, 0, present_air)) fixture.result = 25;"""),
        ('        for (unsigned i = 0; !fixture.result && i < 3; ++i) task_step_level(1);',
         """        for (unsigned i = 0; !fixture.result && i < 3; ++i) {
            task_step_level(1);
            if (!fixture.result && present_air && stream_player_frame(task, textures, aura,
                    air_bound, words, word_count, 0, present_air)) fixture.result = 25;
        }"""),
        ('        input_phase = 0;\n    }\nfinish:', '        input_phase = 0;\n'
         '        work->flags |= charged_flag;\n    }\nfinish:'),
    )
    for old, new in changes:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    start = source.index('                    uint64_t air = 0;')
    end = source.index('                rose |=', start)
    return source[:start] + """                    player->own.sonic.curled_steps = 0;
                    if (stream_player_frame(task, textures, aura, air_bound,
                            words, word_count, 1, present_air)) fixture.result = 23;
                    else ++captured;
                }
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
    prior = manifest['emerald_jump_stream']
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
    assert digest(native/'jp_native_sonic_jump_animation.inc') == manifest['emerald_jump_animation']['adapter_sha256']
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
                    emerald_ground_stream=dict(parent_sha256=digest(parent),
                        
                        adapter_sha256=digest(native/'jp_native_emerald_stream_frame.inc'), source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast ground stream compiled and linked')


if __name__ == '__main__':
    main()
