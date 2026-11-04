"""Compile the entire pinned GCC model producer through existing RZ services.
Generated upstream bodies/objects remain private. Build success is not proof
of whole-producer behavior or a rendered frame. Older qualified files remain.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import adapt,coff_symbols
from rewrite_sprite_float_operations import rewrite,span
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','sh4zam','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();z=a.sh4zam.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pins=[p/'jp_port_tools/jp_task_external_inputs.json',p/'jp_port_tools/jp_renderer_external_inputs.json']
    base,renderer=[json.loads(s.read_text()) for s in pins]
    assert base['upstream_commit']==renderer['game_commit']=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    for n,h in dict(base['files_sha256'],**renderer['additional_game_inputs_sha256']).items():assert sha(u/n)==h,n
    for n,h in renderer['sh4zam_files_sha256'].items():assert sha(z/n)==h,n
    d.mkdir(parents=True)
    raw,changes=adapt((u/'gcc/src/draw/model.c').read_bytes())
    old=b'(const NJS_TEXMEMLIST *)_nj_current_texlist->textures[n].texaddr';assert raw.count(old)==1
    raw=raw.replace(old,b'jp_renderer_texture_lookup(_nj_current_texlist, n)')
    assert raw.count(b'__builtin_sqrtf(')==1;raw=raw.replace(b'__builtin_sqrtf(',b'jp_primitive_sqrt(')
    # Valid loaded records are a caller contract, checked by model preflight.
    # Retain every game function and statement apart from the documented ABI
    # access replacements and typed numerical-boundary substitutions.
    header=(u/'src/draw/model.h').read_bytes();assert header.count(b'#ifndef GCC')==header.count(b'#ifdef GCC')==1
    header=header.replace(b'#ifndef GCC',b'#if !defined(GCC) && !defined(JP_NATIVE_RENDERER)').replace(b'#ifdef GCC',b'#if defined(GCC) || defined(JP_NATIVE_RENDERER)')
    hp=d/'include/draw/model.h';hp.parent.mkdir(parents=True);hp.write_bytes(header)
    initial=d/'external_model_unrewritten.c';initial.write_bytes(raw)
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-fno-strict-aliasing',
        '-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-Wno-unknown-attributes',
        '-include',str(p/'native/jp_host_types.h'),'-include',str(p/'native/jp_renderer_host_compat_rz.h'),'-DJP_NATIVE_RENDERER=1','-I'+str(d/'include')]
    flags+=['-I'+str(u/f) for f in ['sdk/include','src','modules','events','gcc/src','.']]+['-isystem',str(z/'include')]
    ast_cmd=[str(a.compiler),*flags,'-Xclang','-ast-dump=json','-fsyntax-only',str(initial)]
    run=subprocess.run(ast_cmd,capture_output=True);(d/'ast_diagnostics.txt').write_bytes(run.stderr)
    assert run.returncode==0 and not run.stderr,run.stderr.decode(errors='replace')[:5000]
    tree=json.loads(run.stdout);names=[]
    for node in tree['inner']:
        if node.get('kind')!='FunctionDecl' or node.get('loc',{}).get('includedFrom'):continue
        bodies=[v for v in node.get('inner',[]) if v.get('kind')=='CompoundStmt']
        if bodies:
            loc=node['loc'];assert raw[loc['offset']:loc['offset']+loc['tokLen']].decode()==node['name']
            s=span(bodies[0]);assert s and raw[s[0]:s[0]+1]==b'{';names.append(node['name'])
    assert len(names)>=40 and 'draw_model' in names and 'shade_points' in names
    adapted,arithmetic=rewrite(raw,tree,names);source=d/'external_model_native_rz.c';source.write_bytes(adapted)
    obj=d/'model.obj';ir=d/'model.ll'
    cmds=[[str(a.compiler),*flags,'-c',str(source),'-o',str(obj)],[str(a.compiler),*flags,'-S','-emit-llvm',str(source),'-o',str(ir)]]
    results=[]
    for i,cmd in enumerate(cmds):
        run=subprocess.run(cmd,capture_output=True);log=d/f'compile_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    residual=re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text());assert not residual,residual[:15]
    symbols=coff_symbols(obj)
    sources=[Path(__file__),p/'native/jp_renderer_host_compat_rz.h']
    report=dict(status='complete_actual_model_RZ_object_compiled_behavior_unqualified',exit_code=0,diagnostic_bytes=0,commands=results,AST_command=ast_cmd,
        authored_source_sha256={s.resolve().relative_to(r).as_posix():sha(s) for s in sources},compiler_sha256=sha(a.compiler),external_manifests_sha256={s.relative_to(r).as_posix():sha(s) for s in pins},
        reused_adaptor_sha256=sha(p/'jp_port_tools/build_native_renderer_object_v3.py'),reused_float_rewriter_sha256=sha(p/'jp_port_tools/rewrite_sprite_float_operations.py'),
        original_source_sha256=sha(u/'gcc/src/draw/model.c'),adapted_functions=changes,private_generated_source_sha256=sha(source),native_palette_header_sha256=sha(hp),arithmetic_adaptation=arithmetic,
        production_objects_sha256={'model.obj':sha(obj)},LLVM_IR_sha256=sha(ir),residual_native_FP_arithmetic=0,all_source_functions_retained=sorted(names),
        undefined_symbols=sorted(s['name'] for s in symbols if s['section']==0 and s['value']==0),defined_functions=sorted(s['name'] for s in symbols if s['section']>0 and s['function']),
        one_existing_matrix_owner=True,raw_model_entries_require_validated_native_assets_and_complete_reservations=True,original_sources_modified=False,game_source_objects_publishable=False,
        complete_model_behavior_verified=False,full_frame_executed=False,overall_completion_percent=10,playable=False,goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],arithmetic['total_operators_rewritten'],'scalar operators;',len(names),'source functions retained; zero residual native FP arithmetic',flush=True)
if __name__=='__main__':main()
