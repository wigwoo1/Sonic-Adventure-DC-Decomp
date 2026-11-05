"""Package the exact authored native formatter boundary, excluding SDK bodies."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','verification','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');v=read(a.verification/'verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert v['checks']==22072 and v['original_entry_calls']==758 and v['format_cases']==217 and v['complete_fixed_print_cases']==94 and v['complete_variadic_print_cases']==4 and not v['original_formatter_or_print_function_hooks_used'] and v['Texture_Off_zero_sampling_verified'] and not v['host_CRT_formatter_imported']
    assert v['build_report_sha256']==sha(a.build/'build_verification.json') and v['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools/verify_native_print_format.py')
    assert read(a.build/'interleave-verification.json')['checks']==124 and read(a.build/'frame-regression.json')['checks']==8572
    path=a.build/'print_format.obj';assert sha(path)==b['format_objects_sha256'][path.name];cmd=next(row['command'] for row in b['commands'] if row['command'][-1]==str(path.resolve()));assert cmd[cmd.index('-c')+1]==str((r/'outputs/sonic-adventure/native/jp_native_print_format.c').resolve())
    functions=sorted(s['name'] for s in coff_symbols(path) if s['section']>0 and s['function']);assert functions and all(n.startswith('jp_print_') for n in functions)
    d.mkdir();member=d/path.name;shutil.copyfile(path,member);assert sha(member)==sha(path)
    library=d/'jp_native_print_format_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),str(member)];run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;hashes=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:hashes.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and hashes==[sha(member)]
    report=dict(status='one_authored_native_format_argument_and_bounds_member_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_objects_sha256={member.name:sha(member)},defined_functions=functions,exactly_one_authored_member=True,external_game_or_SDK_bodies_packaged=False,retail_assets_embedded=False,artifact_publishable=True,qualification_inputs_sha256={str(p.resolve().relative_to(r)):sha(p) for p in [a.build/'build_verification.json',a.verification/'verification.json',a.build/'interleave-verification.json',a.build/'frame-regression.json']},package_tool_sha256=sha(__file__),librarian_sha256=sha(librarian),dependencies=['namespaced external original formatter and decimal/multiprecision helpers','genuine print SDK state/body and existing native queue/texture/frame/video owners'],qualification='incomplete native research support library; no game/SDK bodies, font or playable executable',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
