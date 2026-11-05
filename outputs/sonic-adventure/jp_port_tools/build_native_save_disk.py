"""Compile authored fixed-format Windows save I/O; import genuine game providers."""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','state','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,state,c,d=a.workspace,a.upstream,a.state,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=json.loads((state/'build_verification.json').read_bytes())
    assert b['save_store_load_update_linked'] and sha(c)==b['compiler_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir();(d/'imports/save').mkdir(parents=True)
    headers={}
    for name,needle in [('save.h',b'extern struct save saves[SAVE_COUNT];'),('card.h',b'extern int save_slot;')]:
        f=u/'src/save'/name;raw=f.read_bytes();assert raw.count(needle)==1;overlay=d/'imports/save'/name;overlay.write_bytes(raw.replace(needle,b'__declspec(dllimport) '+needle));headers[str(f.relative_to(u))]=sha(f)
    source=p/'native/jp_native_save_disk.c';dll=d/'jp_native_save_disk.dll'
    common=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(d/'imports'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'src')]
    commands=[common+['-c',str(source),'-o',str(d/'save_disk.obj')],[str(c),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'save_disk.obj'),str(Path(b['frames_DLL']).with_suffix('.lib')),'-Wl,kernel32.lib','-o',str(dll)]];results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    defined=[s['name'] for s in coff_symbols(d/'save_disk.obj') if s['section']>0 and s['function']];assert all(n.startswith('jp_save_disk_') for n in defined),defined
    files=[source,p/'native/jp_native_save_disk.h',p/'native/jp_host_types.h',Path(__file__).resolve()]
    report=dict(status='authored_native_save_disk_IO_compiled_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,DLL=str(dll),DLL_sha256=sha(dll),object_sha256=sha(d/'save_disk.obj'),compiler_sha256=sha(c),frame_DLL=b['frames_DLL'],frame_DLL_sha256=b['frames_DLL_sha256'],state_build_sha256=sha(state/'build_verification.json'),authored_source_sha256={f.relative_to(r).as_posix():sha(f) for f in files},external_header_sha256=headers,defined_external_function_names=defined,original_game_or_SDK_bodies_linked=False,retail_assets_embedded=False,artifact_publishable=True,full_card_UI_or_game_main_loop_integrated=False,playable=False,overall_completion_percent=10)
    (d/'source-snapshot').mkdir()
    for f in files:(d/'source-snapshot'/f.name).write_bytes(f.read_bytes())
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
