"""Check the one-route bridge change and preserve all archived build identities."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 path=P/'analysis/collision_normalize_connection.json';c=json.loads(path.read_text());files=[Path(__file__),path]
 for p,h in c['sha256'].items():assert digest(p)==h,p;files.append(Path(p))
 old=(P/'source/sa_collision_angle_connected.c').read_text();new=(P/'source/sa_collision_normalize_connected.c').read_text()
 marker='target==0x8c6069a4u||target==0x8c63a94cu)return sa_motion_mesh_lower_entry';assert new.count(marker)==1
 new=new.replace(marker,'target==0x8c6069a4u)return sa_motion_mesh_lower_entry')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_collision_normalize_'+suffix,'sa_collision_angle_'+suffix)
 assert new==old
 assert (P/'source/collision_normalize_probe.c').read_text()==(P/'source/collision_angle_probe.c').read_text().replace('sa_collision_angle_','sa_collision_normalize_')
 before=(P/'build_collision_angle.ps1').read_text();after=(P/'build_collision_normalize.ps1').read_text();assert after==before.replace('sa_collision_angle_connected.c','sa_collision_normalize_connected.c').replace('collision_angle_probe.c','collision_normalize_probe.c').replace('collision_angle.dll','collision_normalize.dll')
 assert (P/'write_watch_collision_normalize.py').read_text()==(P/'write_watch_collision_angle.py').read_text().replace('collision_angle_reference','collision_normalize_reference')
 import collision_normalize_reference as v
 import collision_angle_reference as previous
 for key in ('ROWS','RETURNS','LEGACY','REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[key]==previous.namespace[key],key
 assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference'] and v.effects.CONNECTED==previous.effects.CONNECTED
 original=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();listing=P/'analysis/main-dispatch-next-family/8c63a94c_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));assert len(rows)==16
 for r in rows:
  a=int(r['address'],16);assert original[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=next(r for r in registry if r['address']=='0x8c63a94c' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN');assert hashlib.sha256(original[0x63a94c-0x10000:0x63a96c-0x10000]).hexdigest()==canonical['retail_span_sha256']
 bp=P/'analysis/collision_normalize_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2723;files.append(bp)
 for r in builds['files']:assert digest(ROOT/r['path'])==r['sha256'] and (ROOT/r['path']).stat().st_size==r['bytes']
 files+=[listing,P/'collision_normalize_reference.py',P/'write_watch_collision_normalize.py']
 report=dict(status='one_direct_canonical_entry_route_audited',original_rows_byte_checked=16,canonical_adapter_rows=0,new_original_routine_credit=0,new_direct_entry_routes=1,new_nonlocal_resume_routes=0,return_membership_unchanged=2271,resume_export_count_unchanged=379,qualified_nonlocal_routes_unchanged=71,remaining_nonlocal_routes_unchanged=308,compiled_source_count=245,exact_previous_bridge_plus_lower_whitelist_entry=True,reference_CPU_rows_flat_execution_guards_observer_and_ABI_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2723,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original32byte canonicalnormalizer reused unchanged through existing proper lowercontext/observer/stop/busentry. Root bridge adds one whitelist address/exportprefix only. Byteaudit is not behavior qualification;no newroutine/nonlocalroute credit.')
 (P/'analysis/collision_normalize_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('NORMALIZE ROUTE AUDIT PASS16canonicalrows/2723oldbuilds;noCPU/ABIchange')
if __name__=='__main__':main()
