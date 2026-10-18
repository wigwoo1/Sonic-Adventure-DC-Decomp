"""Differentially verify B_CHAOS6's conditional global-byte synchronization."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

OVERLAY_BASE, ADDRESS, END = 0x0c900000, 0x0c9030a4, 0x0c9030bc
MAIN_BASE = 0x8c010000
RAM_BASE, RAM_SIZE = 0x0c000000, 16 * 1024 * 1024
STACK_GUARD, CASE_COUNT = 0x2000, 128
SOURCE_SLOT, SOURCE_BYTE = 0x8c6b6884, 0x8c020000


def offset(address):
    return (address & 0x1fffffff) - RAM_BASE


def main():
    out = ROOT / 'outputs/sonic-adventure'
    overlay = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS6.BIN').read_bytes()
    main_binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    span_offset = ADDRESS - OVERLAY_BASE
    assert len(overlay[span_offset:span_offset + END - ADDRESS]) == END - ADDRESS

    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos6_sync_global_bytes.dll'))
    function = library.sa_b_chaos6_sync_global_bytes_0c9030a4
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None

    original = bytearray(b'\xa5') * RAM_SIZE
    for address, binary in ((MAIN_BASE, main_binary), (OVERLAY_BASE, overlay)):
        start = offset(address)
        original[start:start + len(binary)] = binary

    rng = random.Random(0x4348414f533630a4)
    cases = []
    for case in range(CASE_COUNT):
        baseline = bytearray(original)
        source_byte_value = rng.randrange(256)
        source = {9: 0x8c6b6888, 10: 0x8c6b6889, 11: 0x8c6b688a}.get(case, SOURCE_BYTE + case * 16)
        baseline[offset(SOURCE_SLOT):offset(SOURCE_SLOT) + 4] = source.to_bytes(4, 'little')
        baseline[offset(source)] = source_byte_value
        retail_ram, native_ram = bytearray(baseline), bytearray(baseline)

        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = ((source_byte_value - 256) if source_byte_value & 0x80 else source_byte_value) & 0xffffffff
        if case & 1:
            initial.r[4] = rng.getrandbits(32)
            if initial.r[4] == ((source_byte_value - 256) if source_byte_value & 0x80 else source_byte_value) & 0xffffffff:
                initial.r[4] ^= 0x100
        initial.r[15] = 0x0cff0000
        initial.sr = (rng.getrandbits(32) & ~1) | (case & 1)
        initial.pr = 0x0c004000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))

        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - RAM_BASE - STACK_GUARD
        stack_end = initial.r[15] - RAM_BASE + STACK_GUARD
        if retail_ram[:stack_start] != native_ram[:stack_start]:
            first = next(i for i, (a, b) in enumerate(zip(retail_ram[:stack_start], native_ram[:stack_start])) if a != b)
            rc = [(i + RAM_BASE, baseline[i], retail_ram[i]) for i in range(len(baseline)) if baseline[i] != retail_ram[i] and i < stack_start][:12]
            nc = [(i + RAM_BASE, baseline[i], native_ram[i]) for i in range(len(baseline)) if baseline[i] != native_ram[i] and i < stack_start][:12]
            raise AssertionError(f'RAM differs case={case} first={first + RAM_BASE:#010x}; retail={retail_ram[first]:02x}; native={native_ram[first]:02x}; retail_changes={rc}; native_changes={nc}; R0-R5 retail={[hex(v) for v in expected.r[:6]]} native={[hex(v) for v in actual.r[:6]]}')
        assert retail_ram[stack_end:] == native_ram[stack_end:], f'RAM differs after caller stack in case {case}'
        assert list(actual.r) == list(expected.r), f'GPR state differs in case {case}'
        assert actual.sr == expected.sr, f'SR differs in case {case}'
        assert actual.pr == expected.pr and actual.pc == expected.pc, f'call boundary differs in case {case}'
        cases.append({'case': case, 'source_byte': hex(source_byte_value),
                      'parameter': hex(initial.r[4]), 'retail_instructions': steps,
                      'ram_and_full_cpu_state_match': True})

    source_path = out / 'source/b_chaos6_sync_global_bytes.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
        'comparison_level': 'All RAM except caller-stack guard; all 16 GPRs, SR, PR and return PC compared.',
        'scope': 'Loads the source pointer from the B_CHAOS6 literal pool, compares its signed byte with R4, and when unequal stores R4 to 0x8c6b6889 then reloads and mirrors the source byte to 0x8c6b6888 with retail register and T-bit effects.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or gameplay.',
                        'The source pointer slot and pointed byte are initialized in cloned RAM; retail instruction bytes remain unchanged.'],
        'dependencies': [
            {'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
             'sha256': hashlib.sha256(overlay).hexdigest(),
             'source_pointer_literal_address': hex(0x0c90310c), 'source_pointer_slot_address': hex(SOURCE_SLOT)},
            {'module': '1ST_READ.BIN', 'load_address': hex(MAIN_BASE),
             'sha256': hashlib.sha256(main_binary).hexdigest()},
        ],
        'routines': [{
            'module': 'B_CHAOS6.BIN', 'load_address': hex(OVERLAY_BASE),
            'address': hex(ADDRESS), 'end_exclusive': hex(END),
            'name': 'sync_global_bytes',
            'retail_span_sha256': hashlib.sha256(overlay[span_offset:span_offset + END - ADDRESS]).hexdigest(),
            'cases_passed': len(cases), 'cases': cases,
        }],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS6.BIN/sync_global_bytes_verification.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'B_CHAOS6 global-byte synchronization passed {len(cases)} RAM and full CPU comparisons')


if __name__ == '__main__':
    main()
