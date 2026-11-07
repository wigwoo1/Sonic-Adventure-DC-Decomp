"""Link and test the original level-ground acceleration and speed integration."""
import argparse
import ctypes as C
import json
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned
from native_packet_test_support import bind
from rewrite_sprite_float_operations import rewrite
from verify_native_input_backend import Sample, Source


PROBE = b'''
int jp_player_init_core(int, struct task *);
int jp_player_release_core(int, struct task *);

__declspec(dllexport) int jp_level_movement_probe(Angle heading, Float start,
    int ticks, Float *speed, Float *drive, Angle *facing)
{
    struct task *task = task_create(TASK_WORK | TASK_MOTION | TASK_FORCE, 1, NULL);
    if (!task) return 1;
    if (jp_player_init_core(0, task)) {
        task_destroy(task);
        return 2;
    }
    struct task_work *work = task->work;
    struct task_motion *motion = task->motion;
    struct player *player = motion->record;
    work->flags = PLAYER_ON_GROUND;
    work->ang.y = heading;
    player->ground_flags = WORLD_NO_SLOPE;
    player->ground_normal.y = player->level = 1.0F;
    player->speed.x = start;
    for (int i = 0; i < ticks; i++) {
        player_accel_level(work, motion, player);
        player_apply_accel(work, motion, player);
        /* The fixture supplies a horizontal contact constraint. */
        motion->speed.y = 0;
        player_take_speed(work, motion, player);
    }
    *speed = player->speed.x;
    *drive = motion->drive;
    *facing = work->ang.y;
    int status = jp_player_release_core(0, task);
    task_destroy(task);
    return status;
}
'''


def verify(dll):
    lib = C.CDLL(str(dll))
    samples = (Sample*4)()

    @Source
    def source(_, output):
        C.memmove(output, samples, C.sizeof(samples))
        return 1

    I, U, P, F = C.c_int32, C.c_uint32, C.c_void_p, C.c_float
    assert bind(lib,'jp_input_initialize',I,P,U,P,P)(None,0,C.cast(source,P),None)==0
    bind(lib,'heap_init',None)()
    bind(lib,'task_init',None)()
    bind(lib,'pad_enable',None)()
    bind(lib,'pad_enable_player',None,C.c_uint8)(255)
    stack = (F*(16*16))()
    bind(lib,'njInitMatrix',None,P,I,I)(stack,16,0)
    gravity = (F*3).in_dll(lib,'world_gravity')
    gravity[:] = (0,-1,0)
    poll = bind(lib,'jp_input_poll',I)
    scan = bind(lib,'pad_scan',None)
    probe = bind(lib,'jp_level_movement_probe',I,I,F,I,P,P,P)

    def sample(x,y):
        samples[0] = Sample(1,0,x,y,0,0,0,0)
        assert poll()==0
        scan()
        return (I*16).in_dll(lib,'pad_sticks')[0]

    def run(heading,start,ticks):
        speed, drive, facing = F(), F(), I()
        assert probe(heading,start,ticks,C.byref(speed),C.byref(drive),C.byref(facing))==0
        return speed.value,drive.value,facing.value

    heading = sample(100,30)
    first = run(heading,0,1)
    held = run(heading,0,12)
    turn = run((heading+0x4000)&0xffff,0,1)
    assert first[0]>0 and first[1]>0 and first[2]==heading,first
    assert held[0]>first[0] and held[2]==heading,held
    assert turn==(0,0,(heading+0x2000)&0xffff),turn
    sample(0,0)
    coast = run(heading,held[0],1)
    stop = run(heading,held[0],60)
    assert 0<=coast[0]<held[0] and coast[1]==0,coast
    assert stop==(0,0,heading),stop
    assert bind(lib,'jp_input_shutdown',I)()==0
    return dict(first=first,held=held,turn=turn,coast=coast,stop=stop,cases=5)


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    base, out = args.baseline.resolve(), args.output.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(manifest['frames_DLL'])==manifest['frames_DLL_sha256']
    player = pinned(args.upstream,'src/player/player.c')
    util = pinned(args.upstream,'src/system/util.c')
    matrix = pinned(args.upstream,'src/system/matrix_push.c')
    functions = ('player_turn_back','player_turn_to_world','player_turn_toward',
                 'player_turn_on_course','player_turn_at_speed','player_accel_level',
                 'player_limit_speed','player_stop_creep','ground_turn_to_world',
                 'ground_turn_back','player_apply_accel','player_take_speed')
    out.mkdir()
    source = out/'sonic_level_movement.c'
    source.write_bytes(b'''#include <mathf.h>
#include <ninja.h>
#include <string.h>
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include "collision/world.h"
#include "player/player.h"
#include "stage/stage.h"
#include "system/task.h"
#include "system/util.h"
#include "system/vector.h"
#include "jp_native_sprite_numeric.h"
#include "jp_native_primitive_numeric.h"
#define njSqrt jp_primitive_sqrt
#define SPEED_NONE 0.001F
extern NJS_MATRIX *_nj_current_matrix_ptr_;
Float njInnerProduct(NJS_VECTOR *first, NJS_VECTOR *second)
{
    const Float a[4] = {second->x, second->y, second->z, 0};
    const Float b[4] = {first->x, first->y, first->z, 0};
    uint32_t av[4], bv[4], word;
    Float result;
    memcpy(av, a, sizeof av);
    memcpy(bv, b, sizeof bv);
    word = sh4_fipr(av, bv, 1);
    memcpy(&result, &word, sizeof result);
    return result;
}
'''+b'\n'.join([b'#define GCC 1',owner_function(matrix,'matrix_push_unit'),b'#undef GCC',
               owner_function(util,'angle_approach'),owner_function(util,'angle_distance'),
               *(owner_function(player,n).removeprefix(b'static ') if n=='player_accel_level'
                 else owner_function(player,n) for n in functions),PROBE]))
    dll, obj = out/'jp_native_frames.dll', out/'sonic_level_movement.obj'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe, driver = manifest['sonic_input_intent']['commands']
    compiler = recipe[:recipe.index('-c')]+['-Wno-unused-label','-Wno-unused-parameter']
    ast = subprocess.run(compiler+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],capture_output=True)
    (out/'ast.stderr').write_bytes(ast.stderr)
    assert not ast.returncode and not ast.stderr,ast.stderr.decode(errors='replace')
    native, stats = rewrite(source.read_bytes(),json.loads(ast.stdout),list(functions))
    adapted = out/'sonic_level_movement_rz.c'
    adapted.write_bytes(native)
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p:digest(p) for p in link if p.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [
        str(obj), '-Wl,/export:player_accel_level', '-Wl,/export:player_apply_accel',
        '-Wl,/export:player_take_speed']
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link),encoding='utf-8')
    commands = [compiler+['-c',str(adapted),'-o',str(obj)],[driver[0],'@'+str(rsp)]]
    for phase, command in zip(('compile','link'),commands):
        run = subprocess.run(command,capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr,run.stderr.decode(errors='replace')
    assert all(digest(p)==sha for p,sha in prior.items())
    verification = verify(dll)
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),sonic_level_movement=dict(
        source_commit=PIN,parent_manifest_sha256=digest(parent),object_sha256=digest(obj),
        generated_source_sha256=digest(adapted),commands=commands,arithmetic=stats,**verification))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Original level movement verified for acceleration, turning, coasting and stopping')


if __name__ == '__main__':
    main()
