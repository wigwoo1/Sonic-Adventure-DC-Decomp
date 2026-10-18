"""Register two original sound routines and a closed native caller bridge."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
DIRECT='analysis/main_dispatch_sound_verification.json'
CONNECTED='analysis/main_dispatch_sound_complete_verification.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    direct=json.loads((PROJECT/DIRECT).read_text());connected=json.loads((PROJECT/CONNECTED).read_text())
    translation=json.loads((PROJECT/'analysis/main_dispatch_sound_translation.json').read_text())
    assert direct['cases_passed']==1130 and direct['counts']=={'select':713,'request':225,'connected':192}
    assert direct['instruction_rows_covered']==241 and direct['feasible_conditional_edges_covered']==74
    assert direct['boundary_comparisons']==320 and direct['partial_guard_error_cases']==201 and direct['independent_invariants']==857
    assert len(direct['infeasible_edges_from_constant_r9'])==4
    assert connected['cases_passed']==868
    assert connected['counts']=={'x':72,'y':72,'z':72,'calc':256,'transform':114,'detach':82,'status':118,'flag':82}
    assert connected['native_boundary_comparisons']==3496 and connected['numerical_operations_compared']==3726
    assert connected['analytic_rotation_vector_checks']==644 and connected['partial_guard_trap_cases']==140
    assert connected['completed_without_external_service_cases']==728
    assert connected['sound_request_reached_cases']==connected['slot_selector_reached_cases']==102
    assert connected['max_analytic_absolute_error']<8e-6
    reports=[direct,connected,translation]
    for name in ('main_dispatch_scalar_complete_verification.json','sh4_multiply_extended_verification.json','sh4_vector_numeric_verification.json'):
        reports.append(json.loads((PROJECT/'analysis'/name).read_text()))
    for report in reports:
        for p,h in report['sha256'].items():assert digest(Path(p))==h,p
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_sound.dll')==direct['dll_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_sound_complete.dll')==connected['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==direct['binary_sha256']
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    for r in translation['routines']:
        start,end=int(r['address'],16),int(r['end_exclusive'],16)
        assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==r['retail_span_sha256']
        entry={'module':'1ST_READ.BIN','load_address':'0x8c010000','name':r['name'],'address':r['address'],
            'end_exclusive':r['end_exclusive'],'indexed_instruction_bytes':r['instruction_bytes'],
            'retail_span_sha256':r['retail_span_sha256'],'source':direct['source'],'verification':DIRECT,
            'native_build_script':'build_main_dispatch_sound.ps1','verifier_script':'verify_main_dispatch_sound.py',
            'instruction_inventory':str(Path(r['listing']).relative_to(PROJECT)).replace('\\','/'),
            'descriptive_name_is_inferred':True,'status':'Recovered buildable static C; full-state and independent invariants checked in bounded direct/native caller scenarios.',
            'comparison_level':'Original instruction execution vs native C: full RAM/CPU/FPU and boundaries; independent slot-selection/request-record invariants.',
            'limitations':direct['limitations']}
        old=next((a for a in registry['routines'] if a.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(a['address'],16)==start),None)
        if old is None:registry['routines'].append(entry)
        else:old.update(entry)
    record={'verification':CONNECTED,'verification_sha256':digest(PROJECT/CONNECTED),
        'direct_verification':DIRECT,'direct_verification_sha256':digest(PROJECT/DIRECT),
        'native_dependencies':['0x8c0940da','0x8c093f02'],'connected_cases':868,'completed_without_external_service_cases':728,
        'scope':connected['scope'],'limitations':connected['limitations']}
    parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==0x8C0AFC98)
    parent.setdefault('integration_evidence',{})['native_sound_request_and_numerics']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['native_sound_request_and_numerics']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered sound request/slot selector and closed numerical caller evidence; {len(registry["routines"])} routine entries. Physical audio/full game remain unfinished.')


if __name__=='__main__':main()
