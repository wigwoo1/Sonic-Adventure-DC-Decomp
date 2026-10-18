"""Regression lifetimes using the new complete register-input matrix bodies."""
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
OLD=PROJECT/'verify_main_dispatch_mesh_families_lifecycle.py'
code=OLD.read_text()
changes={
 'import verify_main_dispatch_mesh_families as combined':'import verify_draw_matrix_renderer as combined',
 'main_dispatch_mesh_families_lifecycle_verification.json':'draw_matrix_renderer_lifecycle_verification.json',
 "PROJECT/'build_main_dispatch_mesh_families.ps1'":"PROJECT/'build_draw_matrix_renderer.ps1'",
 "PROJECT/'source/sa_main_dispatch_environment_constant_alternate_strips_connected.c'":"PROJECT/'source/sa_draw_matrix_renderer_connected.c'",
 'records=[];stages=boundaries=':'matrix_calls={0x8c63a69c:0,0x8c63a878:0};records=[];stages=boundaries=',
 "   assert status==0":"   for target in matrix_calls:matrix_calls[target]+=path.count(target)\n   assert status==0",
 " files=[Path(__file__),":" files=[OLD,PROJECT/'verify_draw_matrix_renderer.py',PROJECT/'analysis/draw_matrix_renderer_connection.json',PROJECT/'analysis/draw_matrix_registers_verification.json',Path(__file__),",
 ' result=dict(lifecycles_passed=':' result=dict(matrix_invocations={hex(a):n for a,n in matrix_calls.items()},lifecycles_passed=',
 "             scope='Native spawn":"             scope='Complete register-input scale/translation bodies in a separate renderer build. Native spawn",
}
for old,new in changes.items():assert code.count(old)==1,(old,code.count(old));code=code.replace(old,new)
# Include every compiled input, the focused matrix proof and their transitive
# source fingerprints. The old regression's baseline remains a frozen input.
needle=" result=dict(matrix_invocations=";assert code.count(needle)==1
code=code.replace(needle," for proof in ('draw_matrix_renderer_connection.json','draw_matrix_registers_verification.json'):\n  files.extend(Path(p) for p in json.loads((PROJECT/'analysis'/proof).read_text())['sha256'])\n result=dict(matrix_invocations=")
exec(compile(code,str(Path(__file__).resolve()),'exec'),globals())
