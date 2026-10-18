"""Register native query recovery and its connection to the shared transition."""
import csv
import hashlib
import json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_status_query_verification.json'
START,END=0x8C06CEBE,0x8C06CEFE


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['address']==hex(START) and report['end_exclusive']==hex(END)
    assert report['cases_passed']==2209 and report['counts']=={'query':1204,'connected':1005}
    assert report['boundary_comparisons']==3423
    assert report['covered_query_rows_by_entry']=={'query':32,'connected':32}
    assert report['covered_query_conditional_edges_by_entry']=={'query':6,'connected':6}
    assert report['covered_parent_rows']==153
    assert report['service_error_cases_passed']==3 and report['atomic_guards_passed']==2
    assert report['unobserved_cases_passed']==8
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_status_query.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    assert hashlib.sha256(binary[START-0x8C010000:END-0x8C010000]).hexdigest()==report['retail_span_sha256']
    diagnostic=PROJECT/'analysis/main-dispatch-status-query/diagnostics.tsv'
    diagnostics=list(csv.DictReader(diagnostic.open(),delimiter='\t'))
    assert len(diagnostics)==1 and diagnostics[0]['address']=='8c06cebe'
    assert diagnostics[0]['body_bytes']=='64' and diagnostics[0]['completed']=='true'
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000',
        'name':'sa_main_dispatch_status_query_8c06cebe','address':hex(START),'end_exclusive':hex(END),
        'indexed_instruction_bytes':64,'retail_span_sha256':report['retail_span_sha256'],
        'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_status_query.ps1',
        'verifier_script':'verify_main_dispatch_status_query.py',
        'status':'Integer query and native parent connection matched 2209 constructed retail cases including explicit MACL effects, 32 query rows / 6 edges and 153 parent rows',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-status-query/8c06cebe_instructions.tsv',
        'descriptive_name_is_inferred':True,
    }
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==START),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependency':hex(START),'connected_cases':1005,'covered_parent_rows':153,
            'macl_effects_explicitly_compared':True,'scope':report['scope'],'limitations':report['limitations']}
    parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                and int(r['address'],16)==0x8C0AFC98)
    parent.setdefault('integration_evidence',{})['native_status_query']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_status_query']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered status query and native parent connection; {len(registry["routines"])} routine entries; full game remains unfinished')


if __name__=='__main__':main()
