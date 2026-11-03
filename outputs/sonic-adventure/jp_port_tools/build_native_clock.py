"""Build authored, asset-free clock core and a real SDK wall-count bridge."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,c,d=(getattr(a,n).resolve() for n in ['workspace','upstream','compiler','output'])
    p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    inputs=json.loads((p/'jp_port_tools/jp_task_external_inputs.json').read_text())
    for n,h in inputs['files_sha256'].items():assert sha(u/n)==h,n
    d.mkdir(parents=True);sources=[p/'native/jp_native_clock.c',p/'native/jp_clock_sdk_bridge.c']
    common=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off',
        '-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas']
    objects=[d/'clock.obj',d/'sdk_clock.obj']
    commands=[common+['-c',str(sources[0]),'-o',str(objects[0])],
        common+['-include',str(p/'native/jp_host_types.h'),'-I',str(u/'sdk/include'),'-c',str(sources[1]),'-o',str(objects[1])],
        [str(c),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
         '-Wl,/export:syTmrGetCount',*map(str,objects),'-Wl,kernel32.lib','-o',str(d/'jp_native_clock.dll')]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:4000]);break
    passed=len(results)==3 and all(x['exit_code']==0 for x in results)
    for n,h in inputs['files_sha256'].items():assert sha(u/n)==h,n
    authored=[*sources,p/'native/jp_native_clock.h',p/'native/jp_host_types.h',Path(__file__)]
    report=dict(status='native_clock_linked_behavior_unverified' if passed else 'build_failed',commands=results,
        compiler_sha256=sha(c),authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in authored},
        object_sha256={o.name:sha(o) for o in objects if o.exists()},
        dll_sha256=sha(d/'jp_native_clock.dll') if passed else None,
        game_source_linked=False,retail_assets_embedded=False,
        wall_counts_per_second=781250,timer1_ticks_per_second=12500000,
        complete_presentation_or_game_qualified=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',sum(x['diagnostic_bytes'] for x in results));raise SystemExit(0 if passed else 1)
if __name__=='__main__':main()
