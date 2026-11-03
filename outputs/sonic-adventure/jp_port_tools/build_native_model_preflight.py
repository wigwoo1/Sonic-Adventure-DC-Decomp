"""Compose real state, texture and queue objects with bounded model validation."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','state','textures','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    objects=[];exports=set();data_exports=set();inputs={}
    for folder in [a.state,a.textures]:
        b=json.loads((folder/'build_verification.json').read_text());inputs[str(folder/'build_verification.json')]=sha(folder/'build_verification.json')
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
        for n,h in b['object_sha256'].items():
            path=folder/n;assert sha(path)==h;objects.append(path)
        for arg in b['commands'][-1]['command']:
            if arg.startswith('-Wl,/export:'):exports.add(arg.split(':',1)[1])
            elif arg.startswith('/export:'):
                name=arg.split(':',1)[1];assert name.endswith(',DATA');data_exports.add(name[:-5])
        if 'linked_queue_object' in b:
            path=Path(b['linked_queue_object']);assert sha(path)==b['linked_queue_object_sha256'];objects.append(path)
    assert len(objects)==14 and all(re.fullmatch(r'\w+',name) for name in exports|data_exports)
    exports.update(['njStartVertexNonTexOpaque','njStartVertexNonTexTrans',
        'njStartVertexTexOpaque','njStartVertexTexTrans','njEndVertex','njSetkmPointer'])
    base=json.loads((p/'jp_port_tools/jp_task_external_inputs.json').read_text())
    for n,h in base['files_sha256'].items():assert sha(a.upstream/n)==h,n
    d.mkdir(parents=True);obj=d/'jp_model_preflight.obj';source=p/'native/jp_model_preflight.c'
    compile=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off',
        '-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas',
        '-include',str(p/'native/jp_host_types.h'),'-I',str(p/'native'),'-I',str(a.upstream/'sdk/include'),
        '-c',str(source),'-o',str(obj)]
    dll=d/'jp_model_preflight.dll'
    link=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
        *['-Wl,/export:'+n for n in sorted(exports)],
        *[arg for n in sorted(data_exports) for arg in ['-Xlinker','/export:'+n+',DATA']],
        str(obj),*map(str,objects),'-o',str(dll)]
    results=[]
    for i,cmd in enumerate([compile,link]):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:5000]);break
    passed=len(results)==2 and all(x['exit_code']==0 for x in results)
    authored=[source,p/'native/jp_model_preflight.h',Path(__file__)]
    report=dict(status='native_model_preflight_linked_behavior_unverified' if passed else 'build_failed',commands=results,
        input_build_reports_sha256=inputs,linked_objects_sha256={str(o):sha(o) for o in objects},
        authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in authored},
        object_sha256=sha(obj) if obj.exists() else None,dll_sha256=sha(dll) if passed else None,
        game_source_linked=True,game_containing_artifact_publishable=False,
        complete_renderer_linked=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',sum(x['diagnostic_bytes'] for x in results));raise SystemExit(0 if passed else 1)
if __name__=='__main__':main()
