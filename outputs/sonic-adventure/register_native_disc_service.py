import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/native_disc_service_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert (report['retail_status_cases_passed'],report['host_guards_passed'])==(21,42)
assert report['synchronous_guest_read_cases_passed']==15
assert report['initialization_cases_passed']==4
for source,value in report['sources'].items():assert sha(project/source)==value,source
for path,key in [(ROOT/'work/sonic-adventure/build/native_disc_queue.dll','dll_sha256'),
 (project/'verify_native_disc_service.py','verifier_sha256'),(project/'verify_startup.py','interpreter_sha256'),
 (project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_disc_service']={
 'verification':report_path,'classification':'New platform service subset; no recovered routine credit',
 'retail_status_cases':21,'host_guards':42,'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded native disc submit/process/status/cancel integration evidence')
