"""Link the pinned player animation state machine and exercise real Sonic actions."""
import argparse
import ctypes as C
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned
from native_packet_test_support import bind
from rewrite_sprite_float_operations import rewrite
from verify_native_motion_store import View


FREE_BLEND = b'''
static void player_anim_free_blend(struct player_anim *anim)
{
    NJS_MOTION *motion = anim->blend->motion;
    int kinds = motion->inp_fn;
    for (int i = 0; i < anim->actions[anim->action].nodes; i++) {
        void **keys;
        Uint32 *counts;
        if (kinds == 3) {
            NJS_MDATA3 *node = (NJS_MDATA3 *)motion->mdata + i;
            keys = node->p;
            counts = node->nb;
        } else {
            NJS_MDATA2 *node = (NJS_MDATA2 *)motion->mdata + i;
            keys = node->p;
            counts = node->nb;
        }
        for (int k = 0; k < kinds; k++) {
            if (keys[k]) heap_free(keys[k]);
            keys[k] = NULL;
            counts[k] = 0;
        }
    }
}
'''

PROBE = b'''
int jp_player_init_core(int, struct task *);
int jp_player_release_core(int, struct task *);

__declspec(dllexport) int jp_player_animation_probe(NJS_ACTION *stand, NJS_ACTION *walk)
{
    struct player_action actions[10] = {
        [0] = STAND_ROW,
        [9] = WALK_ROW
    };
    struct task *task = task_create(TASK_WORK | TASK_MOTION | TASK_FORCE, 1, NULL);
    if (!task) return 1;
    int status = jp_player_init_core(0, task);
    if (status) {
        task_destroy(task);
        return 2;
    }
    struct player *record = task->motion->record;
    struct player_anim *anim = &record->anim;
    anim->actions = actions;
    player_anim_step(anim);
    if (anim->state != PLAYER_ANIM_CHANGE || !anim->blend) status = 3;
    for (int i = 0; i < 16; i++) player_anim_step(anim);
    if (!status && (anim->state != PLAYER_ANIM_LOOP || anim->current != 0 || anim->frame != 0))
        status = 4;
    player_anim_step(anim);
    if (!status && anim->frame != 0.5F) status = 5;
    anim->action = 9;
    record->action_speed = 1;
    for (int i = 0; i < 4; i++) {
        player_anim_step(anim);
        if (!status && i < 3 && (anim->state != PLAYER_ANIM_BLENDING ||
                                anim->frame != (Float)(i+1)*0.25F))
            status = 6;
    }
    if (!status && (anim->state != PLAYER_ANIM_LOOP_PACE || anim->current != 9 || anim->frame != 0))
        status = 7;
    for (int i = 0; i < 128; i++) {
        player_anim_step(anim);
        if (!status && (anim->frame < 0 || anim->frame >= (Float)walk->motion->nbFrame))
            status = 8;
    }
    anim->state = PLAYER_ANIM_FREE;
    player_anim_step(anim);
    if (anim->blend || jp_player_release_core(0, task)) status = 9;
    task_destroy(task);
    return status;
}
'''


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    base, out = args.baseline.resolve(), args.output.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(manifest['frames_DLL']) == manifest['frames_DLL_sha256']
    player = pinned(args.upstream,'src/player/player.c')
    sonic = pinned(args.upstream,'src/player/sonic.c')
    util = pinned(args.upstream,'src/system/util.c')
    macro = re.search(rb'^#define NARROW_ANGLE\(ang\).*?^    }',player,re.M|re.S).group()
    table = re.search(rb'^static struct player_action sonic_actions\[\] = \{.*?^\};',sonic,re.M|re.S).group()
    probe = PROBE
    for label, variable, index in ((b'STAND',b'stand',0),(b'WALK',b'walk',9)):
        row = re.search(rb'\{&sonic_'+variable+rb'_action,[^}]+\}',table).group()
        row = row.replace(b'&sonic_'+variable+b'_action',variable)
        row = row.replace(b'SONIC_NODES',b'73').replace(b'ACTION_'+label,str(index).encode())
        probe = probe.replace(label+b'_ROW',row)
    funcs = ('player_anim_start','player_anim_play_next','player_anim_make_blend',
             'player_anim_blend_from','player_anim_step')
    out.mkdir()
    source = out/'player_animation.c'
    source.write_bytes(b'''#include <ninja.h>
#include "player/player.h"
#include "system/task.h"
#include "system/util.h"
#include "jp_native_sprite_numeric.h"
'''+b'\n'.join([macro,
        owner_function(util,'angle_unwrap'),owner_function(util,'tree_collect_nodes'),
        owner_function(util,'tree_list_nodes'),FREE_BLEND,
        *(owner_function(player,n) for n in funcs),probe]))
    dll, obj = out/'jp_native_frames.dll', out/'player_animation.obj'
    for dep in base.glob('*.dll'):
        if dep.name != dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe, driver = manifest['player_core']['commands']
    compiler = recipe[:recipe.index('-c')]
    ast = subprocess.run(compiler+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],capture_output=True)
    (out/'ast.stderr').write_bytes(ast.stderr)
    assert not ast.returncode and not ast.stderr,ast.stderr.decode(errors='replace')
    native, stats = rewrite(source.read_bytes(),json.loads(ast.stdout),list(funcs))
    adapted = out/'player_animation_rz.c'
    adapted.write_bytes(native)
    assert driver[1].startswith('@')
    link = shlex.split(Path(driver[1][1:]).read_text())
    prior = {p:digest(p) for p in link if p.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [str(obj),'-Wl,/export:player_anim_step']
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link),encoding='utf-8')
    commands = [compiler+['-c',str(adapted),'-o',str(obj)],[driver[0],'@'+str(rsp)]]
    for phase, command in zip(('compile','link'),commands):
        run = subprocess.run(command,capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr,run.stderr.decode(errors='replace')
    assert all(digest(p)==sha for p,sha in prior.items())
    owner = C.CDLL(str(out/Path(manifest['motion_DLL']).name))
    frames = C.CDLL(str(dll))
    assert bind(owner,'jp_motions_initialize',C.c_int,C.c_size_t,C.c_uint32)(16<<20,2)==0
    views = [View(),View()]
    for name, view in zip((b'sonic_stand_action',b'sonic_walk_action'),views):
        assert bind(owner,'jp_motions_acquire',C.c_int,C.c_uint32,C.c_char_p,C.c_void_p)(
            7,name,C.byref(view))==0 and view.kind==5
    bind(frames,'heap_init',None)()
    bind(frames,'task_init',None)()
    status = bind(frames,'jp_player_animation_probe',C.c_int,C.c_void_p,C.c_void_p)(*(v.data for v in views))
    assert status==0,status
    for view in views:
        assert bind(owner,'jp_motions_release',C.c_int,C.c_uint64)(view.token)==0
    assert bind(owner,'jp_motions_shutdown',C.c_int)()==0
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),player_animation=dict(
        source_commit=PIN,parent_manifest_sha256=digest(parent),native_probe_status=status,
        arithmetic=stats,commands=commands,object_sha256=digest(obj),
        generated_source_sha256=digest(adapted),walk_loop_ticks=128))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Original animation verified for standing, walk blend, 128 walk ticks and cleanup')


if __name__=='__main__':
    main()
