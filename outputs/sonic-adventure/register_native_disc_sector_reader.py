"""Record native platform progress without counting it as recovered game code."""
import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
report_path='analysis/native_disc_sector_reader_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert (report['synthetic_cases_passed'],report['host_guard_cases_passed'],len(report['retail_files_compared']))==(25,11,4)
for path,key in [(project/'source/native_disc_sector_reader.c','source_sha256'),
 (ROOT/'work/sonic-adventure/build/native_disc_sector_reader.dll','dll_sha256'),
 (project/'verify_native_disc_sector_reader.py','verifier_sha256'),(project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
manifest=json.loads((project/'disc_inventory.json').read_text())
for file in report['retail_files_compared']:
    entry=next(f for f in manifest['files'] if f['path']==file['path'])
    assert entry['size']==file['bytes'] and entry['sha256']==file['sha256']
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_disc_sector_reader']={
 'verification':report_path,'classification':'New platform support; no recovered game-routine credit',
 'synthetic_cases':25,'host_guards':11,'retail_files':report['retail_files_compared'],
 'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded native disc sector reader; no game-routine or completion-score credit')
