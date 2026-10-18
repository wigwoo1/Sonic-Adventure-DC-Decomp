"""Record numerical support and native caller evidence, not new game routines."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
BACKEND='analysis/sh4_vector_numeric_verification.json'
CONNECTED='analysis/main_dispatch_numeric_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    backend=json.loads((PROJECT/BACKEND).read_text())
    report=json.loads((PROJECT/CONNECTED).read_text())
    assert backend['total_checks_passed']==438455
    assert backend['counts']=={'adapter_primitive_comparisons':90112,'invalid_opcode_guards':65495,
        'reserved_rm_guards':1408,'pr_noop_and_trap_policy_checks':45056,'fsca_status_policy_checks':16384,
        'fsca_angle_mode_checks':131072,'fsrra_positive_normal_checks':58768,'dot_error_bound_checks':26000,
        'matrix_invariant_checks':4096}
    assert backend['zero_status_policy_checks']==64
    assert backend['max_fsca_absolute_error']<2**-21 and backend['max_fsrra_ulp_error']<2.00001
    assert backend['max_dot_error_bound_fraction']<=1
    assert report['cases_passed']==668
    assert report['counts']=={'x':72,'y':72,'z':72,'calc':120,'transform':82,'detach':82,'status':86,'flag':82}
    assert report['native_boundary_comparisons']==2752 and report['numerical_operations_compared']==2742
    assert report['analytic_rotation_vector_checks']==528 and report['partial_guard_trap_error_cases']==140
    assert report['closed_null_external_cases']==198 and report['max_analytic_absolute_error']<8e-6
    for item in (backend,report):
        for file,value in item['sha256'].items():assert digest(Path(file))==value,file
    for filename,key,item in (('sh4_vector_numeric.dll','dll_sha256',backend),
                              ('sh4_vector_numeric_reference.dll','primitive_reference_dll_sha256',backend),
                              ('main_dispatch_numeric.dll','dll_sha256',report)):
        assert digest(ROOT/'work/sonic-adventure/build'/filename)==item[key]
    vendor=PROJECT/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h'
    assert digest(vendor)==backend['vendor_sha256']=='cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
    assert 'The MIT License (MIT)' in vendor.read_text(encoding='utf-8')
    record={'verification':CONNECTED,'verification_sha256':digest(PROJECT/CONNECTED),
        'backend_verification':BACKEND,'backend_verification_sha256':digest(PROJECT/BACKEND),
        'backend_source':'source/sh4_vector_numeric.c','backend_source_sha256':digest(PROJECT/'source/sh4_vector_numeric.c'),
        'vendor_source':'third_party/skmp-sh4-fpu/sh4_fpu_approx.h','vendor_sha256':backend['vendor_sha256'],
        'vendor_revision':backend['vendor_revision'],'vendor_url':backend['vendor_url'],'vendor_license':'MIT; original notice preserved in header',
        'native_instruction_operations':['FSCA','FTRV','FIPR','FSRRA'],
        'local_backend_checks':438455,'analytic_rotation_vector_checks':528,'closed_null_external_cases':198,
        'scope':report['scope'],'limitations':report['limitations']}
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
    count=len(registry['routines'])
    for address in (0x8C639EC8,0x8C639F5C,0x8C63A1CC,0x8C638F28,0x8C078EB0,0x8C0B1D60,0x8C0AFC98,0x8C0B11C0):
        parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'
                    and int(r['address'],16)==address)
        parent.setdefault('integration_evidence',{})['native_numerical_instructions']=record
    assert len(registry['routines'])==count # CPU support code is not an extra original game routine.
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json'
    status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['native_sh4_vector_numerics']=record
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Recorded four native numerical instructions and caller evidence; original routine entries unchanged at {count}. Full game/ports remain unfinished.')


if __name__=='__main__':main()
