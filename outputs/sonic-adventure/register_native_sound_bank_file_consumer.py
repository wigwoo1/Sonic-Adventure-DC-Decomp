"""Validate native file-consumer evidence without adding decompilation credit."""
import hashlib,itertools,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
report_path='analysis/native_sound_bank_file_consumer_verification.json'
report=json.loads((project/report_path).read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert report['cases_passed']==4
assert {(c['alias'],c['path']) for c in report['cases']}==set(itertools.product(
    ('0x8c000000','0xac000000'),('SONICADV/E_00B7.MLT','SONICADV/E_0001.MLT')))
discovery=report['directory_discovery']
assert discovery['boot_directory']=='SONICADV' and discovery['mount_result']==0
assert discovery['mount_completed_natively'] and discovery['context_initialized_natively']
assert discovery['records']==discovery['independent_records_checked']
manifest=json.loads((project/'disc_inventory.json').read_text())
for case in report['cases']:
    original=next(f for f in manifest['files'] if f['path']==case['path'])
    assert case['bytes']==original['size'] and case['sha256']==original['sha256']
    assert case['download_boundary']=='0x8c093ce0'
    assert case['directory_records_preserved']==discovery['records']
    assert case['file_operations'][:2]==[0,2] and case['file_operations'][-1]==1
for path,key in [(ROOT/'work/sonic-adventure/build/native_disc_queue.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/build/sound_bank_file_consumer.dll','consumer_dll_sha256'),
 (ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll','cleanup_dll_sha256'),
 (ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (project/'analysis/native_file_request_lifecycle_verification.json','lifecycle_evidence_sha256'),
 (project/'verify_native_sound_bank_file_consumer.py','verifier_sha256'),
 (project/'verify_startup.py','interpreter_sha256'),(project/'native_disc_directory.py','directory_helper_sha256'),
 (project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
for name,value in {**report['sources'],**report['fixture_hashes']}.items():assert sha(project/name)==value,name
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_sound_bank_file_consumer']={
 'verification':report_path,'cases':4,'classification':'Original sound-bank file consumer through native disc; no new routine or milestone credit',
 'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded four native sound-bank file-consumer comparisons; audio download remains outside scope')
