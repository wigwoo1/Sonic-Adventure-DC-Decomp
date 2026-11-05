"""Compile an owned native AICA boundary around the qualified external core."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','core','baseline','audio','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();u=a.core.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    b=read(a.baseline/'build_verification.json');audio=read(a.audio/'build_verification.json');package=read(p/'analysis/jp_aica_probe_package.json')
    for meta in (b,audio):
        assert sha(meta['DLL'])==meta['DLL_sha256']
        for name,h in meta['authored_source_sha256'].items():assert sha(r/name)==h,name
    assert b['core_RAM_bytes']==2<<20 and b['ARM_RAM_stores_flush_pending_AICA'] and b['AICA_DSP_runs_when_inaudible']
    for n,h in b['upstream_source_sha256'].items():assert sha(u/n)==h,n
    preserved=next(rows for folder,rows in package['private_artifact_sha256'].items() if (r/folder).resolve()==a.baseline.resolve())
    for n,h in preserved.items():assert sha(a.baseline/n)==h,n
    assert sha(a.baseline/'external_dcsound_2MiB.c')==b['generated_source_sha256']
    assert sha(a.baseline/'external_yam_timing.c')==b['generated_YAM_source_sha256']
    bridge=p/'jp_port_tools/aica_reset_boundary.inc';generated=d/'external_dcsound_owned.c';generated.write_bytes((a.baseline/'external_dcsound_2MiB.c').read_bytes()+b'\n'+bridge.read_bytes())
    arm_bridge=p/'jp_port_tools/aica_arm_reset_boundary.inc';generated_arm=d/'external_arm_reset.c';generated_arm.write_bytes((u/'Core/arm.c').read_bytes()+b'\n'+arm_bridge.read_bytes())
    shutil.copyfile(a.baseline/'yam.obj',d/'yam.obj')
    for suffix in ('.dll','.lib'):shutil.copyfile(Path(audio['DLL']).with_suffix(suffix),d/('jp_native_audio'+suffix))
    cc=b['commands'][0]['command'][0]
    common=[cc,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-DEMU_COMPILE','-DEMU_LITTLE_ENDIAN','-DWIN32=1','-I'+str(u/'Core'),'-I'+str(p/'native')]
    commands=[common+['-c',str(generated),'-o',str(d/'dcsound.obj')],common+['-c',str(generated_arm),'-o',str(d/'arm.obj')],common+['-Werror','-c',str(p/'native/jp_native_aica.c'),'-o',str(d/'aica.obj')],
        [cc,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*[str(d/(n+'.obj')) for n in ('arm','yam','dcsound','aica')],str(d/'jp_native_audio.lib'),'-Wl,kernel32.lib','-o',str(d/'jp_native_aica.dll')]]
    results=[]
    for j,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode,(log,log.read_text())
        if j>=2:assert not log.stat().st_size,log.read_text()
    authored=[p/'native/jp_native_aica.c',p/'native/jp_native_aica.h',bridge,arm_bridge,Path(__file__).resolve()]
    result=dict(status='owned_native_AICA_CPU_RAM_register_and_real_output_boundary_linked_unverified',commands=results,DLL=str(d/'jp_native_aica.dll'),DLL_sha256=sha(d/'jp_native_aica.dll'),audio_DLL=str(d/'jp_native_audio.dll'),audio_DLL_sha256=sha(d/'jp_native_audio.dll'),
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},baseline_build_sha256=sha(a.baseline/'build_verification.json'),audio_build_sha256=sha(a.audio/'build_verification.json'),
        generated_DCSOUND_sha256=sha(generated),generated_ARM_sha256=sha(generated_arm),copied_core_objects_sha256={'yam.obj':sha(d/'yam.obj')},ARM_reset_masks_IRQ_and_FIQ=True,core_revision=b['upstream_revision'],core_license=b['upstream_license'],core_RAM_bytes=2<<20,
        original_SDK_or_game_bodies_linked=False,original_sound_system_initialized=False,retail_assets_embedded=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)

if __name__=='__main__':main()
