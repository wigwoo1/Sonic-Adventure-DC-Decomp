"""Regenerate genuine middleware includes using the preserved matching listings."""
import argparse,hashlib,json,shutil,subprocess,sys
from pathlib import Path
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
JP='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','matching-build','binary','output'):
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args(); r,u,m,binary,d=(getattr(a,n).resolve() for n in ('workspace','upstream','matching_build','binary','output'))
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and sha(binary)==JP
    sources=['sdk/sofdec/mw_rna.c','sdk/sofdec/mw_sdcs.c']; originals={n:sha(u/n) for n in sources}
    for n,h in originals.items():
        assert hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    before={str(m/'stub'/Path(n).with_suffix(ext)):sha(m/'stub'/Path(n).with_suffix(ext)) for n in sources for ext in ('.lis','.d')}
    canonical={}
    for n in ('sdk/sofdec/mw_rna','sdk/sofdec/mw_sdcs'):
        for src in sorted((m/'gen'/n).glob('*.inc')): canonical[str(src)]=sha(src)
    assert len(canonical)==4
    d.mkdir(); commands=[]
    for n in sources:
        listing=m/'stub'/Path(n).with_suffix('.lis'); saved=d/'matching-listings'/Path(n).with_suffix('.lis')
        saved.parent.mkdir(parents=True,exist_ok=True); shutil.copyfile(listing,saved)
        shutil.copyfile(listing.with_suffix('.d'),saved.with_suffix('.d'))
        target=d/'includes'/Path(n).with_suffix('').relative_to('sdk')
        cmd=[sys.executable,'-B',str(u/'tools/data.py'),'--bodies',str(binary),n,str(saved),str(target),'1st_read']
        run=subprocess.run(cmd,cwd=u,capture_output=True); log=d/(Path(n).stem+'.log'); log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'attempt.json').write_text(json.dumps(commands,indent=2)+'\n')
        assert run.returncode==0 and not log.stat().st_size,(str(log),log.read_text())
    generated={}
    for src,h in canonical.items():
        original=Path(src); relative=original.relative_to(m/'gen'/'sdk'); target=d/'includes'/relative
        assert sha(target)==h,relative
        generated[relative.as_posix()]=dict(sha256=h,bytes=target.stat().st_size,matching_source=src)
    for src,h in before.items(): assert sha(src)==h,src
    for src,h in canonical.items(): assert sha(src)==h,src
    for n,h in originals.items(): assert sha(u/n)==h,n
    result=dict(status='genuine_RNA_mwSnd_private_includes_regenerated_match_preserved_Japanese_build',
        generated=generated,original_binary_sha256=JP,original_source_sha256=originals,
        matching_listing_sha256=before,matching_generated_includes_sha256=canonical,
        extraction_tool_sha256=sha(u/'tools/data.py'),commands=commands,original_matching_files_unchanged=True,
        authored_source_sha256={Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)},
        placeholder_texts_used=False,retail_data_publishable=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(generated),'private includes',flush=True)
if __name__=='__main__': main()
