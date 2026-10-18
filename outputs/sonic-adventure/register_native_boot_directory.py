import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/native_boot_directory_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert report['cases_passed']==4
assert [c['boot_directory'] for c in report['cases']]==[None,'SONICADV','sonicadv','__MISSING__']
assert [c['mount_result'] for c in report['cases']]==[0,0,0,0xfffffffc]
assert report['cases'][-1]['mounted_flag']==0
for source,value in report['sources'].items():assert sha(project/source)==value,source
for path,key in [(ROOT/'work/sonic-adventure/build/native_disc_queue.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll','cleanup_dll_sha256'),
 (project/'verify_native_boot_directory.py','verifier_sha256'),(project/'native_disc_directory.py','directory_helper_sha256'),
 (project/'verify_startup.py','interpreter_sha256'),(project/'disc_inventory.json','inventory_sha256'),
 (project/'analysis/native_file_request_lifecycle_verification.json','lifecycle_evidence_sha256')]:assert sha(path)==report[key],path
cleanup=json.loads((project/'analysis/stream_cleanup_disc_native_verification.json').read_text())
assert cleanup['dll_sha256']==report['cleanup_dll_sha256']
for source,value in cleanup['sources'].items():assert sha(project/source)==value,source
manifest=json.loads((project/'disc_inventory.json').read_text())
for case in report['cases'][:-1]:
    assert case['independent_records_checked']==case['records'] and case['cached_reload_skips_io']
    for file in case['file_hashes']:
        original=next(f for f in manifest['files'] if f['path']==file['path'])
        assert file['sha256']==original['sha256']
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_boot_directory']={
 'verification':report_path,'cases':4,'classification':'Original mount boot-directory selection; no new routine credit',
 'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded root/SONICADV/case-folded/missing-directory mount evidence')
