import ctypes
import hashlib
import itertools
import json

from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P


def main():
    out = ROOT / 'outputs/sonic-adventure'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    fn = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/input_channel_info_pointer_8c646868.dll')).sa_input_channel_info_pointer
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P, ctypes.c_void_p, Call]
    count = 0
    for callback, stack_offset, pr_value, bank in itertools.product(
        [0x8C6468B8, 0x8C600000], [0, 4], [0x8C123456, 0x8CABCDEF], [0, 0x200000]
    ):
        ram = bytearray(16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        ram[0x6468B8:0x6468BC] = callback.to_bytes(4, 'little')
        native = bytearray(ram)
        view = (ctypes.c_uint8 * len(native)).from_buffer(native)
        observed = []

        @Call
        def call(ctx, target, memory, fpu, sr, regs, pr):
            observed.append((target, regs[4], regs[5], regs[6], regs[7]))
            return 0

        f = Fpu()
        f.fpscr = bank
        nf = Fpu.from_buffer_copy(f)
        s = State()
        s.r[:] = [0x22340000 + i * 0x100 for i in range(16)]
        s.r[15] = 0x8CF00000 - stack_offset
        s.sr = 0x70000063
        s.pr = pr_value
        s.pc = 0x8C646868
        r = (ctypes.c_uint32 * 16)(*s.r)
        sr = ctypes.c_uint32(s.sr)
        pr = ctypes.c_uint32(s.pr)
        fpul = ctypes.c_uint32(0x12345678)
        macl = ctypes.c_uint32(0x87654321)
        hooks = {callback: lambda memory, regs, old_sr: old_sr}
        reference(ram, s, hooks=hooks, fpu=f, special={'macl': macl.value}, stop_pc=pr_value)
        result = fn(view, ctypes.byref(nf), ctypes.byref(sr), r, ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl), None, call)
        assert result == 0 and observed == [(callback, s.r[4], s.r[5], s.r[6], s.r[7])], (result, observed)
        assert native == ram, 'RAM differs from retail execution'
        assert list(r) == list(s.r) and sr.value == s.sr and pr.value == s.pr, 'CPU state differs from retail execution'
        assert bytes(nf) == bytes(f) and fpul.value == 0x12345678 and macl.value == 0x87654321, 'FPU/special state differs'
        count += 1

    source = (out / 'source/sa_input_channel_info_pointer.c').read_bytes()
    span = binary[0x8C646868 - 0x8C010000:0x8C646868 - 0x8C010000 + 14]
    report = {
        'cases_passed': count,
        'verified_entry': '0x8c646868',
        'retail_span_bytes': len(span),
        'retail_span_sha256': hashlib.sha256(span).hexdigest(),
        'source_sha256': hashlib.sha256(source).hexdigest(),
        'scope': '14-byte callback forwarding thunk compared with the retail interpreter in 16 full RAM/GPR/SR/PR/FPU/special-state cases. Callback target, four argument registers, stack offsets, saved PR, and both SH-4 register-bank settings varied. Callback service is an explicit no-effect stub; callback implementation is verified separately.',
    }
    (out / 'analysis/input_channel_info_pointer_8c646868_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{count} input channel info pointer retail comparisons passed')


if __name__ == '__main__':
    main()
