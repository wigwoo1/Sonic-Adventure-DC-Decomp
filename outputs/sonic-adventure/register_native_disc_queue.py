import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/native_disc_queue_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert len(report['scenario_groups_passed'])==9 and len(report['retail_files_compared'])==4 and report['retail_steps']==3304
for path,value in report['sources'].items():assert sha(project/path)==value,path
for path,key in [(ROOT/'work/sonic-adventure/build/native_disc_queue.dll','dll_sha256'),
 (project/'verify_native_disc_queue.py','verifier_sha256'),(project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
manifest=json.loads((project/'disc_inventory.json').read_text())
for file in report['retail_files_compared']:
    original=next(f for f in manifest['files'] if f['path']==file['path'])
    assert file['bytes']==original['size'] and file['sha256']==original['sha256']
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_disc_queue']={
 'verification':report_path,'classification':'New native platform support; no recovered routine or milestone credit',
 'scenario_groups':report['scenario_groups_passed'],'retail_files':report['retail_files_compared'],
 'retail_steps':report['retail_steps'],'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded native disc queue evidence; game-routine count and milestone score unchanged')
