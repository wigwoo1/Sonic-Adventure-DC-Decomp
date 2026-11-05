"""Publish reproducible probe metadata without external binaries or retail data."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path

def sha(p):
    h=hashlib.sha256()
    with Path(p).open('rb') as f:
        for raw in iter(lambda:f.read(8<<20),b''):h.update(raw)
    return h.hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','core','control','build','verification','audit','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure';u=a.core.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(w);d.mkdir()
    files=[a.build/'build_verification.json',a.control/'build_verification.json',a.verification/'verification.json',a.audit/'dependency_inventory.json'];b,c,v,audit=map(read,files)
    assert sha(files[0])==v['build_verification_sha256'] and sha(files[1])==v['baseline_build_sha256'] and sha(files[3])==v['sound_audit_sha256']
    assert v['matched_8MiB_baseline_and_2MiB_adaptation_and_packet_sizes'] and v['control_includes_same_RAM_store_and_DSP_ordering_adaptations'] and not v['control_is_unmodified_upstream']
    assert v['real_Windows_delivery']['frames']==88200 and v['real_Windows_delivery']['session_muted'] and v['runtime_assertions']==1010
    cases=v['cases'];assert len(cases)==3
    for row in cases.values():
        for field in ('PCM_sha256','noteoff_PCM_sha256','driver_RAM_sha256','final_ARM_registers','total_executed_cycles'):
            assert row[field]==cases['baseline_441'][field],field
    binaries={};warnings={};sources={}
    for folder,meta in ((a.build,b),(a.control,c)):
        assert sha(meta['DLL'])==meta['DLL_sha256']
        for name,h in meta['upstream_source_sha256'].items():assert sha(u/name)==h,name
        for name,h in meta['authored_source_sha256'].items():assert sha(r/name)==h,name;sources[name]=h
        assert sha(folder/'external_dcsound_2MiB.c')==meta['generated_source_sha256'] and sha(folder/'external_yam_timing.c')==meta['generated_YAM_source_sha256']
        warnings[str(folder)]=sum(len(re.findall(rb'warning:',(folder/f'build_{j}.txt').read_bytes())) for j in range(3));assert warnings[str(folder)]==17
        assert all(not x['exit_code'] for x in meta['commands']) and all(not x['diagnostic_bytes'] for x in meta['commands'][3:])
        binaries[str(folder)]={path.name:sha(path) for path in sorted(folder.iterdir()) if path.suffix in ('.dll','.lib','.obj')}
    assert sha(p/'jp_port_tools/verify_aica_core_driver.py')==v['verifier_sha256']
    git='C:/Program Files/Git/cmd/git.exe';assert not subprocess.check_output([git,'-C',str(u),'status','--porcelain'])
    track=Path(audit['media_inputs']['data_track']);assert sha(track)==audit['media_inputs']['data_track_sha256']
    private=a.audit/'private_inputs/MASARU.arm.bin';assert sha(private)==audit['actual_Japanese_driver']['program_sha256']
    graph=read(w/'jp-native-system-v3/build_verification.json');preserved={}
    for key,value in graph.items():
        if isinstance(value,str) and (key=='DLL' or key.endswith('_DLL')) and value.lower().endswith('.dll'):
            assert sha(value)==graph[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value))
    audio=read(w/'jp-native-audio-v5/build_verification.json');assert sha(audio['DLL'])==audio['DLL_sha256'];preserved['qualified_PCM_output']=dict(path=audio['DLL'],sha256=audio['DLL_sha256'])
    for path in (p/'jp_port_tools/verify_aica_core_driver.py',p/'jp_port_tools/package_aica_core_probe.py',p/'JP_AICA_DRIVER_PROBE.txt'):
        sources[path.relative_to(r).as_posix()]=sha(path)
    result=dict(status='private_real_ARM_AICA_driver_probe_source_proofs_and_preservation_qualified_public_metadata_only',upstream_repository=b['upstream_repository'],upstream_revision=b['upstream_revision'],upstream_license=b['upstream_license'],upstream_source_sha256=b['upstream_source_sha256'],
        authored_source_sha256=sources,proof_files_sha256={x.resolve().relative_to(r).as_posix():sha(x) for x in files},private_artifact_sha256=binaries,
        external_compile_warning_count=warnings,authored_fixture_and_link_diagnostic_bytes=0,prior_native_DLLs_preserved=preserved,
        physically_cycle_accurate_or_console_waveform_qualified=False,original_game_or_SDK_audio_initialized=False,public_core_binaries_selected=False,retail_assets_or_generated_audio_published=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    for source,name in ((files[0],'jp_aica_core_build.json'),(files[1],'jp_aica_core_control_build.json'),(files[2],'jp_aica_driver_probe.json'),(d/'qualification.json','jp_aica_probe_package.json')):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    print(result['status'],'runtime checks',v['runtime_assertions'],'preserved DLLs',len(preserved),'external warnings per build',17,flush=True)

if __name__=='__main__':main()
