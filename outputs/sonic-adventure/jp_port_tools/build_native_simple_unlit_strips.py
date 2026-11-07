"""Build the original unlit strip drawers on the native projection/queue owners."""
import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_simple_unlit_clip import derive as clip_source, derive_caller, replace
from rewrite_sprite_float_operations import rewrite


def derive(raw):
    prefix = clip_source(raw)
    a = prefix.index(b'Float *jp_simple_emit_unlit_triangles(')
    b = prefix.index(b'#define CLIP_NEAR', a)
    prefix = prefix[:a] + prefix[b:prefix.index(b'Float *jp_simple_emit_unlit_triangles_clip(')]
    # The strip drawer has already made the original winding decision.
    a = prefix.index(b'    view_facing =')
    b = prefix.index(b'    output_count =', a)
    prefix = prefix[:a] + prefix[b:]
    prefix = replace(prefix, b'    Float view_facing;\n', b'')
    prefix += (b'\n#define DRAW_NOOP() ((void)0)\n#define VERTEX_BYTES 32\n#define VERTEX_SHIFT 5\n'
               b'#define STRIP_COUNT 0x7fff\n#define NJM_SET_POLY_X_STRIP(v,c) EMIT_UNLIT_COLOR(v,c)\n')
    funcs = []
    for name in ('simple_draw_unlit_strips_cull', 'simple_draw_unlit_strips_near'):
        body = owner_function(raw, name)
        body = body.replace(b'register ', b'')
        body = re.sub(rb'/\*.*?\*/|^#pragma (?:asm|endasm)[^\r\n]*|^[ \t]*prefetch\(&vertices[^\r\n]*', b'',
                      body, flags=re.S | re.M)
        body = replace(body, ('static void '+name+'(NJS_MESHSET *meshset)').encode(),
                       ('Float *'+name+'(NJS_MESHSET *meshset,Float *sq_cursor'+
                        (',Float projection_x,Float projection_y' if name.endswith('near') else '')+')').encode())
        body = body.replace(b'(Uint32)run_start < (Uint32)sq_cursor', b'(uintptr_t)run_start < (uintptr_t)sq_cursor')
        body = body.replace(b'(Uint32)sq_cursor - (Uint32)run_start', b'(uintptr_t)sq_cursor - (uintptr_t)run_start')
        if name.endswith('near'):
            body = replace(body, b'                Float view_z;\n', b'')
            body = replace(body, b'DECODE_VIEW_POINT(triangle[corner], view_points[corner],\n                                      view_z);',
                           b'decode_unlit_point(triangle[corner], &view_points[corner], projection_x, projection_y);')
            body = replace(body, b'clip_cull_requested = 0;\n                simple_clip_unlit_triangle(v0, v1, v2);',
                           b'sq_cursor = simple_clip_unlit_triangle(v0,v1,v2,sq_cursor,projection_x,projection_y);')
        funcs.append(body.rstrip()[:-1] + b'    return sq_cursor;\n}\n')
    return prefix + b'\n'.join(funcs)


def caller(raw):
    raw = derive_caller(raw)
    raw = raw.replace(b'jp_simple_emit_unlit_triangles_clip', b'simple_draw_unlit_strips_near')
    raw = raw.replace(b'jp_simple_emit_unlit_triangles', b'simple_draw_unlit_strips_cull')
    raw = replace(raw, b'1u+(projection[0]?4u:3u)*model->meshsets->nbMesh', b'1u+dispatch[5]')
    raw = raw.replace(b'jp_simple_draw_unlit_triangles', b'jp_simple_draw_unlit_strips')
    raw = replace(raw, b'!=NJD_MESHSET_3', b'!=NJD_MESHSET_TRIMESH')
    a = raw.index(b'       !mesh->nbMesh')
    b = raw.index(b'    NJS_LIGHT_MATERIAL', a)
    raw = raw[:a] + b'''       !mesh->nbMesh)return 100;
    size_t pos=0;
    unsigned corners=0,bound=0;
    for(unsigned j=0;j<mesh->nbMesh;j++) {
        if(pos>=indices)return 100;
        unsigned n=(Uint16)mesh->meshes[pos++]&0x7fff;
        if(n<3 || n>indices-pos || n>(unsigned)INT_MAX-corners ||
           4u*(n-2u)>UINT32_MAX/32u-bound)return 100;
        for(unsigned i=0;i<n;i++)
            if((Uint16)mesh->meshes[pos+i]>=(unsigned)model->nbPoint)return 100;
        pos+=n;corners+=n;bound+=4u*(n-2u);
    }
    unsigned draw_limit=bound+corners;
    if(draw_limit>(unsigned)INT_MAX || _nj_calc_polygon_>INT_MAX-(int)corners ||
       _nj_draw_polygon_>INT_MAX-(int)draw_limit)return 100;
''' + raw[b:]
    return replace(raw, b'Uint32 dispatch[5];', b'Uint32 dispatch[6];dispatch[5]=bound;')


def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    args=ap.parse_args()
    root,base,out=(x.resolve() for x in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(root/'work/sonic-adventure')
    b=json.loads((base/'build_verification.json').read_bytes())
    assert digest(b['frames_DLL'])==b['frames_DLL_sha256']
    old=b['simple_unlit_clip']
    source='sdk/ninja/njModelS.c'
    raw=(args.upstream/source).read_bytes()
    assert raw==subprocess.check_output(['git','-C',str(args.upstream),'show',PIN+':'+source])
    wrapper=root/'outputs/sonic-adventure/native/jp_native_simple_unlit.c'
    assert digest(wrapper)==b['simple_unlit']['wrapper_sha256']
    recipe=next(x for x in old['commands'] if '-c' in x and Path(x[-1]).name=='caller.obj')
    sdk=recipe[:recipe.index('-c')]
    out.mkdir()
    def run(cmd,tag):
        p=subprocess.run(cmd,capture_output=True)
        (out/(tag+'.stderr')).write_bytes(p.stderr)
        if p.returncode or p.stderr:
            raise RuntimeError(p.stderr.decode(errors='replace'))
        return p.stdout
    src=out/'strip_original.c'
    src.write_bytes(derive(raw))
    tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(src)],'ast'))
    native,stats=rewrite(src.read_bytes(),tree,['decode_unlit_point','simple_clip_unlit_triangle',
                                              'simple_draw_unlit_strips_cull','simple_draw_unlit_strips_near'])
    rz=out/'strip_rz.c'
    rz.write_bytes(native)
    ir=out/'strip.ll'
    run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
    assert not re.search(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
    bridge=out/'strip_caller.c'
    bridge.write_bytes(caller(wrapper.read_bytes()))
    objects=[]
    for f in (rz,bridge):
        obj=f.with_suffix('.obj')
        run(sdk+['-c',str(f),'-o',str(obj)],f.stem)
        objects.append(obj)
    dll=out/'jp_native_frames.dll'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    link=old['link'].copy()
    link[link.index('-o'):link.index('-o')]=list(map(str,objects))
    link[-1]=str(dll)
    rsp=out/'link.rsp'
    rsp.write_text('\n'.join('"'+x.replace('\\','/')+'"' for x in link[1:])+'\n')
    run([link[0],'@'+str(rsp)],'link')
    b.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),simple_unlit_strips=dict(
        baseline_sha256=digest(base/'build_verification.json'),upstream_commit=PIN,arithmetic=stats,link=link,
        generated_sha256={f.name:digest(f) for f in (src,rz,ir,bridge,*objects)}))
    (out/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n')
    print('Unlit strip drawers compiled and linked',flush=True)


if __name__=='__main__':
    main()
