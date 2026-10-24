"""Connect event bodies with explicit readonly VBR state; preserve previous ABI."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;manifest_path=PROJECT/'analysis/boot_event_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_boot_input_connected.c';text=old.read_text()
decl=next(line for line in text.splitlines() if line.startswith('#define SDECL('));vdecl=decl.replace('SDECL','VDECL')[:-1]+',const uint32_t*)'
marker='struct context {';assert text.count(marker)==1;text=text.replace(marker,vdecl+'\n'+''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
text=text.replace('struct sa_sh4_integer_extra *integer;uint32_t checkpoint;','struct sa_sh4_integer_extra *integer;uint32_t *vbr;uint32_t checkpoint;')
text=text.replace('struct sa_sh4_integer_extra *integer,uint32_t checkpoint){','struct sa_sh4_integer_extra *integer,uint32_t *vbr,uint32_t checkpoint){')
text=text.replace('struct sa_sh4_integer_extra *integer){','struct sa_sh4_integer_extra *integer,uint32_t *vbr){')
text=text.replace('if(!integer){','if(!integer||!vbr){')
text=text.replace('bus,gbr,integer,checkpoint}','bus,gbr,integer,vbr,checkpoint}').replace('bus,gbr,integer,checkpoint);','bus,gbr,integer,vbr,checkpoint);').replace('bus,gbr,integer,0);','bus,gbr,integer,vbr,0);')
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 address=r['address'];assert f'target=={address}u' not in text
 routes+=f' if(target=={address}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_boot_input_connected','sa_boot_event_connected').replace('sa_boot_input_probe','sa_boot_event_probe')
bridge=PROJECT/'source/sa_boot_event_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_boot_input.ps1';build=old_build.read_text().replace("'sa_boot_input_connected.c'","'sa_boot_event_connected.c','sa_boot_event.c','sh4_vector_base.c','event_register_bus.c'").replace('boot_input.dll','boot_event.dll');build_path=PROJECT/'build_boot_event.ps1';build_path.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==140
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,PROJECT/'source/event_register_bus.h',PROJECT/'source/sh4_vector_base.h']+[PROJECT/'source'/name for name in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,scope='Separate 140-source native pipeline with an explicit additional VBR pointer. Previous APIs/sources remain intact. VBR reads obey the existing privilege/slot status convention; software event masks do not deliver IRQs.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_event_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated event pipeline:',len(names),'sources; explicit VBR input')
