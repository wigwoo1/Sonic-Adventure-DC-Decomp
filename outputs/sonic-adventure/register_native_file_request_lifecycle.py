import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure';report_path='analysis/native_file_request_lifecycle_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert report['cases_passed']==4
assert report['close_checks_passed']==28 and report['handle_reuse_cycles_passed']==12
assert report['named_open_verified'] and report['named_open_guards_passed']==12
discovery=report['directory_discovery']
assert discovery['records']==768 and discovery['native_comparisons']==6
assert discovery['independent_records_checked']==768
assert discovery['context_initialized_natively'] and discovery['mount_completed_natively'] and len(discovery['mount_calls'])==1
assert discovery['cached_reload_skips_io'] and discovery['directory_fad']==discovery['directory_lba']+150
manifest=json.loads((project/'disc_inventory.json').read_text())
import shlex
track=next(t for t in reversed([shlex.split(line) for line in manifest['gdi'].splitlines()[1:] if line.strip()]) if t[2]=='4')
assert [x[0] for x in discovery['loader_sector_calls']]==[int(track[1])+16+150]+list(range(discovery['directory_fad'],discovery['directory_fad']+(discovery['directory_size']+2047)//2048))
assert [x['index'] for x in discovery['lookups']]==[4,6,0]
assert discovery['resolved'][2] is None
for source,value in report['sources'].items():assert sha(project/source)==value,source
for path,key in [(ROOT/'work/sonic-adventure/build/native_disc_queue.dll','dll_sha256'),
 (ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll','cleanup_dll_sha256'),
 (project/'analysis/stream_cleanup_disc_native_verification.json','cleanup_evidence_sha256'),
 (ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (project/'verify_native_file_request_lifecycle.py','verifier_sha256'),(project/'verify_startup.py','interpreter_sha256'),
 (project/'native_disc_directory.py','directory_helper_sha256'),
 (project/'verify_interrupt_constructor.py','interrupt_fixture_abi_sha256'),
 (project/'disc_inventory.json','inventory_sha256')]:assert sha(path)==report[key],path
cleanup=json.loads((project/'analysis/stream_cleanup_disc_native_verification.json').read_text())
assert cleanup['dll_sha256']==report['cleanup_dll_sha256']
for source,value in cleanup['sources'].items():assert sha(project/source)==value,source
manifest=json.loads((project/'disc_inventory.json').read_text())
for case in report['cases']:
    assert case['directory_preserved_after_read']
    original=next(f for f in manifest['files'] if f['path']==case['path'])
    assert case['sha256']==original['sha256'] and case['bytes']==original['size']
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['native_file_request_lifecycle']={
 'verification':report_path,'cases':4,'named_open_guards':12,'close_checks':28,'handle_reuse_cycles':12,'classification':'Original backend install/mount/named-open/read/completion/close connected to host disc; no new routine credit',
 'directory_discovery':report['directory_discovery'],'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded original handle-init/read/queue/cleanup lifecycle evidence')
