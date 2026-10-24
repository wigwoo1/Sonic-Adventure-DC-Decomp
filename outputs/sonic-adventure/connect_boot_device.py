"""A distinct native pipeline adds actual wait, clock and elapsed helper calls."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest_path=PROJECT/'analysis/boot_device_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_boot_wait_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'BDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n' for r in manifest['routines'])
text=text.replace(marker,routes+marker).replace('sa_boot_wait_connected','sa_boot_device_connected').replace('sa_boot_wait_probe','sa_boot_device_probe')
bridge=PROJECT/'source/sa_boot_device_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_boot_wait.ps1';build=old_build.read_text().replace("'sa_boot_wait_connected.c'","'sa_boot_device_connected.c','sa_boot_device.c'").replace('boot_wait.dll','boot_device.dll')
build_path=PROJECT/'build_boot_device.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==131
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path]+[PROJECT/'source'/name for name in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,reused_routine_credit=0,scope='Separate 131-source pipeline adds the frame/device entry and seven actual dependency bodies. Remaining callees and behavior acceptance stay explicit. Earlier source and DLLs remain preserved.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_device_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated wait pipeline:',len(names),'sources')
