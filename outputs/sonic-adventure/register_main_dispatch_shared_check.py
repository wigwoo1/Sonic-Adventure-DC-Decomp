"""Register the pure shared check and both native transition connections."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_shared_check_verification.json'
START,END=0x8C06C880,0x8C06CA30


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['address']==hex(START) and report['end_exclusive']==hex(END)
    assert report['cases_passed']==4188 and report['counts']=={'check':3408,'status':594,'flag':186}
    assert report['direct_check_rows_covered']==195 and report['direct_conditional_edges_covered']==76
    assert report['parent_rows_covered']=={'status':153,'flag':55}
    assert report['closed_flag_cases_with_null_external_service']==160
    assert report['unobserved_cases_passed']==24
    assert report['missing_service_checks_passed']==report['partial_service_error_checks_passed']==2
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    manifest=json.loads((PROJECT/'analysis/main_dispatch_shared_check_translation.json').read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_shared_check.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    assert hashlib.sha256(binary[START-0x8C010000:END-0x8C010000]).hexdigest()==report['retail_span_sha256']
    diagnostic=PROJECT/'analysis/main-dispatch-shared-check/diagnostics.tsv'
    diagnostics=list(csv.DictReader(diagnostic.open(),delimiter='\t'))
    assert len(diagnostics)==1 and diagnostics[0]['address']=='8c06c880'
    assert diagnostics[0]['body_bytes']=='390' and diagnostics[0]['completed']=='true'
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000',
        'name':'sa_main_dispatch_shared_check_8c06c880','address':hex(START),'end_exclusive':hex(END),
        'indexed_instruction_bytes':390,'retail_span_sha256':report['retail_span_sha256'],
        'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_shared_check.ps1',
        'verifier_script':'verify_main_dispatch_shared_check.py',
        'status':'Pure integer check and both native parents matched 4188 constructed retail comparisons; all 195 check rows / 76 edges covered directly, all 208 parent rows covered in connected tests',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-shared-check/8c06c880_instructions.tsv',
        'descriptive_name_is_inferred':True,
    }
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==START),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_check':hex(START),'native_query':'0x8c06cebe',
            'parent_rows_covered':report['parent_rows_covered'],
            'closed_flag_cases_with_null_external_service':160,
            'scope':report['scope'],'limitations':report['limitations']}
    for address in (0x8C0AFC98,0x8C0B11C0):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['native_shared_check']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_shared_check']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered shared check and both native parent connections; {len(registry["routines"])} entries. Full game/ports remain unfinished.')


if __name__=='__main__':main()
