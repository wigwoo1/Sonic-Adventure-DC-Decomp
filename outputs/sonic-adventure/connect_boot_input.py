"""Extend the interrupt pipeline with original input code and reused dependencies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest_path=PROJECT/'analysis/boot_input_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_boot_interrupt_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'SDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''
for r in manifest['routines']:
 address=r['address'];assert f'target=={address}u' not in text
 routes+=f' if(target=={address}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer);\n'
text=text.replace(marker,routes+marker).replace('sa_boot_interrupt_connected','sa_boot_input_connected').replace('sa_boot_interrupt_probe','sa_boot_input_probe')
bridge=PROJECT/'source/sa_boot_input_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_boot_interrupt.ps1';build=old_build.read_text().replace("'sa_boot_interrupt_connected.c'","'sa_boot_input_connected.c','sa_boot_input.c'").replace('boot_interrupt.dll','boot_input.dll');build_path=PROJECT/'build_boot_input.ps1';build_path.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==137
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path]+[PROJECT/'source'/name for name in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,scope='Separate 137-source original startup/wait/frame/submission/IRQ/input research pipeline. Runtime tables are explicit inputs; hardware device acquisition, natural initialization and ports remain separate.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_input_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated input pipeline:',len(names),'sources')
