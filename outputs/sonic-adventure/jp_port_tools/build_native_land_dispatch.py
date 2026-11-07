"""Build the original land draw callback with a bounded model consumer."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned


BRIDGE = b'''
#include "draw/draw.h"
#include "draw/model.h"
extern NJS_MATRIX *_nj_current_matrix_ptr_;
extern Sint32 frame_step;
static Bool world_land_shown;
static Float world_land_material;
static void (*land_consume)(NJS_MODEL *);

static Bool matrix_load_parent(void)
{
    njSetMatrix(NULL, _nj_current_matrix_ptr_ - 1);
    return TRUE;
}

#define draw_model_unculled(model) land_consume(model)
'''

CALL = b'''
#undef draw_model_unculled
__declspec(dllexport) int jp_land_dispatch_probe(
    struct land_entry *entries, int count, NJS_TEXLIST *textures,
    NJS_POINT3 *position, int flags, Float clip, int shown, int step,
    void (*consume)(NJS_MODEL *))
{
    if (!entries || count < 0 || count > DRAWN || !position || !consume ||
        flags & LAND_MOTIONS || ((flags & (LAND_TEXLIST | LAND_PVM)) && !textures) ||
        land_consume)
        return -1;
    struct task *task = task_create(0, 0, NULL);
    if (!task) return -2;
    task->draw = world_land_draw;
    struct task_work camera = {0};
    struct land_table land = {0};
    camera.pos = *position;
    land.entries = entries;
    land.count = (Sint16)count;
    land.flags = (Sint16)flags;
    land.clip = clip;
    land.texlist = textures;
    struct task_work *saved_camera = camera_work;
    Sint32 saved_step = frame_step;
    struct land_table *saved_land = world_land;
    Sint32 saved_blocks = world_land_blocks;
    world_land = &land;
    world_land_blocks = -1;
    world_land_shown = shown;
    camera_work = &camera;
    frame_step = step;
    land_consume = consume;
    world_drawn_count = 0;
    task_draw_level(0);
    int result = world_drawn_count;
    world_drawn_count = 0;
    land_consume = NULL;
    frame_step = saved_step;
    camera_work = saved_camera;
    world_land_blocks = saved_blocks;
    world_land = saved_land;
    task_destroy(task);
    return result;
}
'''


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    workspace,base,out = (p.resolve() for p in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(workspace/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    visibility = manifest['land_visibility']
    prior_source = base/'land_visibility_rz.c'
    assert digest(prior_source)==visibility['generated_source_sha256']
    assert digest(manifest['frames_DLL'])==manifest['frames_DLL_sha256']
    world = pinned(args.upstream,'src/collision/world.c')
    out.mkdir()
    source = out/'land_dispatch.c'
    draw = pinned(args.upstream,'src/draw/draw.c')
    source.write_bytes(prior_source.read_bytes()+BRIDGE+owner_function(draw,'draw_set_material')+b'\n'.join(
        owner_function(world,name) for name in (
            'world_land_draw_visible','world_land_draw_motions','world_land_draw'))+CALL)
    dll,obj = out/'jp_native_frames.dll',out/'land_dispatch.obj'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe,driver = visibility['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name=='land_visibility.obj')
    assert digest(old)==visibility['object_sha256']
    prior = {p:digest(p) for p in link if p.endswith('.obj') and p!=old}
    link[link.index(old)] = str(obj)
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link),encoding='utf-8')
    commands = [recipe[:recipe.index('-c')]+['-c',str(source),'-o',str(obj)],[driver[0],'@'+str(rsp)]]
    for phase,command in zip(('compile','link'),commands):
        run = subprocess.run(command,capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr,run.stderr.decode(errors='replace')
    assert all(digest(p)==sha for p,sha in prior.items())
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),land_dispatch=dict(
        parent_manifest_sha256=digest(parent),object_sha256=digest(obj),
        generated_source_sha256=digest(source),commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Original land draw callback compiled and linked')


if __name__=='__main__':
    main()
