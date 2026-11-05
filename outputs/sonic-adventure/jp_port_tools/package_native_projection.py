"""Package only the qualified authored projection object; SDK bodies stay local."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','render-proof','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,d,b=a.workspace,a.output,read(a.build/'build_verification.json');v=read(a.build/'display-verification.json');p=read(a.render_proof/'verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert v['checks']==138170 and v['original_calls']==68111 and not v['original_dependency_hooks_used'] and v['all_65536_tangent_angles_verified']
    assert p['checks']==785368 and p['images_compared']==12 and p['original_polygon_producer_called'] and p['Texture_Off_zero_texture_sampler_bindings']
    for row,source in [(v,'verify_native_display_setup.py'),(p,'verify_native_projection_render.py')]:
        assert row['build_report_sha256']==sha(a.build/'build_verification.json') and row['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools'/source)
    obj=a.build/'projection.obj';defined=sorted(s['name'] for s in coff_symbols(obj) if s['section']>0 and s['function'])
    assert defined==['jp_projection_tangent_word','njSetPerspective','njTan'],defined
    # The compiled command must name the authored source, never the extracted
    # external SDK source which supplied the six separate display setters.
    command=next(row['command'] for row in b['commands'] if row['command'][-1]==str(obj))
    assert command[command.index('-c')+1]==str(r/'outputs/sonic-adventure/native/jp_native_projection.c')
    d.mkdir();member=d/obj.name;shutil.copyfile(obj,member);assert sha(obj)==sha(member)
    library=d/'jp_native_projection_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),str(member)]
    run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;members=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:members.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and members==[sha(obj)]
    report=dict(status='authored_projection_support_library_exact_member_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_object_sha256=sha(obj),defined_external_functions=defined,exactly_one_authored_member=True,external_game_or_SDK_bodies_packaged=False,artifact_publishable=True,retail_assets_embedded=False,qualification_inputs_sha256={str(x.relative_to(r)):sha(x) for x in [a.build/'build_verification.json',a.build/'display-verification.json',a.render_proof/'verification.json']},librarian_sha256=sha(librarian),package_tool_sha256=sha(__file__),dependencies=['shared screen-state owner','qualified jp_sprite_fsca/add/divide numerical services'],qualification='incomplete research support library, not a playable executable',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
