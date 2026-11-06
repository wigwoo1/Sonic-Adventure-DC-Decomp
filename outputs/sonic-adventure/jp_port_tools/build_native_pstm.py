"""Reuse the entire pinned PCM port family in the actual shared SDK owner."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','binary','output'): ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output')); p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    b=json.loads((base/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    assert sha(base/'external_game_sound.c')==b['sound_math_generated_source_sha256']
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'): shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports'): shutil.copytree(base/n,d/n)
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    # The matching project extracts this unused four-byte object from the
    # supplied image. Keep the real bytes private, rather than inventing data.
    blob=a.binary.read_bytes()[0x8c6880b4-0x8c010000:0x8c6880b8-0x8c010000]
    assert len(blob)==4
    data=d/'inputs/shinobi/sd_midi_get_total_beat_time/dat_8c6880b4.inc'
    data.parent.mkdir(parents=True,exist_ok=True)
    data.write_text(','.join(hex(x) for x in blob)+',\n')
    for n,h in b['generated_sources_sha256'].items():
        src,dst=base/n,d/n
        assert src.resolve().is_relative_to(base) and dst.resolve().is_relative_to(d) and sha(src)==h,n
        dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists(): assert sha(dst)==h,n
        else: shutil.copyfile(src,dst)
    pstm_sources=sorted((u/'sdk/shinobi').glob('sd_pstm_*.c')); assert len(pstm_sources)==25
    # The real stream GetStat deliberately names sdMidiGetStat in its error
    # trap. Keep that identity and recover the genuine MIDI readback family.
    midi_sources=sorted((u/'sdk/shinobi').glob('sd_midi_get*.c')); assert len(midi_sources)==11
    sources=pstm_sources+midi_sources
    originals={src.relative_to(u).as_posix():sha(src) for src in sources}
    for n,h in originals.items():
        assert hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    prefix=b'''
/* Driver status offsets are32-bit sound-RAM tokens, never host pointers. */
#undef SD_PSTM_PORT_STATUS
#define SD_PSTM_PORT_STATUS(work,number,ch,field) ((Sint32)((uint32_t)(work)->sound_status + (uint32_t)offsetof(struct sd_sound_status,pstm) + (uint32_t)(number)*SDD_PSTM_PORT_CH_MAX*(uint32_t)sizeof(struct sd_ch_status) + (uint32_t)(ch)*(uint32_t)sizeof(struct sd_ch_status) + (uint32_t)offsetof(struct sd_ch_status,field)))
#undef SD_MIDI_STATUS
#define SD_MIDI_STATUS(work,handle,field) ((Sint32)((uint32_t)(work)->sound_status + (uint32_t)((struct sd_port *)(handle))->number*(uint32_t)sizeof(struct sd_ch_status) + (uint32_t)offsetof(struct sd_ch_status,field)))
'''
    pieces=[(base/'external_game_sound.c').read_bytes(),prefix]
    for src in sources:
        raw=src.read_bytes(); macros=re.findall(rb'^#define ([A-Za-z0-9_]+)',raw,re.M)
        undefs=b'\n'.join(b'#undef '+n for n in macros)
        if src.name=='sd_pstm_open_port.c':
            # Win64's compiler builtin advances ABI argument slots, independent
            # of the final named byte parameter's promoted type. Only this
            # unchanged SDK entry uses the byte-before-varargs convention.
            raw=b'#pragma clang diagnostic push\n#pragma clang diagnostic ignored "-Wvarargs"\n'+raw+b'\n#pragma clang diagnostic pop\n'
        pieces.extend([b'\n#undef SOURCE\n'+undefs+b'\n',raw,b'\n'+undefs+b'\n'])
    observer=p/'jp_port_tools/pstm_observation.inc'; pieces.append(observer.read_bytes())
    generated=d/'external_game_sound.c'; generated.write_bytes(b'\n'.join(pieces))
    exports=sorted(set(n.decode() for src in sources for n in re.findall(rb'^SDE_ERR (sd(?:Pstm|Midi)[A-Za-z0-9_]+)\(',src.read_bytes(),re.M)))
    row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name=='external_game_sound.c')
    compile_=[x.replace(str(Path(row[row.index('-c')+1]).parent),str(d)) for x in row]
    compile_.insert(1,'-I'+str(d/'inputs'))
    compile_.insert(1,'-fwrapv')
    compile_[compile_.index('-c')+1]=str(generated)
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link]; link[4:4]=['-Wl,/export:'+n for n in exports]
    commands=[]
    for i,cmd in enumerate((compile_,link)):
        run=subprocess.run(cmd,capture_output=True); log=d/f'pstm_build_{i}.txt'; log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'pstm_compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={}; preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'],key
            preserved[key]=dict(path=value,sha256=sha(value)); updates[key]=str(d/Path(value).name); updates[key+'_sha256']=sha(updates[key])
    updates.update(status='entire_original_PCM_stream_port_family_linked_in_shared_audio_owner_unverified',commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in (Path(__file__).resolve(),observer)}),
        pstm_baseline_sha256=sha(base/'build_verification.json'),pstm_original_source_sha256=originals,
        pstm_generated_source_sha256=sha(generated),pstm_exports=exports,pstm_preserved_prior_DLLs=preserved,
        pstm_private_matching_data_sha256={data.relative_to(d).as_posix():sha(data)},pstm_matching_binary_sha256=sha(a.binary),
        pstm_upstream_GCC_mono_right_pan_and_bounded_stop_all_repairs_selected=True,
        pstm_native_driver_status_offsets=True,pstm_Win64_native_varargs_builtin=True,
        full_ADXT_RNA_PCM_streaming_or_playable_game_verified=False,public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    # Several historical verifier fields designate the same generated sound TU.
    # Preserve their actual new hash, with the original source retained in base.
    for key in ('game_sound_generated_source_sha256','qsound_generated_source_sha256','spatial_generated_source_sha256','sound_math_generated_source_sha256'):
        if key in b: updates[key]=sha(generated)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],len(exports),'real PCM port exports',flush=True)
if __name__=='__main__': main()
