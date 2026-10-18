import ctypes
import hashlib
import itertools
import json
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P


RAM_SIZE = 16 * 1024 * 1024
BASE = 0x8C000000
FUNCTION = 0x8C646654
CALLBACK = 0x8C600100
INFO_CALLBACK = 0x8C600200
INFO_RECORD = 0x8CA40000
TABLE_CALLBACK = 0x8C600300


def put32(ram, address, value):
    struct.pack_into('<I', ram, address & 0xFFFFFF, value & 0xFFFFFFFF)


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    fn = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_service_cleanup_native.dll')).sa_input_service_cleanup_native
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P, ctypes.c_void_p, Call]
    count = 0
    counts = {'negative_exit_cases': 0, 'active_publication_cases': 0, 'callback_table_cases': 0}
    for status_word, flag_word, lock_value, callback_enabled, bank, stack_offset, active_case, record_case, record_index in itertools.product(
        [0, 1], [0, 1], [0, 1], [0, 1], [0, 0x200000], [0, 4], [0, 1], [0, 1], range(48)
    ):
        startup_success = status_word == 0 and flag_word == 0 and lock_value == 0
        if (active_case or record_case) and not startup_success:
            continue
        if active_case and record_case:
            continue
        if not record_case and record_index != 0:
            continue
        ram = bytearray(RAM_SIZE)
        ram[0x10000:0x10000 + len(binary)] = binary
        object_base = 0x8CA10000
        flag_object = 0x8CA20000
        lock_address = 0x8CA30000
        put32(ram, 0x8C8A83B4, object_base)
        put32(ram, object_base + 0x10F4, flag_object)
        put32(ram, flag_object + 0x18, flag_word)
        put32(ram, object_base + 0xFC0, status_word)
        put32(ram, object_base + 0xFB0, int(active_case))
        put32(ram, object_base + 0xFBC, int(active_case))
        for channel_offset in (0, 6, 12, 18):
            ram[(object_base + 0xF90 + channel_offset) & 0xFFFFFF] = 0
        put32(ram, 0x8C6607F4, lock_address)
        ram[lock_address & 0xFFFFFF] = lock_value
        put32(ram, 0x8C893E48, CALLBACK if callback_enabled else 0)
        put32(ram, 0x8C893E18, 0x8CA40000)
        put32(ram, 0x8C893E4C, 0)
        if active_case:
            put32(ram, 0x8C6468B8, INFO_CALLBACK)
            record = 0x893BD0 + 12 * 24
            put32(ram, record, 0x80)
            ram[record + 2] = 0
            ram[record + 4] = 8
            put32(ram, record + 8, 0x8CA60000)
            ram[0xA60008] = 0
            ram[0xA40000] = 0x80
            ram[0x894110] = 1
        if record_case:
            match_value = 0x12345678
            put32(ram, 0x8C6468B8, INFO_CALLBACK)
            put32(ram, object_base + 0x28 + record_index * 44 + 0x20, match_value)
            record = 0x8C893E50 + record_index * 12
            put32(ram, record, record_index)
            put32(ram, record + 4, match_value)
            put32(ram, record + 8, TABLE_CALLBACK)
        mmio = {0xA05F810C: 0, 0xA05F80DC: 0, 0xA05F8040: 0, 0xA05F80E8: 0}
        for address, value in mmio.items():
            put32(ram, address, value)

        native_ram = bytearray(ram)
        view = (ctypes.c_uint8 * RAM_SIZE).from_buffer(native_ram)
        state = State()
        state.r[:] = [((status_word + 1) * 0x13579BDF + i * 0x1020304) & 0xFFFFFFFF for i in range(16)]
        state.r[14] = 0x8CE10000
        state.r[15] = 0x8CF00000 - stack_offset
        state.sr = 0x70000063
        state.pr = 0x8C000100
        state.pc = FUNCTION
        expected = State.from_buffer_copy(state)
        native_r = (ctypes.c_uint32 * 16)(*state.r)
        native_sr = ctypes.c_uint32(state.sr)
        native_pr = ctypes.c_uint32(state.pr)
        expected_fpu = Fpu()
        expected_fpu.fpscr = bank
        native_fpu = Fpu.from_buffer_copy(expected_fpu)
        expected_mac = 0xA1B2C3D4 ^ status_word ^ (flag_word << 8) ^ lock_value
        native_mac = ctypes.c_uint32(expected_mac)
        native_fpul = ctypes.c_uint32(0xCAFEBABE)
        events_expected = []
        events_native = []

        def hook_for(target):
            def hook(memory, regs, sr):
                events_expected.append((target, tuple(regs[4:8]), regs[15], expected.pr))
                return sr
            return hook

        def info_hook(memory, regs, sr_value):
            events_expected.append((INFO_CALLBACK, tuple(regs[4:8]), regs[15], expected.pr))
            regs[0] = INFO_RECORD
            return sr_value

        hooks = {0x8C65F348: hook_for(0x8C65F348)}
        if callback_enabled:
            hooks[CALLBACK] = hook_for(CALLBACK)
        if active_case:
            hooks[INFO_CALLBACK] = info_hook
        if record_case:
            def table_hook(memory, regs, sr_value):
                events_expected.append((TABLE_CALLBACK, tuple(regs[4:8]), regs[15], expected.pr))
                return sr_value
            hooks[INFO_CALLBACK] = info_hook
            hooks[TABLE_CALLBACK] = table_hook
        special = {'macl': expected_mac}
        reference(ram, expected, hooks=hooks, fpu=expected_fpu, special=special, mmio=mmio)
        for address, value in mmio.items():
            put32(ram, address, value)

        @Call
        def call(ctx, target, memory, fpu, sr, regs, pr):
            events_native.append((target, tuple(regs[i] for i in range(4, 8)), regs[15], pr[0]))
            if target == INFO_CALLBACK:
                regs[0] = INFO_RECORD
                return 0
            return 0 if target in (0x8C65F348, CALLBACK, TABLE_CALLBACK) else -9

        result = fn(view, ctypes.byref(native_fpu), ctypes.byref(native_sr), native_r,
                    ctypes.byref(native_pr), ctypes.byref(native_fpul), ctypes.byref(native_mac), None, call)
        case = (status_word, flag_word, lock_value, callback_enabled, bank, stack_offset, active_case, record_case, record_index)
        assert result == 0, (case, 'native service error', result, events_native)
        assert native_ram == ram, (case, 'RAM differs', next((hex(i), a, b) for i, (a, b) in enumerate(zip(native_ram, ram)) if a != b))
        assert list(native_r) == list(expected.r), (case, 'GPR mismatch')
        assert native_sr.value == expected.sr and native_pr.value == expected.pr, (case, 'SR/PR mismatch')
        assert bytes(native_fpu) == bytes(expected_fpu), (case, 'FPU mismatch')
        assert native_mac.value == special['macl'] and native_fpul.value == 0xCAFEBABE, (case, 'FPUL/MACL mismatch')
        assert events_native == events_expected, (case, 'callback trace mismatch', events_native, events_expected)
        if not startup_success:
            counts['negative_exit_cases'] += 1
        if active_case:
            counts['active_publication_cases'] += 1
            assert any(event[0] == INFO_CALLBACK for event in events_expected), (case, 'active-info callback not reached')
        if record_case:
            counts['callback_table_cases'] += 1
            assert sum(event[0] == INFO_CALLBACK for event in events_expected) == 1, (case, 'info callback not reached once')
            assert sum(event[0] == TABLE_CALLBACK for event in events_expected) == 1, (case, 'runtime callback not reached once')
            assert struct.unpack_from('<I', ram, (0x8C893E50 + record_index * 12 + 8) & 0xFFFFFF)[0] == 0, (case, 'callback pointer was not cleared')
        count += 1

    print(f'{count} cleanup retail comparisons passed')
    source = (out / 'source/sa_input_service_cleanup_8c646654.c').read_bytes()
    dispatcher_source = (out / 'source/input_service_cleanup_native.c').read_bytes()
    dll = (ROOT / 'work/sonic-adventure/build/input_service_cleanup_native.dll').read_bytes()
    span = binary[FUNCTION - 0x8C010000:0x8C6467F0 - 0x8C010000]
    (out / 'analysis/input_service_cleanup_8c646654_verification.json').write_text(json.dumps({
        'cases_passed': count,
        **counts,
        'verified_entry': hex(FUNCTION),
        'end_exclusive': '0x8c6467f0',
        'indexed_instruction_bytes': 334,
        'retail_span_bytes': len(span),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'source_sha256': hashlib.sha256(source).hexdigest(),
        'dispatcher_source_sha256': hashlib.sha256(dispatcher_source).hexdigest(),
        'native_dispatcher_sha256': hashlib.sha256(dll).hexdigest(),
        'scope': 'The cleanup routine was invoked at its retail entry in complete SH-4 comparisons covering negative exits, successful cleanup, active-device publication, and one matching runtime callback record in each of all 48 callback-table slots. Both register banks and stack alignment varied. Complete 16 MiB RAM, GPR/SR/PR/FPU/FPUL/MACL and callback traces were compared; dependencies were native, and external services used explicit deterministic stubs.',
        'limitations': [
            'Only one matching callback is enabled per callback-table case; its game-specific behavior is represented by a no-effect service stub.',
            'The optional error callback and information-pointer callback are deterministic service stubs, not recovered callback implementations.',
            'The address span is 412 bytes; the indexed function body is 334 instruction bytes and the enclosing span also contains literal/data bytes.',
        ],
    }, indent=2) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
