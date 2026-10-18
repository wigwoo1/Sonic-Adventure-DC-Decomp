"""Register the bounded native reset recovery and STATUS integration evidence."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_reset_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==912 and report['counts']=={'reset':736,'status':176}
    assert report['reset_rows_covered_by_entry']=={'reset':38,'status':38}
    assert report['reset_conditional_edges_by_entry']=={'reset':4,'status':4}
    assert report['native_clear_call_sites_verified']==1
    assert report['boundary_comparisons']==1780 and report['connected_native_reset_calls']==176
    assert report['unobserved_cases_passed']==24 and report['post_reset_partial_error_cases_passed']==8
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_reset.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    assert report['address']=='0x8c05e788' and report['end_exclusive']=='0x8c05e7d4'
    assert report['indexed_instruction_bytes']==76
    assert hashlib.sha256(binary[0x4E788:0x4E7D4]).hexdigest()==report['retail_span_sha256']
    diagnostics=list(csv.DictReader((PROJECT/'analysis/main-dispatch-reset/diagnostics.tsv').open(),delimiter='\t'))
    assert len(diagnostics)==1 and diagnostics[0]['address']=='8c05e788'
    assert diagnostics[0]['completed']=='true' and int(diagnostics[0]['body_bytes'])==76
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000',
        'name':'sa_main_dispatch_reset_8c05e788','address':report['address'],
        'end_exclusive':report['end_exclusive'],'indexed_instruction_bytes':76,
        'retail_span_sha256':report['retail_span_sha256'],'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_reset.ps1','verifier_script':'verify_main_dispatch_reset.py',
        'status':'Recovered native reset matched 912 direct/caller comparisons; all 38 rows and 4 conditional edges covered at both entries',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-reset/8c05e788_instructions.tsv',
        'descriptive_name_is_inferred':True,
    }
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==0x8C05E788),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':['0x8c05e788','0x8c05e69e'],
            'connected_native_reset_calls':176,'post_reset_partial_error_cases_passed':8,
            'scope':report['scope'],'limitations':report['limitations']}
    parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                and int(r['address'],16)==0x8C0AFC98)
    parent.setdefault('integration_evidence',{})['native_reset']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_reset']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered native reset and bounded STATUS integration; {len(registry["routines"])} entries. Full game/ports remain unfinished.')


if __name__=='__main__':main()
