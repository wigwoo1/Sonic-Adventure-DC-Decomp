"""Connect the selected POINT camera to the native Sonic draw."""
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
    for name in ('workspace', 'upstream', 'baseline', 'movement', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, movement, out = (p.resolve() for p in
        (args.workspace, args.upstream, args.baseline, args.movement, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    prior = manifest['emerald_camera_mode']
    tick = manifest['emerald_stage_tick']
    camera, player = base/'camera_mode.c', movement/'ring_walk.c'
    assert digest(camera) == prior['source_sha256']
    assert digest(player) == tick['source_sha256'][player.name]
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    modes = pinned(upstream, 'src/camera/modes.c').split(b'camera_mode_list[CAMERA_MODES] = {',1)[1]
    names = re.findall(rb'\{(\w+),\s*CAMERA_LEVEL_\w+,\s*CAMERA_\w+,', modes.split(b'\n};',1)[0])
    indices = [i for i, name in enumerate(names) if name == b'camera_step_point']
    assert indices == [39, 40, 41]
    camera_source = camera.read_bytes() + (
        f'\n#define POINT_MODE_FIRST {indices[0]}\n#define POINT_MODE_LAST {indices[-1]}\n'
        '#include "jp_native_emerald_camera_view.inc"\n').encode()
    player_source = player.read_bytes()
    anchor = b'static void fixture_draw(struct task *task)'
    assert player_source.count(anchor) == 1
    player_source = player_source.replace(anchor,
        b'int jp_emerald_point_view(const NJS_POINT3 *, NJS_MATRIX *);\n\n' + anchor)
    anchor = b'    njTranslateV(NULL, &rebase);'
    assert player_source.count(anchor) == 1
    player_source = player_source.replace(anchor, br'''    if (ring_walk_enabled) {
        NJS_MATRIX point_view;
        if (jp_emerald_point_view(&task->work->pos, &point_view)) {
            fixture->result = 12;
            njPopMatrix(1);
            return;
        }
        njSetMatrix(NULL, &point_view);
    } else {
        njTranslateV(NULL, &rebase);
    }''')
    # Preserve a failed camera setup through the probe's draw-count check.
    anchor = b'        if (fixture.draws != 1 || player->anim.frame != *drawn_frame)'
    assert player_source.count(anchor) == 1
    out.mkdir()
    dll = out/'jp_native_frames.dll'
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    link = shlex.split((base/'link.rsp').read_text())
    commands = []
    for name, raw, recipe in (('camera_mode', camera_source, prior['commands'][0]),
                              ('ring_walk', player_source, tick['commands'][1])):
        unit, obj = out/(name + '.c'), out/(name + '.obj')
        unit.write_bytes(raw)
        old = next(p for p in link if Path(p).name == obj.name)
        link[link.index(old)] = str(obj)
        commands.append(recipe[:recipe.index('-c')] + ['-c', str(unit), '-o', str(obj)])
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"' + arg.replace('\\', '/') + '"\n' for arg in link), encoding='utf-8')
    commands.append([prior['commands'][1][0], '@' + str(rsp)])
    for phase, command in zip(('camera', 'movement', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase + '.stderr')).write_bytes(run.stderr)
        assert run.returncode == 0 and not run.stderr, run.stderr.decode(errors='replace')
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll),
                    emerald_camera_view=dict(parent_sha256=digest(parent), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast POINT view compiled and linked')


if __name__ == '__main__':
    main()
