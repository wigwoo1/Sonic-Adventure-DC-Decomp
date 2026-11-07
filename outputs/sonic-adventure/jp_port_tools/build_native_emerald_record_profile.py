"""Relink the qualified jump audio build with native frame recording timings."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def probe(source, stream):
    include = '#include "jp_native_emerald_stream_frame.inc"'
    assert source.count(include) == 1
    source = source.replace(include, stream)
    source = source.replace('int (*present_air)(uint64_t, Float, int)',
                            'int (*present_air)(uint64_t, Float, int, uint64_t, uint64_t)')
    return source


def profile_stream(source):
    changes = (
        ('int (*present)(uint64_t, Float, int)', 'int (*present)(uint64_t, Float, int, uint64_t, uint64_t)'),
        ('    if (jp_model_reserve(bound) || jp_frames_prepare_textures(words, count) ||',
         '    uint64_t started = jp_clock_wall_counts();\n'
         '    if (jp_model_reserve(bound) || jp_frames_prepare_textures(words, count)) return -1;\n'
         '    uint64_t prepared = jp_clock_wall_counts();\n    if ('),
        ('    int delivered = present(frame, task->work->pos.y, airborne);',
         '    uint64_t submitted = jp_clock_wall_counts();\n'
         '    int delivered = present(frame, task->work->pos.y, airborne, prepared-started, submitted-prepared);'),
    )
    for old, new in changes:
        assert source.count(old) == 1, old
        source = source.replace(old, new)
    return '#include "jp_native_clock.h"\n' + source


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
    prior = manifest['emerald_jump_audio']
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
    stream=native/'jp_native_emerald_stream_frame.inc'
    assert digest(stream) == manifest['emerald_ground_stream']['adapter_sha256']
    fixture.write_text(probe(fixture_source.read_text(), profile_stream(stream.read_text())), encoding='utf-8')
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
                    emerald_record_profile=dict(parent_sha256=digest(parent),
                        
                        adapter_sha256=digest(native/'jp_native_sonic_jump_audio.inc'), source_sha256=digest(fixture), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast recording profile compiled and linked')


if __name__ == '__main__':
    main()
