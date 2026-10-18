"""Register detach/vector source and bounded native caller evidence."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_detach_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==4116
    assert report['counts']=={'detach':1708,'status':164,'flag':164,'vector':2080}
    assert report['instruction_rows_covered']==75 and report['conditional_edges_covered']==6
    assert report['native_or_external_call_sites_covered']==2
    assert report['precision_partial_guards_passed']==140
    assert report['partial_service_error_cases_passed']==24 and report['unobserved_cases_passed']==12
    assert report['boundary_comparisons']==3136
    assert report['closed_flag_cases_without_external_call']==112 and report['closed_detach_cases_without_external_call']==1424
    assert report['linked_detach_rows_by_parent']=={'status':64,'flag':64}
    assert report['parent_rows_covered']=={'status':153,'flag':55}
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    manifest=json.loads((PROJECT/'analysis/main_dispatch_detach_translation.json').read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    assert manifest['routines']==report['routines']
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_detach.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    diagnostics={row['address']:row for row in csv.DictReader((PROJECT/'analysis/main-dispatch-detach/diagnostics.tsv').open(),delimiter='\t')}
    expected={0x8C0B1D60:(0x8C0B1DE0,128),0x8C07CB54:(0x8C07CB6A,22)}
    assert set(diagnostics)=={f'{a:08x}' for a in expected}
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    for item in report['routines']:
        address=int(item['address'],16);end,size=expected[address]
        assert item['end_exclusive']==hex(end) and item['instruction_bytes']==size
        assert diagnostics[f'{address:08x}']['completed']=='true'
        assert int(diagnostics[f'{address:08x}']['body_bytes'])==size
        assert hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest()==item['retail_span_sha256']
        routine={
            'module':'1ST_READ.BIN','load_address':'0x8c010000','name':item['name'],
            'address':hex(address),'end_exclusive':hex(end),'indexed_instruction_bytes':size,
            'retail_span_sha256':item['retail_span_sha256'],'source':report['source'],'verification':REPORT,
            'native_build_script':'build_main_dispatch_detach.ps1','verifier_script':'verify_main_dispatch_detach.py',
            'status':'Recovered native detach/vector matched 4116 direct/caller/guard comparisons; all 75 group rows, 6 conditional edges and 2 original call sites covered',
            'comparison_level':report['comparison_level'],'limitations':report['limitations'],
            'instruction_inventory':f'analysis/main-dispatch-detach/{address:08x}_instructions.tsv',
            'descriptive_name_is_inferred':True,
        }
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                  and int(r['address'],16)==address),None)
        if old is None:registry['routines'].append(routine)
        else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':['0x8c0b1d60','0x8c07cb54','0x8c06c880'],
            'closed_flag_cases_without_external_call':report['closed_flag_cases_without_external_call'],
            'scope':report['scope'],'limitations':report['limitations']}
    for address in (0x8C0AFC98,0x8C0B11C0):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['native_detach_vector']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_detach_vector']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered detach/vector and both caller connections; {len(registry["routines"])} entries. Full game/ports remain unfinished.')


if __name__=='__main__':main()
