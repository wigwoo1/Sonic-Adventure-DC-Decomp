import ctypes, itertools, json, struct
from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call, P

out = ROOT / 'outputs/sonic-adventure'
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn = ctypes.CDLL(str(ROOT / 'work/sonic-adventure/build/file_audio_configuration_forward.dll')).sa_file_audio_configuration_forward
fn.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(Fpu), P, P, P, P, P, ctypes.c_void_p, Call]
cases = 0
for handle, rate, extension, result, bank in itertools.product(
        [0x8ca20000, 0x8ca20040], [0, 22050, 44100, 0x80000001],
        [0, 500, 0xffff8000, 0xffffffff], [0, 0xffffffff], [0, 0x200000]):
    ram = bytearray(16 * 1024 * 1024)
    ram[0x10000:0x10000+len(binary)] = binary
    ram[0xa20000:0xa20100] = bytes([0x55])*256
    native = bytearray(ram)
    view = (ctypes.c_uint8*len(native)).from_buffer(native)
    expected, actual = [], []
    def hook(mem, regs, sr):
        expected.append(tuple(regs[i] for i in range(4, 8)))
        struct.pack_into('<HH', mem, (regs[6]&0xffffff), 0x1234, 0xabcd)
        regs[0] = result
        return sr
    @Call
    def call(ctx, target, mem, fpu, sr, regs, pr):
        assert target == 0x8c65adc0
        actual.append(tuple(regs[i] for i in range(4, 8)))
        for i, byte in enumerate(b'\x34\x12\xcd\xab'):
            mem[(regs[6]&0xffffff)+i] = byte
        regs[0] = result
        return 0
    s = State(); s.r[:] = [0x22340000+i for i in range(16)]
    s.r[4:7] = [handle, rate, extension]; s.r[15] = 0x8cf00000
    s.pc = 0x8c644be6; s.pr = 0x8c000100; s.sr = 0x70000063
    regs = (ctypes.c_uint32*16)(*s.r); sr = ctypes.c_uint32(s.sr); pr = ctypes.c_uint32(s.pr)
    fpul = ctypes.c_uint32(); macl = ctypes.c_uint32(); f = Fpu(); f.fpscr = bank
    nf = Fpu.from_buffer_copy(f); special = {'macl': 0}
    reference(ram, s, {0x8c65adc0: hook}, fpu=f, special=special)
    assert fn(view, ctypes.byref(nf), ctypes.byref(sr), regs, ctypes.byref(pr), ctypes.byref(fpul), ctypes.byref(macl), None, call) == 0
    assert native == ram and list(regs) == list(s.r)
    assert sr.value == s.sr and pr.value == s.pr and bytes(nf) == bytes(f) and macl.value == special['macl']
    assert actual == expected == [(extension, rate, handle+0x30, handle+0x32)]
    assert regs[0] == result and regs[15] == 0x8cf00000
    target = bytearray([0x55])*256
    offset = (handle & 0xffffff)-0xa20000+0x30
    target[offset:offset+4] = b'\x34\x12\xcd\xab'
    assert native[0xa20000:0xa20100] == target
    cases += 1
(out/'analysis/file_audio_configuration_forward_verification.json').write_text(json.dumps(dict(
    cases_passed=cases,
    scope='Native 12-instruction file audio configuration wrapper 644be6..644bfe. Full retail RAM/CPU/FPU/SR/PR/MACL and service trace comparisons. Two handles, four rates, four signed-extension bit patterns, success/error returns, both FPU banks. Independent forwarding R4=extension,R5=rate,R6=handle+48,R7=handle+50; restored stack, preserved PR, output guards and tail-call result. Lower routine65adc0 modeled; pitch calculation, full initializer integration, physical audio and fullgame unverified.'
), indent=2)+'\n')
print(cases, 'file audio configuration forwarding comparisons passed')
