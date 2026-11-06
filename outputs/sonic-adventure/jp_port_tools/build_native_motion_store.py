"""Link checked motion ownership and original character selection to qualified data."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_read_queue import PIN
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_source_sha256'].items():assert sha(base/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256']
    original=(u/'src/system/loader.c').read_bytes();assert original==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/system/loader.c'])
    body=function(original,'loader_read_character_motions');old=b'        for (;;) {\n        }';assert body.count(old)==1
    native=body.replace(old,b'        jp_motion_bad_character(character);\n        return;')
    prefix=b'''#include "player/player.h"
#include "system/loader.h"
int jp_motion_source_load(char *,void *);
void jp_motion_bad_character(int);
#define loader_read_prs jp_motion_source_load
#define loader_read_character_motions jp_original_motion_select
'''
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.obj','.lib','.dll'):shutil.copyfile(path,d/path.name)
    source=d/'external_motion_selector.c';source.write_bytes(prefix+native)
    compiler=b['commands'][0]['command'][0];sdk=b['commands'][0]['command'];sdk=sdk[:sdk.index('-c')]+['-I'+str(u/'src')]
    host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    commands=[sdk+['-c',str(source),'-o',str(d/'motion_selector_original.obj')],host+['-c',str(p/'native/jp_native_motion_store.c'),'-o',str(d/'motion_store_owner.obj')]]
    old_link=b['commands'][-1]['command'];link=[]
    for arg in old_link:
        link.append(str(d/Path(arg).name) if arg.endswith('.obj') else arg)
    at=link.index('-o');link[at:at]=[str(d/'motion_selector_original.obj'),str(d/'motion_store_owner.obj'),'-Wl,kernel32.lib'];link[-1]=str(d/'jp_native_motion_data.dll');commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'motion_store_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()[:7000]
    authored=[p/'native/jp_native_motion_store.h',p/'native/jp_native_motion_store.c',Path(__file__).resolve()]
    out=dict(b,status='original_character_motion_selector_and_checked_native_bank_ownership_linked_unverified',
        motion_store_baseline_sha256=sha(base/'build_verification.json'),motion_store_original_source_sha256=sha(u/'src/system/loader.c'),
        motion_store_original_body_sha256=hashlib.sha256(body).hexdigest(),motion_store_native_body_sha256=hashlib.sha256(native).hexdigest(),
        motion_store_kept_objects_sha256={path.name:sha(path) for path in base.glob('*.obj')},
        generated_source_sha256=dict(b['generated_source_sha256'],**{source.name:sha(source)}),
        authored_source_sha256=dict(b['authored_source_sha256'],**{path.relative_to(r).as_posix():sha(path) for path in authored}),
        commands=results+b['commands'],motion_store_new_build_commands=len(results),DLL=link[-1],DLL_sha256=sha(link[-1]),
        native_character_motion_selection_verified=False,full_set_character_or_gameplay_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],flush=True)
if __name__=='__main__':main()
