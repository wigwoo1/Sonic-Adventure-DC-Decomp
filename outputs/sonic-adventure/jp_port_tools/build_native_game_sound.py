"""Owned native startup from the real game's initialization closure and SDK.

Whole-module unresolved dependencies remain documented by audit_native_game_sound.
No unrelated gameplay/stream/math imports are replaced with success stubs.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
UNITS=('sound_system_init','sound_init_driver','midi_open_ports','sound_set_space',
       'sound_reset','sound_set_fx_01','sound_set_fx_23','qsound_reset','midi_reset_all','midi_reset')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','sdk','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();inc=d/'include';(inc/'sound').mkdir(parents=True)
    b=read(a.baseline/'build_verification.json');s=read(a.sdk/'build_verification.json')
    assert sha(s['DLL'])==s['DLL_sha256'];sdk_source=a.sdk/'external_sd_commands.c'
    assert sha(sdk_source)==s['generated_source_sha256'] and sha(a.sdk/'include/shinobi.h')==s['private_header_sha256']
    shutil.copyfile(a.sdk/'include/shinobi.h',inc/'shinobi.h')
    originals={n:sha(u/n) for n in ('src/sound/sound.c','src/sound/sound.h')};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    header=(u/'src/sound/sound.h').read_bytes();assert len(re.findall(rb'\bint id\b',header))==10
    (inc/'sound/sound.h').write_bytes(b'#include <stdint.h>\n'+re.sub(rb'\bint id\b',b'intptr_t id',header))
    raw=(u/'src/sound/sound.c').read_bytes();preamble=raw[:raw.index(b'void sound_system_init(void)\n{')]
    preamble=preamble.replace(b'    Sint32 id;',b'    intptr_t id;')
    # Keep the real module's single global state; retain only used prototypes.
    preamble=re.sub(rb'^static [^;{}\n]*?\b(\w+)\([^;{}]*\);\n',lambda m:m[0] if m[1].decode() in UNITS else b'',preamble,flags=re.M)
    bodies={n:function(raw,n) for n in UNITS};before={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()};changes=[]
    def replace(n,old,new,reason):
        assert bodies[n].count(old)==1,(n,reason,bodies[n].count(old));bodies[n]=bodies[n].replace(old,new)
        changes.append(dict(function=n,reason=reason,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()))
    n='sound_init_driver'
    replace(n,b'    file = gdFsOpen("masaru.drv", NULL);',b'''    file = gdFsOpen(jp_sound_driver_name(), NULL);
    if (!file) {jp_sound_problem(JP_SOUND_FILE,JP_SOUND_OWNS_FILE,0);return;}
    jp_sound_adopt_file(file);''','owned configured driver filename and real open failure')
    replace(n,b'    gdFsGetFileSize(file, &size);',b'''    if (!gdFsGetFileSize(file,&size)) {jp_sound_problem(JP_SOUND_FILE,JP_SOUND_OWNS_FILE,0);return;}
    if (jp_sound_stage_create(size)) return;''','validate actual file size before allocating guarded sector-rounded staging')
    replace(n,b'    gdFsRead(file, sectors, WORK_AREA);',b'    if (gdFsRead(file,sectors,jp_sound_stage_data())!=0) {jp_sound_problem(JP_SOUND_FILE,JP_SOUND_OWNS_FILE,0);return;}','actual GDFS read into owned native staging')
    replace(n,b'    gdFsClose(file);',b'''    gdFsClose(file);jp_sound_release_file();
    if (jp_sound_validate_driver(size)) return;
    jp_sound_owns_sdk();''','close owned file and validate header bounds before real SDK work')
    replace(n,b'    sdMemBlkSetTransferMode(SDE_MEMBLK_TRANSFER_MODE_DMA);',b'    if (jp_sound_record_sdk(sdMemBlkSetTransferMode(SDE_MEMBLK_TRANSFER_MODE_DMA))) return;','propagate actual mode failure')
    replace(n,b'sdMemBlkSetPrm(block, WORK_AREA,',b'sdMemBlkSetPrm(block, jp_sound_stage_data(),','replace fixed console address only')
    replace(n,b'''    if (block != NULL) {
        sdMemBlkDestroy(block);
    }
    sdDrvInit(block);
    njSetVSyncFunction(frame_vsync);''',b'''    if (error!=SDE_ERR_NOTHING) jp_sound_record_sdk(error);
    if (block != NULL && jp_sound_record_sdk(sdMemBlkDestroy(block))) return;
    if (jp_sound_failed()) return;
    if (jp_sound_record_sdk(sdDrvInit(block))) return;
    njSetVSyncFunction(frame_vsync);
    if (jp_wait_status()) jp_sound_problem(JP_SOUND_DEPENDENCY,0,0);''','retain original successful released-slot second initialization; quarantine errors and actual wait failure')
    for n in UNITS:
        if n=='sound_init_driver':continue
        # Original SDK producers return real errors, previously ignored by void game functions.
        for indent,call in re.findall(rb'^( +)(sd(?:MidiOpenPort|MidiSetMes|MidiSendMes|SndSetSpace|SndSetFxPrg|SndSetFxOutPrm)\([^;]+\));',bodies[n],re.M):
            replace(n,indent+call+b';',indent+b'if (jp_sound_record_sdk('+call+b')) return;','propagate actual original SDK producer result')
        for callee in UNITS:
            if callee in ('sound_system_init','sound_set_space') and callee==n:continue
            pattern=rb'^(    +'+callee.encode()+rb'\([^;]+?\)|    +'+callee.encode()+rb'\(\));'
            for call in re.findall(pattern,bodies[n],re.M):
                replace(n,call+b';',call+b';\n    if (jp_sound_failed()) return;','stop void original startup closure after dependent failure')
    n='sound_set_space'
    # Use a bounded retry count for native synchronous config, with explicit failure.
    replace(n,b'            break;\n        }\n    }\n    tries = 0;',b'            jp_sound_problem(JP_SOUND_CONFIG,0,0);return;\n        }\n    }\n    tries = 0;','native configuration errors cannot masquerade as successful default mode')
    replace(n,b'            mode = SYD_CFG_STEREO;\n            break;',b'            syCfgExit();jp_sound_problem(JP_SOUND_CONFIG,0,0);return;','stop on missing actual config instead of implicit stereo')
    replace(n,b'\n    syCfgExit();',b'\n    if (syCfgExit()!=SYD_CFG_OK) jp_sound_problem(JP_SOUND_CONFIG,0,0);','record actual session-close error')
    declarations=b'''
#include "jp_native_game_sound.h"
extern int jp_sound_failed(void);
extern const char *jp_sound_driver_name(void);
extern void jp_sound_problem(int,uint32_t,uint32_t);
extern int jp_sound_record_sdk(long);
extern void jp_sound_owns_sdk(void);
extern void jp_sound_adopt_file(void *);
extern void jp_sound_release_file(void);
extern void *jp_sound_stage_data(void);
extern int jp_sound_stage_create(long);
extern int jp_sound_validate_driver(long);
extern int jp_wait_status(void);
#define sound_system_init jp_game_sound_system_init
#define sound_set_space jp_game_sound_set_space
'''
    prefix=b'#include <stdint.h>\n#include <ninja.h>\n#include <shinobi.h>\n#include <machine.h>\n'
    bridge=p/'jp_port_tools/game_sound_owner_bridge.inc';generated=d/'external_game_sound.c'
    generated.write_bytes(prefix+sdk_source.read_bytes()+declarations+preamble+b'\n#undef CFG_TRIES\n#define CFG_TRIES 8\n'+b'\n'.join(bodies.values())+b'\n'+bridge.read_bytes())
    compiler=s['commands'][0]['command'][0];flags=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter','-Wno-unused-variable','-include',str(p/'native/jp_host_types.h'),'-I'+str(inc),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk'),'-I'+str(u/'src'),'-I'+str(u/'modules')]
    owner=p/'native/jp_native_game_sound.c';header=owner.with_suffix('.h')
    exports=sorted(set(re.findall(r'-Wl,/export:(\w+)',str(s['commands'][1]['command'])))|{'syCfgInit','syCfgExit','syCfgGetSoundMode','syCfgSetSoundMode'})
    commands=[flags+['-c',str(generated),'-o',str(d/'game_sdk.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(owner),'-o',str(d/'sound_owner.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'game_sdk.obj'),str(d/'sound_owner.obj'),str(a.sdk/'jp_native_g2_audio.lib'),str(a.sdk/'jp_native_aica.lib'),str(Path(b['present_DLL']).with_suffix('.lib')),str(Path(b['GDFS_DLL']).with_suffix('.lib')),*['-Wl,/export:'+n for n in exports],'-Wl,legacy_stdio_definitions.lib','-o',str(d/'jp_native_game_sound.dll')]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            if key=='sound_SDK_DLL':continue
            path=Path(value);assert sha(path)==b[key+'_sha256'];target=d/path.name
            shutil.copyfile(path,target)
            if path.with_suffix('.lib').exists():shutil.copyfile(path.with_suffix('.lib'),target.with_suffix('.lib'))
            updates[key]=str(target);updates[key+'_sha256']=sha(target);preserved[key]=dict(path=str(path),sha256=sha(path))
    for name,h in s['dependencies'].items():
        assert sha(a.sdk/name)==h;shutil.copyfile(a.sdk/name,d/name)
    for name,h in b['generated_sources_sha256'].items():
        path=a.baseline/name;assert sha(path)==h;target=d/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
    shutil.copytree(a.baseline/'inputs',d/'inputs')
    authored=(owner,header,bridge,Path(__file__).resolve())
    updates.update(status='owned_original_game_sound_constructor_and_native_config_linked_unverified',commands=results+b['commands'],
        game_sound_DLL=str(d/'jp_native_game_sound.dll'),game_sound_DLL_sha256=sha(d/'jp_native_game_sound.dll'),
        sound_SDK_DLL=str(d/'jp_native_game_sound.dll'),sound_SDK_DLL_sha256=sha(d/'jp_native_game_sound.dll'),
        sound_owner_source_sha256=sha(owner),sound_owner_header_sha256=sha(header),sound_owner_builder_sha256=sha(__file__),
        sound_owner_baseline_sha256=sha(a.baseline/'build_verification.json'),sound_owner_SDK_baseline_sha256=sha(a.sdk/'build_verification.json'),
        game_sound_original_source_sha256=originals,game_sound_initialization_functions=list(UNITS),
        game_sound_original_function_sha256=before,game_sound_adapted_function_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},
        game_sound_native_adaptations=changes,game_sound_generated_source_sha256=sha(generated),
        preserved_prior_sound_owner_DLLs=preserved,original_game_sound_constructor_or_queue_implemented=True,
        original_game_sound_constructor_implemented=True,
        original_full_game_sound_queue_implemented=False,global_audio_exit_or_sustained_clock_pacing_qualified=False,
        public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    updates['authored_source_sha256']=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored})
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],len(UNITS),'actual game functions',flush=True)
if __name__=='__main__':main()
