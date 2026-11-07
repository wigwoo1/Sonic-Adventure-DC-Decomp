"""Recover the pinned Simple light preparation and smooth color triangle path."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit import derive as plain
from build_native_simple_unlit_clip import replace
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    prelude=plain(raw).split(b'Float *jp_simple_emit_unlit_triangles(',1)[0]
    light=owner_function(raw,'_njSimpleDrawModel')
    light=replace(light,b'void _njSimpleDrawModel(NJS_MODEL *model)',
                  b'__declspec(dllexport) int jp_simple_prepare_base_light(NJS_VECTOR *direction,Float *scale,Float *ambient)')
    light=replace(light,b'    rotation = combined;',
                  b'    if(!base_light || !direction || !scale || !ambient)return 100;\n    rotation = combined;')
    light=light.replace(b'base_light_direction.',b'direction->')
    light=replace(light,b'    base_light_scale = base_light->attr.idif;\n    iamb = base_light->attr.iamb;\n    njSimpleDrawModelSub(model);',
                  b'    *scale = base_light->attr.idif;\n    *ambient = base_light->attr.iamb;\n    return 0;')
    intensity=raw.split(b'#define SIMPLE_INTENSITY(normal)',1)[1].split(b'/*',1)[0]
    intensity=re.sub(rb'\\[ \t]*\r?\n',b'\n',intensity).strip().replace(b'(normal).',b'vertex->normal.')
    emit=raw.split(b'#define EMIT_COLOR_INTENSITY(vertex, command)',1)[1].split(b'#define',1)[0]
    emit=re.sub(rb'\\[ \t]*\r?\n',b'\n',emit).strip()
    branch=owner_function(raw,'simple_draw_smooth_polygons')
    branch=branch.split(b'    case NJD_MESHSET_3:',1)[1].split(b'        break;',1)[0]
    branch,n=re.subn(rb'EMIT_SMOOTH_POLYGON\((v[012]), (KM_VERTEXPARAM_\w+)\);',
                     rb'sq_cursor = jp_simple_emit_smooth_vertex(\1,\2,sq_cursor,iamb);',branch)
    assert n==3
    return (prelude+light+b'\nextern NJS_VECTOR light2;\n'
            b'static Float *jp_simple_emit_smooth_vertex(struct calc_vertex *vertex,Uint32 command,Float *sq_cursor,Float iamb)\n'
            b'{\nvertex->intensity = '+intensity+b';\n'+emit+b'\nreturn sq_cursor;\n}\n'
            b'Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *meshset,Float *sq_cursor,Float iamb)\n'
            b'{\nSint16 *mesh_cursor=meshset->meshes;\n'
            b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;\n'+branch+b'    return sq_cursor;\n}\n')


def bridge(raw):
    raw=raw.replace(b'jp_simple_emit_unlit_triangles',b'jp_simple_emit_smooth_triangles')
    raw=raw.replace(b'jp_simple_draw_unlit_triangles',b'jp_simple_draw_smooth_triangles')
    raw=replace(raw,b'Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *,Float *);',
                b'Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *,Float *,Float);\n'
                b'int jp_simple_prepare_base_light(NJS_VECTOR *,Float *,Float *);\n'
                b'struct smooth_context { Uint32 dispatch[5]; Float ambient; };')
    raw=replace(raw,b'    Uint32 *dispatch=context;',
                b'    struct smooth_context *draw=context;\n    Uint32 *dispatch=draw->dispatch;')
    raw=replace(raw,b'    cursor=jp_simple_emit_smooth_triangles(model->meshsets,cursor);',
                b'    cursor=jp_simple_emit_smooth_triangles(model->meshsets,cursor,draw->ambient);')
    raw=replace(raw,b'    NJS_VECTOR *light,Float scale,int32_t *projection)',
                b'    int32_t *projection)')
    raw=replace(raw,b'    if(!model || !light || !projection || model->nbPoint<0 ||',
                b'    if(!model || !model->normals || !projection || model->nbPoint<0 ||')
    raw=replace(raw,b'    Uint32 dispatch[5];\n    jp_simple_material_stage',
                b'    struct smooth_context context;\n    Uint32 *dispatch=context.dispatch;\n    jp_simple_material_stage')
    raw=replace(raw,
                b'    if(dispatch[1]!=NJD_FLAG_IGNORE_LIGHT &&\n       dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_FLAT))return 100;',
                b'    if(dispatch[1])return 100;\n    NJS_VECTOR light;Float scale;\n'
                b'    if(jp_simple_prepare_base_light(&light,&scale,&context.ambient))return 100;')
    return replace(raw,b'model,points,normals,light,scale,projection,consume_unlit,dispatch',
                   b'model,points,normals,&light,scale,projection,consume_unlit,&context')


def main():
    ap = argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    root,base,out=(x.resolve() for x in (a.workspace,a.baseline,a.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    b=json.loads((base/'build_verification.json').read_bytes())
    assert digest(b['frames_DLL'])==b['frames_DLL_sha256']
    source='sdk/ninja/njModelS.c'
    raw=(a.upstream/source).read_bytes()
    assert raw==subprocess.check_output(['git','-C',str(a.upstream),'show',PIN+':'+source])
    wrapper=root/'outputs/sonic-adventure/native/jp_native_simple_unlit.c'
    assert digest(wrapper)==b['simple_unlit']['wrapper_sha256']
    recipe=next(x for x in b['simple_unlit']['commands'] if '-c' in x and Path(x[-1]).name=='caller.obj')
    sdk=recipe[:recipe.index('-c')]
    out.mkdir()
    commands=[]

    def run(cmd,tag):
        p=subprocess.run(cmd,capture_output=True)
        commands.append(cmd)
        (out/(tag+'.stderr')).write_bytes(p.stderr)
        if p.returncode or p.stderr:
            raise RuntimeError(p.stderr.decode(errors='replace'))
        return p.stdout

    src=out/'smooth_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['jp_simple_prepare_base_light','jp_simple_emit_smooth_vertex','jp_simple_emit_smooth_triangles'])
    rz=out/'smooth_rz.c'
    rz.write_bytes(native)
    ir=out/'smooth.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    caller=out/'smooth_caller.c'
    caller.write_bytes(bridge(wrapper.read_bytes()))
    objects=[]
    for f in (rz,caller):
        obj=f.with_suffix('.obj')
        run(sdk+['-c',str(f),'-o',str(obj)],f.stem)
        objects.append(obj)
    dll=out/'jp_native_frames.dll'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    link=b['simple_unlit_tex_strips']['link'].copy()
    link[link.index('-o'):link.index('-o')]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_smooth=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,
        commands=commands,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,caller,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Smooth-lit triangle drawer compiled and linked',flush=True)


if __name__=='__main__':
    main()
