"""Register exceptional scalar support without adding original game routines."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT

PROJECT=Path(__file__).resolve().parent
SCALAR='analysis/sh4_multiply_extended_verification.json'
CONNECTED='analysis/main_dispatch_scalar_complete_verification.json'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    scalar=json.loads((PROJECT/SCALAR).read_text())
    connected=json.loads((PROJECT/CONNECTED).read_text())
    old=json.loads((PROJECT/'analysis/main_dispatch_numeric_verification.json').read_text())
    backend=json.loads((PROJECT/'analysis/sh4_vector_numeric_verification.json').read_text())
    assert scalar['manual_class_cases_passed']==196608
    assert scalar['infinity_only_cases']==39936 and scalar['nan_priority_cases']==156672
    assert scalar['same_operand_alias_cases_passed']==2048
    assert scalar['unsupported_mode_guards_passed']==6000
    assert scalar['finite_delegation_cases_passed']==25728 and scalar['independent_finite_checks_passed']==25248
    assert connected['cases_passed']==836
    assert connected['counts']=={'x':72,'y':72,'z':72,'calc':256,'transform':114,'detach':82,'status':86,'flag':82}
    assert connected['native_boundary_comparisons']==2896 and connected['numerical_operations_compared']==3374
    assert connected['analytic_rotation_vector_checks']==608
    assert connected['zero_normalization_completed_cases']==84 and connected['scalar_invalid_trap_cases']==8
    assert connected['partial_guard_trap_error_cases']==144 and connected['closed_null_external_cases']==230
    assert connected['max_analytic_absolute_error']<8e-6
    # Existing report/source baselines are preserved; this does not rerun them.
    for report in (scalar,connected,old,backend):
        for file,expected in report['sha256'].items():assert digest(Path(file))==expected,file
    for name,key,report in (('sh4_multiply_extended.dll','dll_sha256',scalar),
                            ('main_dispatch_scalar_complete.dll','dll_sha256',connected),
                            ('main_dispatch_numeric.dll','dll_sha256',old),
                            ('sh4_vector_numeric.dll','dll_sha256',backend),
                            ('sh4_vector_numeric_reference.dll','primitive_reference_dll_sha256',backend)):
        assert digest(ROOT/'work/sonic-adventure/build'/name)==report[key],name
    record={'verification':CONNECTED,'verification_sha256':digest(PROJECT/CONNECTED),
        'scalar_verification':SCALAR,'scalar_verification_sha256':digest(PROJECT/SCALAR),
        'source':'source/sh4_multiply_extended.c','source_sha256':scalar['source_sha256'],
        'connected_source':'source/sa_main_dispatch_scalar_complete_connected.c',
        'connected_source_sha256':digest(PROJECT/'source/sa_main_dispatch_scalar_complete_connected.c'),
        'manual':scalar['manual'],'scalar_primary_cases':230384,'independent_finite_subchecks':25248,
        'connected_cases':836,'zero_normalization_completed_cases':84,'scalar_invalid_trap_cases':8,
        'scope':'Infinity-class FMUL, zero-vector normalization and complete numerical caller chains; existing original routine bodies and prior baselines preserved.',
        'limitations':scalar['limitations']+connected['limitations']}
    registry_path=PROJECT/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));count=len(registry['routines'])
    for address in (0x8C638F28,0x8C078EB0,0x8C0B1D60,0x8C0AFC98,0x8C0B11C0):
        entry=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
        entry.setdefault('integration_evidence',{})['native_scalar_infinity_support']=record
    assert len(registry['routines'])==count
    registry_path.write_text(json.dumps(registry,indent=2)+'\n')
    status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress',{})['native_scalar_infinity_support']=record
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(f'Registered scalar infinity support and zero normalization; original routine entries unchanged at {count}.')


if __name__=='__main__':main()
