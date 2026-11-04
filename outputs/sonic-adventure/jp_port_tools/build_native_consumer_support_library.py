"""Package exactly three authored objects, retaining unresolved real frame API.
No SDK producer, retail asset or diagnostic body belongs in this library.
"""
import argparse,collections,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','packets','texels','extended-report','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    sources={};qualifications={}
    for folder in [a.packets,a.texels]:
        b=json.loads((folder/'build_verification.json').read_text());q=json.loads((folder/'verification.json').read_text())
        assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
        assert q['build_report_sha256']==sha(folder/'build_verification.json') and q['DLL_sha256']==sha(Path(b['DLL']))==b['DLL_sha256']
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h;sources[n]=h
        qualifications[str(folder/'verification.json')]=sha(folder/'verification.json')
    ext=json.loads(a.extended_report.read_text());assert ext['status']=='native_consumer_extended_boundaries_verified' and ext['mutated_packet_cases']==6000
    assert ext['packet_build_report_sha256']==sha(a.packets/'build_verification.json') and ext['texel_build_report_sha256']==sha(a.texels/'build_verification.json')
    qualifications[str(a.extended_report)]=sha(a.extended_report);d.mkdir(parents=True)
    members={}
    for source in [a.packets/'jp_native_packets.obj',a.packets/'jp_native_packets_frames.obj',a.texels/'jp_native_texels.obj']:
        shutil.copyfile(source,d/source.name);members[source.name]=sha(source)
    librarian=a.compiler.parent/'llvm-lib.exe';library=d/'jp_native_packet_texel_support.lib';cmd=[str(librarian),'/nologo','/out:'+str(library),*[str(d/n) for n in members]]
    run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not (d/'build.txt').stat().st_size
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;found=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:found.append(hashlib.sha256(body).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and collections.Counter(found)==collections.Counter(members.values())
    sources[Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    report=dict(status='authored_native_packet_texel_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),member_objects_sha256=members,
        component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,qualification_reports_sha256=qualifications,
        exact_authored_members_verified=True,external_game_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,
        qualification='incomplete packet/texel consumer support; frame adapter requires real jp_frame_list owner; no rasterizer or playable game',artifact_publishable=True,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
