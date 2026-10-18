"""Check original instruction evidence; this is not runtime validation."""
import hashlib
import json
import struct
from pathlib import Path

root = Path(__file__).resolve().parents[3]
binary = (root / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
expected = {
    0x8c604718: 0x6d43,  # preserve incoming flags
    0x8c604734: 0x23d9,  # mask flags with 0xfff
    0x8c60473a: 0x1f32,  # save masked flags
    0x8c604886: 0x64b3,  # first selector receives flags & 3
    0x8c604892: 0x54f2,  # second selector receives saved flags
    0x8c6049d4: 0x6d43,
    0x8c6049ea: 0x23d9,
    0x8c6049f2: 0x1f32,
    0x8c604b1c: 0x54f2,
    0x8c010194: 0x1f41,  # startup preserves its input flags
    0x8c0101ce: 0x54f1,  # startup forwards them to initialization
    0x8c10b8c8: 0x6412,  # alternate path loads flags from global
    0x8c10b8e6: 0x6432,
}
for address, opcode in expected.items():
    assert struct.unpack_from('<H', binary, address - 0x8c010000)[0] == opcode, hex(address)
for address in (0x8c604770, 0x8c604aa4):
    assert struct.unpack_from('<H', binary, address - 0x8c010000)[0] == 0x0fff
result = {
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'direct_checks_passed': len(expected) + 2,
    'finding': 'Both initialization paths supply incoming flags & 0xfff as DisplayMode default; selector reduces this to flags & 3 for the constructor selection.',
    'initialization_paths': ['0x8c604702', '0x8c6049c0'],
    'upstream_sources': {'0x8c010180': 'incoming r4 forwarded unchanged', '0x8c10b8a6': ['global 0x8c7a7fe0 when input r4 is zero', 'global 0x8c7a7fec otherwise']},
    'evidence': ['analysis/instructions/8c604702.tsv', 'analysis/instructions/8c6049c0.tsv'],
    'limitations': ['Static instruction checks, not runtime execution.', 'Actual game caller flags remain to be established.', 'No additional native routines recovered by this trace.'],
}
Path(__file__).with_name('video_default_callers.json').write_text(json.dumps(result, indent=2) + '\n')
print(f"Verified {len(expected) + 2} original instruction/literal checks")
