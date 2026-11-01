"""Original RAM inputs and explicit external hardware profile; no context fixture."""
from renderer_context_owner_fixtures import *
import renderer_context_owner_fixtures as previous
import write_watch_renderer_context_owner_capture as fast


def prepare(ram, c):
    _, s, f, cfg = previous.prepare(ram, c)
    # Retain only the declared CPU/FPU input and software peripheral scenario.
    # RAM comes from the original loaded image, without the geometry fixture's
    # supplied polygon/view roots, cursor, header, player records or callbacks.
    m = bytearray(ram)
    assert get(m, 0x8c88fcd4) == get(m, 0x8c88fcb4) == 0
    cfg.update(scan_samples=[0x101ff, 0x400, 0x80000000, 0x10001] + [1, 0] * 64,
        startup_overrides={0x5f689c: 11}, irq_samples=[0, 8],
        render_seed={0x5f8000: 0x17fd11db, 0x5f8004: 0x11})
    return m, s, f, cfg
