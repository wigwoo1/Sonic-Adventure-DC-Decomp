"""Add real compiler/context routines in a distinct native pipeline."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
manifest_path=PROJECT/'analysis/draw_frame_context_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_draw_frame_dependencies_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'BDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n' for r in manifest['routines'])
text=text.replace(marker,routes+marker).replace('sa_draw_frame_dependencies_connected','sa_draw_frame_context_connected').replace('sa_draw_frame_dependencies_probe','sa_draw_frame_context_probe')
bridge=PROJECT/'source/sa_draw_frame_context_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_draw_frame_dependencies.ps1';build=old_build.read_text().replace("'sa_draw_frame_dependencies_connected.c'","'sa_draw_frame_context_connected.c','sa_draw_frame_context.c'").replace('draw_frame_dependencies.dll','draw_frame_context.dll')
build_path=PROJECT/'build_draw_frame_context.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==128
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,PROJECT/'analysis/draw_frame_setup_verification.json',PROJECT/'analysis/draw_frame_prefix_verification.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,reused_routine_credit=0,scope='Separate 128-source native pipeline connects the actual callback selected by the original compiler table. Prior matrix/camera/view/register DLLs remain preserved.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/draw_frame_context_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated compiler/context pipeline:',len(names),'sources.')
