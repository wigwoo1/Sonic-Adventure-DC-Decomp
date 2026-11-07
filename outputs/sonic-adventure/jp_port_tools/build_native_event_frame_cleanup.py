"""Expose existing original event cleanup; retain all compiled providers."""
import argparse
import hashlib
import json
import shlex
import shutil
import subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();base=a.baseline.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            assert sha(v)==b[k+'_sha256']
    cmd=list(next(x['command'] for x in b['commands']
                  if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
    objects={n:sha(n) for n in cmd if n.endswith('.obj')}
    providers=[n for n in objects if any(s['name']=='event_free' and s['section']>0
                                        for s in coff_symbols(Path(n)))]
    assert len(providers)==1 and '-Wl,/export:event_free' not in cmd
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):
            shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):
            shutil.copytree(path,d/path.name)
    cmd.insert(cmd.index('-o'),'-Wl,/export:event_free')
    cmd[-1]=str(d/'jp_native_frames.dll')
    text='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n'
    assert shlex.split(text)==cmd[1:]
    rsp=d/'link.rsp';rsp.write_text(text)
    run=subprocess.run([cmd[0],'@'+str(rsp)],capture_output=True)
    (d/'link.stdout').write_bytes(run.stdout);(d/'link.stderr').write_bytes(run.stderr)
    assert run.returncode==0 and not run.stderr,run.stderr.decode(errors='replace')[-6000:]
    assert all(sha(n)==h for n,h in objects.items())
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
            if k!='frames_DLL':assert updates[k+'_sha256']==b[k+'_sha256']
    updates.update(status='existing_original_event_cleanup_exported_runtime_unverified',
                   event_cleanup_baseline_sha256=sha(base/'build_verification.json'),
                   event_cleanup_existing_provider=providers[0],
                   event_cleanup_objects_sha256=objects,
                   event_cleanup_new_exports=['event_free'],
                   authored_source_sha256=dict(b['authored_source_sha256'],
                        **{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),
                   commands=[dict(command=cmd,exit_code=0,response_file_sha256=sha(rsp))]+b['commands'])
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print('Linked original event_free export; all existing object and other DLL hashes preserved.',flush=True)

if __name__=='__main__':main()
