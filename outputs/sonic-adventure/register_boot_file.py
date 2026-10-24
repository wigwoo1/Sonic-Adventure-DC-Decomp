"""Register eight qualified helpers; preserve old proof/source registrations."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

qpath=PROJECT/'analysis/boot_file_qualification.json';ppath=PROJECT/'analysis/boot_file_prefix_verification.json'
q=json.loads(qpath.read_text());prefix=json.loads(ppath.read_text())
assert q['cases_passed']==403 and q['status_counts']=={'0':377,'-2':16,'-3':9,'-4':1}
assert q['qualified_rows']==222 and q['qualified_conditional_outcomes']==30
assert prefix['cases_passed']==4 and prefix['firmware_boundary']=='0x8c00f100'
for proof in (q,prefix):
 for path,h in proof['sha256'].items():assert digest(path)==h,path
 assert digest(PROJECT.parents[1]/'work/sonic-adventure/build/boot_file.dll')==proof['dll_sha256']
registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text());binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
selected={int(a,16) for a in q['qualified_addresses']};assert len(selected)==8
for routine in q['routines']:
 address=int(routine['address'],16)
 if address not in selected:continue
 row=next(r for r in q['coverage_by_routine'] if r['address']==routine['address']);assert not row['missing_rows'] and not row['missing_edges']
 end=int(routine['end_exclusive'],16);span=hashlib.sha256(binary[address-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=q['source'],verification=qpath.relative_to(PROJECT).as_posix(),status=f"Qualified body: {row['covered_rows']} original rows and {row['conditional_edges_covered']} outcomes, within 403 full-state comparisons. Status/poll tail wrappers end at the original driver transfer; underlying firmware remains partial.",limitations=q['limitations'],instruction_ranges=routine['retail_instruction_ranges'])
 existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
 if existing is None:registry['routines'].append(entry)
 else:
  assert existing['source']==entry['source'] and existing['verification']==entry['verification'],address
  existing.update(entry)
integration=dict(verification=qpath.relative_to(PROJECT).as_posix(),verification_sha256=digest(qpath),prefix_verification=ppath.relative_to(PROJECT).as_posix(),prefix_verification_sha256=digest(ppath),adapter_source=q['source'],adapter_source_sha256=q['source_sha256'],compiled_source_count=141,newly_registered_entries=8,qualified_original_rows=222,qualified_conditional_outcomes=30,direct_comparisons=403,independent_direct_checks=1137,lifecycle_sequences=6,lifecycle_stage_comparisons=30,lifecycle_independent_checks=144,boot_prefix_comparisons=4,boot_prefix_independent_checks=32,boot_rows_covered=prefix['boot_rows_covered'],boot_static_rows=51,boot_recovery_credit=0,loader_recovery_credit=0,next_dependency='Actual GD-ROM firmware vector; reuse existing native_disc_queue/native_disc_sector_reader/native_disc_read_command platform with explicit host policies, then complete cache and service paths.',guide='BOOT_FILE.txt',scope=q['scope'],limitations=q['limitations']+prefix['limitations'])
old=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c09da40');assert old['source']=='source/retail_branch_thunks.c';old['boot_file_integration']=integration
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text());status.setdefault('verified_progress',{})['boot_file_integration']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Registered eight file helpers; partial callers receive no credit; total',len(registry['routines']))
