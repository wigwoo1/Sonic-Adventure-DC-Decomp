"""Adapt complete real strip/clipped-polygon producers and dependencies.
Generated game C/LLVM/objects stay private. Build success is not packet proof.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def expand_vertex(raw):
    lines=raw.splitlines(keepends=True);out=[];i=0;removed=0
    while i<len(lines):
        if lines[i].startswith(b'#define PUT_VERTEX('):
            removed+=1
            while lines[i].rstrip().endswith(b'\\'):i+=1
            i+=1
        else:out.append(lines[i]);i+=1
    assert removed==1;raw=b''.join(out);result=[];at=0
    while True:
        start=raw.find(b'PUT_VERTEX(',at)
        if start<0:result.append(raw[at:]);break
        level=1;i=start+11;arg_start=i;args=[]
        while level:
            c=raw[i];level+=int(c==40)-int(c==41)
            if c==44 and level==1:args.append(raw[arg_start:i].strip());arg_start=i+1
            if level==0:args.append(raw[arg_start:i].strip())
            i+=1
        assert len(args)==7
        control,x,y,z,u,v,color=args
        body=b'do { sq+=7; '+b' '.join(b'SQ_PUT('+typ+b',('+value+b'));' for typ,value in [(b'Uint32',color),(b'Float',v),(b'Float',u),(b'Float',z),(b'Float',y),(b'Float',x),(b'Uint32',control)])+b' prefetch(sq);sq+=8; } while(0)'
        result.extend([raw[at:start],body]);at=i
    return b''.join(result)

def expand_put(raw):
    raw=re.sub(rb'^#define SQ_PUT\([^\n]*\n',b'',raw,flags=re.M)
    result=[];at=0
    while True:
        start=raw.find(b'SQ_PUT(',at)
        if start<0:result.append(raw[at:]);break
        level=1;i=start+7;comma=None
        while level:
            c=raw[i];level+=int(c==40)-int(c==41)
            if c==44 and level==1 and comma is None:comma=i
            i+=1
        assert comma is not None
        typ=raw[start+7:comma].strip();value=raw[comma+1:i-1].strip()
        assert typ in (b'Float',b'Uint32')
        result.extend([raw[at:start],b'(*('+typ+b' *)--sq = ('+value+b'))']);at=i
    return b''.join(result)

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pin_path=p/'jp_port_tools/jp_primitive_external_inputs.json';pin=json.loads(pin_path.read_text())
    base_path=p/'jp_port_tools/jp_polygon2d_external_inputs.json';base=json.loads(base_path.read_text());assert sha(base_path)==pin['base_input_manifest_sha256']
    for n,h in dict(base['files_sha256'],**pin['files_sha256']).items():assert sha(u/n)==h,n
    d.mkdir(parents=True);changes=[]
    def replace(raw,old,new,why):
        assert raw.count(old)==1,(why,raw.count(old));changes.append(dict(reason=why,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def source(name):return (u/name).read_bytes().replace(b'\r\n',b'\n')
    strip=source('sdk/ninja/njDraw.c');strip=strip.split(b'#ifdef GCC\n',1)[1].split(b'#else\n/* clang-format off */',1)[0]
    for name,textured in [('njDrawPolygon',False),('njDrawTexture',True),('njDrawTextureMemList',True)]:
        old=function(strip,name);at=old.index(b'{')+1;pointer=b'polygon'
        mode=b'flag' if textured else b'trans';words=b'TEXTURE_WORDS' if textured else b'POLYGON_WORDS'
        guard=b'\n    if (!polygon || count < 1 || count > INT32_MAX / '+words+b' || jp_queue_reserve('+mode+b' ? JP_QUEUE_TRANSLUCENT : JP_QUEUE_OPAQUE,(uint32_t)count+1)) return;\n'
        strip=replace(strip,old,old[:at]+guard+old[at:],'bounded native '+name)
    strip=strip.replace(b'    njGetkmPointer();',b'    njGetkmPointer();\n    if (jp_queue_status()) return;')
    # Clear exactly the raw producer's reserved vertex range, not prior packets.
    strip=strip.replace(b'    njEndVertex(send_strip(',b'    if (!sq) return;\n    memset(sq,0,(size_t)count*32);\n    njEndVertex(send_strip(')
    names=['sdk/ninja/njFarClipping.c','sdk/ninja/njNearClipping.c','sdk/ninja/njtri_filled.c','sdk/ninja/njtri_filled_with_tex.c','sdk/ninja/njtri_wiredpoly.c','sdk/ninja/njtri_wiredpoly_with_tex.c']
    chain=b'\n'.join(source(n).replace(b'#ifdef GCC',b'#if 1') for n in names)
    parent=source('sdk/ninja/njDrawPolygon3D.c');old=function(parent,'njDrawPolygon3D');at=old.index(b'{')+1
    guard=b'\n    if (!p || !p->p || n < 3 || n > POINTS_MAX || ((attr & NJD_USE_TEXTURE) ? !p->tex : !p->col)) return;\n'
    body=old[:at]+guard+old[at:]
    # A general polygon can add a crossing for each edge. The old 33-slot
    # local scratch only safely held the intended convex input domain.
    body=body.replace(b'[POINTS_MAX + 1]',b'[2 * POINTS_MAX]')
    # Textured/no-color parent input read uninitialized local colors in the
    # original C. Native defines white for that otherwise-indeterminate case.
    body=replace(body,b'    view.p = view_p;',b'    memset(view_col,0xff,sizeof(view_col));\n    view.p = view_p;','deterministic white only for original uninitialized color case')
    body=replace(body,b'    njGetkmPointer();',b'    if (jp_queue_reserve((attr & NJD_TRANSPARENT)?JP_QUEUE_TRANSLUCENT:JP_QUEUE_OPAQUE,1u+(uint32_t)left*((attr & NJD_FILL)?1u:4u))) return;\n    njGetkmPointer();\n    if (jp_queue_status()) return;','reserve actual complete clipped bound after culling')
    body=replace(body,b'    if ((attr & NJD_FILL) && (attr & NJD_USE_TEXTURE)) {',b'    if (!vertex) return;\n    memset(vertex,0,(size_t)left*((attr & NJD_FILL)?32u:128u));\n    if ((attr & NJD_FILL) && (attr & NJD_USE_TEXTURE)) {','zero undefined native packet words within reserved vertices')
    # Dead scratch-pointer stores have no value-level effect and otherwise
    # trigger warnings in the native compiler. Original source stays intact.
    for line in [b'    NJS_POINT3COL spare;\n',b'    NJS_POINT3 spare_p[POINTS_MAX + 2];\n',b'    NJS_COLOR spare_col[POINTS_MAX + 2];\n',b'    NJS_COLOR spare_tex[POINTS_MAX + 2];\n',b'    spare.p = spare_p;\n',b'    spare.col = spare_col;\n',b'    spare.tex = spare_tex;\n']:
        assert body.count(line)==1;body=body.replace(line,b'')
    parent=replace(parent,old,body,'complete bounded clipped polygon entry')
    prefix=b'#include <ninja.h>\n#include <string.h>\n#include <stdint.h>\n#include "jp_native_queue_intensity.h"\n#include "jp_native_primitive_numeric.h"\n#include "gcc/src/system/port.h"\n'
    raw=expand_put(expand_vertex(prefix+strip+b'\n'+chain+b'\n'+parent)).replace(b'_builtin_sqrtf(',b'jp_primitive_sqrt(')
    # SHC register-allocation hints have no native value-level effect and C
    # forbids taking their address for a compound-assignment helper.
    raw=re.sub(rb'^(\s*)register\s+',rb'\1',raw,flags=re.M)
    assert b'__asm__' not in raw and b'SQ_PUT(' not in raw
    initial=d/'external_primitives_unrewritten.c';initial.write_bytes(raw)
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas',
        '-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u)]
    ast_cmd=[str(a.compiler),*flags,'-Xclang','-ast-dump=json','-fsyntax-only',str(initial)];run=subprocess.run(ast_cmd,capture_output=True);(d/'ast_diagnostics.txt').write_bytes(run.stderr)
    assert run.returncode==0 and not run.stderr,run.stderr.decode(errors='replace')[:5000]
    functions=['send_strip','texture_draw','njDrawPolygon','njDrawTexture','njDrawTextureMemList','njFarClipping','njNearClipping','njtri_filled','njtri_filled_with_tex','njtri_wiredpoly','njtri_wiredpoly_with_tex','njDrawPolygon3D']
    adapted,arithmetic=rewrite(raw,json.loads(run.stdout),functions);generated=d/'external_primitives_native.c';generated.write_bytes(adapted)
    ir=d/'primitives.ll';commands=[[str(a.compiler),*flags,'-c',str(generated),'-o',str(d/'primitives.obj')],[str(a.compiler),*flags,'-S','-emit-llvm',str(generated),'-o',str(ir)]]
    authored={'primitive_textures':p/'native/jp_native_primitive_textures.c','primitive_numeric':p/'native/jp_native_primitive_numeric.c','calc_point':p/'native/jp_ninja_calc_point_rz.c'}
    commands += [[str(a.compiler),*flags,'-c',str(s),'-o',str(d/(n+'.obj'))] for n,s in authored.items()]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'compile_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    residual=re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text());assert not residual,residual[:10]
    syms=coff_symbols(d/'primitives.obj');sources=[*authored.values(),p/'native/jp_native_primitive_numeric.h',Path(__file__),pin_path]
    report=dict(status='complete_native_strip_and_clipped_polygon_objects_compiled_behavior_unqualified',exit_code=0,diagnostic_bytes=0,commands=results,AST_command=ast_cmd,arithmetic_adaptation=arithmetic,adaptations=changes,
        authored_source_sha256={s.resolve().relative_to(r).as_posix():sha(s) for s in sources},reused_float_rewriter_sha256=sha(Path(__file__).with_name('rewrite_sprite_float_operations.py')),compiler_sha256=sha(a.compiler),
        external_pin_sha256=sha(pin_path),private_generated_source_sha256=sha(generated),LLVM_IR_sha256=sha(ir),residual_native_FP_arithmetic=0,
        production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in ['primitives',*authored]},
        undefined_symbols=sorted(s['name'] for s in syms if s['section']==0 and s['value']==0),defined_functions=sorted(s['name'] for s in syms if s['section']>0 and s['function']),
        clipped_scratch_capacity=64,textured_null_color_native_policy='opaque white; original uninitialized local color case is not compared as determinate original behavior',original_sources_modified=False,
        game_source_objects_publishable=False,original_packet_behavior_verified=False,full_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],arithmetic['total_operators_rewritten'],'operators; no native FP arithmetic;',len(report['undefined_symbols']),'imports')

if __name__=='__main__':main()
