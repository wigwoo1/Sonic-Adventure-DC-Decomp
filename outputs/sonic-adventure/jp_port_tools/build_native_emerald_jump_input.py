"""Relink the qualified scheduled jump with pad-backend test input."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('    int jump_edge;\n', ''),
        ('static void fixture_step(struct task *task)', """static int input_phase;

__declspec(dllexport) int jp_emerald_probe_input_phase(void)
{
    return input_phase;
}

static void fixture_step(struct task *task)"""),
        ("""        if (fixture->jump_edge) {
            pad_records[0].press |= pad_jump_buttons;
            fixture->jump_edge = 0;
        }
""", ''),
        ('        fixture.neutral_tail = 1;\n        pad_disable();',
         '        fixture.neutral_tail = 1;\n        input_phase = 1;'),
        ('            fixture.jump_edge = 1;\n            task_step_level(1);',
         '            input_phase = 2;\n            task_step_level(1);\n            input_phase = 1;'),
        ('fixture.result || fixture.jump_edge || work->mode != JP_SONIC_JUMP',
         'fixture.result || !(pad_records[0].press & PDD_DGT_TA) || work->mode != JP_SONIC_JUMP'),
        ('                int airborne = work->mode == JP_SONIC_JUMP;',
         '                if (pad_records[0].on & PDD_DGT_TA) fixture.result = 21;\n'
         '                int airborne = work->mode == JP_SONIC_JUMP;'),
        ('        pad_enable();\n    }\nfinish:', '        input_phase = 0;\n    }\nfinish:'),
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
    prior = manifest['emerald_jump_dispatch']
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
    assert digest(native/'jp_native_sonic_level_dispatch.inc') == prior['adapter_sha256']
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
                    emerald_jump_input=dict(parent_sha256=digest(parent),
                        adapter_sha256=digest(native/'jp_native_sonic_level_dispatch.inc'),
                        source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast jump input compiled and linked')


if __name__ == '__main__':
    main()
