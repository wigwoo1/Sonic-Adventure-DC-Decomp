"""Package four authored asset/PVM/bitmap members; codec/game/SDK bodies external."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path
from build_native_gpu_jobs_support_library import archive_members
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','audit','logo-report','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    for n in ['textures','frames','video','present','bitmap']:ap.add_argument('--'+n+'-proof',dest=n+'_proof',type=Path)
    ap.add_argument('--pvm-report',type=Path)
    a=ap.parse_args();r=a.workspace.resolve();build=a.build.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=read(build/'build_verification.json');audit=read(a.audit);q=read(build/'verification.json');logo=read(a.logo_report)
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in dict(b['authored_source_sha256'],**b['vendored_MIT_source_sha256']).items():assert sha(r/n)==h,n
    assert q['checks']==223 and q['actual_GPU_images']==27 and logo['checks']==29 and logo['visible_Kamui_Presents']==2 and logo['pixels_compared']==921600
    counts={'assets':223,'logo':29};proofs=[build/'verification.json',a.audit,a.logo_report]
    for suite,expected in [('textures',1947),('frames',8572),('video',103700),('present',None),('bitmap',722)]:
        path=getattr(a,suite+'_proof') or build/(suite+'-regression/pvm_qualification.json');v=read(path);report=path.parent/'verification.json'
        assert v['actual_build_sha256']==sha(build/'build_verification.json') and v['report_sha256']==sha(report) and v['checks']==read(report)['checks']
        if expected is not None:assert v['checks']==expected
        else:assert read(report)['visible_Kamui_Presents']==8 and read(report)['actual_GPU_images_compared']==4
        counts[suite]=v['checks'];proofs.extend([path,report])
    pvm=a.pvm_report or build/'pvm-regression.json';assert read(pvm)['checks']==1448 and read(pvm)['build_report_sha256']==sha(build/'build_verification.json');proofs.append(pvm);counts['PVM']=1448
    assert audit['combined_remaining_import_count']==36 and audit['all_previous_texture_services_preserved'] and not audit['test_fixture_or_dummy_services_linked'] and audit['asset_build_sha256']==sha(build/'build_verification.json')
    paths=[];members={}
    for name in ['textures_move.obj','assets.obj','memory_pvm.obj','bitmap_source.obj']:
        found=[Path(n) for n in audit['linked_objects_sha256'] if Path(n).name==name];assert len(found)==1;path=found[0];assert sha(path)==audit['linked_objects_sha256'][str(path)];paths.append(path);members[name]=sha(path)
    d.mkdir();library=d/'jp_native_asset_loading_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';command=[str(librarian),'/nologo','/out:'+str(library),*map(str,paths)]
    run=subprocess.run(command,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    assert archive_members(library)==collections.Counter(members.values())
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_asset_support_library.py','audit_native_asset_link.py','verify_native_asset_runtime.py','native_asset_test_support.py','build_native_logo_asset_probe.py','verify_native_logo_assets.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    guide=r/'outputs/sonic-adventure/JP_NATIVE_LOGO_ASSETS.txt';sources[guide.relative_to(r).as_posix()]=sha(guide)
    result=dict(status='authored_asset_file_PRS_interface_texture_moves_and_PVM_bitmap_support_library_verified',exit_code=0,diagnostic_bytes=0,command=command,librarian_sha256=sha(librarian),member_objects_sha256=members,component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,vendored_MIT_source_sha256=b['vendored_MIT_source_sha256'],exact_authored_members_verified=True,external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,PRS_codec_body_packaged=False,retail_assets_embedded=False,artifact_publishable=True,verification_files_sha256={str(p):sha(p) for p in proofs},checks=counts,total_checks=sum(counts.values()),production_link_audit_sha256=sha(a.audit),dependencies='REPLACES prior PVM/bitmap upload support. Four authored owners only. Compile provided MIT prskit separately with its notice; original game/SDK helpers and frame/video/GPU/presenter services remain external.',qualification='incomplete research support library; real original logo draw/frame-wait/presentation verified; full logo startup, boot and gameplay unfinished',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['total_checks'],'checks')
if __name__=='__main__':main()
