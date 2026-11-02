"""Preserve the first emission and annotate an unused ABI parameter for -Werror."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
prior=P/'analysis/model_world_land_dependencies_translation.json'
j=json.loads(prior.read_text())
for name,h in j['sha256'].items():assert sha(Path(name))==h,name
source=P/'source/sa_model_world_land_dependencies.c'
text=source.read_text();assert text.count('(void)entry_pr;')==8
fixed=P/'source/sa_model_world_land_dependencies_v2.c';assert not fixed.exists()
fixed.write_text(text.replace('(void)entry_pr;','(void)entry_pr;(void)m;'))
recipe=P/'build_model_world_land_dependencies.ps1'
build=P/'build_model_world_land_dependencies_v2.ps1';assert not build.exists()
text=recipe.read_text().replace("'sa_model_world_land_dependencies.c'","'sa_model_world_land_dependencies_v2.c'")
text=text.replace('model_world_land_dependencies.dll','model_world_land_dependencies_v2.dll').replace('model_world_land_dependencies.rsp','model_world_land_dependencies_v2.rsp')
build.write_text(text)
files=[Path(__file__),prior,source,fixed,recipe,build]
report=dict(status='original_USA_dependency_rows_with_unused_parameter_annotation',
 routines=j['routines'],instruction_rows=546,original_source_preserved=True,
 change='Only eight (void)m annotations; no instruction/guard/dispatch change.',
 failed_initial_strict_build='work/sonic-adventure/model_world_land_dependencies_build.log',
 new_original_routine_credit=0,upstream_game_C_imported=False,
 sha256={str(path):sha(path) for path in files})
target=P/'analysis/model_world_land_dependencies_v2_translation.json';assert not target.exists()
target.write_text(json.dumps(report,indent=2)+'\n')
print('First source/proof preserved; explicit unused-ABI-parameter annotations emitted.')
