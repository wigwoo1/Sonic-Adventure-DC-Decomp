"""Register the two value routines and closed early-transition integration."""
import csv
import hashlib
import json
from pathlib import Path

from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_value_native_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==2308
    assert report['counts']=={'leaf':1034,'wrapper':1034,'connected':240}
    assert report['persistent_calls_passed']==8 and report['unobserved_cases_passed']==16
    assert report['new_instruction_rows_verified']==report['new_instruction_rows_covered_through_parent']==48
    assert report['closed_chain_external_calls']==0
    assert report['missing_service_partial_state_checks']==report['atomic_precision_guard_checks']==1
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_early_native.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    diagnostic=PROJECT/'analysis/main-dispatch-value-writer/diagnostics.tsv'
    diagnostics={row['address']:row for row in csv.DictReader(diagnostic.open(),delimiter='\t')}
    assert set(diagnostics)=={'8c07c298','8c07c9d2'}
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    for evidence in report['routines']:
        start,end=int(evidence['address'],16),int(evidence['end_exclusive'],16)
        assert (start,end,evidence['instruction_bytes']) in ((0x8C07C298,0x8C07C2D0,56),(0x8C07C9D2,0x8C07C9FA,40))
        assert diagnostics[f'{start:08x}']['completed']=='true'
        assert int(diagnostics[f'{start:08x}']['body_bytes'])==evidence['instruction_bytes']
        assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==evidence['retail_span_sha256']
        routine={
            'module':'1ST_READ.BIN','load_address':'0x8c010000','name':evidence['name'],
            'address':hex(start),'end_exclusive':hex(end),
            'indexed_instruction_bytes':evidence['instruction_bytes'],
            'retail_span_sha256':evidence['retail_span_sha256'],
            'source':report['source'],'verification':REPORT,
            'native_build_script':'build_main_dispatch_early_native.ps1',
            'verifier_script':'verify_main_dispatch_value_native.py',
            'status':'Native integer routine matched standalone and connected retail execution; closed early transition chain has no external callees',
            'comparison_level':report['comparison_level'],'limitations':report['limitations'],
            'instruction_inventory':f'analysis/main-dispatch-value-writer/{start:08x}_instructions.tsv',
            'descriptive_name_is_inferred':True,
        }
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                  and int(r['address'],16)==start),None)
        if old is None:registry['routines'].append(routine)
        else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':['0x8c0afe72','0x8c07c298','0x8c07c9d2'],
            'connected_cases':240,'persistent_calls':8,'unobserved_cases':16,
            'closed_chain_external_calls':0,'scope':report['scope'],'limitations':report['limitations']}
    for address in (0x8C0AFF1A,0x8C0AFE72):
        routine=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                     and int(r['address'],16)==address)
        routine.setdefault('integration_evidence',{})['closed_value_chain']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_value_chain']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered two native value routines and closed early transition chain; {len(registry["routines"])} entries. Full game remains unfinished.')


if __name__=='__main__':main()
