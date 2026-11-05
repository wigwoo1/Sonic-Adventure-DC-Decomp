"""Archive one qualified authored queue object; exclude all game/SDK objects."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','verification','startup-proof','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');v=read(a.verification/'verification.json');boot=read(a.startup_proof/'verification.json');frames=read(a.build/'frame-regression.json');save=read(a.build/'save-verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for row,count,script,parent in [(v,249,'verify_native_queue_allocation.py',a.build),(boot,7732,'verify_native_queue_allocation_startup.py',a.build),(frames,8573,'verify_native_queue_allocation_frames.py',a.build/'queue_view'),(save,591,'verify_native_boot_saves.py',a.build)]:
        assert row['checks']==count and row['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools'/script) and row['build_report_sha256']==sha(parent/'build_verification.json')
    assert v['all_five_failure_positions_tested'] and v['retained_actual_frame_verified'] and not v['production_functions_replaced']
    assert boot['actual_GPU_Ninja_EOR_callbacks']==570 and boot['queue_allocation_via_real_native_caller'] and not boot['njInitVertexBuffer_integrated']
    obj=Path(b['queue_owner_object']);assert sha(obj)==b['queue_owner_object_sha256']
    command=b['commands'][0]['command'];assert command[command.index('-c')+1]==str((r/'outputs/sonic-adventure/native/jp_native_queue_allocation.c').resolve())
    functions=sorted(s['name'] for s in coff_symbols(obj) if s['section']>0 and s['function'])
    assert {'jp_queue_preallocate','jp_queue_capacity_bytes','jp_queue_reserve','jp_frames_submit'}<=set(functions)
    assert not {'njInitVertexBuffer','njBlank','njSetEORflag','jp_test_fail_at','jp_test_live'}&set(functions)
    d.mkdir();member=d/obj.name;shutil.copyfile(obj,member)
    library=d/'jp_native_queue_allocation_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),str(member)];run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;members=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:members.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and members==[sha(obj)]
    proofs=[a.build/'build_verification.json',a.verification/'verification.json',a.startup_proof/'verification.json',a.build/'frame-regression.json',a.build/'save-verification.json']
    result=dict(status='one_authored_real_queue_allocation_member_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_object_sha256=sha(obj),defined_functions=functions,exactly_one_authored_member=True,external_game_or_SDK_bodies_packaged=False,diagnostic_allocator_packaged=False,artifact_publishable=True,retail_assets_embedded=False,qualification_inputs_sha256={str(path.resolve().relative_to(r)):sha(path) for path in proofs},package_tool_sha256=sha(__file__),librarian_sha256=sha(librarian),total_checks=17145,dependencies=['existing native frame resource owner','CRT aligned allocation','pinned external SDK type declarations (compile only)'],qualification='incomplete native research support library, not a playable game',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
