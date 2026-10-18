"""Verify B_CHAOS4 child/sibling flag propagation against retail SH-4."""
import ctypes
import hashlib
import json
import random
import struct

from verify_startup import ROOT, State, reference

BASE, ADDRESS, END = 0x0c900000, 0x0c9038c2, 0x0c903900
RAM_SIZE = 16 * 1024 * 1024
STACK_GUARD = 0x2000


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
    lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/b_chaos4_flag_propagation.dll'))
    function = lib.sa_b_chaos4_propagate_flag_0c9038c2
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE - 0x0c000000:BASE - 0x0c000000 + len(binary)] = binary
    offset = ADDRESS - BASE
    assert binary[offset:offset + END - ADDRESS] == bytes.fromhex(
        'e6 2f 43 6e d6 2f 53 6d 22 4f d8 2d 03 89 e2 60 '
        '08 cb 04 a0 02 2e e2 62 f7 e3 39 22 22 2e eb 51 '
        '18 21 02 89 d3 65 eb bf eb 54 ec 52 28 22 02 89 '
        'd3 64 ea af ec 5e 26 4f f6 6d 0b 00 f6 6e')
    rng = random.Random(0x424348414f533446)
    cases = []
    stack_changes = 0

    for case in range(128):
        node_count = 1 if case < 8 else rng.randrange(1, 25)
        addresses = [0x0c300000 + i * 0x100 for i in range(node_count)]
        children = [[] for _ in addresses]
        depths = [0] * node_count
        for child in range(1, node_count):
            possible = [parent for parent in range(child) if depths[parent] < 5]
            parent = rng.choice(possible)
            children[parent].append(child)
            depths[child] = depths[parent] + 1

        baseline = bytearray(original)
        for i, address in enumerate(addresses):
            flags = rng.getrandbits(32)
            child_address = addresses[children[i][0]] if children[i] else 0
            for ram in (baseline,):
                struct.pack_into('<I', ram, address - 0x0c000000, flags)
                struct.pack_into('<I', ram, address - 0x0c000000 + 44, child_address)
                struct.pack_into('<I', ram, address - 0x0c000000 + 48, 0)
            for siblings in children:
                for position, sibling in enumerate(siblings[:-1]):
                    if sibling == i:
                        struct.pack_into('<I', baseline,
                                         address - 0x0c000000 + 48,
                                         addresses[siblings[position + 1]])
                        break
        # The linked-list sibling pointer belongs to each node, not its parent.
        for siblings in children:
            for left, right in zip(siblings, siblings[1:]):
                struct.pack_into('<I', baseline,
                                 addresses[left] - 0x0c000000 + 48, addresses[right])

        retail_ram, actual_ram = bytearray(baseline), bytearray(baseline)
        initial = State()
        initial.r[:] = [rng.getrandbits(32) for _ in range(16)]
        initial.r[4] = addresses[0]
        initial.r[5] = rng.choice([0, 1, 2, 0xffffffff, rng.getrandbits(32)])
        initial.r[15] = 0x0cff0000
        initial.sr = rng.getrandbits(32)
        initial.pr = 0x0c001000 + case * 4
        initial.pc = ADDRESS
        expected = State.from_buffer_copy(bytes(initial))
        actual = State.from_buffer_copy(bytes(initial))
        steps = reference(retail_ram, expected)
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
        function(ram_view, ctypes.byref(actual))

        stack_start = initial.r[15] - 0x0c000000 - STACK_GUARD
        stack_end = initial.r[15] - 0x0c000000 + STACK_GUARD
        assert retail_ram[:stack_start] == actual_ram[:stack_start], f'RAM mismatch before stack at case {case}'
        assert retail_ram[stack_end:] == actual_ram[stack_end:], f'RAM mismatch after stack at case {case}'
        stack_changes += sum(a != b for a, b in zip(retail_ram[stack_start:stack_end],
                                                    actual_ram[stack_start:stack_end]))
        assert list(actual.r[8:16]) == list(expected.r[8:16]), (
            f'callee-saved CPU-state mismatch at case {case}')
        assert actual.pr == expected.pr and actual.pc == expected.pc, (
            f'call-boundary mismatch at case {case}')
        cases.append({'case': case, 'nodes': node_count, 'enabled': initial.r[5] != 0,
                      'retail_instructions': steps,
                      'non_stack_ram_and_abi_match': True})

    source = out / 'source/b_chaos4_flag_propagation.c'
    report = {
        'functions_verified': 1, 'total_abi_cases': len(cases),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'comparison_level': f'RAM compared across 16 MiB except a +/-{STACK_GUARD:#x}-byte caller-stack scratch guard; preserved GPRs R8-R14, SP, PR and return PC compared. Volatile GPRs, SR and stack scratch are excluded.',
        'stack_scratch_differences_observed': stack_changes,
        'scope': 'B_CHAOS4 routine at 0x0c9038c2 sets or clears flag bit 3 over first-child/next-sibling object trees. Acyclic trees of 1-24 nodes and both flag states were compared against retail execution.',
        'limitations': ['Verification uses the project partial SH-4 interpreter, not Dreamcast hardware or end-to-end boss gameplay.', 'The native implementation uses host recursion and intentionally does not reproduce transient retail stack scratch writes.'],
        'routines': [{'module': 'B_CHAOS4.BIN', 'load_address': hex(BASE),
                      'address': hex(ADDRESS), 'end_exclusive': hex(END),
                      'name': 'flag_propagation',
                      'module_sha256': hashlib.sha256(binary).hexdigest(),
                      'retail_span_sha256': hashlib.sha256(binary[offset:offset + END - ADDRESS]).hexdigest(),
                      'cases_passed': len(cases), 'cases': cases}],
    }
    report_path = out / 'analysis/common-overlays/B_CHAOS4.BIN/flag_propagation_verification.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"B_CHAOS4 flag propagation passed {len(cases)} tree and ABI comparisons")


if __name__ == '__main__':
    main()
