"""Register only the focused, hash-validated backend pump evidence."""
import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
report_path='analysis/file_backend_poll_verification.json'
report=json.loads((project/report_path).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert report['cases_passed']==64 and report['service_entries_compared']==96
for path,key in [(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN','binary_sha256'),
 (ROOT/'work/sonic-adventure/build/file_backend_poll.dll','dll_sha256'),
 (project/report['source'],'source_sha256'),(project/'verify_file_backend_poll.py','verifier_sha256'),
 (project/'verify_startup.py','interpreter_sha256')]:assert sha(path)==report[key],path
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary[0x5f05c2:0x5f05e6]).hexdigest()==report['retail_span_sha256']
path=project/'analysis/recovered_routines.json'
registry=json.loads(path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256']==report['binary_sha256']
entry={'module':'1ST_READ.BIN','load_address':'0x8c010000','address':report['address'],
 'end_exclusive':report['end_exclusive'],'name':'sa_file_backend_poll','source':report['source'],
 'source_sha256':report['source_sha256'],'retail_span_sha256':report['retail_span_sha256'],
 'indexed_instruction_bytes':36,'instruction_body_ranges':[{'start':report['address'],'end_exclusive':report['end_exclusive']}],
 'verification':report_path,'verifier_script':'verify_file_backend_poll.py','native_build_script':'build_file_backend_poll.ps1',
 'status':'Focused native backend pump differential comparisons passed',
 'comparison_level':report['comparison_level'],'limitations':report['limitations']}
prior=next((r for r in registry['routines'] if r.get('module')=='1ST_READ.BIN' and int(r['address'],16)==0x8c6005c2),None)
if prior is None:registry['routines'].append(entry)
else:prior.update(entry)
path.write_text(json.dumps(registry,indent=2)+'\n')
print('Backend pump registered;',len(registry['routines']),'routine entries')
