"""Build the original unlit textured triangle branch with retained textures."""
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
    prelude = plain(raw).split(b'Float *jp_simple_emit_unlit_triangles(', 1)[0]
    emit = raw.split(b'#define EMIT_UNLIT_TEX_NEXT', 1)[1].split(b'#define', 1)[0]
    # A signed 16-bit UV times 1/256 is exact, with no subnormal results.
    for axis in (b'u', b'v'):
        old = b'(Float)(texcoord)->tex.'+axis+b' * TEX_SCALE'
        emit = replace(emit, old, b'jp_sprite_multiply((Float)(texcoord)->tex.'+axis+b',0x1p-8f)')
    branch = owner_function(raw, 'simple_draw_unlit_tex_polygons')
    branch = branch.split(b'    case NJD_MESHSET_3:', 1)[1].split(b'        break;', 1)[0]
    return (prelude + b'#define EMIT_UNLIT_TEX_NEXT' + emit +
            b'Float *jp_simple_emit_unlit_tex(NJS_MESHSET *meshset,Float *sq_cursor)\n'
            b'{\nSint16 *mesh_cursor=meshset->meshes;\nNJS_COLOR *corner_uv=meshset->vertuv;\n'
            b'struct calc_vertex *v0,*v1,*v2;\nSint32 polygon;Float facing;\n' +
            branch + b'    return sq_cursor;\n}\n')


def bridge(raw):
    raw = raw.replace(b'jp_simple_emit_unlit_triangles', b'jp_simple_emit_unlit_tex')
    raw = raw.replace(b'jp_simple_draw_unlit_triangles', b'jp_simple_draw_unlit_tex_triangles')
    raw = replace(raw, b'size_t indices,size_t materials,', b'size_t indices,size_t materials,size_t uvs,')
    raw = replace(raw, b'    unsigned corners=3u*mesh->nbMesh;',
                  b'    unsigned corners=3u*mesh->nbMesh;\n    if(!mesh->vertuv || corners>uvs)return 100;')
    raw = replace(raw, b'dispatch[4]==4', b'dispatch[4]==2')
    raw = replace(raw, b'    njSetkmModelAttr(dispatch[0]);',
                  b'    if(njSetkmTextureNum(dispatch[2])!=1)return 102;\n    njSetkmModelAttr(dispatch[0]);')
    raw = replace(raw, b'    Float *cursor=lane==JP_QUEUE_TRANSLUCENT?njStartVertexNonTexTrans():njStartVertexNonTexOpaque();',
                  b'    Uint32 *color=(Uint32 *)&_nj_light_material_.diffuse;\n'
                  b'    Float *cursor=lane==JP_QUEUE_TRANSLUCENT?njStartVertexTexTransIntens(color):njStartVertexTexOpaqueIntens(color);')
    return replace(raw,
                   b'    if(dispatch[1]!=NJD_FLAG_IGNORE_LIGHT &&\n       dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_FLAT))return 100;',
                   b'    if(dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_TEXTURE) &&\n'
                   b'       dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_TEXTURE|NJD_FLAG_USE_FLAT))return 100;')


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

    src=out/'tex_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['jp_simple_emit_unlit_tex'])
    rz=out/'tex_rz.c'
    rz.write_bytes(native)
    ir=out/'tex.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    caller=out/'tex_caller.c'
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
    link=b['simple_unlit_strips']['link'].copy()
    link[link.index('-o'):link.index('-o')]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_unlit_tex=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,
        commands=commands,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,caller,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Unlit textured triangle drawer compiled and linked',flush=True)


if __name__=='__main__':
    main()
