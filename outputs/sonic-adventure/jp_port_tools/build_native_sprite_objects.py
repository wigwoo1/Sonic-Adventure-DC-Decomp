"""Compile the COMPLETE actual GCC sprite family with integer RZ boundaries.

All generated game C/LLVM/objects stay private. Compilation and removal of
native float arithmetic are gates, not evidence of original packet behavior.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
from build_native_texture_runtime import function
from rewrite_sprite_float_operations import rewrite

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

FUNCTIONS=['sprite_begin','sprite_corners','sprite_scale_x','sprite_place','sprite_off_screen','sprite_send_corner','sprite_send_quad','sprite_send_facing_at',
    'sprite_clip_triangle','sprite_clip_near','sprite_view','sprite_project','sprite_send_plane','njInitSprite','sprite_send_facing','sprite_send_2d','njDrawSprite2D','njDrawSprite3D',
    'sprite_set_depth','sprite_get_depth','game_sprite_send_facing','sprite_draw_3d']

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','sh4zam','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();z=a.sh4zam.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pin_path=p/'jp_port_tools/jp_polygon2d_external_inputs.json';pin=json.loads(pin_path.read_text())
    for n,h in pin['files_sha256'].items():assert sha(u/n)==h,n
    sprite_pin_path=p/'jp_port_tools/jp_sprite_external_inputs.json';sprite_pin=json.loads(sprite_pin_path.read_text())
    assert sprite_pin['base_input_manifest_sha256']==sha(pin_path)
    for n,h in sprite_pin['additional_game_files_sha256'].items():assert sha(u/n)==h,n
    zam=json.loads((p/'jp_port_tools/jp_renderer_external_inputs.json').read_text())
    for n,h in zam['sh4zam_files_sha256'].items():assert sha(z/n)==h,n
    d.mkdir(parents=True);changes=[]
    def replace(raw,old,new,why):
        assert raw.count(old)==1,(why,raw.count(old))
        changes.append(dict(reason=why,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()))
        return raw.replace(old,new)
    header=(u/'gcc/sdk/ninja/njsprite.h').read_bytes().replace(b'\r\n',b'\n')
    start=header.index(b'ALWAYS_INLINE struct sprite_vertex *\nsprite_send_corner(');end=header.index(b'\n}\n',start)+3
    send=b'''ALWAYS_INLINE struct sprite_vertex *
sprite_send_corner(struct sprite_vertex *sq, const struct sprite_corner *corner, Uint32 control)
{
    memset(sq,0,sizeof(*sq));sq->control=control;
    sq->x=corner->x;sq->y=corner->y;sq->rhw=corner->rhw;
    sq->u=corner->u;sq->v=corner->v;sq->intensity=1.0F;
    prefetch(sq);return sq+1;
}
'''
    header=replace(header,header[start:end],send,'native 32-byte vertex writer without SH-4 assembly')
    sdk=(u/'gcc/sdk/ninja/njSprite.c').read_bytes().replace(b'\r\n',b'\n')
    start=sdk.index(b'Int (*port_sprite_record)');end=sdk.index(b'/* send frame n on the screen',start)
    sdk=replace(sdk,sdk[start:end],b'', 'reuse qualified picture-settings owner and helpers')
    sdk=replace(sdk,b'#include "njsprite.h"',header,'flatten sprite inline bodies for typed arithmetic adaptation')
    for name,blocks in [('njDrawSprite2D',5),('njDrawSprite3D',9)]:
        old=function(sdk,name);body=old;at=body.index(b'{')+1
        body=body[:at]+f'\n    if (!sp || !sp->tanim || n < 0 || jp_queue_reserve((attr & NJD_SPRITE_ALPHA) ? JP_QUEUE_TRANSLUCENT : JP_QUEUE_OPAQUE,{blocks})) return;\n'.encode()+body[at:]
        body=body.replace(b'    njGetkmPointer();',b'    njGetkmPointer();\n    if (jp_queue_status()) return;')
        if name=='njDrawSprite2D':
            old_send=b'    sq = sprite_send_2d(sp, n, attr, pri, sprite_begin(sp, n, attr));'
            assert body.count(old_send)==1
            body=body.replace(old_send,b'    sq = sprite_begin(sp,n,attr);\n    if (!sq) return;\n    sq = sprite_send_2d(sp,n,attr,pri,sq);')
        else:
            assert body.count(b'    sq = sprite_begin(sp, n, attr);')==1
            body=body.replace(b'    sq = sprite_begin(sp, n, attr);',b'    sq = sprite_begin(sp, n, attr);\n    if (!sq) return;')
            old_end=b'        njEndVertex((Uint32 *)sq);\n    }'
            assert body.count(old_end)==1
            body=body.replace(old_end,b'        njEndVertex((Uint32 *)sq);\n    } else {\n        (void)jp_queue_discard_active();\n    }')
        sdk=replace(sdk,old,body,'bounded native '+name+' entry and culled batch lifetime')
    game=(u/'gcc/src/draw/sprite.c').read_bytes().replace(b'\r\n',b'\n')
    game=replace(game,b'#include "gcc/sdk/ninja/njsprite.h"',b'', 'share flattened sprite helpers once')
    # Both original files use this private helper name for their different y scales.
    assert game.count(b'sprite_send_facing(')==2
    game=game.replace(b'sprite_send_facing(',b'game_sprite_send_facing(')
    old=function(game,'sprite_draw_3d');body=old
    body=replace(body,b'    Uint32 list;',b'    unsigned int list_index;','full-width queue list selector')
    body=replace(body,b'    present_untimed(TRUE);',b'    list_index=(attr & NJD_SPRITE_ALPHA)?JP_QUEUE_TRANSLUCENT:JP_QUEUE_OPAQUE;\n    if (!sprite || !sprite->tanim || entry < 0 || jp_queue_reserve(list_index,9)) return;\n    present_untimed(TRUE);','complete game sprite preflight')
    body=replace(body,b'    njGetkmPointer();',b'    njGetkmPointer();\n    if (jp_queue_status()) { present_untimed(FALSE); return; }','balanced untimed scope on queue error')
    first=body.index(b'    list = (Uint32)');last=body.index(b'    if (attr & NJD_SPRITE_SCALE)',first)
    body=replace(body,body[first:last],b'    sq=(struct sprite_vertex *)((unsigned char *)jp_queue_parameter_cursor(list_index)+32);\n','remove native-pointer truncation, SQ aliases and QACR MMIO')
    game=replace(game,old,body,'complete native game sprite boundary')
    # The old macros are no longer used, and must not reach native objects.
    game=re.sub(rb'^#define (QACR0|QACR1|SQ_AREA|SQ_ADDRESS|SQ_BASE)\b[^\n]*\n',b'',game,flags=re.M)
    prefix=b'''#include <string.h>
#include <machine.h>
#include "jp_native_queue_intensity.h"
#include "jp_native_sprite_numeric.h"
static inline shz_vec3_t jp_sprite_transform(shz_vec3_t p)
{
    float in[3]={p.x,p.y,p.z},out[3];jp_sprite_transform_point(in,out);
    return shz_vec3_init(out[0],out[1],out[2]);
}
'''
    raw=prefix+sdk+b'\n'+game
    raw=raw.replace(b'shz_xmtrx_transform_point3(',b'jp_sprite_transform(').replace(b'_builtin_fsca(',b'jp_sprite_fsca(')
    assert b'__asm__' not in raw and b'0xff000038' not in raw and b'0xff00003c' not in raw
    initial=d/'external_sprites_unrewritten.c';initial.write_bytes(raw)
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-Wno-unknown-attributes',
        '-include',str(p/'native/jp_host_types.h'),'-include',str(p/'native/jp_renderer_host_compat_v2.h'),'-I'+str(p/'native')]
    flags+=['-I'+str(u/f) for f in ['sdk/include','src','modules','events','gcc/src','.']]+['-isystem',str(z/'include')]
    cmd=[str(a.compiler),*flags,'-Xclang','-ast-dump=json','-fsyntax-only',str(initial)]
    result=subprocess.run(cmd,capture_output=True);(d/'ast_diagnostics.txt').write_bytes(result.stderr)
    assert result.returncode==0 and not result.stderr,result.stderr.decode(errors='replace')[:6000]
    tree=json.loads(result.stdout);adapted,arithmetic=rewrite(raw,tree,FUNCTIONS)
    source=d/'external_sprites_native.c';source.write_bytes(adapted)
    obj=d/'sprites.obj';ir=d/'sprites.ll';numeric=p/'native/jp_native_sprite_numeric.c'
    texture_flags=list(flags);compat=texture_flags.index(str(p/'native/jp_renderer_host_compat_v2.h'))
    assert texture_flags[compat-1]=='-include';del texture_flags[compat-1:compat+1]
    commands=[[str(a.compiler),*flags,'-c',str(source),'-o',str(obj)],
        [str(a.compiler),*flags,'-S','-emit-llvm',str(source),'-o',str(ir)],
        [str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-c',str(numeric),'-o',str(d/'sprite_numeric.obj')],
        [str(a.compiler),*texture_flags,'-c',str(p/'native/jp_native_sprite_texture.c'),'-o',str(d/'sprite_texture.obj')],
        [str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(p/'native/jp_native_ninja_wide_state.c'),'-o',str(d/'ninja_wide_state.obj')]]
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,capture_output=True);log=d/f'compile_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    residual=re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text());assert not residual,residual[:20]
    symbols=coff_symbols(obj)
    authored=[Path(__file__),Path(__file__).with_name('rewrite_sprite_float_operations.py'),numeric,p/'native/jp_native_sprite_numeric.h',p/'native/jp_native_sprite_texture.c',p/'native/jp_native_ninja_wide_state.c',sprite_pin_path]
    report=dict(status='complete_actual_sprite_family_objects_compiled_behavior_unqualified',exit_code=0,diagnostic_bytes=0,commands=results,AST_command=cmd,AST_diagnostics_sha256=sha(d/'ast_diagnostics.txt'),
        arithmetic_adaptation=arithmetic,hardware_and_ownership_adaptations=changes,source_pin_sha256=sha(pin_path),original_sources_modified=False,
        private_generated_source_sha256=sha(source),LLVM_IR_sha256=sha(ir),native_scalar_FP_arithmetic_in_sprite_IR=0,
        authored_source_sha256={v.resolve().relative_to(r).as_posix():sha(v) for v in authored},compiler_sha256=sha(a.compiler),
        production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in ['sprites','sprite_numeric','sprite_texture','ninja_wide_state']},
        defined_functions=sorted(v['name'] for v in symbols if v['section']>0 and v['function']),undefined_symbols=sorted(v['name'] for v in symbols if v['section']==0 and v['value']==0),
        all_visible_culled_and_near_clip_source_paths_retained=True,one_existing_picture_owner=True,one_existing_matrix_owner=True,
        numerical_policy='RZ_DN1_value_services_and_qualified_vector_model',source_operators_changed_without_original_packet_proof=True,
        game_source_and_objects_publishable=False,original_packet_behavior_verified=False,full_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],arithmetic['total_operators_rewritten'],'float operators adapted; no residual native FP arithmetic;',len(report['undefined_symbols']),'real imports')

if __name__=='__main__':main()
