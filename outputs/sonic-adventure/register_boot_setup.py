"""Register one new setup body and attach qualified reuse/caller evidence."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
proof_path=PROJECT/'analysis/boot_setup_verification.json';proof=json.loads(proof_path.read_text())
assert proof['cases_passed']==106 and proof['instruction_rows_covered']==34
assert proof['prefix_cases']==12 and proof['parent_rows_covered']==16 and proof['parent_recovery_credit']==0
assert proof['callback_prs']==['0x8c107aba'] and proof['external_game_service_fixtures']==0
assert all(not r['missing_rows'] and not r['missing_edges'] for r in proof['coverage_by_routine'])
for p,h in proof['sha256'].items():assert digest(p)==h,p
binary=(PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
new=next(r for r in proof['routines'] if int(r['address'],16)==0x8c605c10)
start,end=int(new['address'],16),int(new['end_exclusive'],16)
span=hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest();assert span==new['retail_span_sha256']
routine=dict(module='1ST_READ.BIN',name=new['name'],address=new['address'],end_exclusive=new['end_exclusive'],retail_span_sha256=span,source=proof['source'],verification=proof_path.relative_to(PROJECT).as_posix(),status='All 12 original instructions matched: 36 native returns and six explicit stack/bus stops through real register writer',limitations=proof['limitations'])
existing=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
if existing is None:registry['routines'].append(routine)
else:existing.update(routine)
integration=dict(verification=proof_path.relative_to(PROJECT).as_posix(),verification_sha256=digest(proof_path),adapter_source=proof['source'],adapter_source_sha256=proof['source_sha256'],cases=106,prefix_cases=12,parent_address='0x8c10b840',parent_rows_covered=16,parent_static_rows=51,parent_recovery_credit=0,callback_return_pc='0x8c107aba',external_game_service_fixtures=0,natural_boot_order_verified=False,next_dependency='0x8c6050b0',guide='BOOT_SETUP.txt',scope=proof['scope'],limitations=proof['limitations'])
for address in (0x8c107aaa,0x8c652f56,0x8c09865c):
 matches=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address]
 assert len(matches)==1
 matches[0]['boot_setup_integration']=integration
path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text())
status.setdefault('verified_progress',{})['boot_setup_integration']=integration
status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
if 'recovered_buildable_routines' in status:status['recovered_buildable_routines']=len(registry['routines'])
status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Registered one boot setup body; three existing entries attach evidence; total',len(registry['routines']))
