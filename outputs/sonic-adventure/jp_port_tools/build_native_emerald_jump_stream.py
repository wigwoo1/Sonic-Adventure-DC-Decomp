"""Relink the qualified jump sequence with bounded per-tick frame delivery."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source):
    changes = (
        ('NJS_ACTION *land, uint64_t *air,',
         'NJS_ACTION *land, int (*present_air)(uint64_t, Float),'),
        ('            unsigned captured = 0;\n            Float peak = start.y;',
         '            unsigned captured = 0, airborne_ticks = 0;'),
        ("""                if (work->pos.y > peak) peak = work->pos.y;
                if (air && airborne &&
                    ((captured == 0 && work->pos.y > prior) ||
                     (captured == 1 && work->pos.y < prior) ||
                     (captured == 2 && work->pos.y < start.y + (peak - start.y)*0.5F)) &&
                    player->anim.current == ACTION_SPIN) {""",
         """                airborne_ticks += airborne;
                if (present_air && airborne) {
                    uint64_t air = 0;"""),
        ('&air_scene, air + captured)) fixture.result = 23;\n                    else ++captured;',
         """&air_scene, &air)) fixture.result = 23;
                    if (air) {
                        int presented = present_air(air, work->pos.y);
                        int released = jp_frame_release(air);
                        if (presented || released) fixture.result = 24;
                        else ++captured;
                    }"""),
        ('(air && captured != 3)', '(present_air && (captured < 3 || captured != airborne_ticks))'),
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
    prior = manifest['emerald_jump_sequence']
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
                    emerald_jump_stream=dict(parent_sha256=digest(parent),
                        
                        source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast jump stream compiled and linked')


if __name__ == '__main__':
    main()
