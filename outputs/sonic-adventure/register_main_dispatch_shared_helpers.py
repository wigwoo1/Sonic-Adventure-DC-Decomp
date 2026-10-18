"""Register shared helper native recovery with current byte/behavior evidence."""
import csv
import hashlib
import json
from pathlib import Path

from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_shared_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert [report[k] for k in ('cases_passed','boundary_comparisons','service_error_cases_passed','atomic_entry_guards_passed')]==[2288,7039,10,2]
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    manifest=json.loads((PROJECT/'analysis/main_dispatch_shared_translation.json').read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_shared_helpers.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    diagnostic=PROJECT/'analysis/main-dispatch-shared-helpers/diagnostics.tsv'
    diagnostics={row['address']:row for row in csv.DictReader(diagnostic.open(),delimiter='\t')}
    assert set(diagnostics)=={'8c0afc98','8c0b11c0'}
    expected={0x8C0AFC98:(0x8C0AFE20,153,14,8,2088),0x8C0B11C0:(0x8C0B122E,55,8,2,200)}
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    for item in report['routines']:
        address=int(item['address'],16)
        end,rows,edges,calls,cases=expected[address]
        assert item['end_exclusive']==hex(end)
        assert item['instruction_bytes']==rows*2 and item['covered_rows']==rows
        assert item['covered_conditional_edges']==edges and item['covered_external_call_sites']==calls
        assert item['cases_passed']==cases
        assert diagnostics[f'{address:08x}']['completed']=='true'
        assert int(diagnostics[f'{address:08x}']['body_bytes'])==rows*2
        assert hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest()==item['retail_span_sha256']
        routine={
            'module':'1ST_READ.BIN','load_address':'0x8c010000','name':item['name'],
            'address':hex(address),'end_exclusive':hex(end),'indexed_instruction_bytes':rows*2,
            'retail_span_sha256':item['retail_span_sha256'],'source':report['source'],'verification':REPORT,
            'native_build_script':'build_main_dispatch_shared_helpers.ps1',
            'verifier_script':'verify_main_dispatch_shared_helpers.py',
            'status':f'Native shared transition matched {cases} constructed retail comparisons; all {rows} discovered rows / {edges} conditional edges / {calls} external call sites covered',
            'comparison_level':report['comparison_level'],'limitations':report['limitations'],
            'instruction_inventory':f'analysis/main-dispatch-shared-helpers/{address:08x}_instructions.tsv',
            'descriptive_name_is_inferred':True,
        }
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                  and int(r['address'],16)==address),None)
        if old is None:registry['routines'].append(routine)
        else:old.update(routine)
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_shared_status_and_flag_helpers']={
        'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
        'routines':[r['address'] for r in report['routines']],
        'cases':2288,'instruction_rows':208,'conditional_edges':22,'external_call_sites':10,
        'boundary_comparisons':7039,'service_error_cases':10,
        'scope':report['scope'],'limitations':report['limitations'],
    }
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered two shared dispatcher helpers; {len(registry["routines"])} entries. Full game/ports remain unfinished.')


if __name__=='__main__':main()
