"""Archive exact authored queue/print transport objects; exclude SDK/font code."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','verification','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');v=read(a.verification/'verification.json');inter=read(a.build/'interleave-verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for report,count,script in [(v,24937,'verify_native_print.py'),(inter,124,'verify_native_print_interleave.py')]:
        assert report['checks']==count and report['build_report_sha256']==sha(a.build/'build_verification.json') and report['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools'/script)
    assert v['original_entry_calls']==298 and not v['print_character_string_integer_or_context_hooks_used'] and v['Texture_Off_zero_sampling_verified'] and inter['mixed_print_and_regular_batches']==48
    d.mkdir();members=[]
    for name,source in [('queue_direct','jp_native_queue_direct.c'),('print_transport','jp_native_print_transport.c')]:
        path=a.build/(name+'.obj');assert sha(path)==b['print_objects_sha256'][path.name]
        cmd=next(row['command'] for row in b['commands'] if row['command'][-1]==str(path.resolve()));assert cmd[cmd.index('-c')+1]==str((r/'outputs/sonic-adventure/native'/source).resolve())
        target=d/path.name;shutil.copyfile(path,target);assert sha(target)==sha(path);members.append(target)
    defined={p.name:sorted(s['name'] for s in coff_symbols(p) if s['section']>0 and s['function']) for p in members}
    assert all(n.startswith('jp_print_') for n in defined['print_transport.obj'])
    assert not set(b['print_original_function_sha256'])&{n for row in defined.values() for n in row}
    library=d/'jp_native_print_transport_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),*map(str,members)];run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;hashes=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:hashes.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and len(hashes)==2 and sorted(hashes)==sorted(sha(p) for p in members)
    report=dict(status='two_authored_native_queue_and_print_transport_members_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_objects_sha256={p.name:sha(p) for p in members},defined_functions=defined,exactly_two_authored_members=True,external_game_or_SDK_bodies_packaged=False,retail_assets_embedded=False,artifact_publishable=True,replacement_not_additional_queue_owner=True,qualification_inputs_sha256={str(p.resolve().relative_to(r)):sha(p) for p in [a.build/'build_verification.json',a.verification/'verification.json',a.build/'interleave-verification.json']},package_tool_sha256=sha(__file__),librarian_sha256=sha(librarian),dependencies=['pinned external genuine SDK print functions and user-supplied original font data','same existing native frame/texture/video owners','qualified integer-word RZ scalar multiply and conversion services'],qualification='incomplete research support library; replace the previous queue object; not a playable executable',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
