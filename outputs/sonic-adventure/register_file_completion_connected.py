import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
report_path='analysis/file_completion_connected_verification.json'
report=json.loads((project/report_path).read_text())
assert (report['cases_passed'],report['service_entries_compared'],report['host_guard_cases_passed'])==(324,438,6)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
for path,key in [(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll','dll_sha256'),
 (project/'verify_file_completion_connected.py','verifier_sha256'),(project/'verify_startup.py','interpreter_sha256'),
 (project/'verify_request_abort_8c6013d2.py','fixture_harness_sha256'),
 (project/'analysis/instructions/file_completion_process.tsv','listing_sha256')]:assert sha(path)==report[key],path
for path,value in report['sources'].items():assert sha(project/path)==value,path
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary[0x5efa12:0x5efc98]).hexdigest()==report['retail_span_sha256']
path=project/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256']==report['binary_sha256']
entry={key:report[key] for key in ('module','load_address','address','end_exclusive','source','source_sha256','retail_span_sha256','instruction_body_ranges','indexed_instruction_bytes','comparison_level','limitations')}
entry.update(name='sa_file_completion_process',verification=report_path,
 native_build_script='build_stream_cleanup_abort_native.ps1',verifier_script='verify_file_completion_connected.py',
 status='Native completion processor verified through connected CPU adapter for documented statuses and modes')
prior=next((r for r in registry['routines'] if r.get('module')=='1ST_READ.BIN' and int(r['address'],16)==0x8c5ffa12),None)
if prior is None:registry['routines'].append(entry)
else:prior.update(entry)
path.write_text(json.dumps(registry,indent=2)+'\n')
print('Completion processor registered;',len(registry['routines']),'routine entries')
