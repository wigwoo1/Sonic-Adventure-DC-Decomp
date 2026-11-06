"""Original RNA/mwSnd output integrated into the existing sound service/cleanup."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path): return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','inputs','output'): ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r,u,base,inputs,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','inputs','output')); p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure'); b=read(base/'build_verification.json'); fixtures=read(inputs/'verification.json')
    assert not fixtures['placeholder_texts_used'] and fixtures['original_matching_files_unchanged']
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    for n,row in fixtures['generated'].items(): assert sha(inputs/'includes'/n)==row['sha256'],n
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'): shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports'): shutil.copytree(base/n,d/n)
    shutil.copytree(inputs/'includes',d/'rna_inputs')
    for n,h in b['generated_sources_sha256'].items():
        src,dst=base/n,d/n; assert src.resolve().is_relative_to(base) and dst.resolve().is_relative_to(d) and sha(src)==h,n
        dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists(): assert sha(dst)==h,n
        else: shutil.copyfile(src,dst)
    names=['sdk/sofdec/'+n+'.c' for n in ('mw_sdbuf','mw_rnaer','mw_sdcs','mw_rna','adx_rnas')]
    originals={n:sha(u/n) for n in names}
    for n,h in originals.items():
        assert hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    prefix=b'''#include <stdint.h>
#include <machine.h>
#include <shinobi.h>
#include <stddef.h>
#define GCC
#define JP_G2_API __declspec(dllimport)
#include "jp_native_g2_audio.h"
#define _builtin_get_imask() jp_g2_gate_get()
#define _builtin_set_imask(level) ((void)jp_g2_gate_set(level))
'''
    generated=[]; adaptations={}
    for n in names:
        raw=(u/n).read_bytes(); src=d/('external_'+Path(n).name)
        if n.endswith('mw_sdcs.c'):
            body=function(raw,'mwSndOpenPort'); old=b'mwSndBufDestroy(snd->buf[i]);'
            assert body.count(old)==1
            raw=raw.replace(body,body.replace(old,b'mwSndBufDestroy(snd->buf[j]);'))
            adaptations[n]=['Failed real PCM-port allocation releases each buffer j, rather than out-of-range buf[i].']
            raw+=b'''
__declspec(dllexport) int peer_mwsnd_counts(uint32_t *out){if(!out)return 1;out[0]=0;for(int i=0;i<MWSND_MAX_OBJ;i++)out[0]+=mwg_snd_obj[i].used!=0;out[1]=sizeof(MwSnd);out[2]=offsetof(MwSnd,port);return 0;}
'''
        if n.endswith('mw_sdbuf.c'):
            raw+=b'''
__declspec(dllexport) int peer_sndbuf_used(void){int count=0;for(int i=0;i<MWSNDBUF_MAX;i++)count+=mwg_snd_buf[i].used!=0;return count;}
'''
        if n.endswith('mw_rna.c'):
            for name in ('mwg_rna_zero','mwg_rna_zero_adpcm'):
                old=('static Sint32 '+name+'[').encode();assert raw.count(old)==1
                raw=raw.replace(old,('static _Alignas(32) Sint32 '+name+'[').encode())
            adaptations[n]=['Align both original silence buffers to32 bytes for the real native SDK DMA contract.']
            raw+=b'\n'+(p/'jp_port_tools/rna_observation.inc').read_bytes()
        src.write_bytes(prefix+raw); generated.append(src)
    sdk=(base/'external_game_sound.c').read_bytes()
    old=b'int32_t jp_game_sdk_service(void){return (int32_t)sdSysServer();}'; assert sdk.count(old)==1
    sdk=sdk.replace(old,b'''extern int jp_rna_service_enabled(void);
extern int jp_rna_clock_error(void);
extern void jp_rna_service_observe(void);
extern void mwRnaExecServer(void);
int32_t jp_game_sdk_service(void)
{
    int32_t error=(int32_t)sdSysServer();
    if(!error&&jp_rna_service_enabled()){
        if(jp_rna_clock_error())return (int32_t)SDE_ERR_UNKNOW_NUM;
        jp_rna_service_observe();
        mwRnaExecServer();
        if(jp_rna_clock_error())return (int32_t)SDE_ERR_UNKNOW_NUM;
        return (int32_t)sdSysServer();
    }
    return error;
}''')
    sdk+=b'\nint jp_rna_sdk_pending(void){return sdgWrkPtr?sdgWrkPtr->queue_num+(sdgWrkPtr->transfer!=NULL):-1;}\n'
    (d/'external_game_sound.c').write_bytes(sdk)
    owner=(base/'sound_owner_qsound.c').read_bytes(); marker=b'static int jp_sound_clock_close_owned(void);'; assert owner.count(marker)==1
    owner=owner.replace(marker,marker+b'\n#include "jp_native_rna.h"\nstatic int jp_rna_close_owned(void);')
    marker=b'    if(sound.file){gdFsClose(sound.file);jp_sound_release_file();}'; assert owner.count(marker)==1
    owner=owner.replace(marker,b'    if(jp_rna_close_owned())return JP_SOUND_CLEANUP;\n'+marker)
    owned=d/'sound_owner_rna.c'; owned.write_bytes(owner+b'\n'+(p/'native/jp_native_rna.inc').read_bytes())
    def row(name): return list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name==name))
    def move(cmd):
        original=Path(cmd[cmd.index('-c')+1]).parent
        return [x.replace(str(original),str(d)) for x in cmd]
    sdkcmd=move(row('external_game_sound.c')); common=sdkcmd[:sdkcmd.index('-c')]
    ownercmd=move(row('sound_owner_qsound.c')); ownercmd[ownercmd.index('-c')+1]=str(owned)
    commands=[sdkcmd,ownercmd]
    for src in generated+[p/'native/jp_native_rna_clock.c']:
        commands.append(common+['-I'+str(d/'rna_inputs'),'-Wno-unused-but-set-variable','-c',str(src),'-o',str(d/(src.stem+'.obj'))])
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link]
    exports=sorted(set(n.decode() for n in re.findall(rb'^(?:void|int|Sint32|MWRNA|MWRNA_OBJ\s*\*|MwSnd\s*\*|void\s*\*)\s*((?:mw|ADXRNA_)[A-Za-z0-9_]+)\(',b'\n'.join(src.read_bytes() for src in generated),re.M)))
    link[4:4]=[str(d/(src.stem+'.obj')) for src in generated+[p/'native/jp_native_rna_clock.c']]+['-Wl,/export:'+n for n in exports]+['-Wl,/export:'+n for n in ('syTmrGetCount','jp_rna_clock_begin','jp_rna_clock_error')]
    commands.append(link); results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True); log=d/f'rna_build_{i}.txt'; log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size)); (d/'rna_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={}; preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'],key
            preserved[key]=dict(path=value,sha256=sha(value)); updates[key]=str(d/Path(value).name); updates[key+'_sha256']=sha(updates[key])
    for key in ('game_sound_generated_source_sha256','qsound_generated_source_sha256','spatial_generated_source_sha256','sound_math_generated_source_sha256','pstm_generated_source_sha256'):
        if key in b: updates[key]=sha(d/'external_game_sound.c')
    authored=[Path(__file__).resolve(),p/'native/jp_native_rna.h',p/'native/jp_native_rna.inc',p/'native/jp_native_rna_clock.c',p/'jp_port_tools/rna_observation.inc']
    updates.update(status='original_RNA_mwSnd_ADXRNA_linked_into_shared_service_and_ordered_cleanup_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{src.relative_to(r).as_posix():sha(src) for src in authored}),
        rna_baseline_sha256=sha(base/'build_verification.json'),rna_matching_inputs_sha256=sha(inputs/'verification.json'),
        rna_original_source_sha256=originals,rna_source_adaptations=adaptations,rna_generated_sources_sha256={src.name:sha(src) for src in generated},
        rna_owner_generated_source_sha256=sha(owned),rna_preserved_prior_DLLs=preserved,rna_exports=exports,
        rna_reuses_existing_frame_clock_SDK_G2_AICA_and_output=True,original_source_legacy_flag_applies_to_joint_stage_only=True,
        production_ADXT_file_voice_streaming_or_gameplay_verified=False,public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],len(exports),'genuine middleware exports',flush=True)
if __name__=='__main__': main()
