"""Register nine fully qualified bodies; retain existing math/copy registrations."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
proof_path=PROJECT/'analysis/boot_interrupt_qualification.json';proof=json.loads(proof_path.read_text())
assert proof['cases_passed']==589 and proof['status_counts']=={'0':566,'-4':7,'-3':4,'-1':12}
assert proof['backend_independent_checks']==66 and proof['prefix_comparisons']==16
for path,h in proof['sha256'].items():assert digest(path)==h,path
assert digest(PROJECT.parents[1]/'work/sonic-adventure/build/boot_interrupt.dll')==proof['dll_sha256']
registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text());binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
reused={0x8c10cb10,0x8c10cc80,0x8c65347c,0x8c65253e};new=0
for routine in proof['routines']:
 address=int(routine['address'],16)
 if address in reused:continue
 row=next(r for r in proof['coverage_by_routine'] if int(r['address'],16)==address)
 assert not row['missing_rows'] and not row['missing_edges']
 start,end=int(routine['address'],16),int(routine['end_exclusive'],16);span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=proof['source'],verification=proof_path.relative_to(PROJECT).as_posix(),status=f"All {row['static_rows']} original rows execute across 589 full-state comparisons. All feasible conditional outcomes covered; {len(row['structurally_unreachable_edges'])} explicitly proved unreachable outcomes. Original software IRQ callbacks execute without game-service result substitution; hardware IRQ delivery remains separate.",limitations=proof['limitations']+[proof['structural_scope']])
 existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
 if existing is None:registry['routines'].append(entry)
 else:existing.update(entry)
 new+=1
assert new==9
integration=dict(verification=proof_path.relative_to(PROJECT).as_posix(),verification_sha256=digest(proof_path),adapter_source=proof['source'],adapter_source_sha256=proof['source_sha256'],direct_comparisons=589,newly_registered_entries=9,compiled_source_count=136,total_adapter_rows=2180,reused_entries=4,independent_checks=proof['independent_checks'],backend_independent_checks=66,wait_boot_prefix_comparisons=16,wait_rows_covered=60,wait_static_rows=91,boot_rows_covered=16,boot_static_rows=51,submission_rows=267,submission_feasible_outcomes=30,physical_dma_or_interrupt_completion=False,natural_boot_order_verified=False,next_dependency='0x8c601e2a input processing',guide='BOOT_INTERRUPT.txt',scope=proof['scope'],limitations=proof['limitations'])
for address in (0x8c642440,0x8c6050b0,0x8c10b840):
 existing=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address]
 if existing:existing[0]['boot_interrupt_integration']=integration
registry_path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text());status.setdefault('verified_progress',{})['boot_interrupt_integration']=integration
status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);status['recovered_buildable_routines']=len(registry['routines']);status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Registered nine interrupt/completion/submission bodies; reused four without duplicate credit; total',len(registry['routines']))
