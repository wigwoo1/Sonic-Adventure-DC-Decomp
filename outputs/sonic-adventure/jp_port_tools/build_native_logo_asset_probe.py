"""Privately compile unchanged logo_draw and its original generated metadata.
Only the build workflow is public; no game/SDK bodies, retail metadata or DLL.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--distribution',required=True);ap.add_argument('--generated-root',required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_text())
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists() and sha(a.compiler)==b['compiler_sha256']
    for n,h in dict(b['authored_source_sha256'],**b['vendored_MIT_source_sha256']).items():assert sha(r/n)==h,n
    original=u/'src/progress/demo.c';assert sha(original)=='059b737c7aa48c062331e1d57fd7e09bb0366f3251e3ff623074d7b8ab1b5f15'
    raw=original.read_bytes();draw=function(raw,'logo_draw');d.mkdir();inputs=d/'inputs/progress/demo';inputs.mkdir(parents=True);metadata={}
    for name in ['logo_texanims.inc','logo_texlist.inc','text.inc']:
        command=['wsl.exe','-d',a.distribution,'-u','root','--','cat',a.generated_root.rstrip('/')+'/progress/demo/'+name]
        run=subprocess.run(command,capture_output=True);assert run.returncode==0 and not run.stderr,run.stderr.decode()[:1000]
        target=inputs/name;target.write_bytes(run.stdout);metadata[name]=dict(source=a.generated_root.rstrip('/')+'/progress/demo/'+name,sha256=sha(target),bytes=target.stat().st_size)
    declarations=[]
    for name in ['logo_textures','logo_texlist','logo_texanims']:
        marker=b'static NJS_TEX'+(b'NAME' if name=='logo_textures' else b'LIST' if name=='logo_texlist' else b'ANIM')+b' '+name.encode();start=raw.index(marker);end=raw.index(b'};',start)+2;declarations.append(raw[start:end])
    source=d/'external_original_logo_draw.c';source.write_bytes(b'#include "jp_native_assets.h"\n#include "main.h"\n#define LOGO_PIECES 6\n#include "progress/demo/text.inc"\nvoid texture_load_pvm(char *, NJS_TEXLIST *);\n'+b'\n'.join(declarations)+b'\n'+draw+b'''
__declspec(dllexport) int jp_logo_asset_load(void)
{texture_load_pvm("SEGALOGO",&logo_texlist);return jp_assets_status();}
__declspec(dllexport) void jp_logo_asset_draw(void){logo_draw();}
__declspec(dllexport) NJS_TEXLIST *jp_logo_asset_list(void){return &logo_texlist;}
__declspec(dllexport) int jp_logo_asset_release(void){return njReleaseTexture(&logo_texlist);}
''')
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')];common+=['-ffreestanding','-I'+str(d/'inputs')]
    obj=d/'logo_probe.obj';dll=d/'jp_original_logo_asset_probe.dll';commands=[common+['-c',str(source),'-o',str(obj)],
        [str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(obj),str(a.build/'frames/jp_native_frames.lib'),'-o',str(dll)]]
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:5000]
    result=dict(status='original_logo_draw_and_original_metadata_privately_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),
        asset_build=str(a.build.resolve()),asset_build_sha256=sha(a.build/'build_verification.json'),frame_owner_DLL=b['frames_DLL'],frame_owner_DLL_sha256=b['frames_DLL_sha256'],
        DLL=str(dll),DLL_sha256=sha(dll),external_game_source_sha256=sha(original),original_logo_draw_body_sha256=hashlib.sha256(draw).hexdigest(),original_body_unchanged=True,
        private_generated_metadata=metadata,private_generated_source_sha256=sha(source),private_probe_object_sha256=sha(obj),
        authored_tool_sha256=sha(Path(__file__)),game_source_and_metadata_in_DLL=True,artifact_publishable=False,retail_assets_or_metadata_published=False,logo_start_or_game_loop_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
