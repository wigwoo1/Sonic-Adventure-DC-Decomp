import random

from verify_startup import ROOT, State, reference

BASE = 0x8C010000
RAM_SIZE = 16 * 1024 * 1024
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
rng = random.Random(0x4F12_4F16)
passed = 0

for case in range(64):
    ram = bytearray(RAM_SIZE)
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    # STS.L MACL,@-R15; LDS.L @R15+,MACL; stop at the next instruction.
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + 4] = bytes.fromhex('12 4f 16 4f')
    initial_macl = rng.getrandbits(32)
    stack = 0x8C700000 + case * 0x100
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[15] = stack
    state.pr = BASE + 4
    state.pc = BASE
    special = {'macl': initial_macl}

    expected = bytearray(ram)
    expected_state = State.from_buffer_copy(state)
    expected_special = {'macl': initial_macl}
    reference(expected, expected_state, special=expected_special)

    assert expected_state.r[15] == stack
    assert expected_special['macl'] == initial_macl
    at = (stack - 4 - 0x8C000000) & 0xFFFFFF
    assert int.from_bytes(expected[at:at + 4], 'little') == initial_macl
    passed += 1

print(f'{passed} MACL stack-save/restore interpreter checks passed')
