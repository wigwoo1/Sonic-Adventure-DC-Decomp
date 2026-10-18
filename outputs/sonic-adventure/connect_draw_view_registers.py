"""Connect original register services and word copy in a separate native DLL."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
manifest_path=PROJECT/'analysis/draw_view_registers_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_draw_view_setup_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
decl=''.join(f'BDECL({r["name"]});\n' for r in manifest['routines'])+'DECL(sa_word_copy_dispatch_8c10cddc);\n'
text=text.replace(marker,decl+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n' for r in manifest['routines'])
routes+=' if(target==0x8c10cddcu)return sa_word_copy_dispatch_8c10cddc(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n'
text=text.replace(marker,routes+marker).replace('sa_draw_view_setup_connected','sa_draw_view_registers_connected').replace('sa_draw_view_setup_probe','sa_draw_view_registers_probe')
bridge=PROJECT/'source/sa_draw_view_registers_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_draw_view_setup.ps1';build=old_build.read_text().replace("'sa_draw_view_setup_connected.c'","'sa_draw_view_registers_connected.c','sa_draw_view_registers.c','render_register_bus.c','sa_overlay_model_blend.c'").replace('draw_view_setup.dll','draw_view_registers.dll')
build_path=PROJECT/'build_draw_view_registers.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==126
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,PROJECT/'analysis/draw_view_setup_verification.json',PROJECT/'analysis/draw_view_prefix_verification.json',PROJECT/'analysis/overlay_model_blend_verification.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,reused_routine_credit=0,scope='Separate 126-source draw pipeline connects two original register services and the existing complete original word-copy dispatcher. Caller supplies an explicit software register/queue bus; no GPU pixels or hardware-status behavior is synthesized.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/draw_view_registers_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated register/view connection:',len(names),'sources.')
