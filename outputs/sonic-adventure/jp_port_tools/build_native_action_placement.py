"""Connect table placement and checkpoint restore in the native action path."""
import argparse
import ctypes
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


SOURCE = b'''
#include <ninja.h>
#include "hud/hud.h"
#include "object/savepoint.h"
#include "player/player.h"
#include "stage/stage.h"
#include "system/task.h"

int jp_action_checkpoint_restore(struct task_work *);
int jp_stage_place_table_and_seed_history(struct task_work *, int, int);

/* Action stages entered normally or restarted at a checkpoint. */
__declspec(dllexport) int jp_stage_place_action_player(struct task_work *work)
{
    if (!work || work != player_work[0] || !player_records[0] ||
        PLAYER_VARS(work)->player != 0 || stage_current < 0 ||
        stage_current > STAGE_ACTION_LAST)
        return -1;

    int restored = jp_action_checkpoint_restore(work);
    if (restored < 0) return -1;
    if (!restored) {
        if (jp_stage_place_table_and_seed_history(work, stage_current, act_current))
            return -1;
        if (act_loaded == act_current)
            time_reset();
        if (character_current == CHARACTER_E102)
            time_set_whole(3, 0);
    }
    if (character_current == CHARACTER_E102) {
        Uint8 minutes, seconds;
        time_get_whole(&minutes, &seconds);
        if (minutes * 60 + seconds < 60)
            time_set_whole(1, 0);
    }
    restart_save(&work->pos, &work->ang);
    return restored;
}

__declspec(dllexport) int jp_action_placement_probe(void)
{
    int status = 0;
    struct player player = {0};
    struct task *task = task_create(TASK_WORK, 1, NULL);
    if (!task) return 1;
    struct task_work *work = task->work;
    player_work[0] = work;
    player_records[0] = &player;
    restart_clear();
    character_current = CHARACTER_SONIC;
    PLAYER_VARS(work)->character = CHARACTER_SONIC;
    stage_current = STAGE_BEACH;
    act_current = act_loaded = 0;
    time_set(2, 30, 42);
    time_steps = 321;
    if (jp_stage_place_action_player(work) != 0 ||
        work->pos.x != -9 || work->ang.y != 0x6000 || !restart_keep_this_act() ||
        time_minutes || time_seconds || time_frames || time_steps)
        status = 2;
    work->pos.x = 999;
    rings = 30;
    time_set(1, 0, 0);
    if (!status && (jp_stage_place_action_player(work) != 1 ||
                    work->pos.x != -9 || rings != 0 || time_minutes))
        status = 3;
    act_current = 1;
    time_set(1, 2, 3);
    time_steps = 42;
    if (!status && (jp_stage_place_action_player(work) != 0 ||
                    work->pos.x != -986.5F || work->ang.y != 0x4000 ||
                    time_minutes != 1 || time_seconds != 2 || time_frames != 3 ||
                    time_steps != 42))
        status = 4;
    character_current = CHARACTER_E102;
    PLAYER_VARS(work)->character = CHARACTER_E102;
    stage_current = STAGE_SHELTER;
    act_current = act_loaded = 0;
    if (!status && (jp_stage_place_action_player(work) != 0 ||
                    time_minutes != 3 || time_seconds || time_frames))
        status = 5;
    time_set(0, 37, 10);
    restart_save(&work->pos, &work->ang);
    if (!status && (jp_stage_place_action_player(work) != 1 ||
                    time_minutes != 1 || time_seconds || time_frames))
        status = 6;
    time_set(4, 4, 4);
    if (!status && (jp_stage_place_action_player(work) != 1 ||
                    time_minutes != 1 || time_seconds || time_frames))
        status = 7;
    stage_current = STAGE_STATION_SQUARE;
    if (!status && jp_stage_place_action_player(work) != -1)
        status = 8;
    task_destroy_level(0);
    task_destroy(task);
    player_records[0] = NULL;
    player_work[0] = NULL;
    return status;
}
'''


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    base, out = args.baseline.resolve(), args.output.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(manifest['frames_DLL']) == manifest['frames_DLL_sha256']
    hud = pinned(args.upstream, 'src/hud/hud.c')
    stage = pinned(args.upstream, 'src/stage/stage.c')
    assert b'restart_save(&work->pos, &work->ang);' in owner_function(stage, 'stage_place_player')
    out.mkdir()
    source = out/'action_placement.c'
    source.write_bytes(SOURCE+b'\n'+b'\n'.join(owner_function(hud, n)
                                               for n in ('time_get_whole','time_set_whole')))
    dll, obj = out/'jp_native_frames.dll', out/'action_placement.obj'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out/dep.name)
    recipe, driver = manifest['action_checkpoint']['commands']
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p: digest(p) for p in link if p.endswith('.obj')}
    reader = Path(recipe[0]).with_name('llvm-nm.exe')
    symbols = subprocess.check_output([str(reader), '--defined-only', *prior], text=True)
    if not re.search(r'(?m)^[0-9a-fA-F]+ \w act_loaded$', symbols):
        with source.open('ab') as f:
            f.write(b'\n'+re.search(rb'^Sint16 act_loaded;', stage, re.M).group()+b'\n')
    link[link.index('-o'):link.index('-o')] = [str(obj)]
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')]+['-c', str(source), '-o', str(obj)], [driver[0], '@'+str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr, run.stderr.decode(errors='replace')
    assert all(digest(p) == sha for p, sha in prior.items())
    lib = ctypes.CDLL(str(dll))
    lib.heap_init.restype = lib.task_init.restype = None
    lib.jp_action_placement_probe.restype = ctypes.c_int
    lib.heap_init()
    lib.task_init()
    status = lib.jp_action_placement_probe()
    assert status == 0, status
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), action_placement=dict(
        source_commit=PIN, parent_manifest_sha256=digest(parent), object_sha256=digest(obj),
        generated_source_sha256=digest(source), commands=commands, native_probe_status=status))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    print('Action placement verified for fresh starts, act changes, checkpoints and E-102 timing')


if __name__ == '__main__':
    main()
