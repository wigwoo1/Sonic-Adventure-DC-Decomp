"""Pin evidence for complete native sound-bank loading; no new routine credit."""
import hashlib,itertools,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
report_path='analysis/sound_bank_load_native_verification.json'
guard_path='analysis/sound_bank_load_native_guards_verification.json'
report=json.loads((project/report_path).read_text());guards=json.loads((project/guard_path).read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert report['cases_passed']==4 and guards['checks_passed']==15
assert {(c['alias'],c['mode'],c['path']) for c in report['cases']}==set(itertools.product(
    ('0x8c000000','0xac000000'),(0,1),('SONICADV/E_00B7.MLT',)))
assert sum(c['service_boundaries_compared'] for c in report['cases'])==1240
discovery=report['directory_discovery']
assert discovery['boot_directory']=='SONICADV' and discovery['mount_completed_natively']
assert discovery['records']==discovery['independent_records_checked']
manifest=json.loads((project/'disc_inventory.json').read_text())
for case in report['cases']:
    original=next(f for f in manifest['files'] if f['path']==case['path'])
    assert case['bytes']==original['size'] and case['sha256']==original['sha256']
    assert case['returned_to']=='0x8c74913c' and case['payloads_verified']==4
    assert case['directory_records_preserved']==discovery['records']
    assert case['file_operations'][:2]==[0,2] and case['file_operations'][-1]==1
for path,key in [(ROOT/'work/sonic-adventure/build/native_sound_bank_load.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (project/'analysis/native_file_request_lifecycle_verification.json','lifecycle_evidence_sha256'),
 (project/'build_native_sound_bank_load.ps1','builder_sha256'),
 (project/'verify_sound_bank_load_native.py','verifier_sha256'),
 (project/'verify_startup.py','interpreter_sha256'),(project/'native_disc_directory.py','directory_helper_sha256'),
 (project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
for name,value in {**report['sources'],**report['fixture_hashes']}.items():assert sha(project/name)==value,name
assert guards['dll_sha256']==report['dll_sha256'] and guards['native_evidence_sha256']==sha(project/report_path)
assert guards['verifier_sha256']==sha(project/'verify_sound_bank_load_native_guards.py')
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['sound_bank_load_native']={
 'verification':report_path,'guard_verification':guard_path,'cases':4,'service_boundaries':1240,'host_guards':15,
 'classification':'Complete original sound-bank loader with native file/disc and modeled audio MMIO; no new routine or milestone credit',
 'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded complete native bank load: 4 cases, 1240 boundaries, 15 guards; audio playback unproven')
