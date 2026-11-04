"""Real Ninja context/header generation and original title-fade providers.
SDK/game bodies and resulting diagnostic DLL remain private external inputs.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();p=r/'outputs/sonic-adventure';base=a.baseline.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=read(base/'build_verification.json');frontier=read(r/'work/sonic-adventure/JP_NEXT_NATIVE_TITLE_FADE_AFTER_BACKGROUND.json');assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in frontier['source_files_sha256'].items():assert sha(u/n)==h,n
    ninja=(u/'sdk/ninja/njSystem.c').read_bytes();km=(u/'sdk/ninja/kmvertex_kamui_lib.c').read_bytes();title=(u/'src/progress/title.c').read_bytes();d.mkdir();(d/'frames').mkdir()
    provenance={};bodies={}
    for path,raw,names in [('sdk/ninja/njSystem.c',ninja,['njInitContext','njSetCurrentContext','njColorBlendingMode']),('sdk/ninja/kmvertex_kamui_lib.c',km,['kmProcessVertexRenderState','kmiConvertUVnum']),('src/progress/title.c',title,['title_fade_set','title_fade_step','title_fade_draw','title_fade_draw_white'])]:
        for name in names:
            body=function(raw,name);bodies[name]=body;provenance[name]=dict(source=path,source_sha256=hashlib.sha256(raw).hexdigest(),original_body_sha256=hashlib.sha256(body).hexdigest())
    definitions=[];lines=km.splitlines(keepends=True);i=0
    while i<len(lines):
        if lines[i].startswith(b'#define '):
            line=lines[i]
            while lines[i].rstrip().endswith(b'\\'):i+=1;line+=lines[i]
            definitions.append(line)
        i+=1
    init_define=ninja[ninja.index(b'#define INIT_RENDER_STATE'):ninja.index(b'void njInitContext(KMVERTEXCONTEXT *context)\n{')]
    process=bodies['kmProcessVertexRenderState'];cast=b'((KMDWORD)pVertexContext->pTextureSurfaceDesc->pSurface >> 3)';assert process.count(cast)==1
    process=process.replace(cast,b'jp_texture_surface_word((const NJS_TEXSURFACE *)pVertexContext->pTextureSurfaceDesc)')
    process=process.replace(b'kmProcessVertexRenderState(',b'private_process_vertex_context(',1)
    sdk=b'#include "jp_native_context_bridge.h"\n#include "jp_native_textures.h"\n#include "njkamui.h"\n#include <stddef.h>\n#include <string.h>\n'+b''.join(definitions)+b'\nstatic KMDWORD two_volumes=1;\n'+bodies['kmiConvertUVnum']+b'\n'+process+b'''
_Static_assert(sizeof(NJS_TEXSURFACE)==sizeof(KMSURFACEDESC),"shared SDK surface layout");
_Static_assert(offsetof(NJS_TEXSURFACE,nWidth)==offsetof(KMSURFACEDESC,u0),"width layout");
_Static_assert(offsetof(NJS_TEXSURFACE,pSurface)==offsetof(KMSURFACEDESC,pSurface),"surface pointer layout");
KMSTATUS kmProcessVertexRenderState(PKMVERTEXCONTEXT context)
{
    if(!context||jp_context_update_guard(context))return KMSTATUS_INVALID_PARAMETER;
    if((context->RenderState&KM_FILTERMODE)&&(unsigned)context->FilterMode>=4)return KMSTATUS_INVALID_PARAMETER;
    if((context->ShadingMode>>1)==1 && context->pTextureSurfaceDesc && !jp_texture_surface_owned((const NJS_TEXSURFACE *)context->pTextureSurfaceDesc))return KMSTATUS_INVALID_PARAMETER;
    KMVERTEXCONTEXT copy=*context;KMSTATUS status=private_process_vertex_context(&copy);
    if(status==KMSTATUS_SUCCESS)*context=copy;return status;
}
'''+init_define
    for name in ['njInitContext','njColorBlendingMode','njSetCurrentContext']:
        body=bodies[name];guard=b'    if (jp_queue_select_context((unsigned)n)) return;\n' if name=='njSetCurrentContext' else b'    if (jp_context_update_guard(context)) return;\n' if name=='njInitContext' else b'    if (jp_context_update_guard(_nj_current_ctx_)) return;\n'
        body=body.replace(b'{\n',b'{\n'+guard,1);sdk+=b'\n'+body
    whole=function(ninja,'njInitGlobal');start=whole.index(b'    for (i = 0; i < NJD_CONTEXT_MAX; i++)');end=whole.index(b'    njInitSprite();',start);defaults=whole[start:end]
    defaults=defaults.replace(b'_nj_context_',b'contexts').replace(b'_nj_ctx_color_mode_',b'colors').replace(b'&_nj_background_ctx_',b'background').replace(b'_nj_background_ctx_.',b'background->')
    sdk+=b'\nvoid jp_context_make_defaults(KMVERTEXCONTEXT *contexts,Uint32 *colors,KMVERTEXCONTEXT *background)\n{\n    Int i;\n'+defaults+b'}\n'
    sdkpath=d/'external_native_context.c';sdkpath.write_bytes(sdk)
    draw=bodies['title_fade_draw'];old=b'alpha = alpha << 24;';assert draw.count(old)==1;draw=draw.replace(old,b'alpha = (int)((Uint32)alpha << 24);')
    white=bodies['title_fade_draw_white'];old=b'color = alpha << 24 | 0xffffff;';assert white.count(old)==1;white=white.replace(old,b'color = (Uint32)alpha << 24 | 0xffffff;').replace(b'static void title_fade_draw_white(',b'void title_fade_draw_white(',1)
    game=b'#include <ninja.h>\n#include "main.h"\n#define FADE_END (-30)\nstatic int title_fade_level;\n'+bodies['title_fade_set']+bodies['title_fade_step']+draw+white+b'\n__declspec(dllexport) int jp_title_fade_level(void){return title_fade_level;}\n'
    gamepath=d/'external_title_fade.c';gamepath.write_bytes(game)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]+['-ffreestanding','-I'+str(u/'sdk/ninja')];commands=[]
    for path,name in [(p/'native/jp_native_context_bridge.c','context_bridge.obj'),(sdkpath,'context_sdk.obj'),(gamepath,'game_title_fade.obj')]:commands.append(common+['-c',str(path),'-o',str(d/name)])
    commands.append(common+['-S','-emit-llvm',str(sdkpath),'-o',str(d/'external_native_context.ll')])
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']));kept={str(Path(v).resolve()):sha(v) for v in link if v.endswith('.obj')}
    exports=['njInitContext','njSetCurrentContext','njColorBlendingMode','kmProcessVertexRenderState','kmSetVertexRenderState','kmiConvertUVnum','title_fade_set','title_fade_step','title_fade_draw','title_fade_draw_white']
    at=link.index('-o');link[at:at]=[str(d/name) for name in ['context_bridge.obj','context_sdk.obj','game_title_fade.obj']]+['-Wl,/export:'+name for name in exports]+['-Xlinker','/export:_nj_color_mode_,DATA'];link[link.index('-o')+1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,cwd=r,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6500]
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(d/'external_native_context.ll').read_text())
    sources=dict(b['authored_source_sha256'])
    for path in [p/'native/jp_native_context_bridge.h',p/'native/jp_native_context_bridge.c',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    external=dict(b['external_source_sha256'])
    for name in ['sdk/ninja/njSystem.c','sdk/ninja/kmvertex_kamui_lib.c','sdk/ninja/njkamui.h','sdk/include/kamui.h','src/progress/title.c']:external[name]=sha(u/name)
    result=dict(b,status='original_title_fade_Ninja_context_header_generation_and_native_transport_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,authored_source_sha256=sources,external_source_sha256=external,kept_frame_owner_objects_sha256=kept,production_objects_sha256={name:sha(d/name) for name in ['context_bridge.obj','context_sdk.obj','game_title_fade.obj']},frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),original_function_provenance=provenance,private_generated_context_sha256=sha(sdkpath),private_generated_fade_sha256=sha(gamepath),native_texture_address_adaptations=1,original_context_initialization_block_sha256=hashlib.sha256(whole[start:end]).hexdigest(),explicit_unsigned_fade_alpha_shift=True,residual_native_SDK_FP_arithmetic=0,single_context_texture_queue_video_frame_device_owner=True,original_sources_and_proofs_modified=False,game_or_SDK_objects_publishable=False,full_logo_start_or_boot_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
