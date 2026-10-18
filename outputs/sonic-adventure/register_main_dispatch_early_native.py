"""Register the integer early dependency and bounded native caller connection."""
import csv
import hashlib
import json
from pathlib import Path

from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
NAME='sa_main_dispatch_early_gate_8c0afe72'
START,END,CALLER=0x8C0AFE72,0x8C0AFF1A,0x8C0AFF1A
REPORT='analysis/main_dispatch_early_native_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['address']==hex(START) and report['end_exclusive']==hex(END)
    assert report['cases_passed']==3276
    assert report['counts']=={'leaf':2120,'connected':1156,'callback_failures':4}
    assert report['entry_guards_passed']==3 and report['boundary_comparisons']==1282
    assert report['instruction_rows']==55 and report['conditional_edges']==12
    assert report['instruction_rows_covered_by_entry']=={'leaf':55,'connected':55}
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_early_native.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    span=binary[START-0x8C010000:END-0x8C010000]
    assert len(span)==168 and hashlib.sha256(span).hexdigest()==report['retail_span_sha256']
    diagnostic=PROJECT/'analysis/main-dispatch-early-dependency/diagnostics.tsv'
    rows=list(csv.DictReader(diagnostic.open(),delimiter='\t'))
    assert len(rows)==1 and rows[0]['address']=='8c0afe72'
    assert rows[0]['body_bytes']=='110' and rows[0]['completed']=='true'
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000','name':NAME,
        'address':hex(START),'end_exclusive':hex(END),
        'indexed_instruction_bytes':110,'retail_span_sha256':report['retail_span_sha256'],
        'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_early_native.ps1',
        'verifier_script':'verify_main_dispatch_early_native.py',
        'status':'Integer early helper and native transition connection matched 3276 constructed retail comparisons; all 55 rows and 12 conditional edges covered directly and through caller',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-early-dependency/8c0afe72_instructions.tsv',
        'descriptive_name_is_inferred':True,
    }
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==START),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    caller=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                and int(r['address'],16)==CALLER)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependency':hex(START),'connected_cases':1156,'boundary_comparisons':1282,
            'scope':report['scope'],'limitations':report['limitations']}
    caller.setdefault('integration_evidence',{})['native_early_gate']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})[NAME]=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered early dependency and native caller connection; {len(registry["routines"])} routine entries; no whole-game milestone completed')


if __name__=='__main__':main()
