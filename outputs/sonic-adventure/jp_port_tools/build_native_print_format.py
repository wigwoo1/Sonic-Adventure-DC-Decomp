"""Reuse complete external SHC formatter/decimal helpers with bounded native I/O.
Replace only the previous private print SDK object; preserve its shared state.
External runtime objects, genuine print functions and font stay private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
NAMES=['_fmtout','_xtostr','_xti','_its','_unpack','_log10','_duchek','_allzero','_pow5','_calcnpw','_power','_pow10','_mult64','_mult','_div64','_sub','_umemcmp','_rsfts','_rsft','_lsfts','_lsft','_setsbit','_rnd','_add','_mvito4b','_mvsto2b','_ctype']
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']));objects={n:sha(n) for n in link if n.endswith('.obj')};old=str(base/'print_sdk.obj');assert old in objects
    d.mkdir();(d/'frames').mkdir();(d/'runtime').mkdir();raw=(u/'sdk/ninja/njDebug.c').read_bytes();assert sha(u/'sdk/ninja/njDebug.c')==b['external_debug_source_sha256'];src=(base/'external_native_print.c').read_bytes()
    src=b'#define GCC\n#include <stdarg.h>\n#undef GCC\n#include "jp_native_print_format.h"\n'+src
    provenance={}
    for name in ['njPrintF','njPrint']:
        body=function(raw,name);provenance[name]=hashlib.sha256(body).hexdigest()
        if name=='njPrintF':
            body=body.replace(b'{',b'{\n    int native_count=jp_print_fixed_arguments(&val,digit);\n    if(native_count<0)return;',1)
            for axis,expr in [('x',b'(Sint16)(loc >> 16)'),('y',b'(Sint16)loc')]:
                before=axis.encode()+b' = '+expr+b' * print_size;';assert body.count(before)==1;body=body.replace(before,axis.encode()+b' = jp_print_coordinate('+expr+b',(Uint16)print_size);')
            body=body.replace(b'x += print_size;',b'x = jp_print_advance(x,(Uint16)print_size);')
            before=b'sprintf(buf, "%0.*f", 5, val);';assert body.count(before)==1;body=body.replace(before,b'jp_print_format(buf,sizeof buf,"%0.*f",5,val);')
        else:
            before=b'vsprintf(buf, fmt, args);';assert body.count(before)==1;body=body.replace(before,b'if(jp_print_vformat(buf,sizeof buf,fmt,args)<0){va_end(args);return;}')
        src+=b'\n'+body
    sdk=d/'external_native_print_format.c';sdk.write_bytes(src)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')];common+=['-fno-strict-aliasing','-I'+str(u/'sdk')]
    namespace=['-D'+n+'=jp_sdk_'+n.lstrip('_') for n in NAMES]+['-D_errno=jp_sdk_errno','-DGCC','-include',str(p/'native/jp_native_print_format.h')]
    commands=[];generated={};external={};runtime=[];changes=[]
    def change(text,before,after,label):
        assert text.count(before)==1,(label,text.count(before));changes.append(dict(label=label,before_sha256=hashlib.sha256(before).hexdigest(),after_sha256=hashlib.sha256(after).hexdigest()));return text.replace(before,after)
    for name in NAMES:
        original=u/'sdk/runtime'/(name+'.c');text=original.read_bytes();external['sdk/runtime/'+name+'.c']=sha(original)
        if name=='_fmtout':
            text=change(text,b'#include <machine.h>',b'',name+' host-free FPSCR interface')
            text=change(text,b'    int fpscr;\n',b'',name+' value-only mode')
            text=change(text,b'                fpscr = get_fpscr();\n                fpscr = fpscr & ~FPSCR_PR;\n                set_fpscr(fpscr);',b'',name+' no synthetic FPSCR writes')
            text=change(text,b'            mode = (int)put;',b'            mode = (int)(uintptr_t)put;',name+' native pointer cast')
            text=change(text,b'long value = (long)va_arg(arguments, void *);',b'long value = (long)(uintptr_t)va_arg(arguments, void *);',name+' original 32-bit pointer display')
            text=change(text,b'if (parts->width > 512)',b'if (parts->width < 0 || parts->width > 512)',name+' INT_MIN star width')
            text=change(text,b'            format++;\n        } else {',b'            if (*format) format++; else break;\n        } else {',name+' truncated directive bounds')
            emit=function(text,'emit_text');before=b'    if (output->mode == PRINT_TO_STRING) {';assert emit.count(before)==1
            text=change(text,emit,emit.replace(before,before+b'\n        if (!jp_print_format_span(output->destination,length) || !length) return;'),name+' checked string writes')
            terminate=function(text,'terminate_text');assert terminate.count(before)==1
            text=change(text,terminate,terminate.replace(before,before+b'\n        if (!jp_print_format_span(output->destination,0)) return;'),name+' checked NUL write')
            error=function(text,'output_error');assert error.count(b'        return 0;')==1
            text=change(text,error,error.replace(b'        return 0;',b'        return jp_print_format_failed();'),name+' bounded output error')
            text=change(text,b'#include <string.h>',b'#include <string.h>\n#define strlen jp_print_format_strlen',name+' bounded format string arguments')
        elif name=='_xtostr':
            before=b'#ifdef GCC\n    /* a float argument arrives as a double, which is float under\n     * -m4-single-only. GCC compiles va_arg of float to a trap */\n    value = va_arg(*arguments, double);\n#else\n    value = va_arg(*arguments, float);\n#endif'
            text=change(text,before,b'value = jp_print_format_argument(va_arg(*arguments,double));',name+' native promotion and original single value')
        elif name=='_log10':text=change(text,b'exp * 0.301L',b'(long double)jp_print_format_scaled_exponent(exp)',name+' exact scaled exponent value in single input domain')
        elif name=='_power':text=change(text,b'    unsigned char unused[12];',b'    unsigned char unused[12];\n    (void)unused;',name+' original layout-only local')
        elif name=='_rnd':text=change(text,b'while ((*last & last_mask) && last >= bytes)',b'while (last >= bytes && (*last & last_mask))',name+' carry bounds before dereference')
        elif name=='_its':text=change(text,b"            while (--out >= digits) {\n                *out = '0';\n            }",b"            while (out > digits) {\n                *--out = '0';\n            }",name+' zero-fill pointer bounds')
        path=d/'runtime'/(name+'.c');path.write_bytes(text);generated[path.relative_to(d).as_posix()]=sha(path);obj=d/'runtime'/(name+'.obj');runtime.append(obj);commands.append(common+namespace+['-c',str(path),'-o',str(obj)])
    for source,name in [(sdk,'print_sdk'),(p/'native/jp_native_print_format.c','print_format')]:commands.append(common+['-c',str(source),'-o',str(d/(name+'.obj'))])
    commands.append(common+['-S','-emit-llvm',str(p/'native/jp_native_print_format.c'),'-o',str(d/'print_format.ll')])
    link[link.index(old)]=str(d/'print_sdk.obj');kept={n:h for n,h in objects.items() if n!=old};link[link.index('-o'):link.index('-o')]=[str(d/'print_format.obj'),*map(str,runtime),'-Wl,/export:njPrintF','-Wl,/export:njPrint'];link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
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
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp|sitofp|uitofp|fptosi|fptoui|fptrunc|fpext)\b',(d/'print_format.ll').read_text())
    sources=dict(b['authored_source_sha256'])
    for path in [p/'native/jp_native_print_format.c',p/'native/jp_native_print_format.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    for n,h in b['generated_sources_sha256'].items():target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    generated.update(b['generated_sources_sha256']);generated[sdk.name]=sha(sdk);shutil.copytree(base/'inputs',d/'inputs')
    objects_new={path.relative_to(d).as_posix():sha(path) for path in runtime+[d/'print_sdk.obj',d/'print_format.obj']}
    shared=dict(status='original_float_variadic_and_decimal_formatter_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=kept,format_objects_sha256=objects_new,format_external_sources_sha256=external,format_adaptations=changes,additional_original_print_function_sha256=provenance,previous_print_build_sha256=sha(base/'build_verification.json'),all_prior_print_state_and_queue_owners_preserved=True,private_print_sdk_object_replaced=True,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),genuine_print_float_and_variadic_compiled=True,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
