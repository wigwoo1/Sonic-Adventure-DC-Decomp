import ctypes
import hashlib
import itertools
import json
import struct

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P


def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    out = ROOT / 'outputs/sonic-adventure'
    args = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P, ctypes.c_void_p, Call]
    active = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_active_device_select_8c64658a.dll')).sa_input_active_device_select_8c64658a
    active.argtypes = args
    status_fn = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_channel_status_pointer_8c646868.dll')).sa_input_channel_status_pointer
    status_fn.argtypes = args
    info_fn = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_channel_info_pointer_8c646868.dll')).sa_input_channel_info_pointer
    info_fn.argtypes = args
    count = 0
    callback_target = 0x8C600000

    for mask, info_flags, state_byte, info_type, inner_mask, old_status, bank in itertools.product(
        [0, 1, 5, 15], [0, 0x80], [0, 1], [7, 8], [0, 4], [0, 1], [0, 0x200000]
    ):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        struct.pack_into('<I', ram, 0x8A83B4, 0x8CA10000)
        # The selector reads a signed displacement from the instruction's literal at 0x8c646616.
        displacement = struct.unpack_from('<h', ram, 0x646616)[0]
        ram[(0x8CA10000 & 0xFFFFFF) + displacement] = mask
        ram[0x66D348:0x66D34C] = bytes([12, 0, 6, 0])
        ram[0x894110:0x894114] = bytes([old_status] * 4)
        ram[0x89410C:0x894110] = bytes([1] * 4)
        record_base = 0x893BD0
        info_addresses = {}
        for table_index in [0, 6, 12]:
            rec = record_base + table_index * 24
            struct.pack_into('<I', ram, rec, info_flags)
            ram[rec + 2] = state_byte
            ram[rec + 4] = info_type
            info = 0x8CA40000 + table_index * 0x100
            nested = 0x8CA60000 + table_index * 0x100
            struct.pack_into('<I', ram, rec + 8, nested)
            ram[(nested & 0xFFFFFF) + 8] = inner_mask
            info_addresses[table_index] = info
        struct.pack_into('<I', ram, 0x6468B8, callback_target)

        native = bytearray(ram)
        view = (ctypes.c_uint8 * len(native)).from_buffer(native)
        nf = Fpu()
        nf.fpscr = bank
        rf = Fpu.from_buffer_copy(nf)
        s = State()
        s.r[:] = [0x22340000 + i * 0x100 for i in range(16)]
        s.r[15] = 0x8CF00000
        s.sr = 0x70000063
        s.pr = 0x8C123456
        s.pc = 0x8C64658A
        r = (ctypes.c_uint32 * 16)(*s.r)
        sr = ctypes.c_uint32(s.sr)
        pr = ctypes.c_uint32(s.pr)
        fpul = ctypes.c_uint32(0x12345678)
        macl = ctypes.c_uint32(0x87654321)
        nfpul = ctypes.c_uint32(fpul.value)
        nmacl = ctypes.c_uint32(macl.value)
        called = []

        @Call
        def call(ctx, target, memory, fpu, native_sr, regs, native_pr):
            called.append(target)
            if target == 0x8C646856:
                return status_fn(memory, fpu, native_sr, regs, native_pr, ctypes.byref(nfpul), ctypes.byref(nmacl), None, call)
            if target == 0x8C646868:
                return info_fn(memory, fpu, native_sr, regs, native_pr, ctypes.byref(nfpul), ctypes.byref(nmacl), None, call)
            if target == callback_target:
                regs[0] = info_addresses.get(regs[4], 0x8CA40000)
                return 0
            return -9

        def reference_info(memory, regs, old_sr):
            regs[0] = info_addresses.get(regs[4], 0x8CA40000)
            return old_sr

        hooks = {callback_target: reference_info}
        reference(ram, s, hooks=hooks, fpu=rf, special={'macl': macl.value})
        result = active(view, ctypes.byref(nf), ctypes.byref(sr), r, ctypes.byref(pr), ctypes.byref(nfpul), ctypes.byref(nmacl), None, call)
        case = (mask, info_flags, state_byte, info_type, inner_mask, old_status, bank)
        assert result == 0, (case, 'service error', result, called)
        assert native == ram, (case, 'RAM mismatch', next((hex(i), a, b) for i, (a, b) in enumerate(zip(native, ram)) if a != b))
        assert list(r) == list(s.r) and sr.value == s.sr and pr.value == s.pr, (case, 'CPU state mismatch')
        assert bytes(nf) == bytes(rf) and nfpul.value == fpul.value and nmacl.value == macl.value, (case, 'FPU/special state mismatch')
        assert called.count(0x8C646856) == called.count(0x8C646868), (case, 'helper callback pairing', called)
        count += 1

    print(f'{count} active-device selection retail comparisons passed')
    source = (out / 'source/sa_input_active_device_select_8c64658a.c').read_bytes()
    span = binary[0x8C64658A - 0x8C010000:0x8C646654 - 0x8C010000]
    (out / 'analysis/input_active_device_select_8c64658a_verification.json').write_text(json.dumps({
        'cases_passed': count,
        'verified_entry': '0x8c64658a',
        'end_exclusive': '0x8c646654',
        'indexed_instruction_bytes': 168,
        'retail_span_bytes': len(span),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'source_sha256': hashlib.sha256(source).hexdigest(),
        'scope': 'Active-device selection compared against retail execution in 256 cases with varied availability masks, channel-info flags, record states, inner flags, prior status bytes, and both SH-4 register banks. Complete 16 MiB RAM/GPR/SR/PR/FPU/FPUL/MACL comparisons. Internal status/info helpers were native; their external info callback returned the same explicit mock record pointer in both executions. The indexed code body is 168 bytes; the 202-byte enclosing address span includes its embedded literal pool.',
    }, indent=2) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
