"""Register transform sequencing and native-pop integration evidence."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_transform_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==2030
    assert report['counts']=={'transform':1606,'detach':140,'status':144,'flag':140}
    assert report['transform_rows_covered']==55 and report['transform_conditional_edges_covered']==6
    assert report['transform_call_sites_covered']==6
    assert report['transform_rows_by_entry']=={'transform':55,'detach':55,'status':55,'flag':55}
    assert report['native_pop_rows_covered']==28 and report['boundary_comparisons']==11116
    assert report['partial_precision_guards_passed']==1272 and report['partial_service_error_cases_passed']==188
    assert report['unobserved_cases_passed']==8 and report['null_external_cases_passed']==56
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    manifest=json.loads((PROJECT/'analysis/main_dispatch_transform_translation.json').read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_transform.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    address,end=0x8C078EB0,0x8C078F1E
    assert report['address']==hex(address) and report['end_exclusive']==hex(end) and report['instruction_bytes']==110
    assert hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest()==report['retail_span_sha256']
    diagnostic=list(csv.DictReader((PROJECT/'analysis/main-dispatch-transform/diagnostics.tsv').open(),delimiter='\t'))
    assert len(diagnostic)==1 and diagnostic[0]['address']=='8c078eb0'
    assert diagnostic[0]['completed']=='true' and int(diagnostic[0]['body_bytes'])==110
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000','name':report['name'],
        'address':hex(address),'end_exclusive':hex(end),'indexed_instruction_bytes':110,
        'retail_span_sha256':report['retail_span_sha256'],'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_transform.ps1','verifier_script':'verify_main_dispatch_transform.py',
        'status':'Recovered transform sequencing matched 2030 direct/caller/partial comparisons; all 55 rows, 6 conditional edges and 6 call sites covered, with native matrix pop',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-transform/8c078eb0_instructions.tsv',
        'descriptive_name_is_inferred':True,
    }
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==address),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':['0x8c078eb0','0x8c639b98'],
            'transform_rows_by_entry':report['transform_rows_by_entry'],
            'native_pop_rows_covered':28,'scope':report['scope'],'limitations':report['limitations']}
    for parent_address in (0x8C0B1D60,0x8C0AFC98,0x8C0B11C0):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==parent_address)
        parent.setdefault('integration_evidence',{})['native_transform_sequence_and_pop']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_transform_sequence_and_pop']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered transform sequence/native pop and caller evidence; {len(registry["routines"])} entries. Matrix algorithms and full game/ports remain unfinished.')


if __name__=='__main__':main()
