import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/native_disc_mount_services_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert (report['retail_wrapper_cases_passed'],report['guard_cases_passed'])==(18,42)
for source,value in report['sources'].items():assert sha(project/source)==value,source
for path,key in [(ROOT/'work/sonic-adventure/build/native_disc_queue.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (project/'verify_native_disc_mount_services.py','verifier_sha256'),(project/'verify_startup.py','interpreter_sha256')]:assert sha(path)==report[key],path
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_disc_mount_services']={
 'verification':report_path,'classification':'Host mount-service prerequisites; no recovered routine credit',
 'wrapper_cases':18,'host_guards':42,'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded INIT/check-drive native mount-service evidence')
