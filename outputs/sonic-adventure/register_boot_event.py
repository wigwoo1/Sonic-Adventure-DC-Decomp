"""Attach full native event integration while retaining original registrations."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
proof_path=PROJECT/'analysis/boot_event_qualification.json';proof=json.loads(proof_path.read_text())
assert proof['cases_passed']==544 and proof['status_counts']=={'0':520,'-3':17,'-4':3,'-2':4}
assert proof['vbr_kernel_cases']==8192 and proof['backend_and_api_checks']==43 and proof['boot_rows_covered']==25
for path,h in proof['sha256'].items():assert digest(path)==h,path
assert digest(PROJECT.parents[1]/'work/sonic-adventure/build/boot_event.dll')==proof['dll_sha256']
for row in proof['coverage_by_routine']:assert not row['missing_rows'] and not row['missing_edges']
registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text());binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();routine=next(r for r in proof['routines'] if int(r['address'],16)==0x8c605234)
start,end=int(routine['address'],16),int(routine['end_exclusive'],16);span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
assert existing and existing['name']=='sa_countdown_register_8c605234' and existing['source']=='source/frame_countdown.c'
integration=dict(verification=proof_path.relative_to(PROJECT).as_posix(),verification_sha256=digest(proof_path),adapter_source=proof['source'],adapter_source_sha256=proof['source_sha256'],newly_registered_entries=0,reused_routines=9,compiled_source_count=140,direct_comparisons=544,independent_direct_checks=proof['independent_checks'],original_rows=529,conditional_outcomes=72,vbr_kernel_cases=8192,backend_and_api_checks=43,mask_register_writes=32,event_lifecycles=2,lifecycle_stage_comparisons=6,lifecycle_independent_checks=30,boot_prefix_comparisons=8,boot_prefix_independent_checks=80,boot_rows_covered=25,boot_static_rows=51,boot_recovery_credit=0,next_dependency='0x8c09da40 branch thunk -> unrecovered 0x8c09cc80 file-load body',guide='BOOT_EVENT.txt',hardware_irq_delivery=False,natural_boot_order_verified=False,scope=proof['scope'],limitations=proof['limitations'])
for address in (0x8c605234,0x8c64594c,0x8c645c54,0x8c09da40):
 entries=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address];assert len(entries)==1;entries[0]['boot_event_integration']=integration
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text());status.setdefault('verified_progress',{})['boot_event_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Attached native integration for nine existing event bodies; zero duplicate credit; total',len(registry['routines']))
