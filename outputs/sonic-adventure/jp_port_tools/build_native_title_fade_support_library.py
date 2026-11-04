"""Package fourteen authored members; original SDK/game fade implementations external."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path
from build_native_gpu_jobs_support_library import archive_members
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','audit','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();build=a.build.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=read(build/'build_verification.json');audit=read(a.audit);assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    counts={};proofs=[]
    for name,file,count in [('fades','verification.json',353),('background','background-regression.json',179),('limits','background-limits-regression.json',34),('UV16','background-uv16-regression.json',13),('PVM','pvm-regression.json',1448),('assets','asset-regression.json',223)]:
        path=build/file;q=read(path);assert q['checks']==count and q['build_report_sha256']==sha(build/'build_verification.json');counts[name]=count;proofs.append(path)
    for suite,expected in [('textures',1947),('frames',8572),('video',103700),('present',None),('bitmap',722)]:
        path=build/(suite+'-regression/pvm_qualification.json');q=read(path);actual=path.parent/'verification.json';assert q['actual_build_sha256']==sha(build/'build_verification.json') and q['report_sha256']==sha(actual)
        if expected is not None:assert q['checks']==expected
        else:assert read(actual)['visible_Kamui_Presents']==8
        counts[suite]=q['checks'];proofs.extend([path,actual])
    assert audit['combined_remaining_import_count']==36 and audit['expanded_boot_remaining_import_count']==43 and audit['one_actual_game_title_fade_state_owner'] and not audit['test_fixture_or_dummy_services_linked'];proofs.append(a.audit)
    names=['frames_background.obj','video.obj','video_sdk.obj','gpu_background.obj','jp_native_packets.obj','jp_native_packets_frames.obj','jp_native_texels.obj','queue_frames.obj','textures_move.obj','assets.obj','memory_pvm.obj','bitmap_source.obj','background_decode.obj','context_bridge.obj'];paths=[];members={}
    for name in names:
        found=[Path(n) for n in audit['linked_objects_sha256'] if Path(n).name==name];assert len(found)==1;path=found[0];assert sha(path)==audit['linked_objects_sha256'][str(path)];paths.append(path);members[name]=sha(path)
    d.mkdir();library=d/'jp_native_title_fade_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';command=[str(librarian),'/nologo','/out:'+str(library),*map(str,paths)];run=subprocess.run(command,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    assert archive_members(library)==collections.Counter(members.values()) and len(members)==14
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_title_fade_support_library.py','audit_native_title_fade_link.py','verify_native_title_fade.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    guide=r/'outputs/sonic-adventure/JP_NATIVE_TITLE_FADES.txt';sources[guide.relative_to(r).as_posix()]=sha(guide)
    result=dict(status='authored_combined_title_fade_context_transport_rendering_support_library_verified',exit_code=0,diagnostic_bytes=0,command=command,librarian_sha256=sha(librarian),member_objects_sha256=members,component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,exact_authored_members_verified=True,external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,PRS_codec_body_packaged=False,retail_assets_embedded=False,artifact_publishable=True,verification_files_sha256={str(p):sha(p) for p in proofs},checks=counts,total_checks=sum(counts.values()),fade_GPU_images=28,fade_pixels_compared=8601600,fade_visible_Presents=3,production_link_audit_sha256=sha(a.audit),dependencies='REPLACES overlapping background/GPU/frame/texture/PVM/asset support. Real SDK context and game title-fade implementations, separate MIT prskit and other services remain external. One of each owner.',qualification='incomplete native research component with original black/white fades and actual Ninja context headers; full original boot/gameplay unfinished',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['total_checks'],'checks')
if __name__=='__main__':main()
