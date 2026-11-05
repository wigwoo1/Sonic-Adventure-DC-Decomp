"""Compose qualified shared frame/presentation owners with existing startup.
Reusable projection only: it never replaces or pretends to run missing code.
"""
import argparse,hashlib,json,shutil
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','owner','qualification','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();base=a.baseline.resolve();owner=a.owner.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(base/'build_verification.json');c=read(owner/'build_verification.json');q=read(a.qualification/'verification.json')
    assert q['build_report_sha256']==sha(owner/'build_verification.json') and 'verified' in q['status'] and q['checks']>0
    for n,h in c['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['authored_source_sha256'].items():
        if sha(r/n)==h:continue
        previous=c.get('superseded_authored_source',{}).get(n)
        assert previous and previous['sha256']==h and sha(previous['preserved_path'])==h,n
        assert n in c['authored_source_sha256'] and sha(r/n)==c['authored_source_sha256'][n],n
    for n,h in c['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    for kind in ('frames','present'):assert sha(c[kind+'_DLL'])==c[kind+'_DLL_sha256']
    d.mkdir();generated=dict(b['generated_sources_sha256']);generated.update(c['generated_sources_sha256'])
    for n,h in generated.items():
        source=owner/n if n in c['generated_sources_sha256'] else base/n
        target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target);assert sha(target)==h
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    sources=dict(b['authored_source_sha256']);sources.update(c['authored_source_sha256']);sources[Path(__file__).resolve().relative_to(r).as_posix()]=sha(__file__)
    shared=dict(status='qualified_shared_owner_projected_into_preserved_startup_chain',frames_DLL=c['frames_DLL'],frames_DLL_sha256=c['frames_DLL_sha256'],present_DLL=c['present_DLL'],present_DLL_sha256=c['present_DLL_sha256'],commands=c['commands']+b['commands'],authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=c['kept_frame_objects_sha256'],projected_owner_build_sha256=sha(owner/'build_verification.json'),projected_owner_qualification_sha256=sha(a.qualification/'verification.json'),projected_owner_qualification_status=q['status'],artifact_publishable=False,playable=False,overall_completion_percent=10)
    fb=read(Path(b['frame_build'])/'build_verification.json');frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:
        target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
