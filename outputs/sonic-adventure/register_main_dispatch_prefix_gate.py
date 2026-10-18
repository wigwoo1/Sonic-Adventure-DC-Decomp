"""Register the shared dispatcher gate and bounded native-sound evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_prefix_gate_verification.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    translation=json.loads((PROJECT/'analysis/main_dispatch_prefix_gate_translation.json').read_text())
    assert report['cases_passed']==2298 and report['covered_rows']==153
    assert report['conditional_edges_covered']==22 and report['external_call_sites_covered']==7
    assert report['native_sound_cases']==640 and report['service_error_cases']==14
    assert report['independent_invariants']>1000 and report['partial_guard_trap_error_cases']>0
    for item in (report,translation):
        for p,h in item['sha256'].items():assert digest(Path(p))==h,p
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_prefix_gate.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    start,end=int(report['address'],16),int(report['end_exclusive'],16)
    assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==report['retail_span_sha256']
    registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    entry={'module':'1ST_READ.BIN','load_address':'0x8c010000','name':report['name'],'address':report['address'],
        'end_exclusive':report['end_exclusive'],'indexed_instruction_bytes':report['instruction_bytes'],
        'retail_span_sha256':report['retail_span_sha256'],'source':report['source'],'verification':REPORT,
        'native_build_script':'build_main_dispatch_prefix_gate.ps1','verifier_script':'verify_main_dispatch_prefix_gate.py',
        'instruction_inventory':'analysis/main-dispatch-interrupt-gate/8c0afb10_instructions.tsv',
        'descriptive_name_is_inferred':True,'status':'Recovered buildable state gate; all 153 rows, 22 conditional outcomes and seven call sites covered in bounded retail comparisons.',
        'comparison_level':report['scope'],'limitations':report['limitations']}
    old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
    if old is None:registry['routines'].append(entry)
    else:old.update(entry)
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['main_dispatch_prefix_gate']={
        'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),'cases_passed':report['cases_passed'],
        'native_sound_cases':report['native_sound_cases'],'scope':report['scope'],'limitations':report['limitations']}
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered prefix gate and native sound integration; {len(registry["routines"])} original routine entries. Full game/ports remain unfinished.')


if __name__=='__main__':main()
