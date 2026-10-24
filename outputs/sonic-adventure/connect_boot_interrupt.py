"""Connect actual interrupt and buffer-building code with explicit VRAM/IRQ storage."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest_path=PROJECT/'analysis/boot_interrupt_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_boot_submit_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'SDECL({r["name"]});\n' for r in manifest['routines'])+marker)
def route(address,name):return f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer);'
overrides={'0x8c65253e','0x8c10cc80','0x8c641f00'}
for routine in manifest['routines']:
 if routine['address'] not in overrides:continue
 pattern=r' if\(target=='+routine['address']+r'u\)return [^;]+;';assert len(re.findall(pattern,text))==1
 text=re.sub(pattern,route(routine['address'],routine['name']),text)
marker=' return -2;';assert text.count(marker)==1
routes='\n'.join(route(r['address'],r['name']) for r in manifest['routines'] if r['address'] not in overrides)+'\n'
text=text.replace(marker,routes+marker).replace('sa_boot_submit_connected','sa_boot_interrupt_connected').replace('sa_boot_submit_probe','sa_boot_interrupt_probe')
bridge=PROJECT/'source/sa_boot_interrupt_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_boot_submit.ps1';build=old_build.read_text().replace("'sa_boot_submit_connected.c'","'sa_boot_interrupt_connected.c','sa_boot_interrupt.c','interrupt_bus.c'").replace('boot_submit.dll','boot_interrupt.dll')
build_path=PROJECT/'build_boot_interrupt.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==136
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,PROJECT/'source/interrupt_bus.h',PROJECT/'source/submit_register_bus.h',PROJECT/'source/frame_command_bus.h']+[PROJECT/'source'/name for name in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,reused_routine_credit=0,scope='Separate 136-source pipeline with original interrupt and buffer-building dependencies, guarded existing math, VRAM-aware original word copy, and explicit bounded VRAM/event-ack storage. No asynchronous IRQ/DMA completion or GPU pixels are supplied.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_interrupt_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated interrupt pipeline:',len(names),'sources')
