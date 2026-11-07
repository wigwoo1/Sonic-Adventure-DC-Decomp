"""Keep the original land callback registered across native draw frames."""
import argparse
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest


OWNER = b'''
static struct {
    struct task *task;
    struct land_table land;
    void (*consume)(NJS_MODEL *);
    Bool drawing;
} land_owner;

static void land_task_draw(struct task *task)
{
    if (task == land_owner.task && land_owner.drawing)
        world_land_draw(task);
}

static void land_task_retire(struct task *task)
{
    if (task == land_owner.task) {
        land_owner.task = NULL;
        land_owner.land = (struct land_table){0};
        land_owner.consume = NULL;
    }
}

__declspec(dllexport) int jp_land_task_open(
    struct land_entry *entries, int count, NJS_TEXLIST *textures, int flags,
    Float clip, void (*consume)(NJS_MODEL *))
{
    if (land_owner.task || land_owner.drawing || land_consume ||
        !entries || count < 0 || count > DRAWN || !consume ||
        (flags & LAND_MOTIONS) || ((flags & (LAND_TEXLIST | LAND_PVM)) && !textures))
        return -1;
    struct task *task = task_create(0, 0, NULL);
    if (!task) return -2;
    land_owner.task = task;
    land_owner.land = (struct land_table){
        .count = (Sint16)count, .flags = (Sint16)flags, .clip = clip,
        .entries = entries, .texlist = textures
    };
    land_owner.consume = consume;
    task->draw = land_task_draw;
    task->destructor = land_task_retire;
    return 0;
}

__declspec(dllexport) int jp_land_task_frame(
    const NJS_POINT3 *position, Angle yaw, Angle pitch, Sint32 blocks, Bool shown, Sint32 step)
{
    if (!land_owner.task || land_owner.drawing || land_consume || !position)
        return -1;
    struct task_work view = {0};
    view.pos = *position;
    view.ang.x = pitch;
    view.ang.y = yaw;
    struct task_work *saved_camera = camera_work;
    struct land_table *saved_land = world_land;
    Sint32 saved_step = frame_step, saved_blocks = world_land_blocks;
    Bool saved_shown = world_land_shown;
    land_owner.drawing = TRUE;
    land_consume = land_owner.consume;
    world_land = &land_owner.land;
    world_land_blocks = blocks;
    world_land_shown = shown;
    world_drawn_count = 0;
    camera_work = &view;
    frame_step = step;
    task_draw_level(0);
    int count = world_drawn_count;
    world_drawn_count = 0;
    world_land = saved_land;
    world_land_blocks = saved_blocks;
    world_land_shown = saved_shown;
    camera_work = saved_camera;
    frame_step = saved_step;
    land_consume = NULL;
    land_owner.drawing = FALSE;
    return count;
}

__declspec(dllexport) int jp_land_task_close(void)
{
    if (land_owner.drawing) return -1;
    if (land_owner.task) task_destroy(land_owner.task);
    return 0;
}
'''


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    workspace,base,out = (p.resolve() for p in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(workspace/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    dispatch = manifest['land_dispatch']
    prior_source = base/'land_dispatch.c'
    assert digest(prior_source)==dispatch['generated_source_sha256']
    assert digest(manifest['frames_DLL'])==manifest['frames_DLL_sha256']
    out.mkdir()
    source = out/'land_task_owner.c'
    source.write_bytes(prior_source.read_bytes()+OWNER)
    dll,obj = out/'jp_native_frames.dll',out/'land_task_owner.obj'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe,driver = dispatch['commands']
    link = shlex.split((base/'link.rsp').read_text())
    old = next(p for p in link if Path(p).name=='land_dispatch.obj')
    assert digest(old)==dispatch['object_sha256']
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
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),land_task_owner=dict(
        parent_manifest_sha256=digest(parent),object_sha256=digest(obj),
        generated_source_sha256=digest(source),commands=commands))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Persistent native land task compiled and linked')


if __name__=='__main__':
    main()
