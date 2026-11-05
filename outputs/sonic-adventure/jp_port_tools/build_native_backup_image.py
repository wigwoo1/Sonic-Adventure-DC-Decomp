"""Compile the original complete vmsutil image/CRC module with native bounds.
Original source/object and linked SDK-containing DLL remain private.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.baseline/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    original=u/'sdk/shinobi/vmsutil.c';raw=original.read_bytes();names=['buMakeBackupFileImage','buCalcBackupFileSize','buAnalyzeBackupFileImage','buCalcCRC'];d.mkdir()
    # Parse hardware header declarations in their existing native-compatible
    # mode; enable only the upstream array-bound repair in the function body.
    sdk=d/'external_backup_image.c';sdk.write_bytes(b'#include <sg_xpt.h>\n#include <shinobi.h>\n#include <string.h>\n#define GCC\n'+raw)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')];source=r/'outputs/sonic-adventure/native/jp_native_backup_image.c'
    commands=[common+['-c',str(sdk),'-o',str(d/'backup_sdk.obj')],common+['-c',str(source),'-o',str(d/'backup_boundary.obj')]]
    dll=d/'jp_native_backup_image.dll';commands.append([common[0],'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'backup_sdk.obj'),str(d/'backup_boundary.obj'),*['-Wl,/export:'+n for n in names],'-o',str(dll)]);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    result=dict(status='complete_original_backup_image_and_native_bounds_linked_unverified',commands=results,DLL=str(dll),DLL_sha256=sha(dll),original_source_sha256=sha(original),original_functions_sha256={n:hashlib.sha256(function(raw,n)).hexdigest() for n in names},generated_source_sha256=sha(sdk),authored_source_sha256={str(p.relative_to(r)):sha(p) for p in [source,source.with_suffix('.h'),Path(__file__).resolve()]},baseline_build_sha256=sha(a.baseline/'build_verification.json'),GCC_header_array_overwrite_repair_enabled=True,original_image_function_bodies_unchanged=True,artifact_publishable=False,buInit_or_card_init_implemented=False,disk_IO_implemented=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
