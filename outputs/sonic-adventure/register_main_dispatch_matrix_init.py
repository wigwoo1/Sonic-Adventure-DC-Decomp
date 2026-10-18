"""Register native matrix initialization and its connected caller evidence."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_matrix_init_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==2240
    assert report['counts']=={'init':1232,'transform':240,'detach':256,'status':256,'flag':256}
    entries=('init','transform','detach','status','flag')
    assert report['init_rows_by_entry']==dict.fromkeys(entries,50)
    assert report['init_conditional_edges_by_entry']==dict.fromkeys(entries,2)
    assert report['boundary_comparisons']==6544 and report['independent_save_identity_checks_passed']==162
    assert report['partial_precision_guards_passed']==924 and report['partial_service_error_cases_passed']==136
    assert report['unobserved_cases_passed']==16 and report['null_external_cases_passed']==40
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    manifest=json.loads((PROJECT/'analysis/main_dispatch_matrix_init_translation.json').read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_matrix_init.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    address,end=0x8C057B40,0x8C057BAC
    assert report['address']==hex(address) and report['end_exclusive']==hex(end) and report['instruction_bytes']==100
    assert hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest()==report['retail_span_sha256']
    diagnostic=list(csv.DictReader((PROJECT/'analysis/main-dispatch-matrix-init/diagnostics.tsv').open(),delimiter='\t'))
    assert len(diagnostic)==1 and diagnostic[0]['address']=='8c057b40'
    assert diagnostic[0]['completed']=='true' and int(diagnostic[0]['body_bytes'])==100
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000','name':report['name'],
        'address':hex(address),'end_exclusive':hex(end),'indexed_instruction_bytes':100,
        'retail_span_sha256':report['retail_span_sha256'],'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_matrix_init.ps1','verifier_script':'verify_main_dispatch_matrix_init.py',
        'status':'Recovered native matrix push/initialization matched 2240 direct/caller/partial comparisons; all 50 rows and 2 conditional edges covered through five entries, plus 162 independent save/identity checks',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-matrix-init/8c057b40_instructions.tsv',
        'descriptive_name_is_inferred':True,
    }
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==address),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':['0x8c057b40','0x8c639b98'],
            'init_rows_by_entry':report['init_rows_by_entry'],'independent_save_identity_checks_passed':162,
            'scope':report['scope'],'limitations':report['limitations']}
    for parent_address in (0x8C078EB0,0x8C0B1D60,0x8C0AFC98,0x8C0B11C0):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==parent_address)
        parent.setdefault('integration_evidence',{})['native_matrix_push_init_and_pop']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_matrix_push_init_and_pop']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered native matrix push/init and caller evidence; {len(registry["routines"])} entries. Rotations/vector calculation and full game/ports remain unfinished.')


if __name__=='__main__':main()
