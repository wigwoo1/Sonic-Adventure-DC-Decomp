"""Raw game RAM plus the existing explicit external host-disc service vector."""
from renderer_context_producer_fixtures import *
import renderer_context_producer_fixtures as previous


def prepare(ram, c):
    m, s, f, cfg = previous.prepare(ram, c)
    # 10d974 tail-calls the firmware vector at8c0000bc. The retail game image
    # contains no BIOS RAM/vector table. Bind the existing declared host-disc
    # adapter,not a game helper return or a renderer/context fixture.
    assert get(m, 0x8c0000bc) == 0
    put(m, 0x8c0000bc, v.namespace['BIOS'])
    return m, s, f, cfg
