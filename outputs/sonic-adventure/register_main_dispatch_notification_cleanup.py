"""Record native RAM cleanup evidence without recounting existing routines."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
STANDALONE='analysis/main_dispatch_task_disposal_verification.json'
CONNECTED='analysis/main_dispatch_notification_cleanup_verification.json'
INVENTORY='analysis/main_dispatch_task_disposal_inventory.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    standalone=json.loads((PROJECT/STANDALONE).read_text());connected=json.loads((PROJECT/CONNECTED).read_text())
    inventory=json.loads((PROJECT/INVENTORY).read_text())
    assert standalone['cases_passed']==1564 and standalone['counts']=={'dispose':795,'work':48,'children':193,'update':96,'heap':432}
    assert standalone['boundary_comparisons']==3372 and standalone['partial_service_error_cases']==12
    assert standalone['independent_invariants']==1552 and standalone['completed_without_external_calls']==992
    assert standalone['service_error_cases']==4
    assert [r['rows_covered'] for r in standalone['routines']]==[119,24,28,17,95]
    assert [r['conditional_edges_covered'] for r in standalone['routines']]==[35,4,8,4,31]
    assert connected['cases_passed']==1113 and connected['counts']=={'notify':678,'create':168,'ready':72,'gate':195}
    assert connected['boundary_comparisons']==10654 and connected['partial_guard_error_cases']==91
    assert connected['independent_invariants']==918 and connected['completed_without_external_calls']==958
    assert connected['independent_failure_cleanup_restorations']==96 and connected['cleanup_rows_reached']==123
    assert connected['service_error_cases']==3
    for report in (standalone,connected,inventory):
        for p,h in report['sha256'].items():assert digest(Path(p))==h,p
    assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_notification_cleanup.dll')==standalone['dll_sha256']==connected['dll_sha256']
    record={'verification':CONNECTED,'verification_sha256':digest(PROJECT/CONNECTED),
        'standalone_verification':STANDALONE,'standalone_verification_sha256':digest(PROJECT/STANDALONE),
        'inventory':INVENTORY,'inventory_sha256':digest(PROJECT/INVENTORY),
        'native_sources':[{k:r[k] for k in ('address','source','source_sha256')} for r in inventory['routines']],
        'standalone_cases':1564,'caller_cases':1113,'independent_failure_cleanup_restorations':96,
        'scope':connected['scope'],'coverage_limits':standalone['routines'],
        'limitations':standalone['limitations']+connected['limitations']}
    registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));count=len(registry['routines'])
    for address in (0x8C09865C,0x8C098998,0x8C098E60,0x8C098E98,0x8C098B42,0x8C109FDE,0x8C109E50,0x8C0AFB10,0x8C09852E):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['guest_ram_disposal_and_notification_cleanup']=record
    assert len(registry['routines'])==count
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['guest_ram_disposal_and_notification_cleanup']=record
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Recorded native cleanup and 96 restored allocation-failure states; original routine count unchanged at {count}. Full game/ports remain unfinished.')


if __name__=='__main__':main()
