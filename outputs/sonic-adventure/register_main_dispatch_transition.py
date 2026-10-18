"""Register bounded native helper recovery with current, hash-checked evidence."""
import hashlib
import json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
NAME='sa_main_dispatch_transition_8c0aff1a'
ADDRESS,END=0x8C0AFF1A,0x8C0B07E4
REPORT='analysis/main_dispatch_transition_verification.json'
SOURCE=f'source/{NAME}.c'

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    report=json.loads((PROJECT/REPORT).read_text())
    assert [report[key] for key in ('cases_passed','entry_guards_passed',
            'covered_retail_instruction_rows','covered_conditional_and_switch_edges',
            'covered_external_call_sites','selector_byte_values','table_entries','unique_switch_targets')]==[2424,2,957,109,34,256,50,45]
    assert report['address']==hex(ADDRESS) and report['end_exclusive']==hex(END)
    for file,value in report['sha256'].items():assert digest(Path(file))==value,file
    manifest=json.loads((PROJECT/'analysis/main_dispatch_transition_translation.json').read_text())
    for file,value in manifest['sha256'].items():assert digest(Path(file))==value,file
    assert digest(PROJECT/SOURCE)==report['source_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    assert hashlib.sha256(binary[ADDRESS-0x8C010000:END-0x8C010000]).hexdigest()==report['retail_span_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_transition.dll')==report['dll_sha256']
    diagnostic=PROJECT/'analysis/main-dispatch-helpers/diagnostics.tsv'
    assert '\t1914\t8c0aff1a\t8c0b07e3\ttrue\t' in diagnostic.read_text()
    routine={
        'module':'1ST_READ.BIN','load_address':'0x8c010000','name':NAME,
        'address':hex(ADDRESS),'end_exclusive':hex(END),
        'indexed_instruction_bytes':1914,'retail_span_sha256':report['retail_span_sha256'],
        'source':SOURCE,'verification':REPORT,
        'native_build_script':'build_main_dispatch_transition.ps1',
        'verifier_script':'verify_main_dispatch_transition.py',
        'status':'Native helper instruction set matched 2424 constructed retail cases with complete discovered instruction/conditional/switch/call-site coverage',
        'comparison_level':report['comparison_level'],'limitations':report['limitations'],
        'instruction_inventory':'analysis/main-dispatch-helpers/8c0aff1a_instructions.tsv',
        'descriptive_name_is_inferred':True,
    }
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    assert registry['binary_sha256']==report['binary_sha256']
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
              and int(r['address'],16)==ADDRESS),None)
    if old is None:registry['routines'].append(routine)
    else:old.update(routine)
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})[NAME]={
        'verification':REPORT,'cases':2424,'instruction_rows':957,
        'conditional_and_switch_edges':109,'external_call_sites':34,
        'source_sha256':report['source_sha256'],
        'scope':report['scope'],'limitations':report['limitations'],
        'ghidra_diagnostic_sha256':digest(diagnostic),
    }
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered {NAME}; {len(registry["routines"])} routine entries. Full dispatcher/game recovery remains incomplete.')

if __name__=='__main__':main()
