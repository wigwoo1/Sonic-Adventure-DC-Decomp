import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/stream_cleanup_disc_native_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert (report['cases_passed'],report['service_entries_compared'],report['host_guard_cases_passed'])==(12,256,2)
for source,value in report['sources'].items():assert sha(project/source)==value,source
for path,key in [(ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (project/'verify_stream_cleanup_disc_native.py','verifier_sha256'),(project/'verify_startup.py','interpreter_sha256'),
 (project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['stream_cleanup_disc_native']={
 'verification':report_path,'classification':'Recovered game chain connected to new host platform policy; no new routine credit',
 'cases':12,'service_entries':256,'host_guards':2,'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded native cleanup/disc integration; no routine-count or milestone-score credit')
