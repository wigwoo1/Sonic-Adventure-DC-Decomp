"""Register the complete draining/transfer-record helpers, never the partial parent."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
proof_path=PROJECT/'analysis/boot_submit_verification.json';proof=json.loads(proof_path.read_text())
assert proof['cases_passed']==580 and proof['status_counts']=={'0':510,'-2':11,'-3':23,'-4':36}
assert proof['frame_producer_sequences']==6 and proof['sequence_stage_comparisons']==12 and proof['submission_recovery_credit']==0
assert proof['independent_checks']==2406 and proof['backend']['independent_checks']==36 and proof['dma_register_writes']==1064
for path,h in proof['sha256'].items():assert digest(path)==h,path
assert digest(PROJECT.parents[1]/'work/sonic-adventure/build/boot_submit.dll')==proof['dll_sha256']
parent=next(r for r in proof['coverage_by_routine'] if r['address']=='0x8c641f00');assert parent['covered_rows']==236 and parent['conditional_edges_covered']==18 and len(parent['missing_rows'])==31
registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text());binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
for address in (0x8c641e00,0x8c650bc8):
 row=next(r for r in proof['coverage_by_routine'] if int(r['address'],16)==address);assert not row['missing_rows'] and not row['missing_edges']
 routine=next(r for r in proof['routines'] if int(r['address'],16)==address);start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
 span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=proof['source'],verification=proof_path.relative_to(PROJECT).as_posix(),status=f"All {row['static_rows']} original rows and {row['conditional_edges_covered']} outcomes covered in 580 full-state comparisons, including bounded research-only stops for nonprogressing waits. Original loops also compile without the research limit.",limitations=proof['limitations'])
 existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
 if existing is None:registry['routines'].append(entry)
 else:existing.update(entry)
integration=dict(verification=proof_path.relative_to(PROJECT).as_posix(),verification_sha256=digest(proof_path),adapter_source=proof['source'],adapter_source_sha256=proof['source_sha256'],direct_comparisons=580,newly_registered_entries=2,compiled_source_count=134,helper_rows=295,helper_conditional_outcomes=34,parent_rows_covered=236,parent_static_rows=267,parent_recovery_credit=0,frame_producer_sequences=6,sequence_stage_comparisons=12,independent_checks=2406,backend_independent_checks=36,dma_register_writes=1064,physical_dma_or_interrupt_completion=False,natural_boot_order_verified=False,next_dependency='0x8c651bc0 interrupt/completion behavior',guide='BOOT_SUBMIT.txt',scope=proof['scope'],limitations=proof['limitations'])
for address in (0x8c653476,0x8c642440):
 existing=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address];assert len(existing)==1
 existing[0]['boot_submit_integration']=integration
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text());status.setdefault('verified_progress',{})['boot_submit_integration']=integration
status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Registered two complete submission helpers; parent remains partial; total',len(registry['routines']))
