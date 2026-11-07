"""Link the allocation and registration portion of original player initialization."""
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


def substitute(raw, old, new):
    assert raw.count(old) == 1, old
    return raw.replace(old, new)


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
    player = pinned(args.upstream, 'src/player/player.c')
    target = pinned(args.upstream, 'src/player/target.c')
    progress = pinned(args.upstream, 'src/progress/progress.c')
    constants = re.search(rb'^static struct character_constants character_constants\[.*?^\};',
                          player, re.M|re.S).group()
    init = owner_function(player, 'player_init')
    init = substitute(init, b'void player_init(int player, struct task *task)',
                      b'__declspec(dllexport) int jp_player_init_core(int player, struct task *task)')
    init = substitute(init, b'    struct task_work *work = task->work;',
                      b'''    if (!task || !task->work || !task->motion ||
        player < 0 || player >= PLAYER_COUNT || player_tasks[player] ||
        player_work[player] || player_motion[player] || player_records[player] ||
        task->motion->record || PLAYER_VARS(task->work)->player != player ||
        (unsigned)PLAYER_VARS(task->work)->character >= CHARACTER_COUNT)
        return 1;
    struct task_work *work = task->work;''')
    init = substitute(init, b'    struct task *bubbles;', b'''
    if (!record) return 2;
    struct event_work *event = work->event;
    if (!event) event = heap_calloc(1, sizeof(*event));
    record->contact = heap_calloc(1, sizeof(struct polygon_ball));
    player_targets_alloc(record);
    if (!event || !record->contact || !record->targets) {
        if (!work->event && event) heap_free(event);
        if (record->contact) heap_free(record->contact);
        player_targets_free(record);
        heap_free(record);
        return 2;
    }
    if (!work->event) {
        work->event = event;
        event->flags = EVENT_GROUND | EVENT_SLOPE;
    }''')
    init = substitute(init, b'    event_alloc(work);\n    record->contact = heap_calloc(1, sizeof(struct polygon_ball));\n', b'')
    init = substitute(init, b'    player_targets_alloc(record);\n    work->scl', b'    work->scl')
    init = substitute(init, b'    bubbles = task_create(TASK_WORK, 2, bubbles_start);\n    bubbles->work->scl.z = player;',
                      b'    return 0;')
    out.mkdir()
    source = out/'player_core.c'
    source.write_bytes(b'''#include <ninja.h>
#include "collision/polygon.h"
#include "event/event.h"
#include "object/savepoint.h"
#include "player/player.h"
#include "player/target.h"
#include "progress/progress.h"
#include "stage/stage.h"
#include "system/task.h"
'''+b'\n'.join([
        constants, owner_function(player, 'player_clear_speed'),
        owner_function(target, 'player_targets_alloc'), owner_function(target, 'player_targets_free'),
        owner_function(progress, 'event_flag_check'), init,
    ])+b'''
int jp_stage_place_action_player(struct task_work *);

/* The pre-animation owner gives way to the full character destructor. */
__declspec(dllexport) int jp_player_release_core(int player, struct task *task)
{
    if (!task || player < 0 || player >= PLAYER_COUNT || player_tasks[player] != task)
        return 1;
    struct player *record = player_records[player];
    if (!record || task->motion->record != record || record->anim.blend || record->anim.welds)
        return 1;
    player_tasks[player] = NULL;
    player_work[player] = NULL;
    player_motion[player] = NULL;
    player_records[player] = NULL;
    task->motion->record = NULL;
    heap_free(record->contact);
    player_targets_free(record);
    heap_free(record);
    return 0;
}

__declspec(dllexport) int jp_player_core_probe(void)
{
    int status = 0;
    struct task *task = task_create(TASK_WORK | TASK_MOTION | TASK_FORCE, 1, NULL);
    if (!task) return 1;
    struct task_work *work = task->work;
    PLAYER_VARS(work)->character = CHARACTER_SONIC;
    event_flags[FLAG_SONIC_LIGHT_SHOES] = 1;
    event_flags[FLAG_SONIC_ANCIENT_LIGHT] = 1;
    if (jp_player_init_core(0, task)) {
        status = 2;
        goto finish;
    }
    struct player *record = task->motion->record;
    if (player_tasks[0] != task || player_work[0] != work ||
        player_motion[0] != task->motion || player_records[0] != record ||
        !record->contact || !record->targets || record->target_count ||
        !work->event || work->event->flags != (EVENT_GROUND | EVENT_SLOPE) ||
        record->upgrades != (PLAYER_SONIC_LIGHT_SHOES | PLAYER_SONIC_ANCIENT_LIGHT) ||
        record->anim.state != PLAYER_ANIM_NEW || record->anim.weld_mode != PLAYER_WELD_MAKE ||
        record->anim.speed != &record->speed.x || record->anim.pace != &record->action_speed ||
        task->motion->gravity != 1 || work->scl.x != 1 || work->scl.y != 1 || work->scl.z != 1)
        status = 3;
    if (!status && jp_player_init_core(0, task) != 1) status = 4;
    stage_current = STAGE_BEACH;
    act_current = act_loaded = 0;
    character_current = CHARACTER_SONIC;
    restart_clear();
    if (!status && (jp_stage_place_action_player(work) != 0 ||
                    work->pos.x != -9 || work->ang.y != 0x6000))
        status = 5;
    if (jp_player_release_core(0, task) || task->motion->record ||
        player_tasks[0] || player_work[0] || player_motion[0] || player_records[0])
        status = 6;
finish:
    task_destroy(task);
    return status;
}
''')
    dll, obj = out/'jp_native_frames.dll', out/'player_core.obj'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out/dep.name)
    recipe, driver = manifest['action_placement']['commands']
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p: digest(p) for p in link if p.endswith('.obj')}
    reader = Path(recipe[0]).with_name('llvm-nm.exe')
    symbols = subprocess.check_output([str(reader), '--defined-only', *prior], text=True)
    if not re.search(r'(?m)^[0-9a-fA-F]+ \w player_motion$', symbols):
        with source.open('ab') as f:
            f.write(b'\n'+re.search(rb'^struct task_motion \*player_motion\[PLAYER_COUNT\];',
                                    player, re.M).group()+b'\n')
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
    lib.jp_player_core_probe.restype = ctypes.c_int
    lib.heap_init()
    lib.task_init()
    status = lib.jp_player_core_probe()
    assert status == 0, status
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), player_core=dict(
        source_commit=PIN, parent_manifest_sha256=digest(parent), object_sha256=digest(obj),
        generated_source_sha256=digest(source), commands=commands, native_probe_status=status))
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    print('Native Sonic core allocation, registration, action placement and release verified')


if __name__ == '__main__':
    main()
