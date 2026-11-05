"""Real Ninja boot fog family and shared register/context owner.

The two old diagnostic fog setters are removed. Their events become passive
observations after actual SDK calls. Game/SDK source and DLLs stay private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from rewrite_sprite_float_operations import rewrite
from build_native_renderer_object_v3 import coff_symbols

NAMES=['njGenerateFogTable','njSetFogColor','njSetFogDensity','njSetFogTable','njSetFadeColor','njFogEnable','njFogDisable']
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert sha(c)==b['compiler_sha256'] and b['complete_original_save_source_linked'] and sha(Path(b['frame_build'])/'build_verification.json')==b['frame_build_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir();(d/'frames').mkdir()
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')];common+=['-ffp-contract=off']
    inputs={n:u/'sdk/ninja'/(n+'.c') for n in NAMES}
    original=d/'external_boot_fog_original.c';original.write_bytes(b'#include "jp_native_sprite_numeric.h"\n'+b'\n'.join(f.read_bytes() for f in inputs.values()))
    astcmd=common+['-Xclang','-ast-dump=json','-fsyntax-only',str(original)];ast=subprocess.run(astcmd,capture_output=True);(d/'ast_diagnostics.txt').write_bytes(ast.stderr);assert ast.returncode==0 and not ast.stderr,ast.stderr.decode()[:4000]
    raw,arithmetic=rewrite(original.read_bytes(),json.loads(ast.stdout),NAMES);source=d/'external_boot_fog_rz.c';source.write_bytes(raw)
    fog=d/'boot_fog.obj';commands=[common+['-c',str(source),'-o',str(fog)],common+['-S','-emit-llvm',str(source),'-o',str(d/'boot_fog.ll')]]
    fl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']));kept={n:sha(n) for n in fl if n.endswith('.obj')}
    assert not any(s['section']>0 and s['name'] in NAMES for n in kept for s in coff_symbols(Path(n)))
    fl[fl.index('-o'):fl.index('-o')]=[str(fog),*['-Wl,/export:'+n for n in NAMES]];fl[-1]=str(d/'frames/jp_native_frames.dll');commands.append(fl)
    pl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['present_DLL']));old_obj=next(n for n in pl if n.endswith('present_imported_demo_state.obj'));owner=Path(old_obj).parent;ob=read(owner/'build_verification.json')
    assert sha(owner/'build_verification.json')==b['previous_boot_build_sha256']
    old_source=owner/'present_imported_demo_state.c';assert sha(old_source)==ob['generated_sources_sha256'][old_source.name]
    text=old_source.read_bytes()
    for line in [b'void njSetFogColor(Uint32 color){(void)color;event(5);}',b'void njSetFogTable(NJS_FOG_TABLE table){(void)table;event(6);}']:
        assert text.count(line)==1;text=text.replace(line,b'/* Real SDK fog provider imported from the shared frame owner. */')
    # Header declarations precede these passive observation macros. The inner
    # same-name call is the actual imported function, with its argument once.
    anchor=b'static void act_idle(void);';assert text.count(anchor)==1
    macros=b'#define njSetFogColor(c) (njSetFogColor(c),event(5))\n#define njSetFogTable(t) (njSetFogTable(t),event(6))\n'
    text=text.replace(anchor,macros+anchor);present=d/'present_real_fog.c';present.write_bytes(text)
    pc=list(next(x['command'] for x in ob['commands'] if '-c' in x['command'] and x['command'][-1]==old_obj));pc[-3]=str(present);pc[-1]=str(d/'present_real_fog.obj');commands.append(pc)
    pl[pl.index(old_obj)]=str(d/'present_real_fog.obj');old=str(Path(b['frames_DLL']).with_suffix('.lib'));assert pl.count(old)==1;pl[pl.index(old)]=str(d/'frames/jp_native_frames.lib');pl[-1]=str(d/'jp_native_present.dll');commands.append(pl)
    for label,name,imports in [('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b[label]))
        for n in imports:
            old=str(Path(b[n]).with_suffix('.lib'));new=d/('frames/jp_native_frames.lib' if n=='frames_DLL' else 'jp_native_present.lib' if n=='present_DLL' else 'jp_native_exit.lib');assert link.count(old)==1;link[link.index(old)]=str(new)
        link[-1]=str(d/name);commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(d/'boot_fog.ll').read_text())
    for n,h in kept.items():assert sha(n)==h,n
    for n,h in b['generated_sources_sha256'].items():shutil.copyfile(base/n,d/n);assert sha(d/n)==h
    shutil.copytree(base/'inputs',d/'inputs')
    authored=dict(b['authored_source_sha256'])
    for f in [Path(__file__).resolve(),p/'jp_port_tools/rewrite_sprite_float_operations.py',p/'native/jp_native_sprite_numeric.h']:authored[f.relative_to(r).as_posix()]=sha(f)
    generated=dict(b['generated_sources_sha256']);generated.update({f.name:sha(f) for f in [original,source,present]})
    frame=dict(fb,status='real_original_boot_fog_family_linked_behavior_unverified',commands=results,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),kept_frame_objects_sha256=kept,authored_source_sha256=authored,generated_sources_sha256=generated,original_fog_SDK_sources_sha256={str(f.relative_to(u)):sha(f) for f in inputs.values()},fog_function_names=NAMES,SDK_arithmetic_adaptation=arithmetic,residual_native_FP_arithmetic=0,diagnostic_only_fog_setters_removed=True,fog_observation_macros_passive_after_real_calls=True,one_existing_native_context_and_video_register_owner=True,visible_GPU_table_fog_implemented=False)
    fd=d/'frame_view';fd.mkdir()
    for n in generated:shutil.copyfile(d/n,fd/n)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,status=frame['status'],commands=results,DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),frames_DLL=frame['frames_DLL'],frames_DLL_sha256=frame['frames_DLL_sha256'],present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'),authored_source_sha256=authored,generated_sources_sha256=generated,previous_save_state_build_sha256=sha(base/'build_verification.json'),fog_function_names=NAMES,original_fog_SDK_sources_sha256=frame['original_fog_SDK_sources_sha256'],SDK_arithmetic_adaptation=arithmetic,residual_native_FP_arithmetic=0,diagnostic_only_fog_setters_removed=True,fog_observation_macros_passive_after_real_calls=True,visible_GPU_table_fog_implemented=False,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
