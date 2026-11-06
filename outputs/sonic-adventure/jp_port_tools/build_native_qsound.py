"""Actual SDK QSound producer and game copy-ownership source family."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
GAME=('sound_set_qsound','qsound_range','qsound_alloc','qsound_free')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();p=r/'outputs/sonic-adventure';base=a.baseline.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['sound_clock_generated_sha256'].items():assert sha(base/n)==h,n;shutil.copyfile(base/n,d/n)
    assert sha(base/'external_game_sound.c')==b['game_sound_generated_source_sha256']
    originals={n:sha(u/n) for n in ('sdk/shinobi/sd_qsnd_set_pos.c','sdk/shinobi/sd_work.h','src/sound/sound.c','src/sound/sound.h')};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            source=Path(value);assert sha(source)==b[key+'_sha256'];target=d/source.name
            if not target.exists():shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
            updates[key]=str(target);updates[key+'_sha256']=sha(target);preserved[key]=dict(path=str(source),sha256=sha(source))
    for name in ('jp_native_g2_audio.dll','jp_native_g2_audio.lib','jp_native_aica.dll','jp_native_aica.lib','jp_native_audio.dll'):shutil.copyfile(base/name,d/name)
    shutil.copytree(base/'include',d/'include');shutil.copytree(base/'inputs',d/'inputs')
    for n,h in b['generated_sources_sha256'].items():
        assert sha(base/n)==h,n;target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target)
    raw=(u/'src/sound/sound.c').read_bytes();bodies={n:function(raw,n) for n in GAME};before={n:hashlib.sha256(body).hexdigest() for n,body in bodies.items()}
    sdk=(u/'sdk/shinobi/sd_qsnd_set_pos.c').read_bytes();original_sdk=function(sdk,'sdQsndSetPos')
    source=(base/'external_game_sound.c').read_bytes();old=b'static int qsound_owners[QSOUND_SES][QSOUND_COPIES];';assert source.count(old)==1
    source=source.replace(old,b'static uintptr_t qsound_owners[QSOUND_SES][QSOUND_COPIES];')
    expression=b'sound_slots[slot].id + 1'
    for n in ('qsound_alloc','qsound_free'):
        assert bodies[n].count(expression)==1;bodies[n]=bodies[n].replace(expression,b'((uintptr_t)sound_slots[slot].id + (uintptr_t)1)')
    old=b'    sdQsndSetPos(pos);';assert bodies['sound_set_qsound'].count(old)==1
    bodies['sound_set_qsound']=bodies['sound_set_qsound'].replace(old,b'    jp_game_qsound_result = sdQsndSetPos(pos);').replace(b'void sound_set_qsound(',b'void jp_game_sound_set_qsound(')
    observer=p/'jp_port_tools/qsound_observation.inc'
    source+=b'\n#undef SOURCE\n'+sdk+b'\nstatic SDE_ERR jp_game_qsound_result=SDE_ERR_NO_INIT;\n'+b'\n'.join(bodies.values())+b'\n'+observer.read_bytes()
    generated=d/'external_game_sound.c';generated.write_bytes(source)
    owner=(base/'sound_owner_clock.c').read_bytes();assert owner.count(b'#include "jp_native_game_sound.h"')==1
    owner=owner.replace(b'#include "jp_native_game_sound.h"',b'#include "jp_native_game_sound.h"\n#include "jp_native_qsound.h"')+b'\n'+(p/'native/jp_native_qsound.inc').read_bytes()
    (d/'sound_owner_qsound.c').write_bytes(owner)
    sdk_cmd=list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1])==base/'external_game_sound.c'))
    owner_cmd=list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1])==base/'sound_owner_clock.c'))
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    commands=[]
    for cmd in (sdk_cmd,owner_cmd,link):
        cmd=[x.replace(str(base),str(d)) for x in cmd]
        if '-c' in cmd and Path(cmd[cmd.index('-c')+1]).name=='sound_owner_clock.c':cmd[cmd.index('-c')+1]=str(d/'sound_owner_qsound.c')
        if '-shared' in cmd:cmd.insert(4,'-Wl,/export:sdQsndSetPos')
        commands.append(cmd)
    # Actual32bit compiler lays out original records; assertions and constants
    # feed the unhooked original-instruction comparisons, not native pointers.
    preamble=raw[:raw.index(b'void sound_system_init(void)\n{')]
    layout=d/'qsound_layout32.c';layout.write_bytes(b'#include "shinobi/sd_work.h"\n'+preamble+b'''
_Static_assert(sizeof(void *)==4 && sizeof(long)==4,"guest layout");
_Static_assert(sizeof(struct sd_work)==0x474,"original work layout");
_Static_assert(__builtin_offsetof(struct sd_work,host)==0x158,"original host layout");
_Static_assert(sizeof(struct sound_slot)==52 && __builtin_offsetof(struct sound_slot,qsound)==48,"original slot layout");
const unsigned long qs_layout[]={0x51534c59,sizeof(struct sd_work),__builtin_offsetof(struct sd_work,host),sizeof(struct sd_host_command_entry),sizeof(struct sd_host_command),sizeof(struct sound_slot),__builtin_offsetof(struct sound_slot,id),__builtin_offsetof(struct sound_slot,se),__builtin_offsetof(struct sound_slot,qsound)};
''')
    compiler=sdk_cmd[0];layout_cmd=[compiler,'--target=i686-pc-windows-msvc','-std=c17','-O2','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk'),'-I'+str(u/'src'),'-I'+str(u/'modules'),'-c',str(layout),'-o',str(d/'qsound_layout32.obj')]
    commands.insert(0,layout_cmd);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for key in ('game_sound_DLL','sound_SDK_DLL'):updates[key+'_sha256']=sha(updates[key])
    authored=[p/'native/jp_native_qsound.h',p/'native/jp_native_qsound.inc',observer,Path(__file__).resolve()]
    updates.update(status='original_QSound_SDK_game_producer_and_native_copy_identity_family_linked_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        qsound_baseline_sha256=sha(base/'build_verification.json'),qsound_original_source_sha256=originals,
        qsound_original_function_sha256=dict(before,sdQsndSetPos=hashlib.sha256(original_sdk).hexdigest()),
        qsound_adapted_game_function_sha256={n:hashlib.sha256(body).hexdigest() for n,body in bodies.items()},
        qsound_SDK_function_body_unchanged=True,qsound_ID_storage_bits=64,qsound_wire_position_bytes=7,
        qsound_generated_source_sha256=sha(generated),game_sound_generated_source_sha256=sha(generated),qsound_owner_generated_sha256=sha(d/'sound_owner_qsound.c'),
        qsound_guest_layout_source_sha256=sha(layout),qsound_guest_layout_object_sha256=sha(d/'qsound_layout32.obj'),
        qsound_preserved_prior_DLLs=preserved,original_complete_game_queue_or_spatial_streams_qualified=False,
        public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
