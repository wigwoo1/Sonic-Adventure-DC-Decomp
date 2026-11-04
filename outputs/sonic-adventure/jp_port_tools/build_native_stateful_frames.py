"""Extend the frozen authored frame owner in a fresh generated translation unit.
Exactly one frame owner is linked; original source and proof bytes stay intact.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','upstream','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();base=a.baseline.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((base/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    original=p/'native/jp_native_frames.c';assert sha(original)=='35fba315a8dd65d6c0cbc94524c7eeeb3472c8ad4224aa24fe11d42d2dacd456'
    raw=original.read_text();changes={
        '#include "jp_native_frames_internal.h"':'#include "jp_native_frames_internal.h"\n#include "jp_native_video_state.h"',
        'uint32_t count,*resources;};':'uint32_t count,*resources;struct jp_video_snapshot video;};',
        'if(pending_open || !jp_frame_queue_idle())return fail(JP_FRAME_STATE);':'if(pending_open || !jp_frame_queue_idle())return fail(JP_FRAME_STATE);\n    struct jp_video_snapshot video;if(jp_video_capture(&video))return fail(JP_FRAME_STATE);',
        'f->allocation=allocation;f->data=bytes;f->cost=cost;f->id=next_id++;':'f->video=video;f->allocation=allocation;f->data=bytes;f->cost=cost;f->id=next_id++;'}
    for old,new in changes.items():assert raw.count(old)==1,old;raw=raw.replace(old,new)
    raw+='\nJP_FRAME_API int jp_frame_video(uint64_t id,struct jp_video_snapshot *out)\n{\n    if(!out)return fail(JP_FRAME_ARGUMENT);\n    struct frame *f=find(id);if(!f)return status;\n    if(!guard(f))return fail(JP_FRAME_GUARD);\n    *out=f->video;return JP_FRAME_OK;\n}\n'
    d.mkdir(parents=True);source=d/'authored_stateful_frames.c';source.write_text(raw)
    names=['kmSetAlphaThreshold','kmSetCullingRegister','kmSetColorClampValue','kmSetFogTableColor','kmSetFogVertexColor','kmSetFogTable','kmSetFogDensity',
        'kmSetPaletteMode','kmSetPaletteData','kmSetPaletteBankData','kmSetBorderColor','kmSetBackGroundRenderState','kmSetAutoSortMode','kmSetGlobalClipping','kmSetPixelClipping','kmSetStrideWidth']
    inputs=[a.upstream/'sdk/ninja/kmglobal_kamui_lib.c',a.upstream/'sdk/ninja/kmtex_kamui_lib.c'];bodies={};defines=[]
    assert sha(inputs[0])=='b689f3f6be582f89bcd2e66a1b5475a55f5ba194dd308832137283f392d0c762'
    assert sha(inputs[1])=='62ce4d3ea0d1f11316c534ad53c06ca6103375e3fc58a5265cb8edc65b21d333'
    assert sha(a.upstream/'sdk/include/kamui.h')=='c87ce01634acc830b434a1efc706c2982b2a5bb3c1f5ee1a0a76b3b58998057c'
    for path in inputs:
        text=path.read_text();lines=text.splitlines(keepends=True);i=0
        while i<len(lines):
            if lines[i].startswith('#define '):
                definition=lines[i]
                while lines[i].rstrip().endswith('\\'):i+=1;definition+=lines[i]
                defines.append(definition)
            i+=1
        for name in names:
            match=re.search(r'^KMSTATUS '+name+r'\(',text,re.M)
            if not match:continue
            start=match.start();at=text.index('{',start);depth=1;i=at+1
            # Braces in these pinned function comments are absent; assert
            # exact extracted bodies and retained source hashes in the proof.
            while depth:
                if text[i]=='{':depth+=1
                elif text[i]=='}':depth-=1
                i+=1
            body=text[start:i]
            if name=='kmSetBackGroundRenderState':
                while 'PACK_ARGB(' in body:
                    at=body.index('PACK_ARGB(');end=body.index(')',at);args=[v.strip() for v in body[at+10:end].split(',')];assert len(args)==4
                    # SH-4 shifts preserve the low 32 bits. Make the left
                    # shifts unsigned rather than relying on signed C overflow
                    # for an alpha byte >=128 in the original macro spelling.
                    expression='((KMDWORD)('+ '|'.join('((KMDWORD)(KMINT32)(('+v+')*255.0F)'+('<<'+str(shift) if shift else '')+')' for v,shift in zip(args,[24,16,8,0]))+'))'
                    body=body[:at]+expression+body[end+1:]
            bodies[name]=body
    assert set(bodies)==set(names)
    initial=d/'external_video_setters_unrewritten.c';initial.write_text('#include <kamui.h>\n#include <sg_xpt.h>\n#undef NULL\n#include "jp_native_sprite_numeric.h"\n'+''.join(defines)+'\n'+'\n\n'.join(bodies[n] for n in names)+'\n')
    link=list(next(c['command'] for c in b['commands'] if '-shared' in c['command']));objects=[Path(v) for v in link if v.endswith('.obj')]
    old=[v for v in objects if v.name=='frames.obj'];assert len(old)==1 and sha(old[0])==b['production_objects_sha256']['frames.obj']
    kept=[v for v in objects if v!=old[0]]
    flags=list(b['commands'][0]['command'][1:]);flags=flags[:flags.index('-c')];flags+=['-I'+str(p/'native')]
    sdkflags=[*flags,'-DGCC=1','-Wno-unused-parameter'];astcmd=[str(a.compiler),*sdkflags,'-Xclang','-ast-dump=json','-fsyntax-only',str(initial)]
    ast=subprocess.run(astcmd,capture_output=True);(d/'ast_diagnostics.txt').write_bytes(ast.stderr);assert ast.returncode==0 and not ast.stderr,ast.stderr.decode()[:5000]
    adapted,arithmetic=rewrite(initial.read_bytes(),json.loads(ast.stdout),names);sdk=d/'external_video_setters_rz.c';sdk.write_bytes(adapted)
    commands=[[str(a.compiler),*flags,'-c',str(source),'-o',str(d/'frames.obj')],
        [str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(p/'native/jp_native_video_state.c'),'-o',str(d/'video.obj')],
        [str(a.compiler),*flags,'-c',str(p/'native/jp_native_video_sdk.c'),'-o',str(d/'video_sdk.obj')],
        [str(a.compiler),*sdkflags,'-c',str(sdk),'-o',str(d/'external_video_setters.obj')],
        [str(a.compiler),*sdkflags,'-S','-emit-llvm',str(sdk),'-o',str(d/'external_video_setters.ll')]]
    link=[v for v in link if not v.endswith('.obj')];link[-1]=str(d/'jp_native_frames.dll');at=link.index('-o');link[at:at]=[*map(str,kept),*[str(d/n) for n in ['frames.obj','video.obj','video_sdk.obj','external_video_setters.obj']]]
    link[1:1]=['-Wl,/export:'+n for n in names]+['-Wl,/export:kmiWriteRegister','-Wl,/export:kmiReadRegister'];commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:5000]
    authored=dict(b['authored_source_sha256'])
    for path in [p/'native/jp_native_video_state.c',p/'native/jp_native_video_state.h',p/'native/jp_native_video_sdk.c',p/'jp_port_tools/rewrite_sprite_float_operations.py',Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(d/'external_video_setters.ll').read_text())
    report=dict(status='native_stateful_frames_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),authored_source_sha256=authored,
        baseline_report_sha256=sha(base/'build_verification.json'),generated_authored_source_sha256=sha(source),original_owner_source_sha256=sha(original),exact_owner_adaptations=changes,
        kept_objects_sha256={str(v):sha(v) for v in kept},production_objects_sha256={n:sha(d/n) for n in ['frames.obj','video.obj']},
        replaced_owner_object_sha256=sha(old[0]),DLL=str(d/'jp_native_frames.dll'),DLL_sha256=sha(d/'jp_native_frames.dll'),game_source_in_DLL=True,
        original_SDK_setters_retained=names,external_source_sha256={str(v):sha(v) for v in inputs},SDK_arithmetic_adaptation=arithmetic,residual_native_FP_arithmetic=0,
        original_sources_modified=False,one_frame_queue_texture_matrix_owner=True,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
