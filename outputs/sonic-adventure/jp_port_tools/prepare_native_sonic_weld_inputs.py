"""Read-only extract existing verified WSL Super Sonic module/data inputs."""
import argparse,hashlib,json,subprocess,sys
from pathlib import Path
ap=argparse.ArgumentParser()
for n in ('workspace','output','track'):ap.add_argument('--'+n,type=Path,required=True)
ap.add_argument('--distribution',default='sa-dc-source-review');ap.add_argument('--checkout',default='/root/sa-native-integration');a=ap.parse_args();r=a.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(w);prefix=['wsl.exe','-d',a.distribution,'--cd',a.checkout,'--'];sys.path.insert(0,str(p));from decompress_prs import decompress
from verify_native_read_queue import Disc
def sha(x):return hashlib.sha256(x).hexdigest()
def run(*args):
 x=subprocess.run(prefix+list(args),capture_output=True);assert not x.returncode and not x.stderr,x.stderr.decode(errors='replace');return x.stdout
assert run('git','rev-parse','HEAD').decode().strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0' and not run('git','status','--porcelain','--untracked-files=no');q=json.loads((p/'analysis/jp_gcc_baseline_v2_verification.json').read_bytes());compiler='tools/sh-elf/bin/sh-elf-gcc';assert run('sha256sum',compiler).decode().split()[0]==next(x['sha256'] for x in q['toolchain_files'] if x['path']==compiler)
target=['-ml','-m4-single-only','-std=gnu17','-ffreestanding','-nostdinc','-isystem','tools/sh4zam/include','-iwithprefix','include','-DGCC','-DNDEBUG','-DSHZ_TLS_MODEL=SHZ_TLS_DISABLED'];includes=['-I'+n for n in ('sdk/include','src','modules','events','build/gen/src','build/gen/modules','build/gen/events','build/gen/sdk')];cmd=[compiler,'-E','-P',*target,*includes,'modules/b_chaos7/super.c'];cpp=run(*cmd);body=run('cat','build/gen/modules/b_chaos7/super/bodies.txt');packed=run('cat','orig/SONICADV/B_CHAOS7.PRS');raw,consumed=decompress(packed);generated=run('cat','build/gen/b_chaos7/content.bin');assert packed and raw and cpp and body and consumed<=len(packed);d.mkdir()
disc=Disc(a.track)
try:
 fad,size,isdir=disc.files()['B_CHAOS7.PRS'];assert not isdir;actual=disc.sectors(fad,(size+2047)//2048)[:size]
finally:disc.f.close()
assert actual==packed,'Preserved WSL module differs from supplied Japanese disc'
actual_decoded,actual_consumed=decompress(actual);assert actual_decoded==raw
files={'super.i':cpp,'super.txt':body,'B_CHAOS7.PRS':packed,'B_CHAOS7.BIN':raw,'generated_content.bin':generated,'generated_content.c':run('cat','build/gen/b_chaos7/content.c'),'generated_content.txt':run('cat','build/gen/b_chaos7/content.txt')};rows={}
for n,data in files.items():(d/n).write_bytes(data);rows[n]=dict(bytes=len(data),sha256=sha(data))
out=dict(status='read_only_original_Super_Sonic_source_data_and_retail_module_extracted',files=rows,preprocessor_command=prefix+cmd,upstream_commit='25a3d3154dbe657325e97dbe8c5291086f6af5c0',module_load_address='0x0c900000',generated_content_is_entire_decompressed_module=generated==raw,source_script_sha256=sha(Path(__file__).read_bytes()),actual_supplied_JP_disc_module_verified=True,actual_supplied_JP_disc_module_FAD=fad,PRS_consumed_bytes=consumed,original_source_and_builds_modified=False,retail_or_generated_inputs_publishable=False,whole_project_completion_percent=10,goal_active=True);(d/'inputs.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],len(raw),'module bytes;',len(cpp),'preprocessed bytes;content equality',generated==raw)
