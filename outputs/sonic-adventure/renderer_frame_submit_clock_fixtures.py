"""Keep retained service clocks; supply frame clocks only at the frame entry."""
from renderer_context_producer_disc_fixtures import *
import renderer_context_producer_disc_fixtures as previous
import write_watch_renderer_frame_submit as fast
FRAME_TICKS=(0,20000000,20000001,20000002)


def prepare(ram,c):
    return previous.prepare(ram,c)
