import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/file_mount_registration_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert (report['cases_passed'],report['constructor_entries_compared'])==(32,32)
assert report['native_primary_installation']
for source,value in report['sources'].items():assert sha(project/source)==value,source
for path,key in [(ROOT/'work/sonic-adventure/build/file_mount_registration.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (project/'verify_file_mount_registration.py','verifier_sha256'),(project/'verify_startup.py','interpreter_sha256')]:assert sha(path)==report[key],path
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['file_mount_registration']={
 'verification':report_path,'cases':32,'constructor_entries':32,
 'native_primary_installation':True,'classification':'Original mount registration prefix with native handler installation; no new routine credit','limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded original mount/native callback-constructor evidence')
