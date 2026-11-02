"""Pin the two successful strict builds and compare each parent dispatcher."""
import hashlib,json,re,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'outputs/sonic-adventure';W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
flags=['-shared','-O2','-DSA_BOUND_RETAIL_STALLS=1','-Wall','-Wextra','-Werror','-fuse-ld=lld','-nostdlib','-Wl,/noentry']
compiler=Path(r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe')
profiles=[('stage_world_render_gate',315,'model_stage_resources','sa_model_stage_resources_connected.c','sa_stage_world_render_gate_connected.c','sa_stage_world_render_gate.c','stage_world_render_gate_translation',True),('stage_texture_load',316,'stage_world_render_gate','sa_stage_world_render_gate_connected.c','sa_stage_texture_load_connected.c','sa_stage_texture_load.c','stage_texture_load_translation',False)]
for stem,count,parent,old_connected,new_connected,new_source,manifest,is_v in profiles:
 prior=P/('build_'+parent+'.ps1');recipe=P/('build_'+stem+'.ps1')
 old=re.findall(r"'([^']+\.c)'",prior.read_text());names=re.findall(r"'([^']+\.c)'",recipe.read_text())
 assert len(old)==count-1 and len(names)==len(set(names))==count
 assert set(old)-set(names)=={old_connected} and set(names)-set(old)=={new_connected,new_source}
 rsp=W/('build/tmp/'+stem+'.rsp');assert not rsp.read_bytes().startswith(b'\xef\xbb\xbf')
 lines=rsp.read_text().splitlines();assert all(s.startswith('"') and s.endswith('"') for s in lines);args=[s[1:-1] for s in lines]
 assert args[:len(flags)]==flags and args[len(flags):len(flags)+count]==[str((P/'source'/n).resolve()).replace('\\','/') for n in names]
 dll=W/('build/'+stem+'.dll');library=dll.with_suffix('.lib');assert Path(args[-1]).resolve()==dll.resolve()
 assert dll.read_bytes().startswith(b'MZ') and library.read_bytes().startswith(b'!<arch>') and not (W/(stem+'_build.log')).read_text().strip()
 j=json.loads((P/('analysis/'+manifest+'.json')).read_text());text=(P/'source'/old_connected).read_text()
 if is_v:
  macro=next(s for s in text.splitlines() if s.startswith('#define VDECL('));text=text.replace(macro,macro+'\n'+'\n'.join('VDECL('+r['name']+');' for r in j['routines']),1)
 else:text=text.replace('DECL(sa_draw_float_state_wrapper_8c043a20);','\n'.join('DECL('+r['name']+');' for r in j['routines'])+'\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
 marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';extra=',c->gbr,c->bus,c->integer,c->vbr' if is_v else ''
 text=text.replace(marker,marker+''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop{extra});' for r in j['routines']))
 assert text==(P/'source'/new_connected).read_text()
 files=[Path(__file__),prior,recipe,rsp,compiler,dll,library,Path(args[len(flags)+count]),P/('analysis/'+manifest+'.json')]+[P/'source'/n for n in names]+list((P/'source').glob('*.h'))
 report=dict(status='verified_strict_'+str(count)+'_source_response_file_build',source_count=count,DLL_path=str(dll),DLL_sha256=sha(dll),import_library_sha256=sha(library),original_instruction_rows=j['instruction_rows'],statically_emitted_entries=len(j['routines']),parent_dispatcher_preserved_except_declared_new_entries=True,new_wait_or_return_guard_relaxations=False,upstream_game_C_imported=False,behavior_qualification_separate=True,flags=flags,compiler_version=subprocess.check_output([str(compiler),'--version'],text=True).splitlines()[0],sha256={str(f):sha(f) for f in files})
 target=P/('analysis/'+stem+'_build_verification.json');assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Strict',count,'source build and new routes verified.',flush=True)
