"""Register six complete frame/copy/select/readiness bodies; retain reuse credit."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
proof_path=PROJECT/'analysis/boot_device_verification.json';proof=json.loads(proof_path.read_text())
assert proof['cases_passed']==418 and proof['status_counts']=={'0':369,'-1':8,'-3':14,'-2':26,'-4':1}
assert proof['native_producer_sequences']==12 and proof['sequence_stage_comparisons']==24
assert proof['boot_wait_prefix_cases']==8 and proof['wait_rows_covered']==57 and proof['boot_rows_covered']==16
assert proof['command_packets']==5 and proof['backend']['independent_checks']==737 and proof['backend']['overflow_stops']==2
for path,h in proof['sha256'].items():assert digest(path)==h,path
assert digest(PROJECT.parents[1]/'work/sonic-adventure/build/boot_device.dll')==proof['dll_sha256']
registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text())
binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
new=(0x8c642440,0x8c641bc0,0x8c65253e,0x8c6564e0,0x8c65661c,0x8c652520)
for address in new:
 row=next(r for r in proof['coverage_by_routine'] if int(r['address'],16)==address);assert not row['missing_rows'] and not row['missing_edges']
 routine=next(r for r in proof['routines'] if int(r['address'],16)==address)
 start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
 span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=proof['source'],verification=proof_path.relative_to(PROJECT).as_posix(),status=f"All {row['static_rows']} original rows and {row['conditional_edges_covered']} conditional outcomes covered in the 418-case connected frame/dependency suite; qualified full-state comparisons and independent semantic checks.",limitations=proof['limitations'])
 existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
 if existing is None:registry['routines'].append(entry)
 else:existing.update(entry)
integration=dict(verification=proof_path.relative_to(PROJECT).as_posix(),verification_sha256=digest(proof_path),adapter_source=proof['source'],adapter_source_sha256=proof['source_sha256'],direct_comparisons=418,newly_registered_entries=6,frame_rows=631,frame_conditional_outcomes=70,compiled_source_count=132,native_producer_sequences=12,sequence_stage_comparisons=24,boot_wait_prefix_cases=8,wait_rows_covered=57,wait_static_rows=91,wait_recovery_credit=0,boot_rows_covered=16,boot_static_rows=51,boot_recovery_credit=0,command_packets=5,backend_independent_checks=737,external_game_service_fixtures=0,natural_boot_order_verified=False,next_dependency='0x8c641f00',guide='BOOT_DEVICE.txt',scope=proof['scope'],limitations=proof['limitations'])
for address in (0x8c650f78,0x8c6514e0):
 existing=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address];assert len(existing)==1
 existing[0]['boot_device_integration']=integration
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text());status.setdefault('verified_progress',{})['boot_device_integration']=integration
status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines'])
status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Registered six complete frame/dependency bodies; wait/boot remain partial; total',len(registry['routines']))
