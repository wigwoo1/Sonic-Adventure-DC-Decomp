"""Private original ISO directory parser over the real native sector reader."""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','media','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((a.media/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256']
    names=['sdk/shinobi/gdfsdir.c','sdk/shinobi/gdfshn.c','sdk/shinobi/gdfs.h','sdk/include/shinobi.h','sdk/include/sg_xpt.h']
    originals={n:sha(u/n) for n in names};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+n])).hexdigest()==h,n
    d.mkdir();raw=(u/'sdk/shinobi/gdfsdir.c').read_bytes();selected=['name_upper','gdFsMemcpy','name_same','name_same_any_case','name_sid','read_dirrec','gdFsGetDirEnt','gdFsSearchName']
    bodies={n:function(raw,n) for n in selected};bodies['gdFsCreateDirhn']=function((u/'sdk/shinobi/gdfshn.c').read_bytes(),'gdFsCreateDirhn')
    prefix=raw[:raw.index(bodies['name_upper'])]+b'''\n#define GCC
#define JP_GD_API __declspec(dllimport)
#include "jp_native_gd_media.h"
'''
    fixture=p/'jp_port_tools/gd_iso_fixture.inc';generated=d/'external_gd_iso_probe.c';generated.write_bytes(prefix+b'\n'.join(bodies.values())+b'\n'+fixture.read_bytes())
    compiler=b['commands'][0]['command'][0];common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk')]
    commands=[common+['-c',str(generated),'-o',str(d/'gd_iso_probe.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'gd_iso_probe.obj'),str(a.media/'jp_native_gd_media.lib'),'-o',str(d/'gd_iso_probe.dll')]]
    results=[]
    for j,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    meta=dict(status='original_ISO_directory_bodies_real_native_sector_reader_linked_private_unverified',commands=results,DLL=str(d/'gd_iso_probe.dll'),DLL_sha256=sha(d/'gd_iso_probe.dll'),
        media_DLL=b['DLL'],media_DLL_sha256=b['DLL_sha256'],media_build_sha256=sha(a.media/'build_verification.json'),
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in (fixture,Path(__file__).resolve())},original_source_sha256=originals,
        original_function_sha256={n:hashlib.sha256(body).hexdigest() for n,body in bodies.items()},original_function_bodies_unchanged=True,
        generated_source_sha256=sha(generated),original_GCC_dr_initialization_enabled=True,original_ISO_parser_behind_bounded_record_validation=True,
        full_GDFS_bootstrap_or_GD_DCF_implemented=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(meta,indent=2)+'\n');print(meta['status'],flush=True)
if __name__=='__main__':main()
