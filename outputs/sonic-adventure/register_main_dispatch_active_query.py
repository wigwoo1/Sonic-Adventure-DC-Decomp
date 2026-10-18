"""Register three active-query routines and native QUERY/STATUS connections."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_active_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==3112
    assert report['counts']=={'clear':44,'predicate':2048,'active':238,'query':494,'status':288}
    assert report['instruction_rows_covered']==88 and report['conditional_edges_covered']==20
    assert report['native_call_sites_covered']==5 and report['boundary_comparisons']==3704
    assert report['closed_query_cases_with_null_external']==474
    assert report['floating_guard_and_trap_checks_passed']==40 and report['unobserved_cases_passed']==8
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    manifest=json.loads((PROJECT/'analysis/main_dispatch_active_translation.json').read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_active_query.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    diagnostic=PROJECT/'analysis/main-dispatch-active-query/diagnostics.tsv'
    diagnostics={row['address']:row for row in csv.DictReader(diagnostic.open(),delimiter='\t')}
    expected={0x8C05E69E:(0x8C05E6C4,38),0x8C05E6C4:(0x8C05E6E8,36),0x8C05E6E8:(0x8C05E788,102)}
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
            'native_build_script':'build_main_dispatch_active_query.ps1','verifier_script':'verify_main_dispatch_active_query.py',
            'status':'Recovered native active-query group matched 3112 constructed retail/guard comparisons; all 88 group rows, 20 conditional edges and 5 native call sites covered',
            'comparison_level':report['comparison_level'],'limitations':report['limitations'],
            'instruction_inventory':f'analysis/main-dispatch-active-query/{address:08x}_instructions.tsv',
            'descriptive_name_is_inferred':True,
        }
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                  and int(r['address'],16)==address),None)
        if old is None:registry['routines'].append(routine)
        else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':[hex(a) for a in expected],
            'closed_query_cases_with_null_external':474,'floating_guard_and_trap_checks':40,
            'scope':report['scope'],'limitations':report['limitations']}
    for address in (0x8C06CEBE,0x8C0AFC98):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['native_active_query_group']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_active_query_group']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered three active-query routines and native caller chain; {len(registry["routines"])} entries. Full game/ports remain unfinished.')


if __name__=='__main__':main()
