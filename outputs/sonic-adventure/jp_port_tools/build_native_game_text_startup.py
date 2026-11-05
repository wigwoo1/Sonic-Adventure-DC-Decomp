"""Project the qualified new frame DLL into the preserved startup chain.
All dependencies retain DLL basenames; the harness preloads this frame owner.
"""
import argparse,hashlib,json,shutil
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','game-text','packet-proof','gpu-proof','interleave-proof','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();base=a.baseline.resolve();text=a.game_text.resolve();b=read(base/'build_verification.json');t=read(text/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in t['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    assert t['baseline_build_sha256']==sha(base/'build_verification.json') and sha(t['frames_DLL'])==t['frames_DLL_sha256'] and t['builder_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools/build_native_game_text.py')
    for folder,count in [(a.packet_proof,32220),(a.gpu_proof,16),(a.interleave_proof,24)]:v=read(folder/'verification.json');assert v['checks']==count and v['build_report_sha256']==sha(text/'build_verification.json')
    d.mkdir();generated=dict(b['generated_sources_sha256'])
    for n,h in generated.items():target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    shutil.copyfile(text/'external_game_text.c',d/'external_game_text.c');generated['external_game_text.c']=sha(d/'external_game_text.c');shutil.copytree(base/'inputs',d/'inputs');shutil.copyfile(base/'vblank_chain_sdk.obj',d/'vblank_chain_sdk.obj')
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_game_text','verify_native_game_text','verify_native_game_text_gpu','verify_native_game_text_interleave','build_native_game_text_startup']:
        p=r/'outputs/sonic-adventure/jp_port_tools'/(name+'.py');sources[p.relative_to(r).as_posix()]=sha(p)
    shared=dict(status='qualified_original_game_debug_text_frame_owner_in_native_startup_chain',frames_DLL=t['frames_DLL'],frames_DLL_sha256=t['frames_DLL_sha256'],commands=t['commands']+b['commands'],authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256={n:sha(n) for n in t['commands'][-1]['command'] if n.endswith('.obj')},game_text_build_sha256=sha(text/'build_verification.json'),game_text_original_source_sha256=t['original_source_sha256'],game_text_original_functions_sha256=t['original_functions_sha256'],game_text_qualification_inputs_sha256={str((f/'verification.json').resolve().relative_to(r)):sha(f/'verification.json') for f in [a.packet_proof,a.gpu_proof,a.interleave_proof]},artifact_publishable=False,playable=False,overall_completion_percent=10)
    fb=read(Path(b['frame_build'])/'build_verification.json');frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'));(d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
