"""Pin the bank-selection path and its architectural alignment evidence."""
import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
verification='analysis/arm_sound_bank_select_verification.json';guard_path='analysis/arm_sound_bank_select_guards_verification.json'
report=json.loads((project/verification).read_text());guards=json.loads((project/guard_path).read_text())
alignment_path=project/'analysis/arm_sound_alignment_verification.json';alignment=json.loads(alignment_path.read_text())
boot=json.loads((project/'analysis/arm_sound_boot_native_verification.json').read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert (report['cases_passed'],report['native_batch_cases_passed'],report['reference_alignment_corrections'])==(1152,128,640)
assert (guards['instruction_budget_cases_passed'],guards['aligned_mmio_failure_cases_passed'])==(20,12)
assert (alignment['word_load_cases_passed'],alignment['word_store_cases_passed'],alignment['byte_store_cases_passed'])==(32,16,16)
assert report['dll_sha256']==guards['dll_sha256']==alignment['dll_sha256']==boot['dll_sha256']==sha(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll')
assert report['source_sha256']==guards['source_sha256']==boot['bank_select_source_sha256']==sha(project/'source/sa_arm_sound_bank_select.c')
assert report['header_sha256']==guards['header_sha256']==alignment['header_sha256']==boot['memory_header_sha256']==sha(project/'source/arm_sound_memory.h')
assert report['adapter_sha256']==guards['adapter_sha256']==alignment['adapter_sha256']==sha(project/'arm_legacy_alignment_reference.py')
assert report['translation_sha256']==sha(project/'analysis/arm_sound_bank_select_translation.json')
assert report['verifier_sha256']==sha(project/'verify_arm_sound_bank_select.py')
assert guards['verifier_sha256']==sha(project/'verify_arm_sound_bank_select_guards.py')
assert alignment['verifier_sha256']==sha(project/'verify_arm_sound_alignment.py')
assert guards['bank_select_evidence_sha256']==sha(project/verification)
assert report['alignment_evidence_sha256']==sha(alignment_path)
path=project/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
assert len(registry['routines'])==42
registry['connected_flows']['bank_selection']={'verification':verification,'native_type':4,
 'classification':'Complete command subpath, not a separately identified complete function',
 'limitations':report['limitations']}
path.write_text(json.dumps(registry,indent=2)+'\n')
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_bank_select_native']={
 'verification':verification,'guard_verification':guard_path,'alignment_verification':'analysis/arm_sound_alignment_verification.json',
 'direct_cases':1152,'native_batch_cases':128,'guard_cases':32,'alignment_cases':64,
 'classification':'Native type-4 bank/program selection and documented legacy memory alignment; no routine-count or milestone credit',
 'limitations':report['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded 1280 bank-selection comparisons, 32 guards and 64 alignment cases')
