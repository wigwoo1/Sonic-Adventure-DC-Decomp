import hashlib
import json
from verify_startup import ROOT

PROJECT=ROOT/'outputs/sonic-adventure'
REPORT='analysis/device_cancel_path_verification.json'
report=json.loads((PROJECT/REPORT).read_text())
assert (report['cases_passed'],report['leaf_cases_passed'],report['chain_cases_passed'])==(192,128,64)
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
assert digest(binary)==report['binary_sha256']
assert digest(ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll')==report['dll_sha256']
assert digest(PROJECT/report['source'])==report['source_sha256']
for path,key in [('verify_device_cancel_path.py','verifier_sha256'),('verify_startup.py','interpreter_sha256'),
                 ('analysis/request_device_callback_bindings.json','bindings_sha256')]:
    assert digest(PROJECT/path)==report[key],path
registry_path=PROJECT/'analysis/recovered_routines.json'
registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256']==report['binary_sha256']
for evidence in report['routines']:
    address,end=int(evidence['address'],16),int(evidence['end_exclusive'],16)
    assert hashlib.sha256(binary.read_bytes()[address-0x8C010000:end-0x8C010000]).hexdigest()==evidence['retail_span_sha256']
    routine={'module':'1ST_READ.BIN','load_address':'0x8c010000','name':evidence['name'],
             'address':evidence['address'],'end_exclusive':evidence['end_exclusive'],
             'indexed_instruction_bytes':evidence['instruction_bytes'],
             'instruction_body_ranges':[{'start':evidence['address'],'end_exclusive':evidence['end_exclusive']}],
             'retail_span_sha256':evidence['retail_span_sha256'],'source':report['source'],
             'source_sha256':report['source_sha256'],'verification':REPORT,
             'native_build_script':'build_stream_cleanup_abort_native.ps1','verifier_script':'verify_device_cancel_path.py',
             'discovery_basis':'Default device table and byte-checked tail dispatch; absent from the Ghidra draft function index.',
             'status':'Cancellation tail verified separately and in the real two-wrapper chain',
             'comparison_level':report['comparison_level'],'limitations':report['limitations']}
    prior=next((r for r in registry['routines'] if r.get('module')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
    if prior is None:registry['routines'].append(routine)
    else:prior.update(routine)
registry_path.write_text(json.dumps(registry,indent=2)+'\n',encoding='utf-8')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
progress=status.setdefault('verified_progress',{})
progress['device_cancel_path']={'verification':REPORT,'leaf_cases':128,'chain_cases':64,
                               'bindings':'analysis/request_device_callback_bindings.json',
                               'comparison_level':report['comparison_level'],'limitations':report['limitations']}
progress['recovered_buildable_routines']=len(registry['routines'])
status_path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')
print(f'Registered two cancellation wrappers; {len(registry["routines"])} routine entries')
