"""Link the pinned placement leave policy to Windows scheduler tasks."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    root, upstream, base, out = (path.resolve() for path in
                                  (args.workspace, args.upstream, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    prior = manifest['emerald_kind_table']
    source = base/'kind_table.c'
    assert digest(source) == prior['source_sha256']
    assert digest(base/'jp_native_frames.dll') == manifest['frames_DLL_sha256']
    raw = pinned(upstream, 'src/object/placement.c')
    leave_by = owner_function(raw, 'placement_try_leave_by')
    leave = owner_function(raw, 'placement_try_leave')
    prefix = br'''
#include "player/player.h"
#define LEAVE_RANGE (410.0F * 410.0F)
#define NEAR_RANGE (40.0F * 40.0F)
static struct task_work placement_viewers[3];
static struct task_work *placement_camera;
static struct task_work *placement_players[2];
static int placement_redrawing;

/* Copy the relevant fields for the task phase, so a task removing a
 * player during that phase cannot leave dangling viewer pointers. */
__declspec(dllexport) void jp_emerald_placement_viewers(struct task_work *player,
    struct task_work *second, struct task_work *camera, int redrawing)
{
    placement_redrawing = redrawing;
    placement_players[0] = player ? &placement_viewers[0] : NULL;
    placement_players[1] = second ? &placement_viewers[1] : NULL;
    placement_camera = camera ? &placement_viewers[2] : NULL;
    if (player) placement_players[0]->pos = player->pos;
    if (second) {
        placement_players[1]->pos = second->pos;
        PLAYER_VARS(placement_players[1])->character = PLAYER_VARS(second)->character;
    }
    if (camera) placement_camera->pos = camera->pos;
}
'''
    begin = leave_by.index(b'#ifdef GCC')
    end = leave_by.index(b'#endif', begin) + len(b'#endif')
    leave_by = (leave_by[:begin] + b'    if (placement_redrawing) return FALSE;\n' + leave_by[end:])
    leave_by = leave_by.replace(b'camera_work', b'placement_camera').replace(b'player_work', b'placement_players')
    out.mkdir()
    unit, obj, dll = (out/name for name in ('placement_leave.c', 'placement_leave.obj', 'jp_native_frames.dll'))
    unit.write_bytes(source.read_bytes() + prefix + leave_by + leave)
    for dependency in base.glob('*.dll'):
        if dependency.name != dll.name:
            shutil.copyfile(dependency, out/dependency.name)
    recipe, driver = prior['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name == 'kind_table.obj')
    link[link.index(old)] = str(obj)
    link[link.index('-o'):link.index('-o')] = [
        '-Wl,/export:placement_try_leave', '-Wl,/export:placement_try_leave_by']
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
                    emerald_placement_leave=dict(parent_sha256=digest(parent),
                                                 source_sha256=digest(unit), commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print('Native Emerald Coast placement retirement compiled and linked')


if __name__ == '__main__':
    main()
