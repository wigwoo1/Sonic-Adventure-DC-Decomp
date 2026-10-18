"""Compare the initializer-call wrapper with retail SH-4 execution."""
import ctypes
import hashlib
import json
import random

from verify_startup import ROOT, State, reference

ADDRESS, LENGTH, SERVICE = 0x8c093b1e, 12, 0x8c093ce0
BASE, RAM_SIZE = 0x8c010000, 16 * 1024 * 1024
CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State))


def main():
    project = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original = bytearray(b'\xa5') * RAM_SIZE
    original[BASE & 0xffffff:(BASE & 0xffffff) + len(binary)] = binary
    library = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/sa_initializer_service_wrapper_8c093b1e.dll'))
    function = library.sa_initializer_service_wrapper_8c093b1e
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State), CALLBACK]
    function.restype = None
    rng = random.Random(0x8c093b1e)
    cases = []

    for case in range(128):
        return_pc = 0x8c00e000 + case * 4
        data_address = 0x8c700000 + case * 4
        data_value = rng.getrandbits(32)
        write_byte = rng.getrandbits(8)
        output_sr = rng.getrandbits(32)
        initial = bytearray(original)
        initial[data_address & 0xffffff] = 0
        retail_ram, native_ram = bytearray(initial), bytearray(initial)
        expected = State()
        expected.r[:] = [rng.getrandbits(32) for _ in range(16)]
        expected.r[15] = 0x8c800000 + case * 0x100 + 0x80
        expected.sr = rng.getrandbits(32)
        expected.pr = return_pc
        expected.pc = ADDRESS
        actual = State.from_buffer_copy(bytes(expected))
        retail_service_state = []
        native_service_state = []

        def retail_service(ram, regs, _sr):
            retail_service_state.append((list(regs), expected.pr))
            for reg in range(8):
                regs[reg] ^= (0x13579bdf * (reg + 1) + case) & 0xffffffff
            ram[data_address & 0xffffff] = write_byte
            return output_sr

        def native_service(ram_pointer, cpu_pointer):
            cpu = cpu_pointer.contents
            native_service_state.append((list(cpu.r), cpu.pr, cpu.pc))
            for reg in range(8):
                cpu.r[reg] = (cpu.r[reg] ^
                              ((0x13579bdf * (reg + 1) + case) & 0xffffffff))
            ram_pointer[data_address & 0xffffff] = write_byte
            cpu.sr = output_sr

        service_callback = CALLBACK(native_service)
        steps = reference(retail_ram, expected, hooks={SERVICE: retail_service})
        ram_view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        function(ram_view, ctypes.byref(actual), service_callback)
        assert steps == 6, (case, steps)
        assert len(retail_service_state) == len(native_service_state) == 1
        retail_regs, retail_pr = retail_service_state[0]
        native_regs, native_pr, native_pc = native_service_state[0]
        assert native_regs == retail_regs and native_pr == retail_pr and native_pc == SERVICE, (
            f'service entry mismatch case {case}')
        assert native_ram == retail_ram, f'RAM mismatch case {case}'
        assert bytes(actual) == bytes(expected), (
            f'CPU mismatch case {case}: '
            f'native r0/r1/r15/pr/pc={actual.r[0]:#x}/{actual.r[1]:#x}/'
            f'{actual.r[15]:#x}/{actual.pr:#x}/{actual.pc:#x}, '
            f'retail={expected.r[0]:#x}/{expected.r[1]:#x}/{expected.r[15]:#x}/'
            f'{expected.pr:#x}/{expected.pc:#x}')
        cases.append({'case': case, 'service_ram_write': hex(write_byte),
                      'service_entry_state_matched': True,
                      'full_ram_and_cpu_match': True})

    source = project / 'source/sa_initializer_service_wrapper_8c093b1e.c'
    report = {
        'module': '1ST_READ.BIN', 'address': hex(ADDRESS),
        'end_exclusive': hex(ADDRESS + LENGTH),
        'name': 'sa_initializer_service_wrapper_8c093b1e',
        'retail_span_sha256': hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cases_passed': len(cases),
        'comparison_level': 'All 16 MiB RAM and complete GPR/SR/PR/PC state across the initializer service call and wrapper return.',
        'scope': '128 randomized CPU fixtures with a modeled initializer that clobbers caller-saved registers, updates SR, and writes RAM.',
        'limitations': ['The initializer body at 0x8c093ce0 remains a separate unrecovered routine.'],
        'cases': cases,
    }
    (project / 'analysis/sa_initializer_service_wrapper_8c093b1e_verification.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f"Initializer service wrapper passed {len(cases)} full RAM/CPU comparisons")


if __name__ == '__main__':
    main()
