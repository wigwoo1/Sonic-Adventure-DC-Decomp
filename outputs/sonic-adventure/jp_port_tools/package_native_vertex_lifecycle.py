"""Package four authored objects for their separate native module roles.
No original SDK/game object or linked game-containing DLL is packaged.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','original-proof','state-proof','startup-build','startup-proof','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();v=read(a.build/'verification.json');o=read(a.original_proof/'verification.json');state=read(a.state_proof/'verification.json');boot=read(a.startup_proof/'verification.json');startup=read(a.startup_build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    assert v['checks']==25 and o['checks']==2081 and o['original_entry_calls']==130 and state['checks']==82 and boot['checks']==7714 and boot['actual_native_njInitVertexBuffer_calls']==6 and boot['actual_GPU_images']==570
    assert o['build_proof_sha256']==state['build_proof_sha256']==startup['lifecycle_proof_sha256']==sha(a.build/'verification.json') and boot['build_report_sha256']==sha(a.startup_build/'build_verification.json')
    assert not o['original_initialization_or_float_operations_hooked'] and not state['native_functions_replaced'] and not boot['native_function_bodies_replaced']
    assert v['source_sha256']==sha(r/'outputs/sonic-adventure/native/jp_native_vertex_lifecycle.c') and v['cache_source_sha256']==sha(r/'outputs/sonic-adventure/native/jp_native_texture_cache.c') and v['wait_source_sha256']==sha(r/'outputs/sonic-adventure/native/jp_native_vblank_cancel.c')
    roles={'lifecycle':a.build/'lifecycle.obj','wait_cancel':a.build/'wait_cancel.obj','texture_cache':a.build/'texture_cache.obj'}
    roles['vertex_lists']=next(Path(n) for n in v['commands'][1]['command'] if Path(n).name=='vertex_lists.obj')
    compiler=Path(v['commands'][0]['command'][0]);librarian=compiler.with_name('llvm-lib.exe');d.mkdir();artifacts={}
    for role,obj in roles.items():
        functions=sorted(s['name'] for s in coff_symbols(obj) if s['section']>0 and s['function'])
        assert not {'njCalcTexture','njBlank','njSetEORflag','jp_chain_original_add','jp_chain_original_vblank','logo_start'}&set(functions)
        expected={'lifecycle':'njInitVertexBuffer','wait_cancel':'jp_vblank_cancel_ninja_blank','texture_cache':'jp_texture_cache_maximum','vertex_lists':'jp_vertex_lists_configure'}[role];assert expected in functions
        member=d/obj.name;shutil.copyfile(obj,member);library=d/('jp_native_'+role+'_qualified_support.lib');cmd=[str(librarian),'/nologo','/out:'+str(library),str(member)];run=subprocess.run(cmd,capture_output=True);assert run.returncode==0 and not run.stdout and not run.stderr
        raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;members=[]
        while at<len(raw):
            header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
            if name not in ['/','//','/<ECSYMBOLS>/']:members.append(hashlib.sha256(body).hexdigest())
            at+=60+size+(size&1)
        assert at==len(raw) and members==[sha(obj)]
        artifacts[role]=dict(path=str(library),sha256=sha(library),member_object_sha256=sha(obj),command=cmd,exactly_one_authored_member=True,defined_functions=functions,module_role='higher lifecycle DLL' if role=='lifecycle' else 'presentation DLL' if role=='wait_cancel' else 'shared frame DLL')
    proofs=[a.build/'verification.json',a.original_proof/'verification.json',a.state_proof/'verification.json',a.startup_build/'build_verification.json',a.startup_build/'save-verification.json',a.startup_proof/'verification.json']
    result=dict(status='four_separate_role_authored_support_members_verified',artifacts=artifacts,qualification_inputs_sha256={str(p.resolve().relative_to(r)):sha(p) for p in proofs},package_tool_sha256=sha(__file__),librarian_sha256=sha(librarian),external_game_or_SDK_bodies_packaged=False,retail_assets_embedded=False,artifact_publishable=True,not_combined_into_one_owner=True,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
