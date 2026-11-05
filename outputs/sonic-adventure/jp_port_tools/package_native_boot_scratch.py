"""Package the authored scratch owner only; original SDK/game objects stay local."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','verification','display-proof','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');v=read(a.verification/'verification.json');p=read(a.display_proof/'verification.json');m=read(a.build/'model-borrowed-verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert v['checks']==1076 and v['original_entry_calls']==427 and not v['original_dependency_hooks_used'] and v['borrowed_storage_never_freed_or_cleared_outside_projected_extent']
    assert p['checks']==1841664 and p['Texture_Off_zero_sampling_verified'] and p['original_main_texture_and_vertex_buffer_is_same'] and m['checks']==200675 and m['borrowed_buffer_used_by_actual_model_producer']
    for row,script,parent in [(v,'verify_native_boot_buffers.py',a.build),(p,'verify_native_boot_buffers_display.py',a.build),(m,'verify_native_boot_buffer_models.py',a.build/'owner_view')]:assert row['build_report_sha256']==sha(parent/'build_verification.json') and row['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools'/script)
    obj=a.build/'boot_scratch_owner.obj';assert sha(obj)==b['boot_scratch_owner_sha256'];command=next(x['command'] for x in b['commands'] if x['command'][-1]==str(obj.resolve()));assert command[command.index('-c')+1]==str((r/'outputs/sonic-adventure/native/jp_native_boot_scratch.c').resolve())
    functions=sorted(s['name'] for s in coff_symbols(obj) if s['section']>0 and s['function']);assert 'njClipZ' in functions and 'jp_render_state_register_buffer' in functions and all(n=='njClipZ' or n.startswith('jp_render_state_') or n in ['ready','fail','forget_registration'] for n in functions),functions
    d.mkdir();member=d/obj.name;shutil.copyfile(obj,member);assert sha(member)==sha(obj)
    library=d/'jp_native_boot_scratch_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),str(member)];run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;members=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:members.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and members==[sha(obj)]
    report=dict(status='one_authored_shared_boot_scratch_owner_member_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_object_sha256=sha(obj),defined_functions=functions,exactly_one_authored_member=True,external_game_or_SDK_bodies_packaged=False,artifact_publishable=True,retail_assets_embedded=False,qualification_inputs_sha256={str(x.resolve().relative_to(r)):sha(x) for x in [a.build/'build_verification.json',a.verification/'verification.json',a.display_proof/'verification.json',a.build/'model-borrowed-verification.json']},package_tool_sha256=sha(__file__),librarian_sha256=sha(librarian),dependencies=['genuine njInit3D and njSetScreen SDK providers','original boot setters and main-owned buffers for game integration'],qualification='incomplete research support library; not a playable game',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],flush=True)
if __name__=='__main__':main()
