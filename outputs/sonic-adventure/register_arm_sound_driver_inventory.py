"""Record draft ARM inventory without treating it as recovered game source."""
import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
path=project/'analysis/arm_sound_driver_inventory_verification.json'
report=json.loads(path.read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert {d['path'] for d in report['drivers']}=={'MANATEE.DRV','SONICADV/MASARU.DRV'}
for file,value in {**report['inputs'],**report['outputs']}.items():assert sha(ROOT/file)==value,file
payloads=json.loads((project/'analysis/arm_sound_driver_payloads.json').read_text())
for driver in payloads['drivers']:
    assert sha(ROOT/driver['payload_path'])==driver['upload_sha256']
for driver in report['drivers']:
    assert driver['candidate_functions']==108 and driver['vectors_checked']==8
    assert driver['statuses']=={'draft_pseudocode':108}
    assert driver['instructions']>6000 and driver['draft_warning_count']>0
status_path=project/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_driver_draft_inventory']={
 'verification':'analysis/arm_sound_driver_inventory_verification.json',
 'drivers':report['drivers'],'classification':'ARM draft inventory only; no verified routine or milestone credit',
 'limitations':report['limitations']}
status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded two ARM driver draft inventories: 216 module-local candidates, no verified routine credit')
