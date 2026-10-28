"""Expose the already compiled canonical normalizer through its existing lower ABI."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 old=P/'source/sa_collision_angle_connected.c';text=old.read_text()
 marker='target==0x8c6069a4u)return sa_motion_mesh_lower_entry';assert text.count(marker)==1
 text=text.replace(marker,'target==0x8c6069a4u||target==0x8c63a94cu)return sa_motion_mesh_lower_entry')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_collision_angle_'+suffix,'sa_collision_normalize_'+suffix)
 bridge=P/'source/sa_collision_normalize_connected.c';bridge.write_text(text)
 old_probe=P/'source/collision_angle_probe.c';probe=P/'source/collision_normalize_probe.c';probe.write_text(old_probe.read_text().replace('sa_collision_angle_','sa_collision_normalize_'))
 old_build=P/'build_collision_angle.ps1';build=P/'build_collision_normalize.ps1';build.write_text(old_build.read_text().replace('sa_collision_angle_connected.c','sa_collision_normalize_connected.c').replace('collision_angle_probe.c','collision_normalize_probe.c').replace('collision_angle.dll','collision_normalize.dll'))
 names=re.findall(r"'([^']+\.c)'",build.read_text());assert len(names)==len(set(names))==245
 pins=[Path(__file__),old,old_probe,old_build,bridge,probe,build,P/'source/sa_draw_motion_mesh_lower_connected.c',P/'source/sa_motion_reuse_sa_main_dispatch_face_lit_primitives.c',P/'analysis/main-dispatch-next-family/8c63a94c_instructions.tsv']+[P/'source'/n for n in names]
 report=dict(compiled_source_count=245,new_recovered_routines=0,canonical_registered_entry='0x8c63a94c',new_direct_entry_routes=1,new_nonlocal_resume_routes=0,return_membership_unchanged=2271,scope='Exactprevious245bridge plusone existing lower-entry whitelist address and exportprefix. Canonicalbody,lowercontext,observer,bus,CPU,guards,ABI unchanged. No newroutine or nonlocalroutecredit.',sha256={str(p):digest(p) for p in pins})
 (P/'analysis/collision_normalize_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('NORMALIZER ROUTE GENERATED245sources;canonicalreuse,no newroutinecredit')
if __name__=='__main__':main()
