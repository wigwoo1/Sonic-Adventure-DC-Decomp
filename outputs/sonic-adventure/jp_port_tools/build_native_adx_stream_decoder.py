"""Reuse actual ADXB/SJD/SPSD/error modules in the qualified joint/audio graph."""
import argparse, hashlib, json, re, shutil, subprocess
from pathlib import Path
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'): ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'))
    p=r/'outputs/sonic-adventure'; b=json.loads((base/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'): shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports'): shutil.copytree(base/n,d/n)
    for n,h in b['generated_sources_sha256'].items():
        src,dst=base/n,d/n
        assert src.resolve().is_relative_to(base) and dst.resolve().is_relative_to(d) and sha(src)==h,n
        dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists(): assert sha(dst)==h,n
        else: shutil.copyfile(src,dst)
    names=['sdk/sofdec/'+n+'.c' for n in ('adx_bsc','adx_sjd','adx_spsd','adx_errs')]
    originals={n:sha(u/n) for n in names}
    for n,h in originals.items():
        assert hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    sources=[]; edits={}
    for n in names:
        raw=(u/n).read_bytes()
        if n.endswith('adx_sjd.c'):
            old=b'    ADXB_EntryGetWrFunc(sjd->adxb, adxsjd_get_wr, sjd);\n'
            assert raw.count(old)==1
            raw=raw.replace(old,b'')
            marker=b'    if (sjd->adxb == NULL) {\n        return NULL;\n    }\n'
            assert raw.count(marker)==1
            raw=raw.replace(marker,marker+old)
            assert raw.count(b'memset(sjd, 0, sizeof(ADXSJD));')==1
            raw=raw.replace(b'memset(sjd, 0, sizeof(ADXSJD));',b'memset(sjd, 0, 4); /* original four-byte record prefix, not native pointer size */')
            edits[n]=['Register get-writer only after actual ADXB allocation succeeds; failed creation returns NULL without a null-address write.',
                      'Preserve the original four-byte Destroy prefix clear; native pointers are eight bytes.']
        if n.endswith(('adx_sjd.c','adx_bsc.c')):
            pattern=rb'#define ADX_SWAP16\(x\)[^\n]*'
            assert len(re.findall(pattern,raw))==1
            raw=re.sub(pattern,b'#define ADX_SWAP16(x) (((((uint32_t)(x))&0xffu)<<8)|((((uint32_t)(x))>>8)&0xffu))',raw)
            edits.setdefault(n,[]).append('Define byte swapping with unsigned shifts, including signed input halfwords.')
        # Original GCC repairs for invalid SPSD codes are selected only after
        # SDK declarations, so native Clang never parses SH-4 inline assembly.
        prefix=b'#include <stdint.h>\n'
        if n.endswith(('adx_spsd.c','adx_errs.c')): prefix+=b'#include <sofdec.h>\n#define GCC\n'
        src=d/('external_'+Path(n).name); src.write_bytes(prefix+raw); sources.append(src)
    row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name=='external_adx_joints.c')
    common=[x.replace(str(base),str(d)) for x in row[:row.index('-c')]]
    commands=[]
    for src in sources:
        commands.append(common+['-c',str(src),'-o',str(src.with_suffix('.obj'))])
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link]
    exports=sorted(set(n.decode() for src in sources for n in re.findall(
        rb'^(?:void|ADX(?:B|SJD|PD)|Sint(?:16|32)\s*\*?|char\s*\*|void\s*\*)\s*((?:ADXB_|ADXSJD_|ADXERR_|ADX_DecodeInfo|nlsndStream|adxb_|adxsjd_|memcpy2)[A-Za-z0-9_]*)\(',src.read_bytes(),re.M)))
    link[4:4]=[str(x.with_suffix('.obj')) for x in sources]+['-Wl,/export:'+n for n in exports]
    commands.append(link); results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True); log=d/f'stream_decoder_build_{i}.txt'; log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'stream_decoder_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={}; preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'],key
            preserved[key]=dict(path=value,sha256=sha(value)); updates[key]=str(d/Path(value).name); updates[key+'_sha256']=sha(updates[key])
    updates.update(status='genuine_ADXB_SJD_SPSD_error_modules_linked_in_original_joint_audio_graph_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),
        stream_decoder_baseline_sha256=sha(base/'build_verification.json'),stream_decoder_original_source_sha256=originals,
        stream_decoder_generated_source_sha256={src.name:sha(src) for src in sources},stream_decoder_source_adaptations=edits,
        stream_decoder_exports=exports,stream_decoder_preserved_prior_DLLs=preserved,
        full_ADXT_GDFS_RNA_PCM_streaming_or_playable_game_verified=False,public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],len(exports),'additional real SDK exports',flush=True)
if __name__=='__main__': main()
