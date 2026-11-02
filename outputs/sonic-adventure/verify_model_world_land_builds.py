"""Pin successful strict builds and prove the retained dispatcher additions."""
import hashlib,json,re,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'outputs/sonic-adventure';W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
flags=['-shared','-O2','-DSA_BOUND_RETAIL_STALLS=1','-Wall','-Wextra','-Werror','-fuse-ld=lld','-nostdlib','-Wl,/noentry']
compiler=Path(r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe')
base=(P/'source/sa_model_world_writers_connected.c').read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);'
for stem,count,manifests,connected in [
 ('model_world_land',312,['model_world_land_translation'],'sa_model_world_land_connected.c'),
 ('model_world_land_dependencies_v2',313,['model_world_land_translation','model_world_land_dependencies_v2_translation'],'sa_model_world_land_dependencies_connected.c')]:
 recipe=P/('build_'+stem+'.ps1');names=re.findall(r"'([^']+\.c)'",recipe.read_text());assert len(names)==len(set(names))==count
 prior=re.findall(r"'([^']+\.c)'",(P/'build_model_world_writers.ps1').read_text())
 assert set(prior)-set(names)=={'sa_model_world_writers_connected.c'}
 assert set(names)-set(prior)==({'sa_model_world_land.c',connected} | ({'sa_model_world_land_dependencies_v2.c'} if count==313 else set()))
 response=W/('build/tmp/'+stem+'.rsp');raw=response.read_bytes();assert not raw.startswith(b'\xef\xbb\xbf')
 lines=response.read_text().splitlines();assert all(x.startswith('"') and x.endswith('"') for x in lines)
 args=[x[1:-1] for x in lines];assert args[:len(flags)]==flags
 assert args[len(flags):len(flags)+count]==[str((P/'source'/x).resolve()).replace('\\','/') for x in names]
 dll=W/('build/'+stem+'.dll');library=dll.with_suffix('.lib');assert Path(args[-1]).resolve()==dll.resolve()
 assert dll.read_bytes().startswith(b'MZ') and library.read_bytes().startswith(b'!<arch>')
 assert not (W/(stem+'_build.log')).read_text().strip()
 expected=base;rows=0
 for manifest in manifests:
  j=json.loads((P/('analysis/'+manifest+'.json')).read_text());rows+=j['instruction_rows']
  routines=j['routines']
  expected=expected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);','\n'.join('DECL('+x['name']+');' for x in routines)+'\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
  expected=expected.replace(marker,marker+''.join(f'\n if(target=={x["address"]}u)return {x["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for x in routines))
 assert expected==(P/'source'/connected).read_text()
 files=[recipe,response,compiler,dll,library,Path(args[len(flags)+count])]+[P/'source'/x for x in names]+list((P/'source').glob('*.h'))
 report=dict(status='verified_strict_'+str(count)+'_source_build',source_count=count,DLL_path=str(dll),DLL_sha256=sha(dll),import_library_sha256=sha(library),original_instruction_rows=rows,prior_dispatcher_preserved_except_declared_new_routes=True,new_wait_or_return_guard_relaxations=False,upstream_game_C_imported=False,flags=flags,compiler_version=subprocess.check_output([str(compiler),'--version'],text=True).splitlines()[0],sha256={str(path):sha(path) for path in files})
 target=P/('analysis/'+stem+'_build_verification.json');assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 print('Verified strict',count,'source build and retained dispatcher;',rows,'new original rows.',flush=True)
