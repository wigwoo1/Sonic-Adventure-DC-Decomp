"""Link the pinned original aura callback to the native Simple action path."""
import argparse
import json
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
    source=args.upstream/'src/player/effects.c'
    raw=source.read_bytes()
    assert raw==subprocess.check_output(['git','-C',str(args.upstream),'show',PIN+':src/player/effects.c'])
    original=owner_function(raw,'aura_draw')
    body=replace(original,b'void aura_draw(struct task_work *work, struct task_motion *motion,\n               struct player *record)',
                 b'__declspec(dllexport) void jp_sonic_aura_draw(NJS_OBJECT *aura,struct player *record)')
    body=replace(body,b'action.object = &sonic_aura_object;',b'action.object = aura;')
    obj=out/'aura.obj'
    dll=out/'jp_native_frames.dll'
    out.mkdir()
    unit=out/'aura_original.c'
    unit.write_bytes(b'#include <ninja.h>\n#include "player/player.h"\n#include "jp_native_sprite_numeric.h"\n'
                     b'extern int frame_step;\nint play_time_get(void);\n'+body+
                     b'''
/* Private fixture seam builds the actual native player layout. */
__declspec(dllexport) void jp_sonic_aura_probe(NJS_OBJECT *aura,NJS_ACTION *action,Float frame,int blending)
{
    struct player record={0};
    struct player_action row={0};
    row.action=action;
    record.anim.actions=&row;
    record.anim.blend=action;
    record.anim.state=blending?PLAYER_ANIM_BLENDING:PLAYER_ANIM_LOOP;
    record.anim.frame=frame;
    jp_sonic_aura_draw(aura,&record);
}
''')
    sdk=recipe[:recipe.index('-c')]
    sdk_source=root/'outputs/sonic-adventure/native/jp_native_simple_sdk.c'
    assert digest(sdk_source)==baseline['simple_sdk']['source_sha256']
    sdk_unit=out/'aura_sdk_bridge.c'
    sdk_unit.write_bytes(replace(sdk_source.read_bytes(),
        b'        int32_t projection;\n',
        b'        /* The aura bank shares material zero through a non-null pointer,\n'
        b'         * although its model descriptor declares zero materials. */\n'
        b'        NJS_MODEL checked;\n'
        b'        if(!model->nbMat && model->mats && entry->materials==1){\n'
        b'            checked=*model;checked.nbMat=1;model=&checked;\n'
        b'        }\n'
        b'        int32_t projection;\n'))
    sdk_obj=out/'aura_sdk_bridge.obj'
    ast=subprocess.run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(unit)],capture_output=True)
    (out/'ast.stderr').write_bytes(ast.stderr)
    assert ast.returncode==0 and not ast.stderr,ast.stderr.decode(errors='replace')
    native,stats=rewrite(unit.read_bytes(),json.loads(ast.stdout),['jp_sonic_aura_draw'])
    rz=out/'aura_rz.c'
    rz.write_bytes(native)

    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    link=baseline['simple_sdk']['link'].copy()
    inputs={x:digest(x) for x in link if x.endswith('.obj')}
    old=base/'simple_sdk.obj'
    assert digest(old)==baseline['simple_sdk']['object_sha256']
    link[link.index(str(old))]=str(sdk_obj)
    link[link.index('-o'):link.index('-o')]=[str(obj)]
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    commands=[sdk+['-c',str(rz),'-o',str(obj)],sdk+['-c',str(sdk_unit),'-o',str(sdk_obj)],
              [link[0],'@'+str(rsp)]]
    for tag,cmd in zip(('compile','sdk_bridge','link'),commands):
        result=subprocess.run(cmd,capture_output=True)
        (out/(tag+'.stderr')).write_bytes(result.stderr)
        if result.returncode or result.stderr:
            raise RuntimeError(result.stderr.decode(errors='replace'))
    assert all(digest(x)==sha for x,sha in inputs.items())
    baseline.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),sonic_aura=dict(
        source_sha256=digest(source),source_commit=PIN,arithmetic=stats,generated_sha256=digest(rz),object_sha256=digest(obj),parent_sha256=digest(manifest),
        link=link,commands=commands,unchanged_objects_sha256=inputs))
    (out/'build_verification.json').write_text(json.dumps(baseline,indent=2)+'\n')
    print('Original Sonic aura callback compiled and linked',flush=True)


if __name__=='__main__':
    main()
