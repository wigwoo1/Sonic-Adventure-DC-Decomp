"""Link a scheduler fixture that updates and draws the same native Sonic task."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


SOURCE = b'''
#include <ninja.h>
#include "collision/collision.h"
#include "player/player.h"
#include "system/task.h"

int jp_player_init_core(int, struct task *);
int jp_player_release_core(int, struct task *);
int jp_sonic_draw_with_assets(struct task *, NJS_TEXLIST *, NJS_TEXLIST *, NJS_OBJECT *);

struct animation_fixture {
    NJS_TEXLIST *textures;
    NJS_OBJECT *aura;
    int ticks;
    int draws;
    int result;
};

static void fixture_step(struct task *task)
{
    struct animation_fixture *fixture = task->extra;
    struct player *player = task->motion->record;
    if (++fixture->ticks == 18) {
        player->anim.action = 9;
        player->action_speed = 1.0F;
    }
    player_anim_step(&player->anim);
}

static void fixture_draw(struct task *task)
{
    struct animation_fixture *fixture = task->extra;
    fixture->draws++;
    fixture->result = jp_sonic_draw_with_assets(task, fixture->textures,
                                              fixture->textures, fixture->aura);
}

__declspec(dllexport) int jp_sonic_animation_task_probe(
    NJS_TEXLIST *textures, NJS_OBJECT *aura, NJS_ACTION *stand, NJS_ACTION *walk,
    int charged, int hidden, Float *drawn_frame)
{
    struct player_action actions[10] = {ACTION_ROWS};
    struct animation_fixture fixture = {textures, aura, 0, 0, 0};
    struct collision_work collision = {0};
    struct collision_shape shape = {0};
    struct task *task = task_create(TASK_WORK | TASK_MOTION | TASK_FORCE, 1, fixture_step);
    if (!task) return 1;
    if (jp_player_init_core(0, task)) {
        task_destroy(task);
        return 2;
    }
    struct player *player = task->motion->record;
    task->extra = &fixture;
    task->draw = fixture_draw;
    collision.shapes = &shape;
    task->work->collision = &collision;
    task->work->ang.y = 0x8000;
    task->work->flags = charged ? SONIC_CHARGED : 0;
    task->work->timer = hidden ? 2 : 0;
    player->anim.actions = actions;

    for (int tick = 0; tick < 24; tick++) task_step_level(1);
    if (fixture.ticks != 24 || player->anim.current != 9 ||
        player->anim.state != PLAYER_ANIM_LOOP_PACE || player->anim.frame <= 10 ||
        player->anim.frame >= 11) {
        fixture.result = 3;
    } else {
        *drawn_frame = player->anim.frame;
        task_draw_level(1);
        if (fixture.draws != 1 || player->anim.frame != *drawn_frame) fixture.result = 4;
    }
    player->anim.state = PLAYER_ANIM_FREE;
    player_anim_step(&player->anim);
    task->extra = NULL;
    task->work->collision = NULL;
    if (jp_player_release_core(0, task)) fixture.result = 5;
    task_destroy(task);
    return fixture.result;
}
'''


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    base, out = args.baseline.resolve(), args.output.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    animation = manifest['player_animation']
    assert digest(manifest['frames_DLL']) == manifest['frames_DLL_sha256']
    assert digest(base/'player_animation_rz.c') == animation['generated_source_sha256']
    rows = re.findall(rb'^        \[(?:0|9)\] = \{[^}]+\}',(base/'player_animation_rz.c').read_bytes(),re.M)
    assert len(rows) == 2 and b'{stand,' in rows[0] and b'{walk,' in rows[1]
    out.mkdir()
    source = out/'sonic_animation_task.c'
    source.write_bytes(SOURCE.replace(b'ACTION_ROWS',b',\n'.join(rows)))
    dll, obj = out/'jp_native_frames.dll', out/'sonic_animation_task.obj'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe, driver = animation['commands']
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
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),sonic_animation_task=dict(
        parent_manifest_sha256=digest(parent),object_sha256=digest(obj),
        generated_source_sha256=digest(source),commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Sonic scheduler animation/draw fixture compiled and linked')


if __name__ == '__main__':
    main()
