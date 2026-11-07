"""Link original action-stage checkpoint state into the native player build."""
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


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'baseline', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    out, base = args.output.resolve(), args.baseline.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(manifest['frames_DLL']) == manifest['frames_DLL_sha256']
    save = pinned(args.upstream, 'src/object/savepoint.c')
    hud = pinned(args.upstream, 'src/hud/hud.c')
    target = pinned(args.upstream, 'src/player/target.c')
    stage = pinned(args.upstream, 'src/stage/stage.c')
    checkpoint_type = re.search(rb'^struct restart_point \{.*?^\};', save, re.M|re.S).group()
    invincible_steps = re.search(rb'^#define INVINCIBLE_STEPS \d+', target, re.M).group()
    restore = owner_function(save, 'restart_restore')
    assert restore.count(b'ring_add(-ring_count());') == 1
    # For action stages and nonnegative rings, this is the original ring
    # mutation without the unreachable field HUD and extra-life paths.
    restore = restore.replace(b'void restart_restore(', b'static void restore_action_checkpoint(')
    restore = restore.replace(b'ring_add(-ring_count());', b'ring_reset();')
    out.mkdir()
    source = out/'action_checkpoint.c'
    source.write_bytes(b'''#include <ninja.h>
#include "hud/hud.h"
#include "object/savepoint.h"
#include "player/player.h"
#include "player/target.h"
#include "stage/stage.h"
#include "system/task.h"
'''+b'\n'.join([
        checkpoint_type, b'static struct restart_point restart_point;', invincible_steps,
        owner_function(stage, 'stage_get_stage_act'),
        owner_function(hud, 'time_get'), owner_function(hud, 'time_set'),
        owner_function(target, 'player_invincible_step'),
        owner_function(target, 'player_grant_invincibility'),
        *(owner_function(save, n) for n in ('restart_clear', 'restart_keep_this_act', 'restart_save')),
        restore,
    ])+b'''
/* One means restored, zero means no checkpoint for this act, and negative
 * means the caller is outside the action-stage player-zero contract. */
__declspec(dllexport) int jp_action_checkpoint_restore(struct task_work *work)
{
    if (!work || work != player_work[0] || !player_records[0] ||
        stage_current < 0 || stage_current > STAGE_ACTION_LAST || rings < 0)
        return -1;
    if (!restart_keep_this_act()) return 0;
    restore_action_checkpoint(&work->pos, &work->ang);
    player_set_history(0, 0, &work->pos, NULL);
    return 1;
}

int jp_stage_place_table_and_seed_history(struct task_work *, int, int);

__declspec(dllexport) int jp_action_checkpoint_probe(void)
{
    int result = 0;
    struct player record = {0};
    struct task *task = task_create(TASK_WORK, 1, NULL);
    NJS_POINT3 history;
    struct angles saved_angles = {17, 0x6000, -23};
    if (!task) return 1;
    struct task_work *work = task->work;
    player_work[0] = work;
    player_records[0] = &record;
    PLAYER_VARS(work)->character = CHARACTER_SONIC;
    stage_current = STAGE_BEACH;
    act_current = 0;
    if (jp_stage_place_table_and_seed_history(work, stage_current, act_current)) result = 2;
    if (!result && jp_action_checkpoint_restore(work) != 0) result = 3;
    work->ang = saved_angles;
    time_set(3, 27, 41);
    restart_save(&work->pos, &work->ang);
    work->pos.x = 0;
    work->ang.y = 0;
    time_set(0, 0, 0);
    rings = -1;
    if (!result && (jp_action_checkpoint_restore(work) != -1 ||
                    work->pos.x != 0 || time_minutes != 0 || rings != -1)) result = 9;
    rings = 237;
    if (!result && (jp_action_checkpoint_restore(work) != 1 ||
        work->pos.x != -9 || work->pos.y != 16 || work->pos.z != 4 ||
        work->ang.x != 17 || work->ang.y != 0x6000 || work->ang.z != -23 ||
        time_minutes != 3 || time_seconds != 27 || time_frames != 41 || rings != 0 ||
        !player_get_history(0, 0, &history, NULL) || history.x != -9)) result = 4;
    for (int tick = 1; !result && tick <= 121; tick++) {
        task_step_level(0);
        if (!!(record.effects & PLAYER_INVINCIBLE) != (tick <= 120)) result = 5;
    }
    act_current = 1;
    if (!result && (jp_action_checkpoint_restore(work) != 0 || restart_keep_this_act())) result = 6;
    act_current = 0;
    if (!result && jp_action_checkpoint_restore(work) != 0) result = 7;
    if (!result && jp_action_checkpoint_restore(NULL) != -1) result = 8;
    task_destroy_level(0);
    player_records[0] = NULL;
    player_work[0] = NULL;
    task_destroy(task);
    return result;
}
''')
    dll, obj = out/'jp_native_frames.dll', out/'action_checkpoint.obj'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out/dep.name)
    recipe = manifest['player_history']['commands'][0]
    driver = manifest['player_history']['commands'][-1][0]
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p: digest(p) for p in link if p.endswith('.obj')}
    reader = Path(recipe[0]).with_name('llvm-nm.exe')
    symbols = subprocess.check_output([str(reader), '--defined-only', *prior], text=True)
    owns_act = not re.search(r'(?m)^[0-9a-fA-F]+ \w act_current$', symbols)
    if owns_act:
        with source.open('ab') as f:
            f.write(b'\n'+re.search(rb'^Sint16 act_current;', stage, re.M).group()+b'\n')
    exports = ('restart_clear', 'restart_keep_this_act', 'restart_save',
               'stage_get_stage_act', 'time_get', 'time_set', 'player_grant_invincibility')
    link[link.index('-o'):link.index('-o')] = [str(obj), *('-Wl,/export:'+n for n in exports)]
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link), encoding='utf-8')
    commands = [recipe[:recipe.index('-c')]+['-c', str(source), '-o', str(obj)], [driver, '@'+str(rsp)]]
    for phase, command in zip(('compile', 'link'), commands):
        run = subprocess.run(command, capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr, run.stderr.decode(errors='replace')
    assert all(digest(p) == sha for p, sha in prior.items())
    lib = ctypes.CDLL(str(dll))
    lib.heap_init.restype = lib.task_init.restype = None
    lib.jp_action_checkpoint_probe.restype = ctypes.c_int
    lib.heap_init()
    lib.task_init()
    status = lib.jp_action_checkpoint_probe()
    assert status == 0, status
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), action_checkpoint=dict(
        source_commit=PIN, parent_manifest_sha256=digest(parent), object_sha256=digest(obj),
        generated_source_sha256=digest(source), commands=commands, invincibility_steps=120,
        native_checkpoint_probe_status=status, defines_act_current=owns_act,
        unchanged_objects_sha256=prior))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    print('Action checkpoint restores pose, timer and history, clears rings and grants 120 invincible steps')


if __name__ == '__main__':
    main()
