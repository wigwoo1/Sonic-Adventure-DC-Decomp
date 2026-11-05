"""Package two exact authored members; all genuine game/SDK bodies stay local."""
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
    r,d,b=a.workspace,a.output,read(a.build/'build_verification.json');v=read(a.verification/'verification.json');random=read(a.build/'random-verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert v['checks']==88 and v['images_compared']==11 and v['pixels_compared']==720896 and v['SDK_record_reset_preserves_direct_surface'] and v['explicit_full_owner_reset_retires_direct_surface'] and v['Texture_Off_zero_sampled_bindings']
    assert random['checks']==430666 and random['original_entry_calls']==17480 and not random['original_function_hooks_used'] and random['complete_shuffle_cases']==160
    for row,source in [(v,'verify_native_standalone_texture.py'),(random,'verify_native_game_random.py')]:assert row['build_report_sha256']==sha(a.build/'build_verification.json') and row['verifier_sha256']==sha(r/'outputs/sonic-adventure/jp_port_tools'/source)
    for name,key,count in [('texture-host-regression.json','checks',1947),('texture-original-regression.json','comparisons',15232),('frame-regression.json','checks',8572)]:assert read(a.build/name)[key]==count
    owner=a.build/'standalone_texture_owner.obj';helper=Path(next(n for n in b['kept_frame_objects_sha256'] if Path(n).name=='random_pick.obj'));assert sha(owner)==b['standalone_texture_owner_object_sha256'] and sha(helper)==b['kept_frame_objects_sha256'][str(helper)]==b['game_random_objects_sha256']['random_pick.obj']
    aliases=['jp_texture_copy_surface','jp_texture_surface_owned','jp_texture_surface_word','jp_texture_surface_data','jp_texture_surface_bytes','jp_texture_live_surface_view','jp_textures_allocated_bytes','jp_textures_live_surfaces','kmFreeTexture']
    owner_functions=sorted(s['name'] for s in coff_symbols(owner) if s['section']>0 and s['function']);helper_functions=sorted(s['name'] for s in coff_symbols(helper) if s['section']>0 and s['function'])
    assert set(owner_functions)==set(b['old_authored_owner_functions_preserved'])|{'jp_record_only_'+n for n in aliases}|{'jp_texture_replace_standalone_surface'}
    assert helper_functions==['jp_random_pick','jp_random_positive_float_word','jp_shuffle_arguments','jp_shuffle_status'] and not any(n.startswith('nj') for n in owner_functions)
    cmd=next(row['command'] for row in b['commands'] if row['command'][-1]==str(owner));assert cmd[cmd.index('-c')+1]==str(r/'outputs/sonic-adventure/native/jp_native_standalone_texture.c')
    prior=read(helper.parent/'build_verification.json');assert sha(helper.parent/'build_verification.json')==b['previous_game_random_build_sha256'];cmd=next(row['command'] for row in prior['commands'] if row['command'][-1]==str(helper));assert cmd[cmd.index('-c')+1]==str(r/'outputs/sonic-adventure/native/jp_native_random_pick.c')
    d.mkdir();members=[]
    for source in [owner,helper]:target=d/source.name;shutil.copyfile(source,target);assert sha(source)==sha(target);members.append(target)
    library=d/'jp_native_random_direct_surface_support.lib';librarian=a.compiler.with_name('llvm-lib.exe');cmd=[str(librarian),'/nologo','/out:'+str(library),*map(str,members)];run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;hashes=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();data=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:hashes.append(hashlib.sha256(data).hexdigest())
        at+=60+size+(size&1)
    # llvm-lib's physical member order can differ from its /list index order.
    # Require exactly the two complete objects, independently of that order.
    assert at==len(raw) and len(hashes)==2 and sorted(hashes)==sorted(sha(member) for member in members)
    files=[a.build/'build_verification.json',a.verification/'verification.json',*[a.build/n for n in ['random-verification.json','texture-host-regression.json','texture-original-regression.json','frame-regression.json']]]
    report=dict(status='two_authored_random_numeric_and_direct_surface_owner_members_verified',command=cmd,exit_code=0,diagnostic_bytes=0,component_library=str(library),component_library_sha256=sha(library),member_objects_sha256={member.name:sha(member) for member in members},defined_owner_functions=owner_functions,defined_numeric_functions=helper_functions,exactly_two_authored_members=True,external_game_or_SDK_bodies_packaged=False,artifact_publishable=True,retail_assets_embedded=False,qualification_inputs_sha256={str(path.relative_to(r)):sha(path) for path in files},librarian_sha256=sha(librarian),package_tool_sha256=sha(__file__),replacement_not_additional_texture_owner=True,dependencies=['actual original SDK PRNG/state and shuffle bodies','real guarded texture operations and frame/asset ownership services','qualified RZ scalar multiply service'],genuine_print_family_implemented=False,qualification='incomplete research support library; replace prior texture owner; no playable executable',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
