"""Compare the recovered input-service caller plus native cleanup against retail."""
import ctypes
import hashlib
import json
import struct

from verify_fpu_cleanup_transfer import Fpu
from verify_startup import ROOT, State, reference
from verify_tikal_controller import Call, P

PROJECT = ROOT / 'outputs/sonic-adventure'
RAM_SIZE = 16 * 1024 * 1024
FUNCTION = 0x8C647198
CLEANUP = 0x8C646654
OBJECT_BASE = 0x8CA10000
FLAG_OBJECT = 0x8CA20000
LOCK_ADDRESS = 0x8CA30000
INFO_CALLBACK = 0x8C600200
INFO_RECORD = 0x8CA40000

entry_lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_service_entry_8c647198.dll'))
ENTRY = entry_lib.sa_input_service_entry_8c647198
ENTRY.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P,
                  ctypes.c_void_p, Call]
ENTRY.restype = ctypes.c_int
cleanup_lib = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_service_cleanup_native.dll'))
CLEANUP_NATIVE = cleanup_lib.sa_input_service_cleanup_native
CLEANUP_NATIVE.argtypes = ENTRY.argtypes
CLEANUP_NATIVE.restype = ctypes.c_int


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def run_case(case_number, service_result):
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram = bytearray(RAM_SIZE)
    ram[0x10000:0x10000 + len(binary)] = binary

    # Set the same global/object fields the cleanup verifier models, and make
    # the input-service cleanup condition true so control enters 0x8c646654.
    put32(ram, 0x8C8A83B4, OBJECT_BASE)
    put32(ram, OBJECT_BASE + 0x10E8, 1)
    put32(ram, OBJECT_BASE + 0x10F4, FLAG_OBJECT)
    put32(ram, FLAG_OBJECT + 0x18, 0)
    put32(ram, OBJECT_BASE + 0xFC0, 0)
    put32(ram, OBJECT_BASE + 0xFB0, 0)
    put32(ram, OBJECT_BASE + 0xFBC, 0)
    for channel_offset in (0, 6, 12, 18):
        ram[(OBJECT_BASE + 0xF90 + channel_offset) & 0xFFFFFF] = 0
    put32(ram, 0x8C6607F4, LOCK_ADDRESS)
    ram[LOCK_ADDRESS & 0xFFFFFF] = 0
    put32(ram, 0x8C893E48, 0)
    put32(ram, 0x8C893E18, INFO_RECORD)
    put32(ram, 0x8C893E4C, 0)
    table_at = 0x8C893E50 & 0xFFFFFF
    ram[table_at:table_at + 48 * 12] = bytes(48 * 12)
    put32(ram, 0x8C6468B8, INFO_CALLBACK)
    # The retail test fixtures use RAM-backed values for these hardware words.
    mmio = {0xA05F810C: 0, 0xA05F80DC: 0, 0xA05F8040: 0, 0xA05F80E8: 0}
    for address, value in mmio.items():
        put32(ram, address, value)

    initial = State()
    initial.r[:] = [((case_number + 1) * 0x13579BDF + i * 0x1020304) & 0xFFFFFFFF
                    for i in range(16)]
    initial.r[4] = (0x8CA20000 + case_number * 0x100) & 0xFFFFFFFF
    initial.r[7] ^= 0x5A5AA5A5
    initial.r[14] = 0x8CE10000 + case_number * 16
    initial.r[15] = 0x8CF00000
    initial.sr = (0x70000000 | (case_number * 0x101)) & 0xFFFFFFFF
    initial.pr = 0x8C000100
    initial.pc = FUNCTION
    initial_fpu = Fpu()
    initial_fpu.fpscr = 0x200000 if case_number & 1 else 0
    initial_mac = (0xA1B2C3D4 ^ case_number) & 0xFFFFFFFF

    expected_ram = bytearray(ram)
    expected = State.from_buffer_copy(initial)
    expected_fpu = Fpu.from_buffer_copy(initial_fpu)
    expected_special = {'macl': initial_mac}
    expected_calls = []

    def hook(target):
        def body(memory, regs, sr):
            expected_calls.append((target, tuple(regs[4:8]), regs[15], expected.pr))
            if target == 0x8C6604C0:
                regs[0] = service_result & 0xFFFFFFFF
            if target == INFO_CALLBACK:
                regs[0] = INFO_RECORD
            return sr
        return body

    hooks = {0x8C6604C0: hook(0x8C6604C0),
             0x8C646F68: hook(0x8C646F68),
             0x8C65F348: hook(0x8C65F348),
             INFO_CALLBACK: hook(INFO_CALLBACK)}
    reference(expected_ram, expected, hooks=hooks, fpu=expected_fpu,
              special=expected_special, mmio=mmio)

    native_ram = bytearray(ram)
    native = State.from_buffer_copy(initial)
    native_fpu = Fpu.from_buffer_copy(initial_fpu)
    view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
    sr = ctypes.c_uint32(initial.sr)
    pr = ctypes.c_uint32(initial.pr)
    fpul = ctypes.c_uint32(0xCAFEBABE)
    macl = ctypes.c_uint32(initial_mac)
    native_calls = []
    errors = []

    @Call
    def dispatch(_context, target, _memory, fpu_ptr, sr_ptr, regs_ptr, pr_ptr):
        target &= 0xFFFFFFFF
        native_calls.append((target, tuple(regs_ptr[i] for i in range(4, 8)),
                             regs_ptr[15], pr_ptr[0]))
        if target == CLEANUP:
            result = CLEANUP_NATIVE(view, ctypes.byref(native_fpu), sr_ptr, regs_ptr,
                                    pr_ptr, ctypes.byref(fpul), ctypes.byref(macl),
                                    None, dispatch)
            if result != 0:
                errors.append(f'cleanup returned {result}')
            return result
        if target == 0x8C6604C0:
            regs_ptr[0] = service_result & 0xFFFFFFFF
            return 0
        if target == INFO_CALLBACK:
            regs_ptr[0] = INFO_RECORD
            return 0
        if target in (0x8C646F68, 0x8C65F348):
            return 0
        errors.append(hex(target))
        return -9

    result = ENTRY(view, ctypes.byref(native_fpu), ctypes.byref(sr), native.r,
                   ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl), None, dispatch)
    assert result == 0, f'Native caller returned {result} in case {case_number}'
    assert not errors, f'Unexpected native dispatch results: {errors}'
    assert native_ram == expected_ram, f'RAM mismatch for service result {service_result:#x}'
    assert list(native.r) == list(expected.r), f'GPR mismatch for service result {service_result:#x}'
    assert sr.value == expected.sr and pr.value == expected.pr
    assert bytes(native_fpu) == bytes(expected_fpu)
    assert fpul.value == 0xCAFEBABE and macl.value == expected_special['macl']
    # CLEANUP is a native dispatch boundary, while the reference interpreter
    # enters its retail instructions directly; compare the external effects.
    assert [call for call in native_calls if call[0] != CLEANUP] == expected_calls, (
        native_calls, expected_calls)
    assert CLEANUP in [call[0] for call in native_calls]
    return {'service_result': hex(service_result), 'calls': [hex(c[0]) for c in native_calls],
            'full_ram_cpu_fpu_and_callback_trace_match': True}


def main():
    values = (0, 1, 0x7FFFFFFF, 0xFFFFFFFF)
    cases = [run_case(i, value) for i, value in enumerate(values)]
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source = PROJECT / 'source/sa_input_service_entry_8c647198.c'
    cleanup_source = PROJECT / 'source/sa_input_service_cleanup_8c646654.c'
    report = {
        'module': '1ST_READ.BIN', 'entry_address': hex(FUNCTION),
        'integrated_cleanup_address': hex(CLEANUP), 'cases_passed': len(cases),
        'entry_source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
        'cleanup_source_sha256': hashlib.sha256(cleanup_source.read_bytes()).hexdigest(),
        'binary_sha256': hashlib.sha256(binary).hexdigest(),
        'comparison_level': 'Full 16 MiB RAM, all GPR/SR/PR/FPU state and external callback order compared against retail SH-4 interpretation; the native caller invokes the recovered cleanup and its recovered native dependency graph.',
        'limitations': [
            'Device service, record writer, and fatal callback remain explicit stubs.',
            'Runtime callback records are zeroed in these integration scenarios; this does not verify every runtime callback implementation.',
            'This is input-service integration evidence, not whole-game or port verification.'
        ],
        'cases': cases,
    }
    output = PROJECT / 'analysis/input_service_cleanup_integration_verification.json'
    output.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')

    registry_path = PROJECT / 'analysis/recovered_routines.json'
    registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
    routine = next(item for item in registry['routines']
                   if item.get('module') == '1ST_READ.BIN'
                   and int(item['address'], 16) == FUNCTION)
    routine['integration_verification'] = 'analysis/input_service_cleanup_integration_verification.json'
    routine['integration_status'] = (
        f"Native input-service entry plus recovered cleanup matched retail in {len(cases)} "
        'full-state scenarios, including positive and zero service results.')
    routine['integration_limitations'] = report['limitations']
    registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

    status_path = PROJECT / 'project_status.json'
    status = json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress', {})['input_service_cleanup_integration'] = {
        'verification': 'analysis/input_service_cleanup_integration_verification.json',
        'cases': len(cases),
        'comparison_level': report['comparison_level'],
        'limitations': report['limitations'],
    }
    status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
    print(f'{len(cases)} input-entry plus native-cleanup retail comparisons passed')


if __name__ == '__main__':
    main()
