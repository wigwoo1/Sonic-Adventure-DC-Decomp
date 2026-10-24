"""Connect original submission proposals with real native dependencies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest_path=PROJECT/'analysis/boot_submit_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_boot_device_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
decl='#define SDECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*,const struct sa_vertex_bus*,struct sa_sh4_integer_extra*)\n'
text=text.replace(marker,decl+''.join(f'SDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer);\n' for r in manifest['routines'])
text=text.replace(marker,routes+marker).replace('sa_boot_device_connected','sa_boot_submit_connected').replace('sa_boot_device_probe','sa_boot_submit_probe')
bridge=PROJECT/'source/sa_boot_submit_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_boot_device.ps1';build=old_build.read_text().replace("'sa_boot_device_connected.c'","'sa_boot_submit_connected.c','sa_boot_submit.c','submit_register_bus.c'").replace('boot_device.dll','boot_submit.dll')
build_path=PROJECT/'build_boot_submit.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==134
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,PROJECT/'source/submit_register_bus.h',PROJECT/'source/frame_command_bus.h']+[PROJECT/'source'/name for name in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,reused_routine_credit=0,scope='Separate 134-source native pipeline adds submission, draining, record builder and guarded existing register reader. Seven hardware command words are bounded host storage; no transfer or interrupt completion is manufactured.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_submit_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated submission pipeline:',len(names),'sources')
