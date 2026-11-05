"""Link genuine SDK print state, init/exit, character, string/integer callers.
Generated SDK/font objects and runtime DLLs stay private. No formatting stubs.
Float/variadic formatting remain unimplemented pending separate qualification.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','font-input','output']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output
    p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    front=read(r/'work/sonic-adventure/JP_NATIVE_PRINT_FRONTIER_AFTER_RANDOM_DIRECT.json')
    raw=(u/'sdk/ninja/njDebug.c').read_bytes();assert hashlib.sha256(raw).hexdigest()==front['debug_source_sha256']
    font=read(a.font_input/'verification.json');include=a.font_input/'include/ninja/debug/_nj_debug_char_.inc'
    assert sha(include)==font['private_include_sha256'] and font['private_include_roundtrip_exact']
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']))
    objects={n:sha(n) for n in link if n.endswith('.obj')};owners=[n for n in objects if Path(n).name=='queue_frames.obj'];assert len(owners)==1;old=owners[0]
    names=['njInitPrint','njExitPrint','njPrintSize','njPrintColor','njCharacter','njPrintC','njPrintD','njPrintB','njPrintH']
    assert not any(s['section']>0 and s['name'] in names for n in objects for s in coff_symbols(Path(n)))
    d.mkdir();(d/'frames').mkdir()
    src=b'''#include "jp_native_print_transport.h"
#include "jp_native_context_bridge.h"
#include <kamui.h>
#include <sg_xpt.h>
#include <string.h>
#include "njkamui.h"
#define DEBUG_CONTEXT 4
#define FONT_SIZE 128
#define FONT_COLUMNS 16
#define FONT_CELL (1.0F / FONT_COLUMNS)
void njCharacter(Sint16,Sint16,char);
static char print_digits[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static Uint32 print_color;
static Float print_size;
static NJS_TEXSURFACE font;
static Uint32 tex_param,tsp_param,global_param,isp_param;
static Uint32 *print_sq;
#define SQ_PUT(type,value) (*(type *)--print_sq=(value))
#define prefetch(pointer) ((void)(pointer))
#define PRINT_OPEN() do { \\
    print_sq=(Uint32 *)jp_print_open((uint32_t)native_count); \\
    if(!print_sq)return; \\
    _nj_current_texture=(Uint32)-2; \\
} while(0)
#define PRINT_CLOSE() do { \\
    uint32_t native_header[4]={global_param,isp_param,tsp_param,tex_param}; \\
    jp_print_close((const uint32_t *)print_sq,native_header);print_sq=NULL; \\
} while(0)
'''
    provenance={}
    for name in names:
        body=function(raw,name);provenance[name]=hashlib.sha256(body).hexdigest();assert provenance[name]==front['original_functions_sha256'][name]
        if name=='njInitPrint':
            body=body.replace(b'{',b'''{
    if (!jp_print_texture_arguments(texture,size,attr) || jp_context_update_guard(&_nj_context_[DEBUG_CONTEXT])) return;
    NJS_TEXSURFACE old_font=font;
    Float old_size=print_size; Uint32 old_color=print_color;
''',1)
            calls=[(b'kmCreateTextureSurface((PKMSURFACEDESC)&font, FONT_SIZE, FONT_SIZE,\n                               NJD_TEXFMT_TWIDDLED | NJD_TEXFMT_ARGB_1555);',b'jp_print_create_surface(&font,FONT_SIZE,FONT_SIZE,NJD_TEXFMT_TWIDDLED | NJD_TEXFMT_ARGB_1555);'),
                   (b'kmLoadTexture((PKMSURFACEDESC)&font, (PKMDWORD)_nj_debug_char_, FALSE,\n                      FALSE);',b'if (!jp_print_load_surface(&font,_nj_debug_char_)) {font=old_font;print_size=old_size;print_color=old_color;return;}'),
                   (b'kmCreateTextureSurface((PKMSURFACEDESC)&font, size, size, attr);',b'jp_print_create_surface(&font,size,size,attr);'),
                   (b'kmLoadTexture((PKMSURFACEDESC)&font, texture, FALSE, FALSE);',b'if (!jp_print_load_surface(&font,texture)) {font=old_font;print_size=old_size;print_color=old_color;return;}')]
            for before,after in calls:assert body.count(before)==1;body=body.replace(before,after)
        elif name=='njCharacter':
            body=body.replace(b'{',b'{\n    if(!jp_print_character_room((const uint32_t *)print_sq))return;',1)
        elif name in ['njPrintC','njPrintD','njPrintB','njPrintH']:
            count={'njPrintC':b'jp_print_string_count(s)','njPrintD':b'jp_print_digit_count(digit,32766)+(val<0)','njPrintB':b'jp_print_digit_count(digit,32)','njPrintH':b'jp_print_digit_count(digit,8)'}[name]
            # Validate decimal count separately so a negative val cannot turn
            # the rejection -1 into a successful zero-sized allocation.
            guard=b'    if(jp_print_digit_count(digit,32766)<0)return;\n' if name=='njPrintD' else b''
            body=body.replace(b'{',b'{\n'+guard+b'    int native_count='+count+b';\n    if(native_count<0)return;',1)
            for axis,expr in [('x',b'(Sint16)(loc >> 16)'),('y',b'(Sint16)loc')]:
                before=axis.encode()+b' = '+expr+b' * print_size;';assert body.count(before)==1
                body=body.replace(before,axis.encode()+b' = jp_print_coordinate('+expr+b',(Uint16)print_size);')
            body=body.replace(b'x += print_size;',b'x = jp_print_advance(x,(Uint16)print_size);')
        src+=b'\n'+body
    src+=b'\n__declspec(dllexport) const NJS_TEXSURFACE *jp_print_font_surface(void){return &font;}\n'
    sdk=d/'external_native_print.c';sdk.write_bytes(src)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]+['-I'+str(u),'-I'+str(u/'sdk/ninja'),'-I'+str(a.font_input/'include')]
    commands=[]
    for source,name in [(p/'native/jp_native_queue_direct.c','queue_direct'),(p/'native/jp_native_print_transport.c','print_transport'),(sdk,'print_sdk'),(u/'sdk/ninja/debug.c','private_font')]:
        commands.append(common+['-c',str(source),'-o',str(d/(name+'.obj'))])
        if name=='print_transport':commands.append(common+['-S','-emit-llvm',str(source),'-o',str(d/(name+'.ll'))])
    link[link.index(old)]=str(d/'queue_direct.obj');kept={n:h for n,h in objects.items() if n!=old}
    link[link.index('-o'):link.index('-o')]=[str(d/(n+'.obj')) for n in ['print_transport','print_sdk','private_font']]+['-Wl,/export:'+n for n in names+['jp_print_font_surface','jp_print_coordinate','jp_print_advance']]
    link[link.index('-o'):link.index('-o')]=[v for n in ['_nj_context_','_nj_current_ctx_'] for v in ['-Xlinker','/export:'+n+',DATA']]
    link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    newlibs={'frames_DLL':d/'frames/jp_native_frames.lib'}
    for label,name,imports in [('GPU_DLL','jp_native_window_input.dll',['frames_DLL']),('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b[label]))
        for n in imports:
            previous=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(previous)==1;cmd[cmd.index(previous)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(n)==h,n
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp|sitofp|uitofp|fptosi|fptoui)\b',(d/'print_transport.ll').read_text())
    before={s['name'] for s in coff_symbols(Path(old)) if s['section']>0 and s['function']};after={s['name'] for s in coff_symbols(d/'queue_direct.obj') if s['section']>0 and s['function']};assert before<=after
    sources=dict(b['authored_source_sha256'])
    for path in [p/'native/jp_native_queue_direct.c',p/'native/jp_native_queue_direct.h',p/'native/jp_native_print_transport.c',p/'native/jp_native_print_transport.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    generated=dict(b['generated_sources_sha256']);generated[sdk.name]=sha(sdk)
    for n,h in b['generated_sources_sha256'].items():shutil.copyfile(base/n,d/n);assert sha(d/n)==h
    shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='genuine_font_print_state_character_string_and_integer_callers_compiled_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=kept,replaced_queue_owner_object=dict(path=old,sha256=objects[old]),print_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in ['queue_direct','print_transport','print_sdk','private_font']},print_original_function_sha256=provenance,external_debug_source_sha256=sha(u/'sdk/ninja/njDebug.c'),external_font_source_sha256=sha(u/'sdk/ninja/debug.c'),private_font_include_sha256=sha(include),private_font_payload_sha256=font['font_payload_sha256'],previous_random_direct_build_sha256=sha(base/'build_verification.json'),frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),one_shared_queue_texture_frame_owner=True,all_other_frame_objects_preserved=True,frame_owner_bytes_unchanged=False,genuine_print_family_compiled=True,genuine_print_float_and_variadic_compiled=False,font_assets_and_game_containing_DLL_publishable=False,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:shutil.copyfile(d/n,fd/n)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
