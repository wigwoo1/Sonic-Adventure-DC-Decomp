"""Original effects queue/banks in the existing native sound translation unit.

Only generated private copies contain upstream bodies. Previously qualified
source/proofs and build directories are immutable inputs. No queue, firmware,
MIDI, file or stage-return stubs are supplied.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(base/'external_game_sound.c')==b['game_sound_generated_source_sha256']
    originals={}
    for n in ('src/sound/sound.c','src/sound/sound.h','src/stage/stage.c','src/progress/progress.c','src/player/player.c','src/system/timer.c','src/system/frame.c','src/main.c','sdk/sofdec/mw_sdbuf.c','sdk/sofdec/mw_sdcs.c'):
        originals[n]=sha(u/n);assert originals[n]==hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest(),n
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports','rna_inputs','adxt_inputs'):shutil.copytree(base/n,d/n)
    for n,h in b['generated_sources_sha256'].items():
        src,dst=base/n,d/n;assert sha(src)==h and dst.resolve().is_relative_to(d);dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists():assert sha(dst)==h
        else:shutil.copyfile(src,dst)
    raw=(u/'src/sound/sound.c').read_bytes();source=(base/'external_game_sound.c').read_bytes()
    # Extend only this new generated SDK copy. Its complete original MLT
    # preflight must also protect actual currently owned PCM rings.
    boundary=function(source,'jp_sd_download_multi')
    anchor=b'            bank->address=record.address;bank->size=record.size;'
    assert boundary.count(anchor)==1
    source=source.replace(boundary,boundary.replace(anchor,b'            if(jp_effects_active_ring_conflict(record.address,record.size))return SDE_ERR_PRM_OVER_RANGE;\n'+anchor))
    source=b'int jp_effects_active_ring_conflict(long,long);\n'+source
    names=re.findall(rb'^(?:static )?(?:void|int|Bool)\s+(\w+)\([^;{}]*\)\n\{',raw,re.M)
    pending=[]
    for name in names:
        name=name.decode()
        if name in ('sound_system_init','sound_set_space','sound_set_qsound','sound_fx_zone_step','sound_orbit_step'):continue
        if not re.search(rb'^[A-Za-z_][^;{}\n]*\b'+name.encode()+rb'\([^;{}]*\)\n\{',source,re.M):pending.append(name)
    bodies={n:function(raw,n) for n in pending};original_body_hash={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()};changes=[]
    def edit(n,old,new,count=1):
        old,new=old.encode(),new.encode();assert bodies[n].count(old)==count,(n,old,bodies[n].count(old));bodies[n]=bodies[n].replace(old,new);changes.append(dict(function=n,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest(),count=count))
    for n in pending:
        bodies[n]=re.sub(rb'\bint id\b',b'intptr_t id',bodies[n])
    # Preserve the original authoritative state definitions, not stand-in inputs.
    state=[];state_provenance={}
    for unit,patterns in {
        'src/progress/progress.c':[rb'^int battle_rival = -1;\n'],
        'src/player/player.c':[rb'^struct task_work \*player_work\[PLAYER_COUNT\];\n'],
        'src/system/timer.c':[rb'^int debug_page = 0;\n'],
        'src/stage/stage.c':[rb'^Sint16 '+n.encode()+rb';\n' for n in ('stage_previous','act_previous','stage_current','act_current','stage_next','act_next')]
    }.items():
        unitraw=(u/unit).read_bytes()
        for pattern in patterns:
            hits=re.findall(pattern,unitraw,re.M);assert len(hits)==1,(unit,pattern)
            decl=hits[0];state.append(decl);state_provenance[decl.decode().strip()]=dict(path=unit,sha256=hashlib.sha256(decl).hexdigest())
    stage=(u/'src/stage/stage.c').read_bytes();stage_units=('stage_keep_previous','stage_set_stage_act','stage_set_next_stage_act','stage_get_stage_act','stage_get_previous_stage_act','stage_get_next_stage_act')
    for n in stage_units:state.append(function(stage,n))
    # Every missing static helper gets its original native-ID prototype before use.
    prototypes=[]
    for n in pending:
        body=bodies[n]
        if body.startswith(b'static '):prototypes.append(body[:body.index(b'\n{')]+b';\n')
    # The real GDFS reads sectors; VirtualAlloc supplies alignment and guards.
    # Keep file reads synchronous and close every genuine handle on every path.
    for n,call in [('sound_load_file','sound_send_multi_unit(data)'),('sound_load_shot_bank','sound_send_shot_bank(data, bank, size)'),('sound_load_sequence_bank','sound_send_sequence_bank(data, bank, size)'),('sound_load_program_bank','sound_send_program_bank(data, bank, size)')]:
        body=bodies[n];start=body.index(b'    Sint32 size;');end=body.rindex(b'}')
        bodies[n]=body[:start]+('''    Sint32 size=0;
    void *data=jp_effects_read_bank(name,&size);
    if(!data)return;
    %s;
    jp_effects_stage_complete(jp_rna_sdk_pending(),(uint32_t)size);
'''%call).encode()+body[end:];changes.append(dict(function=n,reason='bounded owned GDFS staging replaces fixed WORK_AREA; release only after actual SDK/G2 drain'))
    for n in ('sound_send_multi_unit','sound_send_shot_bank','sound_send_sequence_bank','sound_send_program_bank'):
        if n=='sound_send_multi_unit':
            edit(n,'sdMemBlkSetPrm(block, data, 0,','sdMemBlkSetPrm(block, data, jp_effects_bank_bytes,')
            edit(n,'error = sdMultiUnitDownload(block);','error = jp_sd_download_multi(block);')
        else:
            edit(n,'SDD_MEMBLK_NO_FUNC','SDD_MEMBLK_SYNC_FUNC')
            kind={'sound_send_shot_bank':'SHOT_BANK','sound_send_sequence_bank':'MIDI_SEQ_BANK','sound_send_program_bank':'MIDI_PRG_BANK'}[n]
            edit(n,'    block = NULL;','    block = NULL;\n    if(!jp_effects_single_bank_valid(data,size,SDE_DATA_TYPE_'+kind+',bank)){error=SDE_ERR_PRM_OVER_RANGE;goto done;}')
        edit(n,'done:\n    if (block != NULL) {','''done:
    jp_effects_error(error);
    for(int drain=0;jp_rna_sdk_pending()&&drain<256;drain++){
        if(jp_effects_error(sdSysServer()))break;
    }
    if(jp_rna_sdk_pending()){jp_effects_error(SDE_ERR_UNKNOW_NUM);return;}
    if (block != NULL) {''')
        edit(n,'        sdMemBlkDestroy(block);','        jp_effects_error(sdMemBlkDestroy(block));')
    # Capture original SDK failures previously discarded by void game routines.
    for n in pending:
        if n.startswith('sound_send_') and n!='sound_send_queue':continue
        for call in set(re.findall(rb'(?m)^    +(sd\w+\([^;]+\));',bodies[n])):
            bodies[n]=bodies[n].replace(call+b';',b'if(jp_effects_error('+call+b'))return;')
    prefix=b'''
#ifndef GCC
#define GCC
#endif
#include "gcc/src/system/port.h"
#include <shinobi/gdfs.h>
#include "jp_native_effects.h"
extern int jp_effects_error(long),jp_effects_failed(void);
extern void *jp_effects_stage_allocate(long);
extern int jp_effects_stage_complete(int,uint32_t);
extern void jp_effects_file_error(void);
extern void jp_effects_pending_file(void);
extern int jp_rna_sdk_pending(void);
static Sint32 jp_effects_bank_bytes;
static GDFS jp_effects_file;
_Static_assert(sizeof(GDFS_HANDLE)==120,"shared native GDFS bank handle layout");
int jp_effects_file_close_owned(void)
{
    if(!jp_effects_file)return 0;
    gdFsClose(jp_effects_file);
    if(((GDFS_HANDLE *)jp_effects_file)->used)return 1;
    jp_effects_file=NULL;return 0;
}
static void *jp_effects_read_bank(const char *name,Sint32 *size)
{
    if(jp_effects_failed())return NULL;
    GDFS file=gdFsOpen(name,NULL);
    if(!file){jp_effects_file_error();return NULL;}
    jp_effects_file=file;
    if(!gdFsGetFileSize(file,size)||*size<32||*size>2*1024*1024){
        if(jp_effects_file_close_owned())jp_effects_pending_file();else jp_effects_file_error();return NULL;
    }
    void *data=jp_effects_stage_allocate(*size);
    if(!data){if(jp_effects_file_close_owned())jp_effects_pending_file();return NULL;}
    int error=gdFsRead(file,SECTORS(*size),data);
    if(jp_effects_file_close_owned()){jp_effects_pending_file();return NULL;}
    if(error){jp_effects_file_error();jp_effects_stage_complete(0,0);return NULL;}
    jp_effects_bank_bytes=*size;return data;
}
'''
    generated=d/'external_game_sound.c';generated.write_bytes(source+b'\n'+(p/'jp_port_tools/effects_memory_boundary.inc').read_bytes()+prefix+b'\n'.join(state)+b'\n'.join(prototypes)+b'\n'.join(bodies.values())+b'\n'+(p/'jp_port_tools/effects_observation.inc').read_bytes())
    buffers=d/'external_mw_sdbuf.c'
    buffers.write_bytes(buffers.read_bytes()+b'''\nuint32_t jp_effects_active_stream_buffers(void){uint32_t mask=0;for(unsigned i=0;i<MWSNDBUF_MAX;i++)if(mwg_snd_buf[i].used)mask|=1u<<(15-mwg_snd_buf[i].no);return mask;}\n''')
    ports=d/'external_mw_sdcs.c';port_source=ports.read_bytes();body=function(port_source,'mwSndOpenPort');at=body.index(b'{')+1
    changed=body[:at]+b'\n    if(!jp_effects_ring_reservation_allowed(nch)){mwRnaErrCallFunc(TEXT_4);return NULL;}'+body[at:]
    ports.write_bytes(b'int jp_effects_ring_reservation_allowed(int);\n'+port_source.replace(body,changed))
    owner=(base/'sound_owner_stream.c').read_bytes()
    marker=b'static int32_t frame_service(void *context)';assert owner.count(marker)==1
    owner=owner.replace(marker,b'#include "jp_native_effects.h"\nstatic int jp_effects_stage_free(void);\nstatic int jp_effects_outside(const void *,uint64_t);\nextern int jp_effects_file_close_owned(void);\nstatic int32_t frame_effects(void *);\n'+marker)
    marker=b'static int cleanup(uint32_t timeout)\n{';assert owner.count(marker)==1
    owner=owner.replace(marker,marker+b'\n    if(jp_effects_file_close_owned())return JP_SOUND_CLEANUP;')
    marker=b'    return p&&bytes<=UINTPTR_MAX-p&&';assert owner.count(marker)==1
    owner=owner.replace(marker,b'    return jp_effects_outside(pointer,bytes)&&p&&bytes<=UINTPTR_MAX-p&&')
    old=b'jp_sound_dispatch_bind(frame_service,NULL,&sound,&sound.info.dispatch_token)';assert owner.count(old)==1;owner=owner.replace(old,b'jp_sound_dispatch_bind(frame_service,frame_effects,&sound,&sound.info.dispatch_token)')
    old=b'    if(sound.info.resources&JP_SOUND_OWNS_SDK){jp_game_sdk_retire();';assert owner.count(old)==1
    owner=owner.replace(old,b'    if(jp_effects_stage_free())return JP_SOUND_MEMORY;\n'+old)
    owned=d/'sound_owner_effects.c';owned.write_bytes(owner+b'\n'+(p/'native/jp_native_effects.inc').read_bytes())
    def command(name):return list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name==name))
    def relocate(cmd):
        result=[]
        for value in cmd:
            value=value.replace(str(base),str(d))
            if value.startswith('-I'):
                for folder in ('include','inputs','sound_imports','rna_inputs','adxt_inputs'):
                    match=re.search(r'[/\\]'+folder+r'([/\\].*)?$',value[2:])
                    if match and str(r/'work/sonic-adventure') in value and not value[2:].startswith(str(u)):
                        target=d/folder
                        if match[1]:target=target/match[1].lstrip('/\\')
                        if target.exists():value='-I'+str(target)
            elif Path(value).suffix.lower() in ('.obj','.lib','.dll') and (d/Path(value).name).exists():value=str(d/Path(value).name)
            result.append(value)
        return result
    sdk=relocate(command('external_game_sound.c'));sdk.insert(1,'-Wno-unused-but-set-variable')
    sdk[sdk.index('-c')+1]=str(generated);sdk[sdk.index('-o')+1]=str(d/'game_sdk.obj')
    own=relocate(command('sound_owner_stream.c'));own[own.index('-c')+1]=str(owned);own[own.index('-o')+1]=str(d/'sound_owner.obj')
    link=relocate(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    link[link.index('-o')+1]=str(d/'jp_native_game_sound.dll')
    # Inherited command rows can reference older frozen stages. Every mutation
    # target must be explicitly inside this newly created private directory.
    for cmd in (sdk,own,link):assert Path(cmd[cmd.index('-o')+1]).is_relative_to(d)
    exports=[n for n in pending if not bodies[n].startswith(b'static ')]+[n for n in stage_units if not n.startswith('stage_keep')]
    # Original raw ABI remains an internal caller-owned contract. New public
    # entry points enforce bounds/ownership; legacy memory download is not exported.
    exports.remove('sound_download')
    link[4:4]=['-Wl,/export:'+n for n in exports]
    # -Wl splits commas, so DATA must remain in one literal linker argument.
    link[4:4]=[arg for n in ('battle_rival','player_work','debug_page','stage_previous','act_previous','stage_current','act_current','stage_next','act_next') for arg in ('-Xlinker','/export:'+n+',DATA')]
    results=[]
    bufcmd=relocate(command('external_mw_sdbuf.c'));bufcmd[bufcmd.index('-c')+1]=str(buffers);bufcmd[bufcmd.index('-o')+1]=str(d/'external_mw_sdbuf.obj')
    portcmd=relocate(command('external_mw_sdcs.c'));portcmd[portcmd.index('-c')+1]=str(ports);portcmd[portcmd.index('-o')+1]=str(d/'external_mw_sdcs.obj')
    for cmd in (bufcmd,portcmd):assert Path(cmd[cmd.index('-o')+1]).is_relative_to(d)
    for i,cmd in enumerate((sdk,own,bufcmd,portcmd,link)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'effects_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'effects_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value));updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    authored=[Path(__file__).resolve(),p/'native/jp_native_effects.h',p/'native/jp_native_effects.inc',p/'jp_port_tools/effects_observation.inc',p/'jp_port_tools/effects_memory_boundary.inc']
    updates.update(status='original_effects_queue_and_owned_file_banks_linked_runtime_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        effects_baseline_sha256=sha(base/'build_verification.json'),effects_original_source_sha256=originals,
        effects_added_functions=pending,effects_exports=exports,effects_original_function_sha256=original_body_hash,
        effects_adapted_function_sha256={n:hashlib.sha256(body).hexdigest() for n,body in bodies.items()},effects_adaptations=changes,
        effects_original_shared_state_provenance=state_provenance,effects_generated_source_sha256=sha(generated),effects_owner_generated_sha256=sha(owned),
        effects_buffers_generated_sha256=sha(buffers),effects_ports_generated_sha256=sha(ports),effects_live_ring_overlap_guards=True,
        rna_generated_sources_sha256=dict(b['rna_generated_sources_sha256'],**{'external_mw_sdbuf.c':sha(buffers),'external_mw_sdcs.c':sha(ports)}),
        effects_preserved_prior_DLLs=preserved,effects_full_game_queue_linked=True,effects_runtime_verified=False,
        combined_SDK_game_binaries_publishable=False,playable=False,overall_completion_percent=10)
    for key in ('game_sound_generated_source_sha256','qsound_generated_source_sha256','spatial_generated_source_sha256','sound_math_generated_source_sha256','pstm_generated_source_sha256'):updates[key]=sha(generated)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],len(pending),'original sound functions added;',len(stage_units),'stage helpers',flush=True)
if __name__=='__main__':main()
