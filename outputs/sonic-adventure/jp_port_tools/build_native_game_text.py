"""Adapt the complete original game debug-text family to the real print queue.
Generated game source/object and game-containing DLL remain private.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
NAMES=['text_style_init','text_exit','text_set_size','text_set_color','text_draw_char','text_draw_char_front','text_print','text_print_front','text_print_number','text_print_binary','text_print_hex','text_print_float','text_printf']
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.baseline/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    link=list(next(x['command'] for x in b['commands'] if x['command'][-1]==b['frames_DLL']));kept={n:sha(n) for n in link if n.endswith('.obj')}
    assert not any(s['section']>0 and s['name'] in NAMES for n in kept for s in coff_symbols(Path(n)))
    source=u/'src/draw/text.c';raw=source.read_bytes();bodies={n:function(raw,n) for n in NAMES};d.mkdir()
    src=b'''#define GCC
#include <stdarg.h>
#undef GCC
#include "jp_native_print_format.h"
#include "jp_native_print_transport.h"
#include <kamui.h>
#include <string.h>
#define DEBUG_CONTEXT 4
#define TEXT_COLOR 0xffbfbfbf
#define TEXT_SIZE 16.0F
#define VERTEX_WRITTEN 28
#define ENTRY_SIZE 32
extern KMVERTEXCONTEXT _nj_context_[];
extern Int frame_step;
static char text_hex_digits[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static Uint32 text_color,text_tex_param,text_tsp_param,text_global_param,text_isp_param;
static Float text_size;
static Uint8 *text_sq;
#define FONT_COLUMNS 16
#define FONT_CELL (1.0F / FONT_COLUMNS)
#define FONT_INSET (1.0F / 1024)
#define FONT_SPAN (FONT_CELL - FONT_INSET)
#define TEXT_INV_W 0.90909094F
#define FRONT_INV_W 2.0F
#define SQ_PUT(type,value) (*(type *)(text_sq-=4)=(value))
#define prefetch(pointer) ((void)(pointer))
#define TEXT_OPEN() do {text_sq=(Uint8 *)jp_print_open((uint32_t)native_count);if(!text_sq)return;_nj_current_texture=(Uint32)-2;}while(0)
#define TEXT_CLOSE() do {uint32_t h[4]={text_global_param,text_isp_param,text_tsp_param,text_tex_param};jp_print_close((const uint32_t *)text_sq,h);text_sq=NULL;}while(0)
'''
    changes=[]
    def replace(body,old,new,label):
        assert body.count(old)==1,(label,body.count(old));changes.append(dict(label=label,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return body.replace(old,new)
    for name,body in bodies.items():
        if name in ['text_draw_char','text_draw_char_front']:
            body=replace(body,b'{',b'{\n    if(!jp_print_character_room((const uint32_t *)text_sq))return;',name+' bounded glyph',) if body.count(b'{')==1 else body.replace(b'{',b'{\n    if(!jp_print_character_room((const uint32_t *)text_sq))return;',1)
        elif name in ['text_print','text_print_front','text_print_number','text_print_binary','text_print_hex','text_print_float']:
            count={'text_print':b'jp_print_string_count(string)','text_print_front':b'jp_print_string_count(string)','text_print_number':b'jp_print_digit_count(digits,32766)+(value<0)','text_print_binary':b'jp_print_digit_count(digits,32)','text_print_hex':b'jp_print_digit_count(digits,8)','text_print_float':b'jp_print_fixed_arguments(&value,digits)'}[name]
            guard=b'    if(jp_print_digit_count(digits,32766)<0)return;\n' if name=='text_print_number' else b''
            body=replace(body,b'    TEXT_OPEN();',guard+b'    int native_count='+count+b';\n    if(native_count<0)return;\n    TEXT_OPEN();',name+' bounded preflight after frame suppression')
            for axis,expr in [('x',b'(Sint16)(loc >> 16)'),('y',b'(Sint16)loc')]:body=replace(body,axis.encode()+b' = '+expr+b' * text_size;',axis.encode()+b' = jp_print_coordinate('+expr+b',(Uint16)text_size);',name+' '+axis+' RZ grid')
            body=body.replace(b'x += text_size;',b'x = jp_print_advance(x,(Uint16)text_size);')
            if name=='text_print_float':body=replace(body,b'sprintf(buffer, "%0.*f", 5, value);',b'jp_print_format(buffer,sizeof buffer,"%0.*f",5,value);',name+' shared original bounded formatter')
        elif name=='text_printf':body=replace(body,b'vsprintf(buffer, format, args);',b'if(jp_print_vformat(buffer,sizeof buffer,format,args)<0){va_end(args);return;}',name+' bounded native varargs')
        src+=b'\n'+body+b'\n'
    src+=b'\n__declspec(dllexport) void jp_game_text_snapshot(uint32_t out[6]){out[0]=text_color;memcpy(out+1,&text_size,4);out[2]=text_global_param;out[3]=text_isp_param;out[4]=text_tsp_param;out[5]=text_tex_param;}\n'
    generated=d/'external_game_text.c';generated.write_bytes(src)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]+['-fno-strict-aliasing'];obj=d/'game_text.obj';commands=[common+['-c',str(generated),'-o',str(obj)]]
    exports=[n for n in NAMES if not n.startswith('text_draw_char')];link[link.index('-o'):link.index('-o')]=[str(obj)]+['-Wl,/export:'+n for n in exports];link[-1]=str(d/'jp_native_frames.dll');commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    for n,h in kept.items():assert sha(n)==h,n
    result=dict(status='complete_original_game_debug_text_family_linked_behavior_unverified',commands=results,kept_frame_objects_sha256=kept,frames_DLL=str(d/'jp_native_frames.dll'),frames_DLL_sha256=sha(d/'jp_native_frames.dll'),original_source_sha256=sha(source),original_functions_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},adaptations=changes,generated_source_sha256=sha(generated),baseline_build_sha256=sha(a.baseline/'build_verification.json'),builder_sha256=sha(__file__),original_function_count=len(NAMES),shared_print_queue_and_formatter_owners_preserved=True,argument_bounds_preserve_frame_step_suppression=True,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
