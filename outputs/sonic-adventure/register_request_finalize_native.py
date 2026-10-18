import hashlib
import json
from verify_startup import ROOT

PROJECT=ROOT/'outputs/sonic-adventure'
REPORT='analysis/request_finalize_native_verification.json'
report=json.loads((PROJECT/REPORT).read_text())
assert (report['finalizer_cases_passed'],report['status_cases_passed'],
        report['service_entries_compared'],report['timer_reads_compared'])==(26,64,166,52)
assert [r['instructions_exercised'] for r in report['routines']]==[73,12]
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
assert digest(binary)==report['binary_sha256']
assert digest(ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll')==report['dll_sha256']
for path,key in [('verify_request_finalize_native.py','verifier_sha256'),
                 ('recover_request_abort_8c6013d2.py','generator_sha256'),('verify_startup.py','interpreter_sha256')]:
    assert digest(PROJECT/path)==report[key],path
for path,expected in report['sources'].items():assert digest(PROJECT/path)==expected,path
registry_path=PROJECT/'analysis/recovered_routines.json'
registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256']==report['binary_sha256']
for evidence in report['routines']:
    address,end=int(evidence['address'],16),int(evidence['end_exclusive'],16)
    assert digest(PROJECT/evidence['source'])==evidence['source_sha256']
    assert digest(PROJECT/f'analysis/instructions/{address:08x}.tsv')==evidence['listing_sha256']
    assert hashlib.sha256(binary.read_bytes()[address-0x8C010000:end-0x8C010000]).hexdigest()==evidence['retail_span_sha256']
    routine={'module':'1ST_READ.BIN','load_address':'0x8c010000',
             'name':evidence['name'],'address':evidence['address'],'end_exclusive':evidence['end_exclusive'],
             'indexed_instruction_bytes':evidence['instruction_bytes'],
             'instruction_body_ranges':[{'start':evidence['address'],'end_exclusive':evidence['end_exclusive']}],
             'retail_span_sha256':evidence['retail_span_sha256'],'source':evidence['source'],
             'source_sha256':evidence['source_sha256'],'verification':REPORT,
             'native_build_script':'build_stream_cleanup_abort_native.ps1',
             'verifier_script':'verify_request_finalize_native.py',
             'status':'Native finalizer/status verified against retail with device-pump fixtures',
             'comparison_level':report['comparison_level'],'limitations':report['limitations']}
    prior=next((r for r in registry['routines'] if r.get('module')=='1ST_READ.BIN' and int(r['address'],16)==address),None)
    if prior is None:registry['routines'].append(routine)
    else:prior.update(routine)
for routine in registry['routines']:
    if routine.get('module')=='1ST_READ.BIN' and int(routine['address'],16) in (0x8C60260E,0x8C602618):
        evidence=routine.setdefault('additional_verification',[])
        if REPORT not in evidence:evidence.append(REPORT)
registry_path.write_text(json.dumps(registry,indent=2)+'\n',encoding='utf-8')
status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
progress=status.setdefault('verified_progress',{})
for routine in report['routines']:
    progress[routine['name']]={'verification':REPORT,'cases':26 if int(routine['address'],16)==0x8C600CF0 else 64,
                               'instructions_exercised':routine['instructions_exercised'],
                               'comparison_level':report['comparison_level'],'limitations':report['limitations']}
progress['recovered_buildable_routines']=len(registry['routines'])
status_path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')
print(f'Registered native finalizer and status helper; {len(registry["routines"])} routine entries')
