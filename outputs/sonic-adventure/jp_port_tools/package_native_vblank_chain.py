"""Package only the authored wait/chain boundary, never original SDK bodies."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','verification','startup-proof','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');v=read(a.verification/'verification.json');boot=read(a.startup_proof/'verification.json');w=read(a.build/'wait-regression-v2.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert v['checks']==79096 and v['original_entry_calls']==679 and not v['original_chain_or_Ninja_function_hooks_used'] and v['original_dependencies_entered']['njBlank']==8
    assert boot['checks']==7714 and boot['actual_GPU_Ninja_EOR_callbacks']==570 and boot['real_GPU_completion_retires_original_Ninja_blank'] and w['checks']==6851 and not w['native_functions_replaced']
    for row,script,parent in [(v,'verify_native_vblank_chain.py',a.build),(boot,'verify_native_vblank_startup.py',a.build),(w,'verify_native_vblank_wait.py',a.build/'frame_view')]:assert row['build_report_sha256']==sha(parent/'build_verification.json') and row['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools'/script)
    obj=a.build/'vblank_wait_owner.obj';assert sha(obj)==b['native_vblank_owner_sha256'];command=next(x['command'] for x in b['commands'] if x['command'][-1]==str(obj.resolve()));assert command[command.index('-c')+1]==str((r/'outputs/sonic-adventure/native/jp_native_vblank_chain.c').resolve())
    functions=sorted(s['name'] for s in coff_symbols(obj) if s['section']>0 and s['function']);assert all(n not in functions for n in ['jp_chain_original_add','jp_chain_original_delete','jp_chain_original_init','jp_chain_original_vblank','njBlank','njSetEORflag'])
    assert {'syChainAddHandler','syChainDeleteHandler','syChainInit','jp_wait_initialize','jp_wait_service','jp_vblank_arm_ninja_blank'}<=set(functions)
    d.mkdir();member=d/obj.name;shutil.copyfile(obj,member);assert sha(member)==sha(obj)
    library=d/'jp_native_vblank_wait_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),str(member)];run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;members=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:members.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and members==[sha(obj)]
    result=dict(status='one_authored_vblank_wait_boundary_member_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_object_sha256=sha(obj),defined_functions=functions,exactly_one_authored_member=True,external_game_or_SDK_bodies_packaged=False,artifact_publishable=True,retail_assets_embedded=False,qualification_inputs_sha256={str(path.resolve().relative_to(r)):sha(path) for path in [a.build/'build_verification.json',a.verification/'verification.json',a.startup_proof/'verification.json',a.build/'wait-regression-v2.json']},package_tool_sha256=sha(__file__),librarian_sha256=sha(librarian),dependencies=['pinned external adapted original Shinobi chain and Ninja blank/EOR object','existing shared native clock and Windows message-idle service'],qualification='incomplete native research support library, not a playable game',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
