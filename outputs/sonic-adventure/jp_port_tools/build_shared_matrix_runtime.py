"""Build one real Ninja/SH4ZAM active-matrix runtime, without game source/assets."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser()
 for name in ['workspace','sh4zam','compiler','output']:ap.add_argument('--'+name,type=Path,required=True)
 a=ap.parse_args();r,z,c,d=(getattr(a,n).resolve() for n in ['workspace','sh4zam','compiler','output'])
 p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure'
 assert d.is_relative_to(w) and not d.is_relative_to(w/'build') and not d.exists();d.mkdir(parents=True)
 dep=json.loads((p/'jp_port_tools/jp_renderer_external_inputs.json').read_text())
 for name,h in dep['sh4zam_files_sha256'].items():assert sha(z/name)==h
 sources=[p/'native'/n for n in ['jp_ninja_math_shared.c','jp_ninja_sh4zam_state.c','jp_sh4zam_probe.c','jp_sh4zam_probe_other.c']]
 command=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin',
  '-Wall','-Wextra','-Werror','-Wno-unknown-attributes','-include',str(p/'native/jp_sh4zam_host_config.h'),
  '-isystem',str(z/'include'),'-shared','-fuse-ld=lld','-Wl,/export:shz_xmtrx_state_',*map(str,sources),'-o',str(d/'jp_shared_matrix.dll')]
 result=subprocess.run(command,capture_output=True);log=d/'build.txt';log.write_bytes(result.stdout+result.stderr)
 authored=sources+[p/'native/jp_ninja_math.h',p/'native/jp_ninja_shared_state.h',p/'native/jp_sh4zam_host_config.h',
  p/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h',Path(__file__)]
 for name,h in dep['sh4zam_files_sha256'].items():assert sha(z/name)==h
 report=dict(status='shared_matrix_runtime_linked_behavior_unverified' if result.returncode==0 else 'shared_matrix_runtime_build_failed',
  command=command,exit_code=result.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log),compiler_sha256=sha(c),
  authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in authored},dependency_manifest_sha256=sha(p/'jp_port_tools/jp_renderer_external_inputs.json'),
  DLL=str(d/'jp_shared_matrix.dll'),DLL_sha256=sha(d/'jp_shared_matrix.dll') if result.returncode==0 else None,
  separate_SH4ZAM_state_source_linked=False,game_source_linked=False,retail_assets_embedded=False,
  one_game_thread_per_runtime=True,original_sources_modified=False,overall_completion_percent=10,goal_paused=False,playable=False)
 (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(report['status'],'diagnostic bytes',log.stat().st_size)
 if result.returncode:print(log.read_text()[:6000])
 raise SystemExit(result.returncode)
if __name__=='__main__':main()
