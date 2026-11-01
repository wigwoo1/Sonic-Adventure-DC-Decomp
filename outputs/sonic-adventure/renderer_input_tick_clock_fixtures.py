"""Finite monotonic external timer samples for the newly reached frame reads."""
from renderer_input_tick_mmio_fixtures import *
import renderer_input_tick_mmio_fixtures as previous
# Preserve the old four-sample profile as a separate exhaustion control.
# These are peripheral inputs, not guest results or an automatic clock model.
FRAME_TICKS=(0,20000000,*range(20000001,20000033))

def prepare(ram,c):return previous.prepare(ram,c)
