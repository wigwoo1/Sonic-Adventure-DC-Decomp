"""Compile unchanged pinned Japanese task/heap source for native Windows x64.

The resulting component DLL contains external game code and stays private.
No retail data is embedded. This is not the complete game or a portable ABI.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);ap.add_argument('--upstream',type=Path,required=True);ap.add_argument('--compiler',type=Path,required=True);a=ap.parse_args()
 root=a.workspace.resolve();p=root/'outputs/sonic-adventure';w=root/'work/sonic-adventure';upstream=a.upstream.resolve();compiler=a.compiler.resolve()
 pinned=json.loads((w/'jp_gcc_worktree_preparation.json').read_text());assert pinned['upstream_commit']=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
 inputs=[upstream/'src/system/task.c']+list((upstream/'sdk/include').rglob('*.h'))+list((upstream/'src').rglob('*.h'))+list((upstream/'modules').rglob('*.h'))
 for source in inputs:
  relative=source.relative_to(upstream).as_posix();assert sha(source)==pinned['source_sha256'][relative],relative
 out=w/'jp-native-heap-probe-v2';assert not out.exists();out.mkdir()
 bridge=p/'native/jp_heap_bridge.c';game=upstream/'src/system/task.c';dll=out/'jp_heap_x64.dll'
 flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-ffunction-sections','-fdata-sections','-nostdinc','-Wno-unknown-pragmas']
 flags+=['-include',str(p/'native/jp_host_types.h')]
 includes=[f'-I{upstream/folder}' for folder in ['sdk/include','src','modules','events']]
 commands=[([str(compiler),*flags,*includes,'-c',str(game),'-o',str(out/'task.obj')],'game_compile'),([str(compiler),*flags,'-Wall','-Wextra','-Werror',*includes,'-c',str(bridge),'-o',str(out/'bridge.obj')],'bridge_compile'),([str(compiler),'--target=x86_64-pc-windows-msvc','-fuse-ld=lld','-shared','-nostdlib','-Wl,/noentry,/opt:ref',str(out/'task.obj'),str(out/'bridge.obj'),'-o',str(dll)],'link')]
 results=[]
 for command,name in commands:
  result=subprocess.run(command,capture_output=True);log=out/(name+'.txt');log.write_bytes(result.stdout+result.stderr)
  results.append(dict(phase=name,command=command,exit_code=result.returncode,log=str(log),log_sha256=sha(log),log_bytes=log.stat().st_size))
  print(name,result.returncode,log.stat().st_size,'diagnostic bytes',flush=True)
  if result.returncode:break
 success=len(results)==3 and all(r['exit_code']==0 for r in results)
 report=dict(status='compiled_native_x64_external_heap_component_behavior_unverified' if success else 'native_heap_component_build_failed',upstream_commit=pinned['upstream_commit'],upstream_source=str(game),upstream_source_sha256=sha(game),upstream_sources_modified=False,game_source_publicly_imported=False,private_output_directory=str(out),DLL_path=str(dll),DLL_sha256=sha(dll) if success else None,commands=results,compiled_original_source_files=1,rendering_or_gameplay_tested=False,native_full_game=False,external_game_code_artifacts_publishable=False,source_and_header_sha256={str(s):sha(s) for s in inputs},authored_bridge_sha256=sha(bridge),host_types_sha256=sha(p/'native/jp_host_types.h'),initial_failed_builder_preserved=True,initial_failure_correction='Provide native compiler size_t through the SDK existing SIZE_T_DEFINED guard; unchanged game source and Uint32 retain their declarations.',compiler_sha256=sha(compiler),builder_sha256=sha(Path(__file__)),overall_completion_percent=10,goal_paused=False)
 target=p/'analysis/jp_native_heap_build_v2_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 raise SystemExit(0 if success else 1)
if __name__=='__main__':main()
