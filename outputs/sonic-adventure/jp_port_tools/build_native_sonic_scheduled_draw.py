"""Link a native scheduler fixture against the qualified Sonic callback."""
import argparse
import json
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


SOURCE = r'''
#include <ninja.h>
#include "player/player.h"
#include "system/task.h"
#include "collision/collision.h"
#include "event/event.h"

int jp_sonic_draw_with_assets(struct task *, NJS_TEXLIST *, NJS_TEXLIST *, NJS_OBJECT *);

struct draw_fixture {
    struct player player;
    NJS_TEXLIST *textures;
    NJS_OBJECT *aura;
    int calls;
    int result;
};

static void scheduled_draw(struct task *task)
{
    struct draw_fixture *fixture = (struct draw_fixture *)task->motion->record;
    fixture->calls++;
    fixture->result = jp_sonic_draw_with_assets(task, fixture->textures,
                                              fixture->textures, fixture->aura);
}

__declspec(dllexport) int jp_sonic_scheduled_draw_probe(
    NJS_TEXLIST *textures, NJS_OBJECT *aura, NJS_ACTION *action,
    Float frame, int charged, int hidden)
{
    struct draw_fixture fixture = {0};
    struct event_work event = {0};
    struct collision_work collision = {0};
    struct collision_shape shape = {0};
    struct player_action row = {0};
    struct task *task = task_create(TASK_WORK | TASK_MOTION, 0, NULL);

    if (!task)
        return -1;
    fixture.textures = textures;
    fixture.aura = aura;
    row.action = action;
    fixture.player.anim.actions = &row;
    fixture.player.anim.state = PLAYER_ANIM_LOOP;
    fixture.player.anim.frame = frame;
    collision.shapes = &shape;
    task->work->collision = &collision;
    task->work->event = &event;
    task->work->ang.y = 0x8000;
    task->work->flags = charged ? SONIC_CHARGED : 0;
    task->work->timer = hidden ? 2 : 0; /* original BLINK_HIDDEN */
    task->motion->record = &fixture.player;
    task->draw = scheduled_draw;
    task_draw_level(0);

    /* The fixture owns these borrowed records. Task owns work and motion. */
    task->work->collision = NULL;
    task->work->event = NULL;
    task->motion->record = NULL;
    task_destroy(task);
    return fixture.calls == 1 ? fixture.result : -2;
}
'''


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'baseline', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    root, base, out = (path.resolve() for path in (args.workspace, args.baseline, args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    previous = base/'build_verification.json'
    manifest = json.loads(previous.read_bytes())
    parent = manifest['sonic_draw']
    assert digest(manifest['frames_DLL']) == manifest['frames_DLL_sha256']
    assert digest(base/'sonic_draw.obj') == parent['object_sha256']
    out.mkdir()
    source = out/'sonic_scheduled_draw.c'
    source.write_text(SOURCE, encoding='utf-8')
    obj = out/'sonic_scheduled_draw.obj'
    dll = out/'jp_native_frames.dll'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep, out/dep.name)
    compile_args = parent['commands'][0]
    compile_args = compile_args[:compile_args.index('-c')]
    link = list(parent['link'])
    unchanged = {arg: digest(arg) for arg in link if arg.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [str(obj)]
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link[1:]), encoding='utf-8')
    commands = [compile_args+['-c', str(source), '-o', str(obj)], [link[0], '@'+str(rsp)]]
    for tag, command in zip(('compile', 'link'), commands):
        result = subprocess.run(command, capture_output=True)
        (out/(tag+'.stderr')).write_bytes(result.stderr)
        if result.returncode or result.stderr:
            raise RuntimeError(result.stderr.decode(errors='replace'))
    assert all(digest(path) == sha for path, sha in unchanged.items())
    manifest.update(frames_DLL=str(dll), frames_DLL_sha256=digest(dll), sonic_scheduled_draw={
        'parent_manifest_sha256': digest(previous), 'source_sha256': digest(source),
        'object_sha256': digest(obj), 'unchanged_objects_sha256': unchanged, 'commands': commands})
    (out/'build_verification.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    print('Sonic scheduler fixture compiled and linked')


if __name__ == '__main__':
    main()
