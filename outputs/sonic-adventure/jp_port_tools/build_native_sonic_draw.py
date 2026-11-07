"""Link original Sonic player drawing with leased texture and aura views."""
import argparse
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path

from build_native_simple_material import digest
from build_native_read_queue import PIN, owner_function
from build_native_simple_unlit_clip import replace
from rewrite_sprite_float_operations import rewrite


def main():
    parser=argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args()
    root,base,out=(p.resolve() for p in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    manifest=base/'build_verification.json'
    baseline=json.loads(manifest.read_bytes())
    assert digest(baseline['frames_DLL'])==baseline['frames_DLL_sha256']
    recipe=next(x for x in baseline['simple_smooth']['commands']
                if '-c' in x and Path(x[-1]).name=='smooth_caller.obj')
    source=args.upstream/'src/player/sonic.c'
    raw=source.read_bytes()
    assert raw==subprocess.check_output(['git','-C',str(args.upstream),'show',PIN+':src/player/sonic.c'])
    original=owner_function(raw,'sonic_draw')
    body=replace(original,b'static void sonic_draw(struct task *task)',
                 b'__declspec(dllexport) int jp_sonic_draw_with_assets(struct task *task,NJS_TEXLIST *sonic,NJS_TEXLIST *super,NJS_OBJECT *aura)')
    body=replace(body,b'        return;\n',b'        return 0;\n')
    body=replace(body,b'njSetTexture(&super_sonic_texlist);',b'njSetTexture(super);')
    body=replace(body,b'njSetTexture(&sonic_texlist);',b'njSetTexture(sonic);')
    body=replace(body,b'        sonic_blend_toes(work, motion, record);',
                 b'        int toe_status=jp_sonic_toes_step(record);\n'
                 b'        if(toe_status){draw_set_palette(0);draw_attr_restore();return toe_status;}')
    body=replace(body,b'aura_draw(work, motion, record);',b'jp_sonic_aura_draw(aura,record);')
    body=replace(body,b'    draw_attr_restore();\n}',b'    draw_attr_restore();\n    return 0;\n}')
    constants=b'\n'.join(re.search(rb'^#define '+name+rb' [^\r\n]+',raw,re.M).group()
                        for name in (b'BLINK_HIDDEN',b'BALL_STEP_MASK',b'TRAIL_COPIES',b'TRAIL_GAP'))
    actions=re.search(rb'enum sonic_action \{.*?^\};',raw,re.M|re.S).group()
    obj=out/'sonic_draw.obj'
    dll=out/'jp_native_frames.dll'
    out.mkdir()
    unit=out/'sonic_draw_original.c'
    unit.write_bytes(b'#include <ninja.h>\n#include "player/player.h"\n#include "system/task.h"\n'
                     b'#include "system/util.h"\n#include "collision/collision.h"\n#include "event/event.h"\n'
                     b'#include "draw/draw.h"\n#include "draw/model.h"\n#include "jp_native_sprite_numeric.h"\n'
                     b'int jp_sonic_toes_step(void *);\nvoid jp_sonic_aura_draw(NJS_OBJECT *,struct player *);\n'+
                     constants+b'\n'+actions+b'\n'+body+
                     b'''
/* Private fixture supplies the native task layout, not replacement draw logic. */
__declspec(dllexport) int jp_sonic_draw_probe(NJS_TEXLIST *textures,NJS_OBJECT *aura,
                                              NJS_ACTION *action,Float frame,int charged,int hidden)
{
    struct task task={0};
    struct task_work work={0};
    struct task_motion motion={0};
    struct event_work event={0};
    struct collision_work collision={0};
    struct collision_shape shape={0};
    struct player record={0};
    struct player_action row={0};
    task.work=&work;task.motion=&motion;
    work.collision=&collision;work.event=&event;
    work.ang.y=0x8000;
    work.flags=charged?SONIC_CHARGED:0;
    work.timer=hidden?BLINK_HIDDEN:0;
    collision.shapes=&shape;
    motion.record=&record;
    row.action=action;
    record.anim.actions=&row;
    record.anim.state=PLAYER_ANIM_LOOP;
    record.anim.frame=frame;
    return jp_sonic_draw_with_assets(&task,textures,textures,aura);
}
''')
    sdk=recipe[:recipe.index('-c')]
    ast=subprocess.run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(unit)],capture_output=True)
    (out/'ast.stderr').write_bytes(ast.stderr)
    assert ast.returncode==0 and not ast.stderr,ast.stderr.decode(errors='replace')
    native,stats=rewrite(unit.read_bytes(),json.loads(ast.stdout),['jp_sonic_draw_with_assets'])
    rz=out/'sonic_draw_rz.c'
    rz.write_bytes(native)

    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    parent=baseline['sonic_aura']
    assert digest(base/'aura.obj')==parent['object_sha256']
    link=[parent['commands'][-1][0],*shlex.split((base/'link.rsp').read_text())]
    inputs={x:digest(x) for x in link if x.endswith('.obj')}
    link[link.index('-o'):link.index('-o')]=[str(obj)]
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    commands=[sdk+['-c',str(rz),'-o',str(obj)], [link[0],'@'+str(rsp)]]
    for tag,cmd in zip(('compile','link'),commands):
        result=subprocess.run(cmd,capture_output=True)
        (out/(tag+'.stderr')).write_bytes(result.stderr)
        if result.returncode or result.stderr:
            raise RuntimeError(result.stderr.decode(errors='replace'))
    assert all(digest(x)==sha for x,sha in inputs.items())
    baseline.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),sonic_draw=dict(
        source_sha256=digest(source),source_commit=PIN,arithmetic=stats,generated_sha256=digest(rz),object_sha256=digest(obj),parent_sha256=digest(manifest),
        link=link,commands=commands,unchanged_objects_sha256=inputs))
    (out/'build_verification.json').write_text(json.dumps(baseline,indent=2)+'\n')
    print('Original Sonic player draw callback compiled and linked',flush=True)


if __name__=='__main__':
    main()
