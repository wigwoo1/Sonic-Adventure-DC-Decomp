"""Register six input bodies and the newly completed 91-row wait caller."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
qpath=PROJECT/'analysis/boot_input_qualification.json';wpath=PROJECT/'analysis/boot_input_sequence_verification.json';q=json.loads(qpath.read_text());w=json.loads(wpath.read_text())
assert q['cases_passed']==503 and q['status_counts']=={'0':493,'-3':10} and q['structurally_unreachable_rows']==4
assert w['cases_passed']==24 and w['coverage_by_routine'][0]['covered_rows']==91 and w['coverage_by_routine'][0]['conditional_edges_covered']==8
assert w['boot_rows_covered']==20
for proof in (q,w):
 for path,h in proof['sha256'].items():assert digest(path)==h,path
 assert digest(PROJECT.parents[1]/'work/sonic-adventure/build/boot_input.dll')==proof['dll_sha256']
registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text());binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
new_addresses={0x8c601e2a,0x8c601df6,0x8c601bb2,0x8c601da6,0x8c601bf0,0x8c601d84};count=0
for proof,proof_path in ((q,qpath),(w,wpath)):
 for routine in proof['routines']:
  address=int(routine['address'],16)
  if proof is q and address not in new_addresses:continue
  row=next(r for r in proof['coverage_by_routine'] if int(r['address'],16)==address);dead=row.get('structurally_unreachable_rows',[])
  assert row['covered_rows']+len(dead)==row['static_rows'] and row['missing_rows']==dead
  assert row['missing_edges']==row.get('structurally_unreachable_edges',[])
  start,end=address,int(routine['end_exclusive'],16);span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=proof['source'],verification=proof_path.relative_to(PROJECT).as_posix(),status=f"Qualified in {proof['cases_passed']} full-state comparisons. {row['covered_rows']}/{row['static_rows']} original rows execute; {len(dead)} explicitly structurally unreachable rows receive no executed-row credit. Every feasible conditional outcome covered.",limitations=proof['limitations'])
  existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
  if existing is None:registry['routines'].append(entry)
  else:existing.update(entry)
  count+=1
assert count==7
integration=dict(verification=qpath.relative_to(PROJECT).as_posix(),verification_sha256=digest(qpath),sequence_verification=wpath.relative_to(PROJECT).as_posix(),sequence_verification_sha256=digest(wpath),adapter_source=q['source'],adapter_source_sha256=q['source_sha256'],compiled_source_count=137,newly_registered_entries=7,new_input_bodies=6,reused_input_adapters=18,direct_comparisons=503,independent_direct_checks=q['independent_checks'],lifecycle_sequences=4,lifecycle_stage_comparisons=12,lifecycle_independent_checks=1608,wait_boot_comparisons=24,sequence_independent_checks=192,wait_rows_covered=91,wait_static_rows=91,boot_rows_covered=20,boot_static_rows=51,boot_recovery_credit=0,hardware_controller_transport_verified=False,natural_boot_order_verified=False,next_dependency='0x8c605234',guide='BOOT_INPUT.txt',scope=q['scope'],limitations=q['limitations']+w['limitations'])
for address in (0x8c6050b0,0x8c601e2a,0x8c642440):
 existing=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address];assert len(existing)==1;existing[0]['boot_input_integration']=integration
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text());status.setdefault('verified_progress',{})['boot_input_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Registered six input bodies and complete wait caller; total',len(registry['routines']))
