"""Register original notification bodies and bounded native dependency evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_notification_verification.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    report=json.loads((PROJECT/REPORT).read_text())
    translation=json.loads((PROJECT/'analysis/main_dispatch_notification_translation.json').read_text())
    assert report['cases_passed']==1114 and report['counts']=={'notify':679,'create':168,'ready':72,'gate':195}
    assert report['rows_covered']==173 and report['conditional_edges_covered']==30 and report['call_sites_covered']==8
    assert report['boundary_comparisons']==10618 and report['partial_guard_error_cases']==124
    assert report['independent_invariants']==886 and report['completed_without_external_calls']==862
    assert report['service_error_cases']==4
    for item in (report,translation):
        for p,h in item['sha256'].items():assert digest(Path(p))==h,p
    assert digest(PROJECT/report['source'])==report['source_sha256']
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_notification.dll')==report['dll_sha256']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
    registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    for routine in translation['routines']:
        start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
        assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
        entry={'module':'1ST_READ.BIN','load_address':'0x8c010000','name':routine['name'],'address':routine['address'],
            'end_exclusive':routine['end_exclusive'],'indexed_instruction_bytes':routine['instruction_bytes'],
            'retail_span_sha256':routine['retail_span_sha256'],'source':report['source'],'verification':REPORT,
            'native_build_script':'build_main_dispatch_notification.ps1','verifier_script':'verify_main_dispatch_notification.py',
            'instruction_inventory':str(Path(routine['listing']).relative_to(PROJECT)).replace('\\','/'),
            'descriptive_name_is_inferred':True,'status':'Recovered original integer control flow; all new rows/branches/calls covered with native dependencies and bounded full-state evidence.',
            'comparison_level':report['scope'],'limitations':report['limitations']}
        old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
        if old is None:registry['routines'].append(entry)
        else:old.update(entry)
    record={'verification':REPORT,'verification_sha256':digest(PROJECT/REPORT),'cases_passed':1114,
        'completed_without_external_calls':862,'native_dependencies':['0x8c109fde','0x8c109e50','0x8c64bed8'],
        'scope':report['scope'],'limitations':report['limitations']}
    for address in (0x8C0AFB10,0x8C09852E,0x8C0985EC,0x8C098A80,0x8C098B0C,0x8C646868,0x8C65F3A0,0x8C604370,0x8C10CA5C):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['native_notification_dependency_chain']=record
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['native_notification_dependency_chain']=record
    status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered notification, creator and ready query; reused native helpers credited as integration only. {len(registry["routines"])} routine entries; full game/ports unfinished.')


if __name__=='__main__':main()
