"""Link the original player stick reader and Sonic walking action selector."""
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
from verify_native_input_backend import Sample, Source


PROBE = b'''
__declspec(dllexport) int jp_sonic_input_intent_probe(
    int scenario, Angle *heading, Float *stroke, int *action, Float *pace)
{
    struct task *task = task_create(TASK_WORK | TASK_MOTION | TASK_FORCE, 1, NULL);
    if (!task) return -1;
    if (jp_player_init_core(0, task)) {
        task_destroy(task);
        return -2;
    }
    struct task_work *work = task->work;
    struct player *record = task->motion->record;
    record->speed.x = 0.25F;
    stage_current = STAGE_BEACH;
    act_current = 0;
    if (scenario == 1 || scenario == 2) record->launch_steps = 1;
    if (scenario == 2) work->flags |= PLAYER_ON_PATH;
    if (scenario == 3) {
        stage_current = STAGE_CASINO;
        act_current = 2;
        work->flags |= PLAYER_BALL;
        work->ang.y = 0x5678;
    }
    if (scenario == 4 || scenario == 5) {
        work->flags |= PLAYER_EVENT_DRIVEN;
        work->event->action = scenario == 5;
    }
    int active = player_read_stick(work, heading, stroke);
    if (active && scenario == 0) sonic_set_walk_action(work, task->motion, record);
    *action = record->anim.action;
    *pace = record->action_speed;
    if (jp_player_release_core(0, task)) active = -3;
    task_destroy(task);
    return active;
}
'''


def verify(dll):
    lib = C.CDLL(str(dll))
    samples = (Sample*4)()

    @Source
    def source(_, output):
        C.memmove(output,samples,C.sizeof(samples))
        return 1

    assert bind(lib,'jp_input_initialize',C.c_int,C.c_void_p,C.c_uint32,C.c_void_p,C.c_void_p)(
        None,0,C.cast(source,C.c_void_p),None)==0
    bind(lib,'heap_init',None)()
    bind(lib,'task_init',None)()
    bind(lib,'pad_enable',None)()
    bind(lib,'pad_enable_player',None,C.c_uint8)(255)
    poll = bind(lib,'jp_input_poll',C.c_int)
    scan = bind(lib,'pad_scan',None)
    probe = bind(lib,'jp_sonic_input_intent_probe',C.c_int,C.c_int,
                 C.c_void_p,C.c_void_p,C.c_void_p,C.c_void_p)

    def sample(x, y, enabled=True):
        samples[0] = Sample(1,0,x,y,0,0,0,0)
        assert poll()==0
        bind(lib,'pad_enable' if enabled else 'pad_disable',None)()
        scan()

    def read(scenario):
        heading, stroke, action, pace = C.c_int32(-1), C.c_float(-1), C.c_int32(-1), C.c_float(-1)
        active = probe(scenario,*(C.byref(x) for x in (heading,stroke,action,pace)))
        return active,heading.value,stroke.value,action.value,pace.value

    sample(0,0)
    assert read(0)==(0,-1,-1,0,0)
    sample(100,30)
    active, heading, stroke, action, pace = read(0)
    assert active==1 and 0<=heading<=65535 and 0<stroke<=1 and action==9 and pace==0.25
    assert read(1)==(0,-1,-1,0,0)
    assert read(2)==(1,heading,stroke,0,0)
    assert read(4)==(0,-1,-1,0,0) and read(5)==(1,-1,-1,0,0)
    sample(-100,-30)
    opposite = read(0)
    assert opposite[0]==1 and abs(((heading-opposite[1])&0xffff)-0x8000)<=1
    sample(127,0,enabled=False)
    assert read(0)==(0,-1,-1,0,0)
    assert read(3)==(1,0x5678,0,0,0)
    assert bind(lib,'jp_input_shutdown',C.c_int)()==0
    return dict(positive_axis_heading=heading,negative_axis_heading=opposite[1],
                positive_axis_stroke=stroke,walk_action=action,walk_pace=pace,cases=9)


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
    sonic = pinned(args.upstream,'src/player/sonic.c')
    actions = re.search(rb'^enum sonic_action \{.*?^\};',sonic,re.M|re.S).group()
    out.mkdir()
    source = out/'sonic_input_intent.c'
    source.write_bytes(b'''#include <ninja.h>
#include "collision/world.h"
#include "event/event.h"
#include "player/player.h"
#include "stage/stage.h"
#include "system/pad.h"
#include "system/task.h"
#include "jp_native_sprite_numeric.h"
int jp_player_init_core(int, struct task *);
int jp_player_release_core(int, struct task *);
'''+b'\n'.join([actions,owner_function(player,'player_read_stick'),
               owner_function(sonic,'sonic_set_walk_action'),PROBE]))
    dll, obj = out/'jp_native_frames.dll', out/'sonic_input_intent.obj'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe, driver = manifest['sonic_animation_task']['commands']
    compiler = recipe[:recipe.index('-c')]
    ast = subprocess.run(compiler+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],capture_output=True)
    (out/'ast.stderr').write_bytes(ast.stderr)
    assert not ast.returncode and not ast.stderr,ast.stderr.decode(errors='replace')
    native, stats = rewrite(source.read_bytes(),json.loads(ast.stdout),['sonic_set_walk_action'])
    adapted = out/'sonic_input_intent_rz.c'
    adapted.write_bytes(native)
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p:digest(p) for p in link if p.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [str(obj),'-Wl,/export:player_read_stick']
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
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),sonic_input_intent=dict(
        source_commit=PIN,parent_manifest_sha256=digest(parent),object_sha256=digest(obj),
        generated_source_sha256=digest(adapted),commands=commands,arithmetic=stats,**verification))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print('Native controller scan, original Sonic stick intent and walk selector verified')


if __name__ == '__main__':
    main()
