import hashlib
import json
from verify_startup import ROOT

PROJECT=ROOT/'outputs/sonic-adventure'
REPORT='analysis/stream_cleanup_abort_native_verification.json'
report=json.loads((PROJECT/REPORT).read_text(encoding='utf-8'))
assert (report['cases_passed'],report['service_entries_compared'],report['timer_reads_compared'])==(90,1286,456)
assert report['abort_body_instructions_exercised']==150
assert report['host_guard_cases_passed']==8

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

assert digest(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN')==report['binary_sha256']
assert digest(ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll')==report['dll_sha256']
for path,key in [('verify_stream_cleanup_abort_native.py','verifier_sha256'),
                 ('verify_request_abort_8c6013d2.py','fixture_harness_sha256'),
                 ('verify_startup.py','interpreter_sha256')]:
    assert digest(PROJECT/path)==report[key],path
for source,expected in report['sources'].items():assert digest(PROJECT/source)==expected,source

registry_path=PROJECT/'analysis/recovered_routines.json'
registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256']==report['binary_sha256']
attached=0
for routine in registry['routines']:
    if routine.get('module')=='1ST_READ.BIN' and int(routine['address'],16) in (0x8C65BF60,0x8C6013D2,0x8C600CF0,0x8C6013A4,0x8C60260E,0x8C602618,0x8C6451A4,0x8C10D9D8,0x8C6005C2,0x8C6452D6,0x8C10D960):
        reports=routine.setdefault('additional_verification',[])
        if REPORT not in reports:reports.append(REPORT)
        attached+=1
assert attached==11,attached
registry_path.write_text(json.dumps(registry,indent=2)+'\n',encoding='utf-8')
status_path=PROJECT/'project_status.json'
status=json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['stream_cleanup_abort_native']={
    'verification':REPORT,'cases':90,'service_entries_compared':1286,'timer_reads_compared':456,
    'abort_body_instructions_exercised':150,'comparison_level':report['comparison_level'],
    'sources':report['sources'],'limitations':report['limitations']}
status_path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')
print('Attached connected cleanup/abort/backend evidence to eleven routines; no new routine count')
