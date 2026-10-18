"""Verify the ADVERTISE binary's header pointers without claiming full function coverage."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
WORK = ROOT.parent.parent / 'work' / 'sonic-adventure'
MODULE = WORK / 'decoded' / 'SONICADV' / 'ADVERTISE.BIN'
BASE = 0x8C900000
EXPECTED_POINTERS = (0x8C9000F4, 0x8C9001C0, 0x8C9001DA)

module = MODULE.read_bytes()
scope = json.loads((ROOT / 'analysis' / 'module_scope.json').read_text(encoding='utf-8'))
entry = next(item for item in scope['unclassified_binaries'] if item['source'] == 'SONICADV/ADVERTISE.PRS')
decoded_sha256 = hashlib.sha256(module).hexdigest()
assert len(module) == entry['decoded_bytes'] == 937008
assert decoded_sha256 == entry['sha256']

records = []
for table_offset, expected in enumerate(EXPECTED_POINTERS):
    table_slot = table_offset * 4
    pointer = int.from_bytes(module[table_slot:table_slot + 4], 'little')
    assert pointer == expected
    assert BASE <= pointer < BASE + len(module) - 2
    offset = pointer - BASE
    opcode = int.from_bytes(module[offset:offset + 2], 'little')
    assert opcode == 0x4F22, f'{pointer:#010x}: expected STS.L PR,@-R15 prologue, got {opcode:#06x}'
    records.append({
        'table_offset': hex(table_slot),
        'pointer': hex(pointer),
        'module_offset': hex(offset),
        'first_instruction': '0x4f22',
        'instruction_name': 'STS.L PR,@-R15',
        'classification': 'verified header pointer to executable prologue',
    })

report = {
    'module': 'ADVERTISE.BIN',
    'source_resource': 'SONICADV/ADVERTISE.PRS',
    'load_address': hex(BASE),
    'decoded_bytes': len(module),
    'decoded_sha256': decoded_sha256,
    'entry_pointers_verified': records,
    'code_bearing_evidence': 'Three leading header pointers resolve inside the decoded module to SH-4 PR-save prologues.',
    'function_boundaries_complete': False,
    'function_index_exported': False,
    'coverage_claim': 'Partial entry-point evidence only; does not establish the full function count, code/data boundaries, or recovered behavior.',
}
(ROOT / 'analysis' / 'advertise_entrypoint_evidence.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print('Verified 3 ADVERTISE header pointers to in-range SH-4 prologues; module boundaries remain incomplete.')
