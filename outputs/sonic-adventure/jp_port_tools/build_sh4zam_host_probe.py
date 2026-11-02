"""Compile actual pinned portable backend with the explicit host configuration."""
import hashlib,json,subprocess
from pathlib import Path
import argparse
ap=argparse.ArgumentParser()
for name in ['workspace','sh4zam','compiler','output']:ap.add_argument('--'+name,type=Path,required=True)
a=ap.parse_args();R,Z,C,D=(getattr(a,n).resolve() for n in ['workspace','sh4zam','compiler','output'])
W=R/'work/sonic-adventure';P=R/'outputs/sonic-adventure'
assert D.is_relative_to(W) and not D.is_relative_to(W/'build') and not D.exists();D.mkdir(parents=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
dependencies=json.loads((P/'jp_port_tools/jp_renderer_external_inputs.json').read_text())
for name,h in dependencies['sh4zam_files_sha256'].items():assert sha(Z/name)==h
sources=[P/'native/jp_sh4zam_probe.c',P/'native/jp_sh4zam_probe_other.c',Z/'source/sw/shz_xmtrx_sw.c']
command=[str(C),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-unknown-attributes','-fuse-ld=lld','-shared',
 '-include',str(P/'native/jp_sh4zam_host_config.h'),'-isystem',str(Z/'include'),*map(str,sources),'-o',str(D/'jp_shz_native.dll')]
result=subprocess.run(command,capture_output=True);log=D/'build.txt';log.write_bytes(result.stdout+result.stderr)
for name,h in dependencies['sh4zam_files_sha256'].items():assert sha(Z/name)==h
report=dict(status='actual_SH4ZAM_host_configuration_build_probe',command=command,exit_code=result.returncode,
 log_sha256=sha(log),diagnostic_bytes=log.stat().st_size,
 authored_source_sha256={s.relative_to(R).as_posix():sha(s) for s in sources if s.is_relative_to(P)},
 config_sha256=sha(P/'native/jp_sh4zam_host_config.h'),external_source_sha256=sha(sources[-1]),
 external_headers_sha256={p.relative_to(Z).as_posix():sha(p) for p in (Z/'include').rglob('*.h')},
 DLL_sha256=sha(D/'jp_shz_native.dll') if result.returncode==0 else None,
 original_dependency_modified=False,third_party_bodies_vendored=False,retail_assets=False,game_source_linked=False,
 runtime_behavior_verified=False,overall_completion_percent=10,goal_paused=False,playable=False)
(D/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print('Host configuration build',result.returncode,'diagnostic bytes',log.stat().st_size)
if result.returncode:print(log.read_text()[:6500])
raise SystemExit(result.returncode)
