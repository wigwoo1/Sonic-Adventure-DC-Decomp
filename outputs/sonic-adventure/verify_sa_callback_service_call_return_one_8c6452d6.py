import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
BASE = 0x8C010000
FUNCTION = 0x8C6452D6
LENGTH = 14
RAM_SIZE = 16 * 1024 * 1024
CALLBACK_LITERAL = 0x8C64533C
binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
span = binary[FUNCTION - BASE:FUNCTION - BASE + LENGTH]
assert span == bytes.fromhex('22 4f 18 d3 0b 43 09 00 26 4f 0b 00 01 e0')
assert int.from_bytes(binary[CALLBACK_LITERAL - BASE:CALLBACK_LITERAL - BASE + 4], 'little') == 0x8C10D960

library = ctypes.CDLL(str(ROOT / 'work' / 'sonic-adventure' / 'build' / 'sa_callback_service_call_return_one_8c6452d6.dll'))
SERVICE = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State))
native = library.sa_callback_service_call_return_one_8c6452d6
native.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), SERVICE]
native.restype = None

rng = random.Random(FUNCTION)
cases = []
for case in range(144):
    ram = bytearray([0xA5]) * RAM_SIZE
    ram[BASE - 0x8C000000:BASE - 0x8C000000 + len(binary)] = binary
    state = State()
    state.r[:] = [rng.getrandbits(32) for _ in range(16)]
    state.r[15] = 0x8C800000 + case * 0x100 + 0x80
    state.sr = rng.getrandbits(32)
    state.pr = 0x8CFED000 + case * 4
    state.pc = FUNCTION

    callback = 0x8C720000 + case * 4
    write_address = 0x8C700000 + rng.randrange(0x10000 // 4) * 4
    write_value = rng.getrandbits(32)
    return_address = state.pr
    if case in (0, 1):
        write_address = state.r[15] - 4
        write_value = 0x8CFED800 + case * 4
        return_address = write_value
    literal_at = CALLBACK_LITERAL - 0x8C000000
    ram[literal_at:literal_at + 4] = callback.to_bytes(4, 'little')

    def service_effect(ram_bytes, regs, case_id, address, value):
        for reg in range(8):
            regs[reg] ^= (0x369CF147 * (reg + 1) + case_id) & 0xFFFFFFFF
        ram_bytes[address - 0x8C000000:address - 0x8C000000 + 4] = value.to_bytes(4, 'little')
        return (0x6B000000 | case_id) & 0xFFFFFFFF

    def retail_hook(ram_bytes, regs, sr, *, case_id=case, address=write_address, value=write_value):
        return service_effect(ram_bytes, regs, case_id, address, value)

    expected_ram, expected_state = bytearray(ram), State.from_buffer_copy(state)
    reference(expected_ram, expected_state, hooks={callback: retail_hook}, stop_pc=return_address)

    def native_service(ram_pointer, cpu_pointer, *, case_id=case, address=write_address, value=write_value):
        native_ram = ctypes.cast(ram_pointer, ctypes.POINTER(ctypes.c_uint8))
        cpu = cpu_pointer.contents
        for reg in range(8):
            cpu.r[reg] ^= (0x369CF147 * (reg + 1) + case_id) & 0xFFFFFFFF
        at = address - 0x8C000000
        for index, byte in enumerate(value.to_bytes(4, 'little')):
            native_ram[at + index] = byte
        cpu.sr = (0x6B000000 | case_id) & 0xFFFFFFFF

    native_callback = SERVICE(native_service)
    actual_ram = bytearray(ram)
    ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(actual_ram)
    actual_state = State.from_buffer_copy(state)
    native(ram_view, ctypes.byref(actual_state), native_callback)
    assert actual_ram == expected_ram, f'RAM mismatch in case {case}'
    assert bytes(actual_state) == bytes(expected_state), f'CPU mismatch in case {case}'
    assert actual_state.r[0] == 1, f'return-value mismatch in case {case}'
    assert actual_state.pc == return_address and actual_state.r[15] == state.r[15]
    cases.append({'case': case, 'callback': hex(callback), 'saved_pr_write_alias': case in (0, 1), 'return_address': hex(return_address), 'return_value': 1, 'matched': True})

report = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'address': hex(FUNCTION),
    'end_exclusive': hex(FUNCTION + LENGTH),
    'retail_span_sha256': hashlib.sha256(span).hexdigest(),
    'source_sha256': hashlib.sha256((PROJECT / 'source' / 'sa_callback_service_call_return_one_8c6452d6.c').read_bytes()).hexdigest(),
    'cases_passed': len(cases),
    'comparison_level': 'complete 16 MiB RAM and all GPR/SR/PR/PC state across callback call, return-value delay slot, and wrapper return against retail SH-4 interpreter',
    'scope': 'Save PR, call the function pointer at 0x8c64533c, restore PR/R15, set R0=1 in the RTS delay slot, and return. Callback service mutates caller-saved registers, SR, and RAM.',
    'limitations': ['The callback body is modeled as a service and remains a separate routine.', 'Differential verification uses the project SH-4 interpreter, not Dreamcast hardware or whole-game execution.'],
    'callback_literal': hex(CALLBACK_LITERAL), 'cases': cases,
}
(PROJECT / 'analysis' / 'sa_callback_service_call_return_one_8c6452d6_verification.json').write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(cases)} callback call/return-one RAM/CPU comparisons passed')
