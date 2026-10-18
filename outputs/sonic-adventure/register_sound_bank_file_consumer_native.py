"""Pin evidence for native C consumer integration; no new routine credit."""
import hashlib,itertools,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
report_path='analysis/sound_bank_file_consumer_native_verification.json'
guard_path='analysis/sound_bank_file_consumer_native_guards_verification.json'
report=json.loads((project/report_path).read_text());guards=json.loads((project/guard_path).read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert report['cases_passed']==4 and guards['checks_passed']==13
assert {(c['alias'],c['path']) for c in report['cases']}==set(itertools.product(
    ('0x8c000000','0xac000000'),('SONICADV/E_00B7.MLT','SONICADV/E_0001.MLT')))
assert sum(c['service_boundaries_compared'] for c in report['cases'])==1652
discovery=report['directory_discovery']
assert discovery['boot_directory']=='SONICADV' and discovery['mount_completed_natively']
assert discovery['records']==discovery['independent_records_checked']
manifest=json.loads((project/'disc_inventory.json').read_text())
for case in report['cases']:
    original=next(f for f in manifest['files'] if f['path']==case['path'])
    assert case['bytes']==original['size'] and case['sha256']==original['sha256']
    assert case['download_boundary']=='0x8c093ce0'
    assert case['directory_records_preserved']==discovery['records']
    assert case['file_operations'][:2]==[0,2] and case['file_operations'][-1]==1
for path,key in [(ROOT/'work/sonic-adventure/build/native_sound_bank_file_consumer.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (project/'analysis/native_file_request_lifecycle_verification.json','lifecycle_evidence_sha256'),
 (project/'build_native_sound_bank_file_consumer.ps1','builder_sha256'),
 (project/'verify_sound_bank_file_consumer_native.py','verifier_sha256'),
 (project/'verify_startup.py','interpreter_sha256'),(project/'native_disc_directory.py','directory_helper_sha256'),
 (project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
for name,value in {**report['sources'],**report['fixture_hashes']}.items():assert sha(project/name)==value,name
assert guards['dll_sha256']==report['dll_sha256'] and guards['native_evidence_sha256']==sha(project/report_path)
assert guards['verifier_sha256']==sha(project/'verify_sound_bank_file_consumer_native_guards.py')
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['sound_bank_file_consumer_native']={
 'verification':report_path,'guard_verification':guard_path,'cases':4,'service_boundaries':1652,'host_guards':13,
 'classification':'Original sound-bank file consumer and mount/file services dispatched in native C; no new routine or milestone credit',
 'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded native C file consumer: 4 cases, 1652 boundaries, 13 guards; audio download remains external')
