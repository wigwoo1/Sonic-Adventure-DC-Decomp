"""Register only the completed elapsed/callback bodies, never the partial wait."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
proof_path=PROJECT/'analysis/boot_wait_verification.json';proof=json.loads(proof_path.read_text())
assert proof['cases_passed']==304 and proof['helper_rows_covered']==32 and proof['wait_rows_covered']==46
assert proof['boot_prefix_cases']==12 and proof['boot_parent_rows_covered']==16 and proof['wait_recovery_credit']==0
assert proof['status_counts']=={'0':248,'-2':46,'-3':6,'-4':4}
for path,h in proof['sha256'].items():assert digest(path)==h,path
assert digest(PROJECT.parents[1]/'work/sonic-adventure/build/boot_wait.dll')==proof['dll_sha256']
binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for address,description in ((0x8c602652,'13-row unsigned elapsed conversion matched in 108 direct comparisons with independent integer arithmetic/MACL/footprint checks'),(0x8c60509a,'11-row optional callback wrapper matched in 24 direct comparisons, including real RTS/NOP callback and explicit missing-callback/stack stops')):
 row=next(r for r in proof['coverage_by_routine'] if int(r['address'],16)==address);assert not row['missing_rows'] and not row['missing_edges']
 routine=next(r for r in proof['routines'] if int(r['address'],16)==address)
 start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
 span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],retail_span_sha256=span,source=proof['source'],verification=proof_path.relative_to(PROJECT).as_posix(),status=description,limitations=proof['limitations'])
 existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
 if existing is None:registry['routines'].append(entry)
 else:existing.update(entry)
integration=dict(verification=proof_path.relative_to(PROJECT).as_posix(),verification_sha256=digest(proof_path),adapter_source=proof['source'],adapter_source_sha256=proof['source_sha256'],direct_comparisons=304,helper_rows=32,wait_rows_covered=46,wait_static_rows=91,wait_recovery_credit=0,boot_prefix_cases=12,boot_parent_rows=16,boot_parent_static_rows=51,external_game_service_fixtures=0,timer_samples='Finite explicit peripheral fixtures; frequency/interrupt timing unverified',natural_boot_order_verified=False,next_dependency='0x8c642440',guide='BOOT_WAIT.txt',scope=proof['scope'],limitations=proof['limitations'])
for address in (0x8c60260e,0x8c602618,0x8c63eeb0):
 existing=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address];assert len(existing)==1
 existing[0]['boot_wait_integration']=integration
path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text())
status.setdefault('verified_progress',{})['boot_wait_integration']=integration
status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
if 'recovered_buildable_routines' in status:status['recovered_buildable_routines']=len(registry['routines'])
status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Registered two complete helpers; wait remains partial; total',len(registry['routines']))
