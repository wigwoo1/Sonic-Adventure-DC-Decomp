"""Build a native task fixture connecting original input, movement and animation."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    base, out = args.baseline.resolve(), args.output.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    manifest_file = base/'build_verification.json'
    manifest = json.loads(manifest_file.read_bytes())
    assert digest(manifest['frames_DLL'])==manifest['frames_DLL_sha256']
    task_path = Path(manifest['sonic_animation_task']['commands'][0][-3])
    intent_path = Path(manifest['sonic_input_intent']['commands'][0][-3])
    assert digest(task_path)==manifest['sonic_animation_task']['generated_source_sha256']
    assert digest(intent_path)==manifest['sonic_input_intent']['generated_source_sha256']
    task = task_path.read_bytes()
    intent = intent_path.read_bytes()
    actions = intent.split(b'enum sonic_action {',1)[1].split(b'\n};',1)[0]
    selector = owner_function(intent,'sonic_set_walk_action')

    def replace(old,new):
        nonlocal task
        assert task.count(old)==1,old
        task = task.replace(old,new)

    replace(b'#include "collision/collision.h"',b'''#include "collision/collision.h"
#include "collision/world.h"
#include "system/pad.h"
#include "jp_native_sprite_numeric.h"
extern int jp_input_poll(void);
void player_accel_level(struct task_work *, struct task_motion *, struct player *);
void player_note_history(Uint8);
enum sonic_action {'''+actions+b'\n};\n')
    replace(b'static void fixture_step',selector+b'\nstatic void fixture_step')
    replace(b'''    if (++fixture->ticks == 18) {
        player->anim.action = 9;
        player->action_speed = 1.0F;
    }''',b'''    struct task_work *work = task->work;
    struct task_motion *motion = task->motion;
    if (++fixture->ticks == 18) pad_enable();
    if (jp_input_poll()) {
        fixture->result = 6;
        return;
    }
    pad_scan();
    player_accel_level(work, motion, player);
    player_apply_accel(work, motion, player);
    motion->speed.y = 0; /* prepared horizontal floor */
    njAddVector(&work->pos, &motion->speed);
    work->collision->shapes->center = work->pos;
    player_take_speed(work, motion, player);
    player_note_history(0);
    if (player->speed.x > 0) sonic_set_walk_action(work, motion, player);''')
    replace(b'jp_sonic_animation_task_probe',b'jp_sonic_movement_task_probe')
    replace(b'    task->work->flags = charged ? SONIC_CHARGED : 0;',b'''    task->work->flags = PLAYER_ON_GROUND | (charged ? SONIC_CHARGED : 0);
    player->ground_flags = WORLD_NO_SLOPE;
    player->ground_normal.y = player->level = 1.0F;''')
    replace(b'    for (int tick = 0; tick < 24; tick++) task_step_level(1);',b'''    pad_enable_player(0);
    pad_disable();
    for (int tick = 0; tick < 24; tick++) task_step_level(1);
    NJS_POINT3 history;
    if (!player_get_history(0, 0, &history, NULL) ||
        history.x != task->work->pos.x || task->work->pos.x >= 0)
        fixture.result = 7;''')
    replace(b'    if (fixture.ticks != 24 || player->anim.current != 9 ||',
            b'    if (fixture.result || fixture.ticks != 24 || player->anim.current != ACTION_WALK ||')
    replace(b'player->anim.frame <= 10 ||\n        player->anim.frame >= 11',
            b'player->anim.frame <= 0 ||\n        player->anim.frame >= 4')
    out.mkdir()
    source = out/'sonic_movement_task.c'
    source.write_bytes(task)
    dll, obj = out/'jp_native_frames.dll', out/'sonic_movement_task.obj'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe, driver = manifest['sonic_level_movement']['commands']
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p:digest(p) for p in link if p.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [str(obj)]
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link),encoding='utf-8')
    commands = [recipe[:recipe.index('-c')]+['-c',str(source),'-o',str(obj)],[driver[0],'@'+str(rsp)]]
    for phase, command in zip(('compile','link'),commands):
        run = subprocess.run(command,capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr,run.stderr.decode(errors='replace')
    assert all(digest(p)==sha for p,sha in prior.items())
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),sonic_movement_task=dict(
        parent_manifest_sha256=digest(manifest_file),object_sha256=digest(obj),
        generated_source_sha256=digest(source),commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Native controller-to-movement scheduler fixture compiled and linked')


if __name__ == '__main__':
    main()
