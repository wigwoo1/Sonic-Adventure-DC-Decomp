"""Connect local Emerald Coast triangles to the scheduled Sonic movement test."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    workspace, base, out = (p.resolve() for p in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(workspace/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    movement = manifest['sonic_movement_task']
    assert digest(manifest['frames_DLL'])==manifest['frames_DLL_sha256']
    task_source = base/'sonic_movement_task.c'
    assert digest(task_source)==movement['generated_source_sha256']
    raw = task_source.read_bytes()

    def replace(old,new):
        nonlocal raw
        assert raw.count(old)==1,old
        raw = raw.replace(old,new)

    replace(b'struct animation_fixture {',b'''#include "stage/stage.h"
struct jp_stage_triangle;
struct jp_stage_floor { Float height; Uint32 entry, flags; };
int jp_stage_floor_below(const struct jp_stage_triangle *, size_t, Float, Float,
                         Float, struct jp_stage_floor *);
int jp_stage_place_table_and_seed_history(struct task_work *, int, int);

struct animation_fixture {
    const struct jp_stage_triangle *triangles;
    size_t triangle_count;
    struct jp_stage_floor floor;
    NJS_VECTOR origin;''')
    replace(b'''    motion->speed.y = 0; /* prepared horizontal floor */
    njAddVector(&work->pos, &motion->speed);''',b'''    NJS_VECTOR next = work->pos;
    motion->speed.y = 0;
    njAddVector(&next, &motion->speed);
    if (!jp_stage_floor_below(fixture->triangles, fixture->triangle_count,
                              next.x, next.y + 4.0F, next.z, &fixture->floor) ||
        fixture->floor.height < next.y - 4.0F) {
        fixture->result = 8;
        return;
    }
    next.y = fixture->floor.height;
    work->pos = next;''')
    replace(b'    fixture->draws++;',b'''    fixture->draws++;
    NJS_VECTOR rebase = {-fixture->origin.x, -fixture->origin.y, -fixture->origin.z};
    if (!njPushMatrix(NULL)) {
        fixture->result = 9;
        return;
    }
    njTranslateV(NULL, &rebase);''')
    replace(b'fixture->textures, fixture->aura);',
            b'fixture->textures, fixture->aura);\n    njPopMatrix(1);')
    replace(b'jp_sonic_movement_task_probe',b'jp_sonic_emerald_task_probe')
    replace(b'int charged, int hidden, Float *drawn_frame)',b'''int charged, int hidden, Float *drawn_frame, const struct jp_stage_triangle *triangles,
    size_t triangle_count, NJS_POINT3 *position, Uint32 *entry)''')
    replace(b'struct animation_fixture fixture = {textures, aura, 0, 0, 0};',
            b'''struct animation_fixture fixture = {
        .textures = textures, .aura = aura, .triangles = triangles, .triangle_count = triangle_count
    };''')
    replace(b'    pad_enable_player(0);',b'''    struct task_work *work = task->work;
    if (jp_stage_place_table_and_seed_history(work, STAGE_BEACH, 0) ||
        !jp_stage_floor_below(triangles, triangle_count, work->pos.x, work->pos.y,
                              work->pos.z, &fixture.floor)) {
        fixture.result = 10;
        goto finish;
    }
    work->pos.y = fixture.floor.height;
    fixture.origin = shape.center = work->pos;
    work->ang.y = 0x8000; /* align the diagnostic input with the start area */
    pad_enable_player(0);''')
    replace(b'history.x != task->work->pos.x || task->work->pos.x >= 0)',
            b'history.x != work->pos.x || history.y != work->pos.y ||\n        history.z != work->pos.z || work->pos.x >= fixture.origin.x)')
    replace(b'    if (fixture.result || fixture.ticks',b'    if (fixture.result) goto finish;\n    if (fixture.ticks')
    replace(b'        *drawn_frame = player->anim.frame;',b'''        *drawn_frame = player->anim.frame;
        *position = work->pos;
        *entry = fixture.floor.entry;''')
    replace(b'    player->anim.state = PLAYER_ANIM_FREE;',b'''finish:
    if (player->anim.blend) {
        player->anim.state = PLAYER_ANIM_FREE;
        player_anim_step(&player->anim);
    }''')
    replace(b'    player_anim_step(&player->anim);\n    task->extra = NULL;',b'    task->extra = NULL;')
    out.mkdir()
    source = out/'sonic_emerald_task.c'
    source.write_bytes(raw)
    floor = workspace/'outputs/sonic-adventure/native/jp_native_stage_floor.c'
    dll, obj, floor_obj = out/'jp_native_frames.dll', out/'sonic_emerald_task.obj', out/'stage_floor.obj'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe, driver = movement['commands']
    compiler = recipe[:recipe.index('-c')]
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p:digest(p) for p in link if p.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [str(obj),str(floor_obj)]
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link),encoding='utf-8')
    commands = [compiler+['-c',str(source),'-o',str(obj)],
                compiler+['-c',str(floor),'-o',str(floor_obj)],[driver[0],'@'+str(rsp)]]
    for phase, command in zip(('compile','floor','link'),commands):
        run = subprocess.run(command,capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr,run.stderr.decode(errors='replace')
    assert all(digest(p)==sha for p,sha in prior.items())
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),sonic_emerald_task=dict(
        parent_manifest_sha256=digest(parent),object_sha256=digest(obj),
        floor_object_sha256=digest(floor_obj),floor_source_sha256=digest(floor),
        generated_source_sha256=digest(source),commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Native Sonic movement task and Emerald Coast floor query compiled and linked')


if __name__ == '__main__':
    main()
