"""Project the qualified original LCD/frame owners into preserved startup."""
import argparse,hashlib,json,shutil
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','lcd','qualification','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();base=a.baseline.resolve();lcd=a.lcd.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(base/'build_verification.json');c=read(lcd/'build_verification.json');q=read(a.qualification/'verification.json')
    assert q['checks']==1733 and q['build_report_sha256']==sha(lcd/'build_verification.json') and not q['original_function_hooks_used']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in c['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in c['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    for kind in ('frames','present'):assert sha(c[kind+'_DLL'])==c[kind+'_DLL_sha256']
    d.mkdir();generated=dict(b['generated_sources_sha256']);generated.pop('present_with_card_service.c')
    for n,h in generated.items():
        target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    for n,h in c['generated_sources_sha256'].items():shutil.copyfile(lcd/n,d/n);assert sha(d/n)==h;generated[n]=h
    shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    sources=dict(b['authored_source_sha256']);sources.update(c['authored_source_sha256'])
    for name in ('build_native_vmu_lcd_startup','verify_native_vmu_lcd'):
        f=r/'outputs/sonic-adventure/jp_port_tools'/(name+'.py');sources[f.relative_to(r).as_posix()]=sha(f)
    shared=dict(status='qualified_original_VMU_LCD_and_card_owners_in_startup_chain',frames_DLL=c['frames_DLL'],frames_DLL_sha256=c['frames_DLL_sha256'],present_DLL=c['present_DLL'],present_DLL_sha256=c['present_DLL_sha256'],commands=c['commands']+b['commands'],authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=c['kept_frame_objects_sha256'],VMU_LCD_build_sha256=sha(lcd/'build_verification.json'),VMU_LCD_qualification_sha256=sha(a.qualification/'verification.json'),original_VMU_functions_sha256=c['original_functions_sha256'],actual_frame_vsync_LCD_service=True,artifact_publishable=False,playable=False,overall_completion_percent=10)
    fb=read(Path(b['frame_build'])/'build_verification.json');frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:
        target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
