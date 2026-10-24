"""Connect boot setup to actual task cleanup and register-writing bodies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
manifest_path=PROJECT/'analysis/boot_setup_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_draw_frame_context_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'BDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n' for r in manifest['routines'])
text=text.replace(marker,routes+marker)
marker='if(target==0x8c6215bau';assert text.count(marker)==1
text=text.replace(marker,'if(target==0x8c09865cu||target==0x8c098998u||target==0x8c098e60u||target==0x8c098e98u||target==0x8c098b42u||target==0x8c6215bau')
text=text.replace('sa_draw_frame_context_connected','sa_boot_setup_connected').replace('sa_draw_frame_context_probe','sa_boot_setup_probe')
bridge=PROJECT/'source/sa_boot_setup_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_draw_frame_context.ps1';build=old_build.read_text().replace("'sa_draw_frame_context_connected.c'","'sa_boot_setup_connected.c','sa_boot_setup.c'").replace('draw_frame_context.dll','boot_setup.dll')
build_path=PROJECT/'build_boot_setup.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==129
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path]+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,reused_routine_credit=0,scope='Separate 129-source pipeline connects boot setup with original task disposal and nested cleanup, background compiler and guarded register output. Previous DLLs and their proofs remain preserved.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_setup_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated boot setup connection:',len(names),'sources')
