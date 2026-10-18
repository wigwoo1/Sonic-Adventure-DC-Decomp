"""Create a new renderer bridge without changing frozen prior integrations."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
OLD_BRIDGE=PROJECT/'source/sa_main_dispatch_environment_constant_alternate_strips_connected.c'
OLD_BUILD=PROJECT/'build_main_dispatch_mesh_families.ps1'
source=OLD_BRIDGE.read_text()
prototype='int NAME(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*);\n'
marker='struct context {';assert source.count(marker)==1
source=source.replace(marker,prototype.replace('NAME','sa_draw_matrix_scale_registers_8c63a69c')+prototype.replace('NAME','sa_draw_matrix_translate_registers_8c63a878')+marker)
for old,target,name in (('sa_model_matrix_scale_registers(ram,f,sr,r)',0x8c63a69c,'sa_draw_matrix_scale_registers_8c63a69c'),('sa_model_matrix_translate_registers_implicit(f,sr,r,s,vector_instruction)',0x8c63a878,'sa_draw_matrix_translate_registers_8c63a878')):
 before=f'if(target==0x{target:x}u)return {old};'
 after=f'if(target==0x{target:x}u){{uint32_t stop=target;return {name}(ram,f,sr,r,pr,s->fpul,s->macl,0,0,&stop);}}'
 assert source.count(before)==1;source=source.replace(before,after)
bridge=PROJECT/'source/sa_draw_matrix_renderer_connected.c';bridge.write_text(source)
build=OLD_BUILD.read_text();old="'sa_main_dispatch_environment_constant_alternate_strips_connected.c'";assert build.count(old)==1
build=build.replace(old,"'sa_draw_matrix_renderer_connected.c','sa_draw_matrix_registers.c'").replace('main_dispatch_mesh_families.dll','draw_matrix_renderer.dll')
build=build.replace('Combined primitive family portable bus chain build failed','Register-input matrix renderer integration build failed')
build_path=PROJECT/'build_draw_matrix_renderer.ps1';build_path.write_text(build)
files=[Path(__file__),OLD_BRIDGE,OLD_BUILD,bridge,build_path,PROJECT/'source/sa_draw_matrix_registers.c']
# Pin every compiled source: this new DLL incorporates the whole native chain.
names=re.findall(r"'([^']+\.c)'",build)
files.extend(PROJECT/'source'/name for name in names)
report=dict(source=bridge.relative_to(PROJECT).as_posix(),newly_recovered_entries=0,reused_routine_credit=0,compiled_source_count=len(names),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Separate complete mesh renderer build routes scale/translate register-input boundaries to the newly completed guarded original bodies. All older sources, builds, and reports preserved. Legacy renderer ABI retains its existing partial-stop limitations.')
(PROJECT/'analysis/draw_matrix_renderer_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Separate native renderer bridge and build generated:',len(names),'compiled sources.')
