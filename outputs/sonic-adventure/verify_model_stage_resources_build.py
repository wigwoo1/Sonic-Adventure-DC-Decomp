"""Seal the strict build, retained source inputs and seven new dispatch routes."""
import hashlib,json,re,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'outputs/sonic-adventure';W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
prior=P/'build_model_world_land_dependencies_v2.ps1';recipe=P/'build_model_stage_resources.ps1'
old=re.findall(r"'([^']+\.c)'",prior.read_text());names=re.findall(r"'([^']+\.c)'",recipe.read_text())
assert len(old)==313 and len(names)==len(set(names))==314
assert set(old)-set(names)=={'sa_model_world_land_dependencies_connected.c'}
assert set(names)-set(old)=={'sa_model_stage_resources.c','sa_model_stage_resources_connected.c'}
previous=json.loads((P/'analysis/model_world_land_dependencies_v2_build_verification.json').read_text())
for name,h in previous['sha256'].items():assert sha(Path(name))==h,name
flags=['-shared','-O2','-DSA_BOUND_RETAIL_STALLS=1','-Wall','-Wextra','-Werror','-fuse-ld=lld','-nostdlib','-Wl,/noentry']
rsp=W/'build/tmp/model_stage_resources.rsp';assert not rsp.read_bytes().startswith(b'\xef\xbb\xbf')
lines=rsp.read_text().splitlines();assert all(s.startswith('"') and s.endswith('"') for s in lines);args=[s[1:-1] for s in lines]
assert args[:len(flags)]==flags
assert args[len(flags):len(flags)+314]==[str((P/'source'/n).resolve()).replace('\\','/') for n in names]
dll=W/'build/model_stage_resources.dll';library=dll.with_suffix('.lib');assert Path(args[-1]).resolve()==dll.resolve()
assert dll.read_bytes().startswith(b'MZ') and library.read_bytes().startswith(b'!<arch>')
assert not (W/'model_stage_resources_build.log').read_text().strip()
j=json.loads((P/'analysis/model_stage_resources_translation.json').read_text());assert j['instruction_rows']==1357
base=P/'source/sa_model_world_land_dependencies_connected.c';text=base.read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace('DECL(sa_draw_float_state_wrapper_8c043a20);','\n'.join('DECL('+r['name']+');' for r in j['routines'])+'\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
text=text.replace(marker,marker+''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for r in j['routines']))
assert text==(P/'source/sa_model_stage_resources_connected.c').read_text()
compiler=Path(r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe')
files=[Path(__file__),prior,recipe,rsp,compiler,Path(args[len(flags)+314]),dll,library,base,P/'analysis/model_stage_resources_translation.json']+[P/'source'/n for n in names]+list((P/'source').glob('*.h'))
report=dict(status='verified_strict_314_source_response_file_build',source_count=314,DLL_path=str(dll),DLL_sha256=sha(dll),import_library_sha256=sha(library),original_instruction_rows=1357,statically_emitted_entries=7,flags=flags,compiler_version=subprocess.check_output([str(compiler),'--version'],text=True).splitlines()[0],prior_313_source_inputs_and_builds_preserved=True,existing_dispatcher_changed_only_for_seven_new_entries=True,new_wait_or_return_guard_relaxations=False,upstream_game_C_imported=False,behavior_qualification_separate=True,sha256={str(f):sha(f) for f in files})
target=P/'analysis/model_stage_resources_build_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Strict 314-source build, all retained inputs and seven new routes verified.',flush=True)
