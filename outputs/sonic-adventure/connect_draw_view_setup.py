"""Extend the preserved complete clipping pipeline without changing old DLLs."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
manifest_path=PROJECT/'analysis/draw_view_setup_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_draw_clipping_state_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
declaration='void sa_empty_stub_8c611708(struct sa_cpu_state*);'
assert text.count(declaration)==1
text=text.replace(declaration,declaration+'\n'+declaration.replace('8c611708','8c63eeb0'))
route=next(line for line in text.splitlines(True) if line.startswith(' if(target==0x8c611708u)'))
text=text.replace(route,route+route.replace('8c611708','8c63eeb0'))
text=text.replace(marker,''.join(f'BDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n' for r in manifest['routines'])
text=text.replace(marker,routes+marker).replace('sa_draw_clipping_state_connected','sa_draw_view_setup_connected').replace('sa_draw_clipping_state_probe','sa_draw_view_setup_probe')
bridge=PROJECT/'source/sa_draw_view_setup_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_draw_clipping_complete.ps1';build=old_build.read_text().replace("'sa_draw_clipping_state_connected.c'","'sa_draw_view_setup_connected.c','sa_draw_view_setup.c'").replace('draw_clipping_complete.dll','draw_view_setup.dll')
build_path=PROJECT/'build_draw_view_setup.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==123
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,PROJECT/'analysis/draw_clipping_complete_verification.json',PROJECT/'analysis/draw_material_dispatch_verification.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_recovered_entries=0,reused_original_empty_entry='0x8c63eeb0',reused_routine_credit=0,scope='Separate 123-source extended-state DLL adds four original view-builder dependencies and the existing byte-proved RTS/NOP function at 8c63eeb0, retaining the full native clipping pipeline. No runtime opcode interpreter or substituted game-service results.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/draw_view_setup_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated view setup connection:',len(names),'sources.')
