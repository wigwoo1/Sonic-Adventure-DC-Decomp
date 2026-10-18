"""Register reused vector calculation and connected native game-call paths."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_vector_calc_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==1832
    assert report['counts']=={'calc':1008,'transform':194,'detach':210,'status':210,'flag':210}
    assert report['rows_covered']==50 and report['conditional_edges_covered']==4
    assert report['vector_instruction_sites_covered']==6 and report['native_multiply_sites_covered']==3
    assert report['linked_implicit_rows_by_caller']==dict.fromkeys(('transform','detach','status','flag'),23)
    assert report['boundary_comparisons']==12400 and report['instruction_inputs_compared']==6540
    assert report['partial_guards_passed']==680 and report['partial_failures_passed']==120
    assert report['unobserved_cases_passed']==24 and report['completed_connected_cases_with_null_external']==56
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    inventory=json.loads((PROJECT/'analysis/main_dispatch_vector_calc_inventory.json').read_text())
    for file,value in inventory['sha256'].items():assert digest(Path(file))==value,file
    assert inventory['reused_existing_source']
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_vector_calc.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    address,end=0x8C638F28,0x8C638F8C
    assert report['address']==hex(address) and report['end_exclusive']==hex(end) and report['instruction_bytes']==100
    assert hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest()==report['retail_span_sha256']
    diagnostic=list(csv.DictReader((PROJECT/'analysis/main-dispatch-vector-calc/diagnostics.tsv').open(),delimiter='\t'))
    assert len(diagnostic)==1 and diagnostic[0]['address']=='8c638f28'
    assert diagnostic[0]['completed']=='true' and int(diagnostic[0]['body_bytes'])==100
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000','name':report['name'],
        'address':hex(address),'end_exclusive':hex(end),'indexed_instruction_bytes':100,
        'retail_span_sha256':report['retail_span_sha256'],'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_vector_calc.ps1','verifier_script':'verify_main_dispatch_vector_calc.py',
        'status':'Existing vector-calculation dataflow reused/revalidated in 1832 direct/caller/partial comparisons; all 50 rows/4 edges/6 vector sites/3 native FMUL sites covered. Numerical vector instruction outputs remain fixtures.',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-vector-calc/8c638f28_instructions.tsv',
        'descriptive_name_is_inferred':True,'existing_source_reused':True,
    }
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==address),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':['0x8c638f28'],'linked_implicit_rows_by_caller':report['linked_implicit_rows_by_caller'],
            'completed_connected_cases_with_null_external':56,'scope':report['scope'],'limitations':report['limitations']}
    for address in (0x8C078EB0,0x8C0B1D60,0x8C0AFC98,0x8C0B11C0):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['native_vector_calc_with_instruction_services']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_vector_calc_with_instruction_services']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered existing vector calculation and native caller evidence; {len(registry["routines"])} entries. Numerical instruction support/full game/ports remain unfinished.')


if __name__=='__main__':main()
