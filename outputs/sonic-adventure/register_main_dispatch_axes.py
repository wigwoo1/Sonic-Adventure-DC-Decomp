"""Register revalidated existing axes and bounded native caller integration."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_axes_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert report['cases_passed']==1488
    assert report['counts']=={'x':268,'y':268,'z':268,'transform':158,'detach':174,'status':178,'flag':174}
    assert report['axis_rows_covered']=={'x':72,'y':75,'z':75}
    assert report['conditional_edges_covered']==6 and report['vector_instruction_sites_covered']==18
    assert report['linked_implicit_rows_by_caller']==dict.fromkeys(('transform','detach','status','flag'),66)
    assert report['boundary_comparisons']==7544 and report['vector_instruction_inputs_compared']==3528
    assert report['partial_precision_guards_passed']==444 and report['partial_failure_cases_passed']==168
    assert report['unobserved_cases_passed']==20 and report['null_instruction_cases_passed']==68
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    inventory=json.loads((PROJECT/'analysis/main_dispatch_axes_inventory.json').read_text())
    for file,value in inventory['sha256'].items():assert digest(Path(file))==value,file
    assert report['routines']==inventory['routines']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_axes.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    diagnostic={r['address']:r for r in csv.DictReader((PROJECT/'analysis/main-dispatch-axes/diagnostics.tsv').open(),delimiter='\t')}
    expected={0x8C639EC8:(0x8C639F58,144),0x8C639F5C:(0x8C639FF2,150),0x8C63A1CC:(0x8C63A262,150)}
    assert set(diagnostic)=={f'{a:08x}' for a in expected}
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    for item in report['routines']:
        address=int(item['address'],16);end,size=expected[address]
        assert item['end_exclusive']==hex(end) and item['instruction_bytes']==size
        assert item['reused_existing_source'] and digest(PROJECT/item['source'])==item['source_sha256']
        assert diagnostic[f'{address:08x}']['completed']=='true' and int(diagnostic[f'{address:08x}']['body_bytes'])==size
        assert hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest()==item['retail_span_sha256']
        routine={
            'module':'1ST_READ.BIN','load_address':'0x8c010000','name':item['name'],
            'address':hex(address),'end_exclusive':hex(end),'indexed_instruction_bytes':size,
            'retail_span_sha256':item['retail_span_sha256'],'source':item['source'],'verification':REPORT,
            'native_build_script':'build_main_dispatch_axes.ps1','verifier_script':'verify_main_dispatch_axes.py',
            'status':'Existing native axis dataflow reused/revalidated in 1488 direct/caller/partial comparisons; all 222 group rows/6 edges/18 vector sites covered. FSCA/FTRV numerical outputs remain instruction fixtures.',
            'comparison_level':report['comparison_level'],'limitations':report['limitations'],
            'instruction_inventory':f'analysis/main-dispatch-axes/{address:08x}_instructions.tsv',
            'descriptive_name_is_inferred':True,'existing_source_reused':True,
        }
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                  and int(r['address'],16)==address),None)
        if old is None:registry['routines'].append(routine)
        else:old.update(routine)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),
            'native_dependencies':[hex(a) for a in expected],
            'linked_implicit_rows_by_caller':report['linked_implicit_rows_by_caller'],
            'vector_instruction_inputs_compared':3528,'scope':report['scope'],'limitations':report['limitations']}
    for address in (0x8C078EB0,0x8C0B1D60,0x8C0AFC98,0x8C0B11C0):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['native_axes_with_instruction_services']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_native_axes_with_instruction_services']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered three existing axis bodies and new caller evidence; {len(registry["routines"])} entries. Numerical vector instructions and full game/ports remain unfinished.')


if __name__=='__main__':main()
