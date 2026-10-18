import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/native_disc_read_command_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert (report['retail_formatter_cases_passed'],report['host_guard_cases_passed'],len(report['retail_files_compared']))==(24,7,2)
for source,value in report['sources'].items():assert sha(project/source)==value,source
for path,key in [(ROOT/'work/sonic-adventure/build/native_disc_queue.dll','dll_sha256'),
 (project/'verify_native_disc_read_command.py','verifier_sha256'),(project/'verify_startup.py','interpreter_sha256'),
 (project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
manifest=json.loads((project/'disc_inventory.json').read_text())
for file in report['retail_files_compared']:
    entry=next(f for f in manifest['files'] if f['path']==file['path'])
    assert file['sha256']==entry['sha256'] and file['bytes']==entry['size']
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_disc_read_command']={
 'verification':report_path,'classification':'New platform integration; no new recovered routine credit',
 'retail_formatter_cases':24,'host_guards':7,'retail_files':report['retail_files_compared'],'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded retail read formatter -> native queue -> original disc evidence')
