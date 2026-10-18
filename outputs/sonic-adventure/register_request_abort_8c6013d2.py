import hashlib
import json
from verify_startup import ROOT

PROJECT=ROOT/'outputs/sonic-adventure'
REPORT='analysis/request_abort_8c6013d2_verification.json'
SOURCE='source/sa_request_abort_8c6013d2.c'
report=json.loads((PROJECT/REPORT).read_text())
assert (report['cases_passed'],report['service_entries_compared'],report['timer_reads_compared'])==(30,170,84)
assert report['body_instructions_exercised']==report['body_instructions_indexed']==150
assert report['host_guard_cases_passed']==3

def digest(path): return hashlib.sha256(path.read_bytes()).hexdigest()

binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
assert digest(binary)==report['binary_sha256']
assert digest(ROOT/'work/sonic-adventure/build/request_abort_8c6013d2.dll')==report['dll_sha256']
for path,key in [('verify_request_abort_8c6013d2.py','verifier_sha256'),
                 ('verify_startup.py','interpreter_sha256'),
                 ('recover_request_abort_8c6013d2.py','generator_sha256'),
                 ('analysis/instructions/8c6013d2.tsv','listing_sha256')]:
    assert digest(PROJECT/path)==report[key],path
for path,expected in report['sources'].items():assert digest(PROJECT/path)==expected,path
assert digest(PROJECT/SOURCE)==report['source_sha256']
assert hashlib.sha256(binary.read_bytes()[0x5F13D2:0x5F1500]).hexdigest()==report['retail_span_sha256']

registry_path=PROJECT/'analysis/recovered_routines.json'
registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256']==report['binary_sha256']
name='sa_request_abort_8c6013d2'
routine={'module':'1ST_READ.BIN','load_address':report['load_address'],
         'name':name,'address':report['address'],'end_exclusive':report['end_exclusive'],
         'indexed_instruction_bytes':300,'instruction_body_ranges':report['instruction_body_ranges'],
         'retail_span_sha256':report['retail_span_sha256'],'source':SOURCE,
         'source_sha256':report['source_sha256'],'verification':REPORT,
         'native_build_script':'build_request_abort_8c6013d2.ps1',
         'verifier_script':'verify_request_abort_8c6013d2.py',
         'status':'Native abort control flow matched 30 scenarios; device/finalizer services remain explicit fixtures',
         'comparison_level':report['comparison_level'],'limitations':report['limitations']}
prior=next((r for r in registry['routines'] if r.get('module')=='1ST_READ.BIN'
            and int(r['address'],16)==0x8C6013D2),None)
if prior is None:registry['routines'].append(routine)
else:prior.update(routine)
for item in registry['routines']:
    if item.get('module')=='1ST_READ.BIN' and int(item['address'],16) in (0x8C60260E,0x8C602618):
        evidence=item.setdefault('additional_verification',[])
        if REPORT not in evidence:evidence.append(REPORT)
registry_path.write_text(json.dumps(registry,indent=2)+'\n',encoding='utf-8')
status_path=PROJECT/'project_status.json'
status=json.loads(status_path.read_text(encoding='utf-8-sig'))
progress=status.setdefault('verified_progress',{})
progress[name]={'verification':REPORT,'cases':30,'service_entries_compared':170,
                'timer_reads_compared':84,'body_instructions_exercised':150,
                'comparison_level':report['comparison_level'],'limitations':report['limitations']}
progress['recovered_buildable_routines']=len(registry['routines'])
status_path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')
print(f'Registered {name}; {len(registry["routines"])} routine entries')
