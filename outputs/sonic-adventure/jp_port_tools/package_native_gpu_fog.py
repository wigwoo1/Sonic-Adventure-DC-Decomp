"""Package exactly the qualified authored GPU object, without game/SDK bodies."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','verification','regressions','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r=a.workspace;build=a.build;d=a.output;b=read(build/'build_verification.json');v=read(a.verification/'verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(a.compiler)==b['compiler_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert v['checks']==3852 and v['images_compared']==319 and v['build_report_sha256']==sha(build/'build_verification.json') and v['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools/verify_native_gpu_fog.py')
    assert v['maximum_RGBA8_channel_error']<=2 and v['Texture_Off_zero_texture_and_sampler_bindings'] and v['async_job_survives_source_frame_release']
    for name,checks in [('verification.json',491158),('extended_verification.json',121451)]:
        q=read(a.regressions/name);assert q['checks']==checks and q['build_report_sha256']==sha(a.regressions/'build_verification.json')
    source=build/'gpu_fog.obj';assert sha(source)==b['authored_GPU_object_sha256'] and not b['authored_GPU_contains_game_or_SDK_bodies']
    defined=[s['name'] for s in coff_symbols(source) if s['section']>0 and s['function']];assert defined and all(n.startswith('jp_') for n in defined),defined
    d.mkdir();obj=d/source.name;shutil.copyfile(source,obj);assert sha(obj)==sha(source)
    library=d/'jp_native_gpu_fog_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),str(obj)]
    run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;members=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:members.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and members==[sha(obj)]
    inputs={str((build/'build_verification.json').relative_to(r)):sha(build/'build_verification.json'),str((a.verification/'verification.json').relative_to(r)):sha(a.verification/'verification.json')}
    for name in ['build_verification.json','verification.json','extended_verification.json','gpu_direct_verification.json','gpu_extended_direct_verification.json']:inputs[str((a.regressions/name).relative_to(r))]=sha(a.regressions/name)
    report=dict(status='authored_GPU_fog_support_library_exact_member_verified',command=cmd,exit_code=0,diagnostic_bytes=0,librarian_sha256=sha(librarian),component_library=str(library),component_library_sha256=sha(library),member_object_sha256=sha(obj),parent_GPU_DLL_sha256=b['GPU_DLL_sha256'],qualification_inputs_sha256=inputs,defined_external_functions=defined,exactly_one_authored_member=True,external_game_or_SDK_bodies_packaged=False,retail_assets_embedded=False,original_source_or_proofs_modified=False,artifact_publishable=True,package_tool_sha256=sha(Path(__file__)),dependencies=['qualified authored packet/texel/background consumers','actual shared frame/register owner','Windows D3D11/D3DCompiler/DXGI/User32/XInput dependencies'],qualification='incomplete research support library; no playable executable; native float fog policy',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
