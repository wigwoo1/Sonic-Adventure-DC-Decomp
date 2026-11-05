"""Exactly one authored owner member; genuine SDK initializer stays private."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','verification','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,d,b=a.workspace,a.output,read(a.build/'build_verification.json');v=read(a.verification/'verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert v['checks']==502 and v['original_calls']==84 and not v['original_function_hooks_used'] and v['images_compared']==7 and v['pixels_compared']==458752 and v['Texture_Off_zero_sampled_bindings']
    assert v['build_report_sha256']==sha(a.build/'build_verification.json') and v['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools/verify_native_texture_initialization.py')
    for name,key,count in [('texture-host-regression.json','checks',1947),('texture-original-regression.json','comparisons',15232),('frame-regression.json','checks',8572)]:assert read(a.build/name)[key]==count
    obj=a.build/'texture_initialization_owner.obj';assert sha(obj)==b['authored_texture_initialization_owner_sha256']
    defined=sorted(s['name'] for s in coff_symbols(obj) if s['section']>0 and s['function'])
    assert set(defined)==set(b['old_owner_external_functions_preserved'])|{'jp_textures_storage_initialize','jp_textures_storage_shutdown','jp_texture_begin_sdk_init','jp_texture_finish_sdk_init','jp_texture_sdk_limits'},defined
    assert not any(name.startswith('nj') for name in defined)
    command=next(row['command'] for row in b['commands'] if row['command'][-1]==str(obj));assert command[command.index('-c')+1]==str(r/'outputs/sonic-adventure/native/jp_native_texture_initialization.c')
    d.mkdir();member=d/obj.name;shutil.copyfile(obj,member);assert sha(member)==sha(obj)
    library=d/'jp_native_texture_initialization_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),str(member)]
    run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;members=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:members.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and members==[sha(obj)]
    files=[a.build/'build_verification.json',a.verification/'verification.json',*[a.build/n for n in ['texture-host-regression.json','texture-original-regression.json','frame-regression.json']]]
    report=dict(status='authored_texture_initialization_owner_support_library_exact_member_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_object_sha256=sha(obj),defined_external_functions=defined,exactly_one_authored_member=True,external_game_or_SDK_bodies_packaged=False,artifact_publishable=True,retail_assets_embedded=False,qualification_inputs_sha256={str(path.relative_to(r)):sha(path) for path in files},librarian_sha256=sha(librarian),package_tool_sha256=sha(__file__),dependencies=['genuine guarded njInitTexture/njInitTexMemList and SDK record operations','existing shared frame retention, memory-upload and asset ownership services'],replacement_not_additional_owner=True,qualification='incomplete research replacement owner; no playable executable',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
